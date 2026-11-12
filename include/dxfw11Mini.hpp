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
#include <functional>

#include "core/_util.hpp"
#include "core/_tex.hpp"


#ifndef _WIN64
	#error System not Windows x64
#endif

// -+-+-+-+-+-+-+- API Flags -+-+-+-+-+-+-+-
// Should be defined if needed

#if defined(DXFW_IMGUI)

	#define NEED_IMGUI 1

	#include <imgui.h>
	#include <imgui_impl_dx11.h>
	#include <imgui_impl_win32.h>

#else

	#define NEED_IMGUI 0

#endif

// -+-+-+-+-+-+-+- used namespaces -+-+-+-+-+-+-+-

// Public DXFW11 functions, classes, structs, consts, pointers, unions, vectors, ints, chars, unsigned ints, strings, bools, arrays, unsigned chars, constexpr s, half of this, dxfw doesn't even contain but idek
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

	#if NEED_IMGUI
		#pragma pack(push, 8)

		struct TMenu_
		{
			std::function<void()> Init{};
			std::function<void()> Render{};
		};

		#pragma pack(pop)
	#endif

#pragma region CDraw

	//! @brief Internal Drawing Functions. Use class @b CRender to draw!
	class CDraw {

	private:

		void DrawTriangle_(tex::displc pA, tex::displc pB, tex::displc pC, tex::color color, float thickness = 1.f);
		void DrawRect_(tex::displc position, tex::displc size, tex::color color, float thickness = 1.f);
		void DrawCircle_(tex::displc middlePos, float radius, tex::color color, float thickness = 1.f, uint16_t smooth = 32);

		void DrawTriangleFilled_(tex::displc pA, tex::displc pB, tex::displc pC, tex::color color);
		void DrawRectFilled_(tex::displc position, tex::displc size, tex::color color);
		void DrawCircleFilled_(tex::displc middlePos, float radius, tex::color color, uint16_t smooth = 32);

		void DrawText_(const std::string& text, tex::displc position, tex::color color, uint16_t fontSize = 16);
		void DrawLine_(tex::displc pA, tex::displc pB, tex::color color, float thickness = 1.f);

	protected:

		CDraw() = default;
		virtual ~CDraw() = default;


#pragma region DrawList Declaration

		struct DrawList
		{
			explicit DrawList(CDraw* owner) : owner(owner){}

			template<bool FILL_>
			void DrawTriangle(const tex::displc pA,
							  const tex::displc pB,
							  const tex::displc pC,
							  const tex::color color,
							  const float thickness = 1.f) const
			{
				if constexpr (FILL_)	{ (owner->*DrawTriangleFilled_)(pA, pB, pC, color);		 }
				else					{ (owner->*DrawTriangle_)(pA, pB, pC, color, thickness); }
			}

			template<bool FILL_>
			void DrawRect(const tex::displc position,
						  const tex::displc size,
						  const tex::color color,
						  const float thickness = 1.f) const
			{
				if constexpr (FILL_)  { (owner->*DrawRectFilled_)(position, size, color);		 }
				else				  { (owner->*DrawRect_)(position, size, color, thickness);	 }
			}

			template<bool FILL_>
			void DrawCircle(const tex::displc middlePos,
							const float radius,
							const tex::color color,
							const float thickness = 1.f,
							const uint16_t smooth = 32) const
			{
				if constexpr (FILL_)	{ (owner->*DrawCircleFilled_)(middlePos, radius, color, smooth);		}
				else					{ (owner->*DrawCircle_)(middlePos, radius, color, thickness, smooth);	}
			}

			//! @brief I know wierd name... but Microslop already defined a DrawText function :I
			void DrawTextt(const std::string& text, tex::displc position, tex::color color, uint16_t fontSize = 16);
			void DrawLine(tex::displc pA, tex::displc pB, tex::color color, float thickness = 1.f);

		protected:

			CDraw* owner;

			void* Buffer{};
			size_t Size{};
			uint32_t Count{};

		private:

			using pDrawTriangle		= void(CDraw::*)(tex::displc, tex::displc, tex::displc, tex::color, float);
			using pDrawRect			= void(CDraw::*)(tex::displc, tex::displc, tex::color, float);
			using pDrawCircle		= void(CDraw::*)(tex::displc, float, tex::color, float, uint16_t);

			using pTriangleFilled	= void(CDraw::*)(tex::displc, tex::displc, tex::displc, tex::color);
			using pDrawRectFilled	= void(CDraw::*)(tex::displc, tex::displc, tex::color);
			using pDrawCircleFilled = void(CDraw::*)(tex::displc, float, tex::color, uint16_t);

			using pDrawText = void(CDraw::*)(const std::string&, tex::displc, tex::color, uint16_t);
			using pDrawLine = void(CDraw::*)(tex::displc, tex::displc, tex::color, float);

			pDrawTriangle DrawTriangle_	= &CDraw::DrawTriangle_;
			pDrawRect	  DrawRect_		= &CDraw::DrawRect_;
			pDrawCircle	  DrawCircle_	= &CDraw::DrawCircle_;

			pTriangleFilled	  DrawTriangleFilled_ = &CDraw::DrawTriangleFilled_;
			pDrawRectFilled	  DrawRectFilled_	  = &CDraw::DrawRectFilled_;
			pDrawCircleFilled DrawCircleFilled_	  = &CDraw::DrawCircleFilled_;

			pDrawText DrawText_ = &CDraw::DrawText_;
			pDrawLine DrawLine_ = &CDraw::DrawLine_;
		};

#pragma endregion

		std::unique_ptr<DrawList> DrawList_{};

	public:

		[[nodiscard]]
		DrawList* GetDrawList() noexcept
		{
			if (!DrawList_)
				DrawList_ = std::make_unique<DrawList>(this);

			return DrawList_.get();
		}
	};

#pragma endregion

}

namespace dxfw
{

#pragma region CRender

	class CRender : public _internal::CDraw
	{

	protected:

		enum class EWindowType
		{
			NORMAL,
			OVERLAY,
			ME_NEED_NAKED
		};

		#pragma pack(push, 8)

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

		#pragma pack(pop)

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

		#if NEED_IMGUI

			//! @brief Sets the ImGUI Menu @b init function. @b Optional!
			//! @attention Needs to be set @b before @b InitWindow!
			template<typename F_>
			void DXFW_API SetMenu_InitFunction(F_&& fun)
			{
				m_menu.Init = std::forward<void()>(fun);
			}

			//! @brief Sets the ImGUI Menu @b rendering function. @b Required!
			//! @attention Needs to be set @b before @b InitWindow!
			template<typename F_>
			void DXFW_API SetMenu_RenderFunction(F_&& fun)
			{
				m_menu.Render = std::forward<void()>(fun);
			}

			//! @brief Use this for @b ImGUI Menu toggling with @b WindowType set to @b Overlay. @n
			//! Toggles Click Through
			void DXFW_API ToggleWindow(TWindow& Window);

		#endif

	private:

		CRender() = default;
		~CRender() override { Terminate(); }

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

		#if NEED_IMGUI
			bool m_showMenu{ false };
			_internal::TMenu_ m_menu{};
		#endif

		TWindow m_window{};

		_internal::comptr<ID3D11Device> m_device{};
		_internal::comptr<ID3D11DeviceContext> m_context{};
		_internal::comptr<ID3D11RenderTargetView> m_targetView{};
		_internal::comptr<IDXGISwapChain> m_swapChain{};
	};

#pragma endregion

}