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
//
//
#pragma once
#include "core/_util.hpp"
#include "core/_tex.hpp"


#ifndef _WIN64
	#error System not Windows x64
#endif


// -+-+-+-+-+-+-+- used namespaces -+-+-+-+-+-+-+-


namespace dxfw{}
namespace dxfw::util{}
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
	#ifdef TRACING

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

namespace dxfw
{
	enum class DrawType	  { FILLED, OUTLINE };
	enum class DrawFigure { TRIANGLE, RECTANGLE, CIRCLE, LINE, TEXT };
}

namespace dxfw::_internal
{
	using namespace Microsoft::WRL;

	template<typename T>
	using comptr = ComPtr<T>;

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
		virtual ~CDraw();



#pragma region DrawList Declaration

		struct DrawList
		{
			void Draw(DrawType type, DrawFigure figure);

		protected:

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
			pDrawCircleFilled DrawCircleFilled	  = &CDraw::DrawCircleFilled_;

			pDrawText DrawText_ = &CDraw::DrawText_;
			pDrawLine DrawLine_ = &CDraw::DrawLine_;
		};

#pragma endregion

		std::unique_ptr<DrawList> DrawList_{};

	public:

		[[nodiscard]]
		DrawList* GetDrawList() const noexcept
		{
			return DrawList_.get();
		}
	};
}

namespace dxfw
{
	class CRender : public _internal::CDraw {

	protected:

		enum EWindowType
		{
			NORMAL,
			OVERLAY,
			ME_NEED_NAKED
		};

		struct TWindow
		{
			std::atomic<bool> Exists{ false };
			std::atomic<bool> ShouldClose{ false };
			std::atomic<bool> Fullscreen{ false };

			std::string ClassName{};
			std::string Title{};
			std::array<int32_t, 2> Size{};
			std::array<int32_t, 2> Position{};
			int32_t TargetFPS{ 60 };

			HWND HWnd{};

			DWORD Style{};
			DWORD ExStyle{};

			EWindowType WindowType{ NORMAL };
		};

	public:

		static CRender& Get() {
			static CRender instance;
			return instance;
		}
		CRender(const CRender&)            = delete;
		CRender& operator=(const CRender&) = delete;

		bool DXFW_API InitWindow(std::array<int32_t, 2> Size,
						std::array<int32_t, 2> Position,
						const std::string& Title,
						int32_t TargetFPS = 60,
						EWindowType WindowType = NORMAL);

		bool DXFW_API Terminate();

		bool DXFW_API WindowShouldClose() noexcept;
		void DXFW_API BeginFrame(const std::array<FLOAT, 4>& clearColor);
		void DXFW_API EndFrame(bool vSync);

	private:

		CRender() = default;
		~CRender() override { Terminate(); }

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT CALLBACK DXFW_WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

		bool DXFW_API CreateAppWindow(TWindow& window);
		bool DXFW_API InitContext(TWindow& window);
		bool DXFW_API OnWindowResize(TWindow& window);

		static bool DXFW_API SetStyle(TWindow& window);

		bool m_bInitialized{ false };
		bool m_bContextExists{ false };
		bool m_bWindowExists{ false };

		TWindow m_window{};

		_internal::comptr<ID3D11Device> m_device{};
		_internal::comptr<ID3D11DeviceContext> m_context{};
		_internal::comptr<ID3D11RenderTargetView> m_targetView{};
		_internal::comptr<IDXGISwapChain> m_swapChain{};
	};
}