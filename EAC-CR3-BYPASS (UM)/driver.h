#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <string>

#include "ioctl.h"
#include "structure.h"

class _driver {
    HANDLE driverHandler = nullptr; // Handle to the driver.

public:
    uint32_t _processid = 0;

    // Default constructor.
    _driver() = default;

    // Constructor with process ID.
    explicit _driver(uint32_t processid) : _processid(processid) {
        initdriver(processid);
    }

    // Destructor to ensure proper resource release.
    ~_driver() {
        if (driverHandler != INVALID_HANDLE_VALUE) {
            CloseHandle(driverHandler);
        }
    }

    // Prevent copy construction and assignment.
    _driver(const _driver&) = delete;
    _driver& operator=(const _driver&) = delete;

    // Allow move semantics for efficient resource management.
    _driver(_driver&& other) noexcept : driverHandler(std::exchange(other.driverHandler, nullptr)) {}

    _driver& operator=(_driver&& other) noexcept {
        if (this != &other) {
            if (driverHandler != INVALID_HANDLE_VALUE) {
                CloseHandle(driverHandler);
            }
            driverHandler = std::exchange(other.driverHandler, nullptr);
        }
        return *this;
    }

    // Initialize the driver with a given process ID.
    void initdriver(int processid) {
        driverHandler = CreateFileA(
            "\\\\.\\cr3Bypass",
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        _processid = processid;
    }

    auto initDefinesCR3() {

        MEMORY_OPERATION_DATA requestData = {};

        DeviceIoControl(
            driverHandler,                  // Device handle
            IOCTL_INIT_DEFINES_CR3,         // Control code
            &requestData,                   // Input buffer
            sizeof(requestData),            // Input buffer size
            &requestData,                   // Output buffer (reuse input buffer for output)
            sizeof(requestData),            // Output buffer size
            nullptr,                        // Bytes returned
            nullptr                         // Overlapped structure (not using overlapped I/O in this example)
        );

    }

    auto getCR3() -> uintptr_t
    {
        MEMORY_OPERATION_DATA requestData = { _processid };

        DeviceIoControl(
            driverHandler,                  // Device handle
            IOCTL_GET_PROCESS_CR3,          // Control code
            &requestData,                   // Input buffer
            sizeof(requestData),            // Input buffer size
            &requestData,                   // Output buffer (reuse input buffer for output)
            sizeof(requestData),            // Output buffer size
            nullptr,                        // Bytes returned
            nullptr                         // Overlapped structure (not using overlapped I/O in this example)
        );

        return requestData.cr3;
    }
};

_driver driver;
