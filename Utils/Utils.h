#include "Math.h"

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define FindSignature(x) Utils::findSig(x);
class Utils {
public:
	static ImVec2 getScreenResolution() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		return ImVec2(desktop.right, desktop.bottom);
	}
	static auto GetDllHMod(void) -> HMODULE {
		MEMORY_BASIC_INFORMATION info;
		size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllHMod, &info, sizeof(info));
		assert(len == sizeof(info));
		return len ? (HMODULE)info.AllocationBase : NULL;
	}

	template <unsigned int index, typename returnType, typename... args>
	static inline auto callVirtualTable(void* ptr, args... argList) -> returnType {
		using function = returnType(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<function**>(ptr))[index](ptr, argList...);
	}
	
	static uintptr_t findSig(const char* sig) {
		const char* pattern = sig;
		uintptr_t firstMatch = 0;
		static const uintptr_t rangeStart = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
		static MODULEINFO miModInfo;
		static bool init = false;
		if (!init) {
			init = true;
			GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		}
		static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

		BYTE patByte = GET_BYTE(pattern);
		const char* oldPat = pattern;

		for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
			if (!*pattern)
				return firstMatch;

			while (*(PBYTE)pattern == ' ')
				pattern++;

			if (!*pattern)
				return firstMatch;

			if (oldPat != pattern) {
				oldPat = pattern;
				if (*(PBYTE)pattern != '\?')
					patByte = GET_BYTE(pattern);
			};

			if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte) {
				if (!firstMatch)
					firstMatch = pCur;

				if (!pattern[2])
					return firstMatch;

				pattern += 2;
			}
			else {
				pattern = sig;
				firstMatch = 0;
			};
		};
	};
};