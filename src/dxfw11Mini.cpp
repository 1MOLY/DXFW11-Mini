//
// Created by Lukas on 11 Nov. 2026.
//
#include "dxfw11Mini.hpp"

#include <iostream>


namespace dxfw
{

#pragma region CRender

	LRESULT CALLBACK CRender::DXFW_WndProc(HWND__ * const hWnd, const UINT msg, const WPARAM wparam, const LPARAM lparam)
	{
		switch (msg)
		{
			case WM_DESTROY:
			{
				m_window.ShouldClose.store(true);
				PostQuitMessage(0);
				return 0;
			}

			case WM_SIZE:
			{

			}

			default:
				break;
		}
		return DefWindowProcA(hWnd, msg, wparam, lparam);
	}

	LRESULT CALLBACK CRender::WndProc(HWND__ * const hWnd, const UINT msg, const WPARAM wparam, const LPARAM lparam)
	{
		CRender* self{};

		if (msg == WM_NCCREATE)
		{
			const auto create =
				reinterpret_cast<CREATESTRUCTA*>(lparam);

			self = static_cast<CRender*>(create->lpCreateParams);

			SetWindowLongPtrA(
				hWnd,
				GWLP_USERDATA,
				reinterpret_cast<LONG_PTR>(self)
			);
		}
		else
		{
			self = reinterpret_cast<CRender*>(
				GetWindowLongPtrA(hWnd, GWLP_USERDATA)
			);
		}

		if (self)
			return self->DXFW_WndProc(hWnd, msg, wparam, lparam);

		return DefWindowProcA(hWnd, msg, wparam, lparam);
	}

	bool DXFW_API CRender::InitWindow(const std::array<int32_t, 2> Size,
							 const std::array<int32_t, 2> Position,
							 const std::string &Title,
							 const int32_t TargetFPS,
							 const EWindowType WindowType)
	{
		m_window.Size		= Size;
		m_window.Position	= Position;
		m_window.Title		= Title;
		m_window.TargetFPS  = TargetFPS;
		m_window.WindowType = WindowType;
		m_window.ClassName  = util::String::GenerateClassName(m_window.Title);

		if (!CreateAppWindow(m_window))
		{
			return false;
		}

		if (!InitContext(m_window))
		{
			return false;
		}

		ShowWindow(m_window.HWnd, SW_SHOW);
		UpdateWindow(m_window.HWnd);

		m_bInitialized = true;

		return true;
	}

	bool DXFW_API CRender::Terminate()
	{
		bool exit{ true };

		if (m_window.HWnd) { exit = DestroyWindow(m_window.HWnd); }
		m_window.HWnd = nullptr;
		m_window.Exists.store(false);

		m_window.ClassName.clear();
		m_window.Title.clear();
		m_window.Position	= {};
		m_window.Size		= {};
		m_window.TargetFPS	= 0;
		m_bWindowExists	 	= false;
		m_bContextExists 	= false;
		m_bInitialized	 	= false;

		return exit != 0;
	}

