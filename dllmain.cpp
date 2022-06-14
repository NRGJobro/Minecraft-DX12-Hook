#include "dllmain.h"

void keyCallback(uint64_t c, bool v) {
	_key(c, v);
	if (keymap[VK_CONTROL] && keymap['L'] || keymap[VK_END]) clientAlive = false;
	keymap[c] = v;
}

void mouseClickCallback(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	if (ImGui::GetCurrentContext() != nullptr) {
		switch (mouseButton) {
		case 1:
			ImGui::GetIO().MouseDown[0] = isDown;
			break;
		case 2:
			ImGui::GetIO().MouseDown[1] = isDown;
			break;
		case 3:
			ImGui::GetIO().MouseDown[2] = isDown;
			break;
		case 4:
			ImGui::GetIO().MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
			break;
		default:
			break;
		}

		if (!ImGui::GetIO().WantCaptureMouse)
			return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);

	} else return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

void Init() {
	if (MH_Initialize() == MH_OK && !initImgui) {
		//Keymap
		uintptr_t keymapAddr = Utils::findSig("48 83 EC ? 0F B6 C1 4C 8D 05");
		if (MH_CreateHook((void*)keymapAddr, &keyCallback, reinterpret_cast<LPVOID*>(&_key)) == MH_OK) {
			MH_EnableHook((void*)keymapAddr);
		};

		//MouseClick
		uintptr_t clickHookAddr = Utils::findSig("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24");
		if (MH_CreateHook((void*)clickHookAddr, &mouseClickCallback, reinterpret_cast<LPVOID*>(&_Mouse)) == MH_OK) {
			MH_EnableHook((void*)clickHookAddr);
		};
	lab:
		while (clientAlive) {};
		Sleep(1);
		if (clientAlive)
			goto lab;

		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		FreeLibraryAndExitThread(GetDllHMod(), 0);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		if (initImgui) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ImguiHooks::InitImgui, hModule, 0, 0);
			initImgui = false;
		}
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, 0);
	}
	return TRUE;
}