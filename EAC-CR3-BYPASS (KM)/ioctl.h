#pragma once

/* ############### */
/* IOCTL FUNCTIONS */
/* ############### */

// Define IOCTL Macro/Shortcut
#define BASE_IOCTL_CODE(code) CTL_CODE(FILE_DEVICE_UNKNOWN, (code), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Define IOCTL commands
#define IOCTL_INIT_DEFINES_CR3 BASE_IOCTL_CODE(0x665fa1f0)
#define IOCTL_GET_PROCESS_CR3 BASE_IOCTL_CODE(0x3e2d6701)
