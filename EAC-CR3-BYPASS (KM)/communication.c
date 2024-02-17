#include <ntifs.h>
#include <ntddk.h>
#include "defines.h"
#include "DirbaseBypass.h"
#include "structure.h"
#include "communication.h"

NTSTATUS IoctlDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) 
{
    ///////////////////////////////
    UNREFERENCED_PARAMETER(DeviceObject);
    ///////////////////////////////

    ///////////////////////////////
    PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
    
    PVOID ioBuffer = Irp->AssociatedIrp.SystemBuffer;
    
    ULONGLONG* ioInformation = &Irp->IoStatus.Information;

    NTSTATUS* IOStatus = &Irp->IoStatus.Status;
    ///////////////////////////////

    ///////////////////////////////
    NTSTATUS status = STATUS_SUCCESS;
    ///////////////////////////////

    ///////////////////////////////
    VERIFY_BUFFER(ioBuffer, STATUS_INVALID_PARAMETER);
    ///////////////////////////////

    if (irpSp->MajorFunction == IRP_MJ_DEVICE_CONTROL) {
        switch (irpSp->Parameters.DeviceIoControl.IoControlCode) {
            ///////////////////////////////
            // findGuardedRegion
            case IOCTL_GET_PROCESS_CR3:
                ((PMEMORY_OPERATION_DATA)ioBuffer)->cr3 = GetDirBase(((PMEMORY_OPERATION_DATA)ioBuffer)->src_pid);
                *ioInformation = sizeof(MEMORY_OPERATION_DATA);
                break;
            ///////////////////////////////

            ///////////////////////////////
            // initDefinesCR3
            case IOCTL_INIT_DEFINES_CR3:
                initDefinesCR3();
                *ioInformation = 0;
                break;
            ///////////////////////////////
            
            ///////////////////////////////
            // default
            default:
                status = STATUS_INVALID_DEVICE_REQUEST;
                *ioInformation = 0;
                break;
            ///////////////////////////////
        }
    } else {
        status = STATUS_INVALID_DEVICE_REQUEST;
    }

    ///////////////////////////////
    *IOStatus = status;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    ///////////////////////////////

    return status;
}