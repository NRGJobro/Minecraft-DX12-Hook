#pragma once
//Module Stuff
#include "../Modules/Module.h"
#include "../Manager/ModuleManager.h"
#include "../Manager/HooksManager.h"
#include "../Utils/SDK.h"

bool clientAlive = true;
bool initImgui = true;
bool renderClickUI = false;

ModuleHandler handler = ModuleHandler();
mc minecraft = mc();
bool toggledModule = false;
std::vector<std::string> categories = std::vector<std::string>();

//Keymap and key hook
std::map<uint64_t, bool> keymap = std::map<uint64_t, bool>();
typedef void(__thiscall* key)(uint64_t keyId, bool held);
key _key;

//Mouse hook
typedef void(__thiscall* Mouse)(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8);
Mouse _Mouse;

//RenderContext
typedef void(__fastcall* RenderCtx)(unsigned __int64, class MinecraftUIRenderContext*);
RenderCtx _RenderCtx;

void keyCallback(uint64_t c, bool v) {
	_key(c, v);
	if (keymap[VK_CONTROL] && keymap['L'] || keymap[VK_END]) clientAlive = false;

	for (int i = 0; i < handler.modules.size(); ++i) {
		if (c == handler.modules[i]->keybind && v == true) {
			handler.modules[i]->enabled = !handler.modules[i]->enabled;
			if (handler.modules[i]->enabled) {
				handler.modules[i]->OnEnable();
				for (auto mod : handler.modules)
					toggledModule = true;
			}
			else {
				handler.modules[i]->OnDisable();
				for (auto mod : handler.modules)
					toggledModule = true;
			}
		}
	}
	keymap[c] = v;
}

void mouseClickCallback(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(mouseX, mouseY);
        
		switch (mouseButton) {
		case 1:
			io.MouseDown[0] = isDown;
			break;
		case 2:
			io.MouseDown[1] = isDown;
			break;
		case 3:
			io.MouseDown[2] = isDown;
			break;
		case 4:
			io.MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
			break;
		default:
			break;
		}

		if (!io.WantCaptureMouse)
			return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);

	}
	else return _Mouse(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

void renderCtxCallback(unsigned __int64 _this, MinecraftUIRenderContext* minecraftUIRenderContext) {

	// Set client instance
	minecraft.clientInstance = minecraftUIRenderContext->clientInstance;
	
	_RenderCtx(_this, minecraftUIRenderContext);
}


void Init() {
	if (MH_Initialize() == MH_OK && !initImgui) {
		handler.InitModules();
		for (auto mod : handler.modules) {
			bool addCategory = true;
			for (auto cat : categories)
				if (mod->category == cat) addCategory = false;
			if (addCategory)
				categories.push_back(mod->category);
		}

		//Keymap
		uintptr_t keymapAddr = FindSignature("48 83 EC ? 0F B6 C1 4C 8D 05");
		HookManager::createHook(keymapAddr, &keyCallback, reinterpret_cast<LPVOID*>(&_key));

		//MouseClick
		uintptr_t clickHookAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24");
		HookManager::createHook(clickHookAddr, &mouseClickCallback, reinterpret_cast<LPVOID*>(&_Mouse));

		//RenderContext/ClientInstance
		uintptr_t renderCtxAddr = FindSignature("48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 8b f2 48 89 54 24 ? 4c 8b e9"); // Ref sig: E8 ? ? ? ? 48 8B 44 24 ? 48 8D 4C 24 ? 48 8B 80
		HookManager::createHook(renderCtxAddr, &renderCtxCallback, reinterpret_cast<LPVOID*>(&_RenderCtx));
		
		while (minecraft.clientInstance == nullptr) {};
	lab:
		while (clientAlive) {};
		Sleep(1);
		if (clientAlive)
			goto lab;

		DisableThreadLibraryCalls(Utils::GetDllHMod());
		DisableThreadLibraryCalls(GetDllMod());
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		FreeLibraryAndExitThread(Utils::GetDllHMod(), 0);
		FreeLibraryAndExitThread(GetDllMod(), 0);
	}
}

class Hooks {
public:
	static void InitShit() {
		Init();
	}
};
