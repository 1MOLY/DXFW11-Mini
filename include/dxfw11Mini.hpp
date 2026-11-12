//
// Created by Lukas on 11 Nov. 2026.
//
//     _______   __    __  ________  __       __    __      __                         ______   _______  ______
//    |       \ |  \  |  \|        \|  \  _  |  \ _/  \   _/  \                       /      \ |       \|      \
//    | $$$$$$$\| $$  | $$| $$$$$$$$| $$ / \ | $$|   $$  |   $$                      |  $$$$$$\| $$$$$$$\\$$$$$$
//    | $$  | $$ \$$\/  $$| $$__    | $$/  $\| $$ \$$$$   \$$$$         ______       | $$__| $$| $$__/ $$ | $$
//    | $$  | $$  >$$  $$ | $$  \   | $$  $$$\ $$  | $$    | $$        |      \      | $$    $$| $$    $$ | $$
//    | $$  | $$ /  $$$$\ | $$$$$   | $$ $$\$$\$$  | $$    | $$         \$$$$$$      | $$$$$$$$| $$$$$$$  | $$
//    | $$__/ $$|  $$ \$$\| $$      | $$$$  \$$$$ _| $$_  _| $$_                     | $$  | $$| $$      _| $$_
//    | $$    $$| $$  | $$| $$      | $$$    \$$$|   $$ \|   $$ \                    | $$  | $$| $$     |   $$ \
//     \$$$$$$$  \$$   \$$ \$$       \$$      \$$ \$$$$$$ \$$$$$$                     \$$   \$$ \$$      \$$$$$$
//
// ... mini :P... size doesn't matter btw
//
#pragma once
#include "core/_util.hpp"
#include "core/_tex.hpp"


#ifndef _WIN64
	#error System not Windows x64
#endif

// -+-+-+-+-+-+-+- Dependencies -+-+-+-+-+-+-+-


#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


// -+-+-+-+-+-+-+- used namespaces -+-+-+-+-+-+-+-


//! @brief Public DXFW11 functions, classes, structs, consts, pointers, unions, vectors, ints, chars, unsigned ints, strings, bools, arrays, unsigned chars, constexpr s... @n
//! Half of this, dxfw doesn't even contain but idek
namespace dxfw{}

// DXFW11 Util things
namespace dxfw::util{}

//! DXFW11 @b internal stuff. @n
//! If it's @b internal, u probably @b shouldn't use it
namespace dxfw::_internal{}


// -+-+-+-+-+-+-+- Macros -+-+-+-+-+-+-+-


//! @brief Base public API Functions
#define DXFW_API __cdecl

//! @brief Base internal API Functions
//! @attention Meant for internal use only!
#define INTERNAL_API __cdecl

//! @brief Macro for a simple test function declaration with void as return type
#define TEST_FUNCTION inline void test()


// -+-+-+-+-+-+-+- Functions -+-+-+-+-+-+-+-


//! @brief Checks if v is nullptr
//! @returns bool
#define _is_valid(v) (bool)(((void*)(v) != nullptr) > 0)

#define _hr_failed_msg(hr) std::cerr << "HRESULT failed! Value = " << hr << '\n'


// -+-+-+-+-+-+-+- Debug Build Specific -+-+-+-+-+-+-+-

#ifdef _DEBUG

	#define BUILD_DEBUG 1

	// Prints Trace Points if defined
	#ifdef DXFW_TRACING

		#define LOG_POINT \
			printf_s("%s | %s @ %d\n", __FILE__, __FUNCSIG__, __LINE__)

	#else

		#define LOG_POINT

	#endif

#else

	#define BUILD_DEBUG 0
	#define LOG_POINT

#endif


// -+-+-+-+-+-+-+- Core Library -+-+-+-+-+-+-+-


namespace dxfw::_internal
{
	using namespace Microsoft::WRL;

	template<typename T>
	using comptr = ComPtr<T>;

	enum class EGetType_ {
		RENDERER,
		WINDOW
	};

	#pragma pack(push, 8)

	struct TMenu_
	{
		std::function<void()> Init{};
		std::function<void()> Render{};
	};

	#pragma pack(pop)
}

namespace dxfw
{

	// Get | From | Renderer
	enum class E_GFR
	{
		DEVICE,
		CONTEXT,
		SWAP_CHAIN,
		RTV
	};

	// Get | From | Window
	enum class E_GFW
	{
		CLASS_NAME,
		TITLE,
		SIZE,
		POSITION,
		TARGET_FPS,
		HWND,
		STYLE,
		EX_STYLE,
		WINDOW_TYPE
	};

	enum class EDrawListType
	{
		BACK,
		FORE,
		WINDOW
	};

	enum class EWindowType
	{
		NORMAL,
		OVERLAY,
		ME_NEED_NAKED
	};

#pragma region CRender

	//! @brief Core Class for rendering. @n
	//! Implemented ImGui should be used for drawing because i was to lazy.
	class CRender
	{

	protected:

		struct TWindow
		{
			std::atomic<bool> Exists{ false };
			std::atomic<bool> ShouldClose{ false };
			std::atomic<bool> Fullscreen{ false };

			std::string ClassName{};
			std::string Title{};
			std::array<int32_t, 2> Size{};
			std::array<int32_t, 2> Position{};
			uint32_t TargetFPS{ 60 };

			HWND HWnd{};

			DWORD Style{ 0x0 };
			DWORD ExStyle{ 0x0 };

			EWindowType WindowType{ EWindowType::NORMAL };
		};

	public:

