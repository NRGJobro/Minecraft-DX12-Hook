#include "D2DShitController.hpp"




//d2dを使用するための準備
D2DController::D2DController() :m_FrameIndex(0)
{}


HRESULT D2DController::CreateD2DOnDX12(
	HWND hWnd,
	ID3D12Device* d3d12Dev, D3D11_CREATE_DEVICE_FLAG flag,
	ID3D12CommandQueue** cmdQueueArr, int countOfCmdQueueArr,
	D2D1_DEBUG_LEVEL debugLevel
)
{
	HRESULT hr;
	WCHAR err[128];
	ComPtr<ID3D11Device> d3d11Device;

	hr = D3D11On12CreateDevice(
		d3d12Dev, flag, nullptr, 0, (IUnknown**)cmdQueueArr, countOfCmdQueueArr, 0,
		&d3d11Device, &m_d3d11DevCon, nullptr
	);
	if (FAILED(hr)) {
		wsprintf(err, L"D3D11On12CreateDevice 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);
		return hr;
	}

	hr = d3d11Device.As(&m_d3d11on12Device);
	if (FAILED(hr)) {
		wsprintf(err, L"ComPtr<ID3D11Device>.As 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}


	D2D1_FACTORY_OPTIONS d2d1Opt = { debugLevel };
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2d1Opt, &m_Factory);
	if (FAILED(hr)) {
		wsprintf(err, L"D2D1CreateFactory 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	hr = m_d3d11on12Device.As(&dxgiDevice);
	if (FAILED(hr)) {
		wsprintf(err, L"ComPtr<ID3D11Device>.As(DXGI) 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	hr = m_Factory->CreateDevice(dxgiDevice.Get(), &m_D2DDevice);
	if (FAILED(hr)) {
		wsprintf(err, L"ID2D1Factory3::CreateDevice 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	hr = m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_D2DDevCon);
	if (FAILED(hr)) {
		wsprintf(err, L"ID2D1Device2::CreateDeviceContext 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_WriteFactory);
	if (FAILED(hr)) {
		wsprintf(err, L"DWriteCreateFactory 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	hr = DCompositionCreateDevice(dxgiDevice.Get(), IID_PPV_ARGS(&m_CompositionDev));
	if (FAILED(hr)) {
		wsprintf(err, L"DCompositionCreateDevice 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}

	hr = m_CompositionDev->CreateTargetForHwnd(hWnd, TRUE, &m_CompositionTarget);
	if (FAILED(hr)) {
		wsprintf(err, L"IDCompositionDevice::CreateTargetForHwnd 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}


	hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_IWICFactory));
	if (FAILED(hr)) {
		wsprintf(err, L"CoCreateInstance 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);

		Release();
		return hr;
	}



	return S_OK;
}


//d2d用のレンダーターゲットを作成する
HRESULT D2DController::CreateRenderTarget(ID3D12Resource** rtTarget, HWND hWnd)
{
	HRESULT hr;
	WCHAR err[128];

	if (!m_d3d11on12Device.Get()) return E_INVALIDARG;

	UINT dpi = GetDpiForWindow(hWnd);
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpi, dpi
	);


	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };

	// Create a RTV, D2D render target
	for (int i = 0; i < BUFFERCOUNT; i++) {

		//InStateはレンダーターゲットに描画するときの、リソースのSTATE
		//OutStateはReleaseWrappedResourcesが呼ばれた後の、リソースのSTATE
		//メニュー画面でD2D単体のみを使用することがあるため、
		//InStateはD3D12_RESOURCE_STATE_PRESENTにしておく
		hr = m_d3d11on12Device->CreateWrappedResource(
			rtTarget[i],
			&d3d11Flags,
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_PRESENT,
			IID_PPV_ARGS(&m_RenderTargetRes[i])
		);
		if (FAILED(hr)) {
			wsprintf(err, L"CreateWrappedResource 0x%x", hr);
			MessageBox(NULL, err, L"ERROR", MB_OK);
			return hr;
		}

		//Create a render target for D2D to draw directly to this back buffer.
		ComPtr<IDXGISurface> surface;
		m_RenderTargetRes[i].As(&surface);
		hr = m_D2DDevCon->CreateBitmapFromDxgiSurface(
			surface.Get(),
			&bitmapProperties,
			&m_RenderTargetBmp[i]
		);
		if (FAILED(hr)) {
			wsprintf(err, L"CreateBitmapFromDxgiSurface 0x%x", hr);
			MessageBox(NULL, err, L"ERROR", MB_OK);
			return hr;
		}

	}


	//render target store用のbitmap
	hr = m_D2DDevCon->CreateBitmap(
		m_RenderTargetBmp[0]->GetPixelSize(),
		D2D1::BitmapProperties(m_RenderTargetBmp[0]->GetPixelFormat(), dpi, dpi),
		&m_RTBmpForInput
	);
	if (FAILED(hr)) {
		wsprintf(err, L"CreateBitmap for input 0x%x", hr);
		MessageBox(NULL, err, L"ERROR", MB_OK);
		return hr;
	}


	return S_OK;
}

void D2DController::SetFrameIndex(int frameIndex)
{
	m_FrameIndex = frameIndex;
}

int D2DController::GetFrameIndex()
{
	return m_FrameIndex;
}


IDWriteFactory* D2DController::GetWriteFactory()
{
	return m_WriteFactory.Get();
}


void D2DController::AcquireD2DResource()
{
	m_d3d11on12Device->AcquireWrappedResources(m_RenderTargetRes[m_FrameIndex].GetAddressOf(), 1);
}

//ReleaseWrappedResourcesを呼ぶと
//CreateWrappedResourceでしたOutStateにtransitionする
void D2DController::ReleaseD2DResource()
{
	m_d3d11on12Device->ReleaseWrappedResources(m_RenderTargetRes[m_FrameIndex].GetAddressOf(), 1);
}


ID2D1Bitmap1* D2DController::GetRenderTarget()
{
	return m_RenderTargetBmp[m_FrameIndex].Get();
}

ID2D1Bitmap* D2DController::GetRTBmpForInput()
{
	return m_RTBmpForInput.Get();
}

ID2D1DeviceContext2* D2DController::GetD2DDeviceContext()
{
	return m_D2DDevCon.Get();
}

ID3D11DeviceContext* D2DController::GetD3D11DeviceContext()
{
	return m_d3d11DevCon.Get();
}

IWICImagingFactory* D2DController::GetIWICImagingFactory()
{
	return m_IWICFactory.Get();
}

ID2D1Factory3* D2DController::GetD2DFactory()
{
	return m_Factory.Get();
}


void D2DController::Release()
{

	for (int i = 0; i < BUFFERCOUNT; i++) {
		m_RenderTargetRes[i].Reset();
		m_RenderTargetBmp[i].Reset();
	}

	m_RTBmpForInput.Reset();

	m_CompositionTarget.Reset();
	m_CompositionDev.Reset();

	m_D2DDevCon.Reset();
	m_d3d11DevCon.Reset();
	m_WriteFactory.Reset();
	m_d3d11on12Device.Reset();
	m_D2DDevice.Reset();
	m_Factory.Reset();

	m_IWICFactory.Reset();
	CoUninitialize();

	m_FrameIndex = 0;
}


D2DController::~D2DController()
{
	Release();
}