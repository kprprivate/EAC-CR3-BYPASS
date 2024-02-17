#include <ntifs.h>
#include <ntimage.h>
#include "defines.h"

// Global variables for storing base addresses
volatile uint64_t g_MmPfnDatabase = 0;
volatile uint64_t g_PXE_BASE = 0;
volatile uint64_t g_idx = 0;

// Initialize definitions and global variables
void initDefinesCR3() {
    KDDEBUGGER_DATA64 kdBlock = { 0 };
    CONTEXT context = { 0 };
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);

    PDUMP_HEADER dumpHeader = ExAllocatePool(NonPagedPool, DUMP_BLOCK_SIZE);
    if (dumpHeader) {
        KeCapturePersistentThreadState(&context, NULL, 0, 0, 0, 0, 0, dumpHeader);
        RtlCopyMemory(&kdBlock, (PUCHAR)dumpHeader + KDDEBUGGER_DATA_OFFSET, sizeof(kdBlock));

        ExFreePool(dumpHeader);

        g_MmPfnDatabase = *(ULONG64*)(kdBlock.MmPfnDatabase);

        // Calculate base addresses for page table entries
        ULONG64 g_PTE_BASE = kdBlock.PteBase;
        ULONG64 g_PDE_BASE = g_PTE_BASE + ((g_PTE_BASE & 0xffffffffffff) >> 9);
        ULONG64 g_PPE_BASE = g_PTE_BASE + ((g_PDE_BASE & 0xffffffffffff) >> 9);
        g_PXE_BASE = g_PTE_BASE + ((g_PPE_BASE & 0xffffffffffff) >> 9);
        g_idx = (g_PTE_BASE >> 39) - 0x1FFFE00;
    }
}

// Retrieve the Directory Base address of a specific process
uintptr_t GetDirBase(uint32_t pid) {
    PHYSICAL_MEMORY_RANGE* memRange = MmGetPhysicalMemoryRanges();
    if (!memRange) {
        // Error: Failed to get physical memory ranges
        return 0;
    }

    // First, obtain the PEPROCESS from the PID
    PEPROCESS pProcess = NULL;
    if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)pid, &pProcess))) {
        // Failed to get the process
        ExFreePool(memRange);
        return 0;
    }

    // Now, obtain the process name
    char* targetProcessName = PsGetProcessImageFileName((uintptr_t)pProcess);
    ObDereferenceObject(pProcess); // We no longer need this

    uintptr_t dirBase = 0;
    const uint64_t cr3PteBase = g_idx * 8 + g_PXE_BASE;

    // Scan through the physical memory ranges
    for (int i = 0; memRange[i].BaseAddress.QuadPart != 0 || memRange[i].NumberOfBytes.QuadPart != 0; ++i) {
        uint64_t startPfn = memRange[i].BaseAddress.QuadPart >> 12;
        uint64_t endPfn = startPfn + (memRange[i].NumberOfBytes.QuadPart >> 12);

        for (uint64_t pfn = startPfn; pfn < endPfn; ++pfn) {
            _MMPFN* currentMmpfn = (_MMPFN*)(g_MmPfnDatabase + 0x30 * pfn);

            if (currentMmpfn->flags && currentMmpfn->flags != 1 && currentMmpfn->pte_address == cr3PteBase) {
                uintptr_t decryptedEprocess = ((currentMmpfn->flags | 0xF000000000000000) >> 0xd) | 0xFFFF000000000000;

                if (MmIsAddressValid((void*)decryptedEprocess)) {
                    char* processName = PsGetProcessImageFileName((uintptr_t)decryptedEprocess);
                    if (_stricmp(processName, targetProcessName) == 0) {
                        dirBase = pfn << 12;
                        goto cleanup;
                    }
                }
            }
        }
    }

cleanup:
    ExFreePool(memRange);
    return dirBase;
}