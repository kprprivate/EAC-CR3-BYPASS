#include "imports.h"
#include <iostream>

auto getprocessid(std::wstring processname) -> uintptr_t
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);
	if (!Process32First(snapshot, &entry)) {
		return 0;
	}
	while (Process32Next(snapshot, &entry)) {
		if (std::wstring(entry.szExeFile) == processname) {
			return entry.th32ProcessID;
		}
	}
	return 0;
}

auto main() -> const NTSTATUS
{
	auto process = getprocessid(L"FortniteClient-Win64-Shipping.exe");
	printf("PID: %i \n", process);

	if (process != 0)
	{
		driver.initdriver(process);

		driver.initDefinesCR3();

		auto cr3 = driver.getCR3();
		printf("[+] CR3: 0x%p\n", cr3);

	}

	getchar();
	exit(0);
	return 0;
}
