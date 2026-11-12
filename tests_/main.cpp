//
// Created by Lukas on 11 Nov. 2026.
//
#include <dxfw11Mini.hpp>


static void render_menu()
{
	ImGui::Begin("Test");
	ImGui::End();
}

int main()
{
	dxfw::CRender::Get().SetMenu_RenderFunction(render_menu);

	if (!dxfw::CRender::Get().InitWindow({ 1920, 1080 }, {}, "TestWindow", 144, dxfw::EWindowType::OVERLAY))
		return -1;

	std::array clearBack { 0.0F, 0.0F, 0.0F, 0.0F };

	while (!dxfw::CRender::Get().WindowShouldClose())
	{
		dxfw::CRender::Get().ToggleClickThrough(VK_INSERT, true);

		dxfw::CRender::Get().BeginFrame(clearBack);

		dxfw::CRender::Get().GetDrawList(dxfw::EDrawListType::BACK)->AddRect({100.f, 100.f}, {200.f, 200.f}, ImGui::ColorConvertFloat4ToU32({1.f, 0.f, 0.f, 1.f}));

		dxfw::CRender::Get().EndFrame(false);
	}

	return 0;
}