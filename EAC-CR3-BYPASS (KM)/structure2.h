#pragma once

/* ################# */
/* MEMORY OPERATIONS */
/* ################# */

typedef struct _MEMORY_OPERATION_DATA {
    uint32_t        src_pid;
    uintptr_t       cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;
