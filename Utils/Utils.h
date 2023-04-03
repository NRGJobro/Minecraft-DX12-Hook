#include "Math.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma warning(disable : 4996)
#include <Windows.h>

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <optional>
#include <Psapi.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "winmm.lib")
#include <filesystem>
#include <playsoundapi.h>

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

	static inline void downloadFile(std::string name, std::string url) {
		// Get File path
		std::string filePath = (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\RickRoll\\");
		if (!std::filesystem::exists(filePath)) std::filesystem::create_directory(filePath.c_str());  // if no path, make it so no crashy crashy :)
		filePath += name;

		// Open the internet connection and URL
		HINTERNET webH = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		HINTERNET urlFile;
		if (webH) {
			urlFile = InternetOpenUrlA(webH, url.c_str(), NULL, NULL, NULL, NULL);
		}
		else {
			// If the internet connection failed to open, return false
			return;
		}

		// Check if the URL file was opened successfully
		if (urlFile) {
			// Open the output file stream
			std::ofstream outputFile(filePath, std::ios::binary);
			if (outputFile.is_open()) {
				// Read the file in chunks and write it to the output file
				char buffer[2000];
				DWORD bytesRead;
				do {
					InternetReadFile(urlFile, buffer, 2000, &bytesRead);
					outputFile.write(buffer, bytesRead);
					memset(buffer, 0, 2000);
				} while (bytesRead);

				// Close the file and internet handles
				outputFile.close();
				InternetCloseHandle(webH);
				InternetCloseHandle(urlFile);

				// Return true to indicate that the download was successful
				return;
			}
			else {
				// If the output file failed to open, close the internet handles and return false
				InternetCloseHandle(webH);
				InternetCloseHandle(urlFile);
				return;
			}
		}
		else {
			// If the URL file failed to open, close the internet connection and return false
			InternetCloseHandle(webH);
			return;
		}
	};
	static void systemPlay(std::string name, bool stop = true) {
		std::string path = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\RickRoll\\") + std::string(name);
		if (stop)
			sndPlaySoundA((LPCSTR)path.c_str(), SND_FILENAME | SND_ASYNC);
		else
			sndPlaySoundA((LPCSTR)path.c_str(), SND_FILENAME | SND_NOSTOP | SND_ASYNC);
	}


	static inline void downloadEverything() {
		//Make a new thread so i dont make game lag 
		//MULTI THREAD BABYYYYYYYYY!!! (epic speeds)

		std::thread makeFilePaths([]() {
			// Make File Paths
			std::string path = (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\RickRoll\\");  // Get Initial Path
			if (!std::filesystem::exists(path)) std::filesystem::create_directory(path.c_str());														   // make Folder
		});
		makeFilePaths.detach();
		
		std::thread downloadImage([]() {
			// Add the RickAstley image to the folder
			downloadFile("RickAstley.jpg", "https://cdn.discordapp.com/attachments/958563981239386112/1092253527914577941/RickAstley.jpg");
		});
		downloadImage.detach();
		
		std::thread downloadSound([]() {
			// Add the Never Gonna Give You Up song to the folder
			downloadFile("Never_Gonna_Give_You_Up.wav", "https://github.com/NRGJobro/music/raw/main/Never_Gonna_Give_You_Up.wav");
		});
		downloadSound.detach();
	};
};