	bool DXFW_API CRender::InitContext(TWindow& window)
	{
		if (!m_bInitialized || !m_bWindowExists || m_bContextExists)
			return false;

		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferCount = 2;
		scd.BufferDesc.Width = window.Size.at(0);
		scd.BufferDesc.Height = window.Size.at(1);
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = window.TargetFPS;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = window.HWnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#ifdef _DEBUG
			flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif
		std::cout << "D3D11 Flags: " << flags << '\n';


		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			nullptr, 0,
			D3D11_SDK_VERSION,
			&scd,
			m_swapChain.GetAddressOf(),
			m_device.GetAddressOf(),
			nullptr,
			m_context.GetAddressOf()
		);
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}


		ID3D11Texture2D* backBuffer{};
		hr = m_swapChain->GetBuffer(
			0,
			IID_PPV_ARGS(&backBuffer)
		);
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}

		hr = m_device->CreateRenderTargetView(backBuffer, nullptr, m_targetView.GetAddressOf());
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}

		ID3D11RenderTargetView* rtv = m_targetView.Get();
		m_context->OMSetRenderTargets(1, &rtv, nullptr);

		D3D11_VIEWPORT vp{};
		vp.Width  = static_cast<FLOAT>(window.Size.at(0));
		vp.Height = static_cast<FLOAT>(window.Size.at(1));
		vp.MinDepth = 0.F;
		vp.MaxDepth = 1.F;

		m_context->RSSetViewports(1, &vp);

		window.Size.at(0) = static_cast<int32_t>(vp.Width);
		window.Size.at(1) = static_cast<int32_t>(vp.Height);

		m_bContextExists = true;

		return true;
	}

	bool DXFW_API CRender::CreateAppWindow(TWindow& window)
	{
		void* hInstance = GetModuleHandleA(nullptr);

		WNDCLASSEXA wc{};
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.lpfnWndProc = WndProc;
		wc.lpszClassName = window.ClassName.c_str();
		wc.hInstance = static_cast<HMODULE>(hInstance);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

		if (!RegisterClassExA(&wc))
		{
			std::cerr << "Failed to register class!\n";
			return false;
		}

		if (!SetStyle(window))
		{
			std::cerr << "Failed to set window style!\n";
			return false;
		}

		RECT r = {
			.left = 0,
			.top = 0,
			.right = window.Size.at(0),
			.bottom = window.Size.at(1)
		};
		AdjustWindowRectEx(&r, window.Style, FALSE, 0);

		const std::array WindowSize {
			static_cast<int32_t>(r.right - r.left),
			static_cast<int32_t>(r.bottom - r.top)
		};

		window.HWnd = CreateWindowExA(
			window.ExStyle,
			window.ClassName.c_str(),
			window.Title.c_str(),
			window.Style,
			window.Position.at(0),
			window.Position.at(1),
			WindowSize.at(0),
			WindowSize.at(1),
			nullptr,
			nullptr,
			static_cast<HMODULE>(hInstance),
			this
		);
		if (!window.HWnd)
		{
			std::cerr << "Failed to create window!\n";
			return false;
		}
		std::cout << "HWND = 0x" << std::hex << std::uppercase << window.HWnd << std::dec << '\n';

		m_bWindowExists = true;
		window.Exists.store(true);

		return true;
	}

	bool DXFW_API CRender::WindowShouldClose() noexcept
	{
		if (!m_bInitialized || !m_bWindowExists || !m_bContextExists)
			return false;

		MSG msg{};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT)
			{
				m_window.ShouldClose.store(true);
				return true;
			}
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return m_window.ShouldClose == 0;
	}

	void DXFW_API CRender::BeginFrame(const std::array<FLOAT, 4>& clearColor)
	{
		if (!m_bInitialized || !m_bWindowExists || !m_bContextExists)
			return;

		ID3D11RenderTargetView* rtv = m_targetView.Get();

		m_context->OMSetRenderTargets(
			1,
			&rtv,
			nullptr
		);

		const FLOAT cc[4] = {
			clearColor.at(0),
			clearColor.at(1),
			clearColor.at(2),
			clearColor.at(3)
		};

		m_context->ClearRenderTargetView(
			m_targetView.Get(),
			cc
		);
	}

	void DXFW_API CRender::EndFrame(const bool vSync)
	{
		if (!m_bInitialized || !m_bWindowExists || !m_bContextExists)
			return;

		// ReSharper disable once CppTooWideScopeInitStatement
		const HRESULT hr = m_swapChain->Present(vSync ? 1 : 0, 0);

		if (FAILED(hr))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				std::cerr << "DXGI_ERROR_DEVICE_REMOVED\n";
			}
			_hr_failed_msg(hr);
		}
	}

	bool DXFW_API CRender::OnWindowResize(TWindow& window)
	{
		if (!m_bInitialized || !m_bWindowExists || !m_bContextExists)
			return false;

		m_context->OMSetRenderTargets(
			0,
			nullptr,
			nullptr
		);

		if (m_targetView)
		{
			m_targetView.Reset();
		}

		HRESULT hr = m_swapChain->ResizeBuffers(
			0,
			window.Size.at(0),
			window.Size.at(1),
			DXGI_FORMAT_UNKNOWN,
			0
		);
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}

		ID3D11Texture2D* backBuffer{};
		hr = m_swapChain->GetBuffer(
			0,
			IID_PPV_ARGS(&backBuffer)
		);
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}

		hr = m_device->CreateRenderTargetView(
			backBuffer,
			nullptr,
			m_targetView.GetAddressOf()
		);
		if (FAILED(hr))
		{
			_hr_failed_msg(hr);
			return false;
		}


		D3D11_VIEWPORT vp{};
		vp.Width  = static_cast<FLOAT>(window.Size.at(0));
		vp.Height = static_cast<FLOAT>(window.Size.at(1));
		vp.MinDepth = 0.F;
		vp.MaxDepth = 1.F;

		m_context->RSSetViewports(
			1,
			&vp
		);

		// Rebind the new target and update the drawing viewport after resizing.
		ID3D11RenderTargetView* target = m_targetView.Get();
		m_context->OMSetRenderTargets(1, &target, nullptr);

		window.Size.at(0) = static_cast<int32_t>(vp.Width);
		window.Size.at(1) = static_cast<int32_t>(vp.Height);

		return true;
	}

	bool DXFW_API CRender::SetStyle(TWindow& window)
	{
		window.Style = 0;
		window.ExStyle = 0;

		switch (window.WindowType)
		{
			case NORMAL:
			{
				window.Style = WS_OVERLAPPEDWINDOW;
				window.ExStyle = 0;
			}

			case OVERLAY:
			{
				window.Style = WS_POPUP;
				window.ExStyle = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT;
			}

			case ME_NEED_NAKED:
			{
				window.Style = WS_POPUP;
				window.ExStyle = 0;
			}

			default:
				break;
		}
		return window.Style != 0 && window.ExStyle != 0;
	}

#pragma endregion

#pragma region CDraw

	namespace _internal
	{

	}

#pragma endregion

}