		static CRender& Get() {
			static CRender instance;
			return instance;
		}
		CRender(const CRender&)            = delete;
		CRender& operator=(const CRender&) = delete;

		//! @brief Initializes a D3D11 Window
		bool DXFW_API InitWindow(std::array<int32_t, 2> Size,
								 std::array<int32_t, 2> Position,
								 const std::string& Title,
								 uint32_t TargetFPS = 60,
								 EWindowType WindowType = EWindowType::NORMAL);

		//! @brief Why would you need a comment for this?
		bool DXFW_API Terminate();

		//! @brief "Window" - "Should" - "Close" @n Like should the window close in a while loop...
		bool DXFW_API WindowShouldClose();

		//! @brief @b Begins a @b frame. @n
		//! Use ts at the @b beginning of a @b WindowShouldClose while loop
		void DXFW_API BeginFrame(std::array<FLOAT, 4> &clearColor);

		//! @brief Unlike @b BeginFrame, this @b Ends a @b Frame. @n
		//! Use ts at the @b end of a @b WindowShouldClose while loop. @n
		//! @attention TargetFPS does nothing if @b vSync is true.
		void DXFW_API EndFrame(bool vSync);

		// -+-+-+-+-+-+-+- Get -+-+-+-+-+-+-+-

		template<typename T>
		T* DXFW_API GetFromRenderer(const E_GFR x) noexcept
		{
			switch (x)
			{
				case E_GFR::CONTEXT:
					return m_context;
				case E_GFR::DEVICE:
					return m_device;
				case E_GFR::RTV:
					return m_targetView;
				case E_GFR::SWAP_CHAIN:
					return m_swapChain;
				default:
					return nullptr;
			}
		}

		template<typename T>
		T DXFW_API GetFromWindow(const E_GFW x) noexcept
		{
			switch (x)
			{
				case E_GFW::CLASS_NAME:
					return m_window.ClassName;
				case E_GFW::TITLE:
					return m_window.Title;
				case E_GFW::POSITION:
					return m_window.Position;
				case E_GFW::SIZE:
					return m_window.Size;
				case E_GFW::TARGET_FPS:
					return m_window.TargetFPS;
				case E_GFW::STYLE:
					return m_window.Style;
				case E_GFW::EX_STYLE:
					return m_window.ExStyle;
				case E_GFW::HWND:
					return m_window.HWnd;
				case E_GFW::WINDOW_TYPE:
					return m_window.WindowType;
				default:
					return {};
			}
		}

		ImDrawList* GetDrawList(const EDrawListType dlType)
		{
			switch (dlType)
			{
				case EDrawListType::BACK:
					return ImGui::GetBackgroundDrawList();
				case EDrawListType::FORE:
					return ImGui::GetForegroundDrawList();
				case EDrawListType::WINDOW:
					return ImGui::GetWindowDrawList();
				default:
					return nullptr;
			}
		}

		//! @brief Sets the ImGUI Menu @b init function. @b Optional!
		//! @attention Needs to be set @b before @b InitWindow!
		template<typename F_>
		void DXFW_API SetMenu_InitFunction(F_&& fun)
		{
			if (m_bInitialized || m_bWindowExists || m_bContextExists)
			{
				throw std::exception("Do not set the menu init function after InitWindow!");
			}

			m_menu.Init = std::forward<F_>(fun);
			m_menuInitFuncSet = true;
		}

		//! @brief Sets the ImGUI Menu @b rendering function. @b Required!
		//! @attention Needs to be set @b before @b InitWindow!
		template<typename F_>
		void DXFW_API SetMenu_RenderFunction(F_&& fun)
		{
			if (m_bInitialized || m_bWindowExists || m_bContextExists)
			{
				throw std::exception("Do not set the menu rendering function after InitWindow!");
			}

			m_menu.Render = std::forward<F_>(fun);
			m_menuRenderFuncSet = true;
		}

		//! @brief @b ImGUI Menu toggling with @b WindowType set to @b Overlay. @n
		//! Toggles Click Through
		//! @attention Call this @b before @b BeginFrame
		void DXFW_API ToggleClickThrough(int vKey, bool togglesMenuVis = false);

		//! @brief Get the current state of ShowMenu
		[[nodiscard]]
		bool GetShowMenu() const noexcept { return m_showMenu; };

	private:

		CRender() = default;
		~CRender() { Terminate(); }

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT CALLBACK DXFW_WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

		bool DXFW_API CreateAppWindow(TWindow& window);
		bool DXFW_API InitContext(TWindow& window);
		bool DXFW_API OnWindowResize(TWindow& window);

		static bool DXFW_API SetStyle(TWindow& window);
		static bool DXFW_API EnsureBackgroundColor(EWindowType type, std::array<FLOAT, 4>& color);

		bool m_bInitialized{ false };
		bool m_bContextExists{ false };
		bool m_bWindowExists{ false };
		bool m_bClassRegistered{ false };

		bool m_bImGuiCoreInit{ false };
		bool m_bImGuiWin32Init{ false };
		bool m_bImGuiDX11Init{ false };

		bool m_showMenu{ false };
		bool m_bToggleMenuVisible{ false };

		bool m_menuInitFuncSet{ false };
		bool m_menuRenderFuncSet{ false };

		_internal::TMenu_ m_menu{};

		TWindow m_window{};

		_internal::comptr<ID3D11Device> m_device{};
		_internal::comptr<ID3D11DeviceContext> m_context{};
		_internal::comptr<ID3D11RenderTargetView> m_targetView{};
		_internal::comptr<IDXGISwapChain> m_swapChain{};
	};

#pragma endregion

}