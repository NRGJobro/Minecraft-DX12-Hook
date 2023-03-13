#pragma once
#include <cstdint>

//Actor class
class Actor {
public:
	void unlockAchievments() {
		using fireEventAward_t = void(__cdecl*)(Actor*, int32_t);
		static auto fireEventAwardFunc = reinterpret_cast<fireEventAward_t>(Utils::findSig("48 85 c9 0f 84 ? ? ? ? 48 89 5c 24 ? 57 48 81 ec ? ? ? ? 48 8b 01"));
		for (int32_t i = 0; i < 122; i++) fireEventAwardFunc(this, i);
	};
};

//ClientInstance class
class ClientInstance {
public:
	class Actor* getLocalPlayer() {
		return (class Actor*)Utils::callVirtualTable<25, class Actor*>(this);
	};
};

//MC class
class mc {
public:
	class ClientInstance* clientInstance;
};

//MinecraftUIRenderCtx class
class MinecraftUIRenderContext {
public:
	class ClientInstance* clientInstance; //0x0008
};