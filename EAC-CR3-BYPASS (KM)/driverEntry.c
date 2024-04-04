#include <ntifs.h>
#include "communication.h"
#include "driverEntry.h"

/* ############ */
/* DRIVER ENTRY */
/* ############ */

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    ///////////////////////////////
    UNREFERENCED_PARAMETER(RegistryPath);
    ///////////////////////////////    

    ///////////////////////////////
    NTSTATUS status;
    ///////////////////////////////

    ///////////////////////////////
    status = IoCreateDevice(&driverName, &ntName);
    // status = IoCreateDevice(DriverObject, 0, &ntName, FILE_DEVICE_UNKNOWN, 0, FALSE, &gDeviceObject);
    if (!NT_SUCCESS(status))
    {
        return status;
    }
    ///////////////////////////////

    ///////////////////////////////
    status = IoCreateSymbolicLink(&symbolicLinkName, &ntName);
    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(gDeviceObject);
        return status;
    }
    ///////////////////////////////

    ///////////////////////////////
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DeviceCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DeviceClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoctlDispatch;
    DriverObject->DriverUnload = DriverUnload;
    ///////////////////////////////

    return STATUS_SUCCESS;
}

/* ############# */
/* CREATE DEVICE */
/* ############# */

NTSTATUS DeviceCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    ///////////////////////////////
    UNREFERENCED_PARAMETER(DeviceObject);
    ///////////////////////////////

    ///////////////////////////////
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    ///////////////////////////////

    ///////////////////////////////
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    ///////////////////////////////

    return STATUS_SUCCESS;
}

/* ############# */
/* DRIVER UNLOAD */
/* ############# */

NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject)
{
    ///////////////////////////////
    UNREFERENCED_PARAMETER(DriverObject);
    ///////////////////////////////

    ///////////////////////////////
    IoDeleteSymbolicLink(&symbolicLinkName);
    IoDeleteDevice(DriverObject->DeviceObject);
    ///////////////////////////////

    return STATUS_SUCCESS;
}

/* ############ */
/* CLOSE DEVICE */
/* ############ */

NTSTATUS DeviceClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    ///////////////////////////////
    UNREFERENCED_PARAMETER(DeviceObject);
    ///////////////////////////////

    ///////////////////////////////
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    ///////////////////////////////

    ///////////////////////////////
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    ///////////////////////////////

    return STATUS_SUCCESS;
}

/* ############## */
/* UNSUPPORTED IO */
/* ############## */

NTSTATUS UnSupportedIO(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    IofCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}