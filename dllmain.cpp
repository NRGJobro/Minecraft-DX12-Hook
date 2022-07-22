#include "dllmain.h"
#include "Hooks/Hooks.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		if (initImgui) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ImguiHooks::InitImgui, hModule, 0, 0);
			initImgui = false;
		}
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Hooks::InitShit, hModule, 0, 0);
	}
	return TRUE;
}