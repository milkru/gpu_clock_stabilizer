#include <iostream>
#include <d3d12.h>
#include <dxgi1_6.h>

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}

int main(int argc, char* argv[])
{
	// Create Factory.
	UINT dxgiFactoryFlags = 0;
	IDXGIFactory4* mFactory = nullptr;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));

	// Pick Adapter.
	IDXGIAdapter1* mAdapter = nullptr;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != mFactory->EnumAdapters1(adapterIndex, &mAdapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		mAdapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create
		// the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}

		// We won't use this adapter, so release it
		mAdapter->Release();
	}

	// Create Device.
	ID3D12Device* mDevice = nullptr;
	ThrowIfFailed(D3D12CreateDevice(mAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDevice)));

	// Stabilize.
	//mDevice->SetStablePowerState(TRUE);
}
