#pragma once

#include <Windows.h>

#ifndef BUFFERCOUNT
#define BUFFERCOUNT 2
#endif

#ifndef CLIP_NEAR_Z 
#define CLIP_NEAR_Z 0.1f
#endif 

#ifndef CLIP_FAR_Z
#define CLIP_FAR_Z 1000.f
#endif

#ifndef FOV_ANGLE
#define FOV_ANGLE 45.f
#endif;


#ifndef DXGI_RGB_DEFAULT_FORMAT
#define DXGI_RGB_DEFAULT_FORMAT DXGI_FORMAT_R8G8B8A8_UNORM
#endif

#ifndef DXGI_RGB_DEFAULT_FORMAT_SRGB
#define DXGI_RGB_DEFAULT_FORMAT_SRGB DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
#endif

//汎用性の高いブラシを格納
enum GeneralBrushList {
	SOLID_LIGHT_GREEN,
	SOLID_BLACK,
	SOLID_GRAY,
	GENERAL_BRUSH_COUNT
};


#ifndef WM_GAMESTART
#define WM_GAMESTART (WM_APP + 1)
#endif

#ifndef WM_GAMEEND
#define WM_GAMEEND (WM_APP + 2)
#endif

#ifndef WM_RETURNTOMAINMENU
#define WM_RETURNTOMAINMENU (WM_APP + 3)
#endif


#ifndef BMP_NAME_RETURN_TO_MAIN_MENU
#define BMP_NAME_RETURN_TO_MAIN_MENU L"returnToMainMenu"
#endif


#ifndef BMP_NAME_EXIT_APPLICATION
#define BMP_NAME_EXIT_APPLICATION L"exitApplication"
#endif