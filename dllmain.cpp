#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <MinHook.h>
#include <Psapi.h>
#include <cassert>
#include <cstdint>
#include <map>
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_win32.h"
#include "include/imgui/imgui_impl_dx12.h"
#include "include/imgui/imgui_impl_dx11.h"
#include <initguid.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_1.h>
#include <dxgi1_4.h>
#include <dwrite_1.h>
#include "include/kiero/kiero.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dwrite.lib")
#include "Utils/Utils.h"
#include "Utils/HookImgui.h"
bool clientAlive = true;
bool initImgui = true;

std::map<uint64_t, bool> keymap = std::map<uint64_t, bool>();
typedef void(__thiscall* key)(uint64_t keyId, bool held);
key _key;
void keyCallback(uint64_t c, bool v) {
	_key(c, v);
	if (keymap[VK_CONTROL] && keymap['L'] || keymap[VK_END]) clientAlive = false;
	keymap[c] = v;
}

typedef void(__thiscall* Mouse)(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8);
Mouse _Mouse;
void mouseClickCallback(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	_Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	//if (mouseButton > 0 && mouseButton < 3)
		//ImGui::GetIO().MouseDown[0] = isDown;

	//if (!ImGui::GetIO().WantCaptureMouse)
		//return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
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