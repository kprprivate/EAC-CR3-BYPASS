#pragma once
#include <ntifs.h>
#include "ioctl.h"
#include <stddef.h>

/* ################ */
/* FUNCTION HANDLER */
/* ################ */

NTSTATUS IoctlDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);

/* ################## */
/* VERIFY NULL BUFFER */
/* ################## */

#define VERIFY_BUFFER(buf, failStatus) \
    if ((buf) == NULL) { \
        *IOStatus = (failStatus); \
        IoCompleteRequest(Irp, IO_NO_INCREMENT); \
        return (failStatus); \
    }