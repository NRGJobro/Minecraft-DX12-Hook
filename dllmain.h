#pragma once
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
#include "Utils/ClientLogs.h"