#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <iostream>

#pragma comment(lib, "Setupapi.lib")

void ListUSBDevices() {
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, nullptr, nullptr, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Failed to get device list.\n";
		return;
	}

	SP_DEVINFO_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	DWORD index = 0;
	while (SetupDiEnumDeviceInfo(hDevInfo, index, &devInfoData)) {
		char deviceName[256];
		if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
			(PBYTE)deviceName, sizeof(deviceName), nullptr)) {
			std::cout << "USB Device : " << deviceName << "\n";
		}
		else {
			std::cout << "USB Device : Unknown\n";
		}
		SetupDiDestroyDeviceInfoList(hDevInfo);
		index++;
	}
}

int main() {
	std::cout << "Listing all connected USB devices:\n";
	ListUSBDevices();
	return 0;
}