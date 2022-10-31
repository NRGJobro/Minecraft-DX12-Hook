#pragma once

#include <d2d1.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <vector>
#include <wincodec.h>
#include <dcomp.h>
#include "d3dx12Shiz.h"
#include "BaseShit.hpp"


using namespace Microsoft::WRL;
using namespace std;

//tips
//もしこの先DirectX12以外のクラス(Dx11など)を作るときは、
//絶対に生のポインタを使わず、ComPtrを使った方がいい
//生のポインタを使うと、システムが勝手にリソースを作ったり、
//して最終的にメモリリークとか起こすため


//使用方法
//CreateDeviceを呼ぶ
// CreateRenderTargetを呼ぶ
//

//directx12上でdirect2dを使用する

//https://docs.microsoft.com/ja-jp/windows/win32/direct2d/getting-started-with-direct2d
class D2DController {
private:
	//dx11
	ComPtr<ID3D11On12Device> m_d3d11on12Device;
	ComPtr<ID3D11DeviceContext> m_d3d11DevCon;
	ComPtr<ID3D11Resource> m_RenderTargetRes[BUFFERCOUNT];


	//direct2d
	ComPtr<ID2D1Factory3> m_Factory;
	ComPtr<ID2D1Device2> m_D2DDevice;
	ComPtr<ID2D1DeviceContext2> m_D2DDevCon;
	ComPtr<ID2D1Bitmap1> m_RenderTargetBmp[BUFFERCOUNT];

	//render targetはID2D1EffectのSetInputに設定できないため、直接の
	//加工ができないそのため、下のbitmapにコピーしてそれを加工したやつを
	//render targetにdrawBitmapする
	ComPtr<ID2D1Bitmap> m_RTBmpForInput;

	//directWrite
	ComPtr<IDWriteFactory> m_WriteFactory;

	//directComposition
	ComPtr<IDCompositionDevice> m_CompositionDev;
	ComPtr<IDCompositionTarget> m_CompositionTarget;

	ComPtr<IWICImagingFactory> m_IWICFactory;

	int m_FrameIndex;

public:

	D2DController();

	HRESULT CreateD2DOnDX12(
		HWND,
		ID3D12Device*, D3D11_CREATE_DEVICE_FLAG, ID3D12CommandQueue**, int,
		D2D1_DEBUG_LEVEL
	);

	HRESULT CreateRenderTarget(ID3D12Resource** swapChain, HWND);

	void SetFrameIndex(int);
	int GetFrameIndex();

	IDWriteFactory* GetWriteFactory();

	void AcquireD2DResource();
	void ReleaseD2DResource();

	ID2D1Bitmap1* GetRenderTarget();
	ID2D1Bitmap* GetRTBmpForInput();

	ID2D1DeviceContext2* GetD2DDeviceContext();

	ID3D11DeviceContext* GetD3D11DeviceContext();

	IWICImagingFactory* GetIWICImagingFactory();



	ID2D1Factory3* GetD2DFactory();

	void Release();

	~D2DController();

};
