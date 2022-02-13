#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>

int main(int argc, char* argv[])
{
	// Create Factory.
	IDXGIFactory4* factory = nullptr;
	if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory))))
	{
		return EXIT_FAILURE;
	}

	// Pick Adapter.
	IDXGIAdapter1* adapter = nullptr;
	for (UINT adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}

		// We won't use this adapter, so release it
		adapter->Release();
	}

	// Create Device.
	ID3D12Device* device = nullptr;
	if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device))))
	{
		return EXIT_FAILURE;
	}

	// Clock stabilization.
	device->SetStablePowerState(TRUE);

	return EXIT_SUCCESS;
}
