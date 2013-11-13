#include "WeMo.h"
#include "win32_exception.h"

WeMo * wemo = nullptr;

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {
	switch(fdwReason) {
	case DLL_PROCESS_ATTACH:
		wemo = new WeMo();	
		break;
	case DLL_PROCESS_DETACH:
		delete wemo;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		// ignore
		break;
	}

	return TRUE;
}

extern "C" {

	__declspec(dllexport) void __cdecl turnOn(void) {
		wemo->turnOn();
	}

	__declspec(dllexport) void __cdecl turnOff(void) {
		wemo->turnOff();
	}

	__declspec(dllexport) int __cdecl getState(void) {
		return (int)wemo->getState();
	}

}