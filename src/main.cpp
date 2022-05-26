#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <thread>
#include <chrono>

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	wchar_t* requestedAdapterName = argc > 1 ? argv[1] : nullptr;

	// Create Factory.
	Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
	if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory))))
	{
		fprintf_s(stderr, "IDXGIFactory4 creation FAILED.\n");

		std::this_thread::sleep_for(std::chrono::minutes(1));
		return EXIT_FAILURE;
	}

	// Pick Adapter.
	bool adapterFound = false;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	DXGI_ADAPTER_DESC1 adapterDescriptor;
	for (UINT adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		adapter->GetDesc1(&adapterDescriptor);

		// Skip software adapters.
		if ((adapterDescriptor.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
		{
			continue;
		}

		// Skip an adapter if it's not requested.
		if (requestedAdapterName != nullptr && wcscmp(requestedAdapterName, adapterDescriptor.Description) != 0)
		{
			continue;
		}

		// Adequate adapter found.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			adapterFound = true;
			break;
		}
	}

	if (adapterFound)
	{
		fwprintf_s(stdout, L"Adapter \"%ls\" successfully FOUND.\n", adapterDescriptor.Description);
	}
	else if (requestedAdapterName != nullptr)
	{
		fwprintf_s(stderr, L"Required adapter \"%ls\" NOT FOUND.\n", requestedAdapterName);

		std::this_thread::sleep_for(std::chrono::minutes(1));
		return EXIT_FAILURE;
	}
	else
	{
		fwprintf_s(stderr, L"NOT a single adapter was FOUND.\n");

		std::this_thread::sleep_for(std::chrono::minutes(1));
		return EXIT_FAILURE;
	}

	// Create Device.
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
	{
		fwprintf_s(stderr, L"ID3D12Device creation FAILED for \"%ls\".\n", adapterDescriptor.Description);

		std::this_thread::sleep_for(std::chrono::minutes(1));
		return EXIT_FAILURE;
	}

	// Stabilize power state.
	if (FAILED(device->SetStablePowerState(TRUE)))
	{
		fwprintf_s(stderr, L"SetStablePowerState(TRUE) FAILED for \"%ls\" adapter.\n", adapterDescriptor.Description);

		std::this_thread::sleep_for(std::chrono::minutes(1));
		return EXIT_FAILURE;
	}

	fwprintf_s(stdout, L"SetStablePowerState(TRUE) SUCCEEDED for \"%ls\" adapter.\n", adapterDescriptor.Description);
	fprintf_s(stdout, "Clock will remain stable while the application is running.\n");

	// Turning off the application reverts the clock.
	std::this_thread::sleep_for(std::chrono::hours(1));
	return EXIT_SUCCESS;
}
