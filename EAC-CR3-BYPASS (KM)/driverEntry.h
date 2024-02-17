#pragma once

/* ############# */
/* IOCTL DEFINES */
/* ############# */

#define DEVICE_NAME L"cr3Bypass"

UNICODE_STRING ntName = RTL_CONSTANT_STRING(L"\\Device\\" DEVICE_NAME);
UNICODE_STRING symbolicLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\" DEVICE_NAME);
PDEVICE_OBJECT gDeviceObject = NULL;


/* ############ */
/* DRIVER ENTRY */
/* ############ */

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);

/* ############# */
/* CREATE DEVICE */
/* ############# */

NTSTATUS DeviceCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);

/* ############# */
/* DRIVER UNLOAD */
/* ############# */

NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject);

/* ############ */
/* CLOSE DEVICE */
/* ############ */

NTSTATUS DeviceClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);


/* ############## */
/* UNSUPPORTED IO */
/* ############## */

NTSTATUS UnSupportedIO(PDEVICE_OBJECT DeviceObject, PIRP Irp);
