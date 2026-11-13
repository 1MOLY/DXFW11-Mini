//
// Created by Lukas on 11 Nov. 2026.
//
#include <dxfw11Mini.hpp>


static void render_menu()
{
	ImGui::ShowDemoWindow();
}

int main()
{
	auto& render = dxfw::CRender::Get();
	render.SetMenu_RenderFunction(render_menu);

	if (!render.InitWindow({ 1920, 1080 }, {}, "TestWindow", 144, dxfw::EWindowType::OVERLAY))
		return -1;


	std::array clearBack { 0.0F, 0.0F, 0.0F, 0.0F };

	while (!render.WindowShouldClose())
	{
		render.ToggleClickThrough(VK_INSERT, true);

		render.BeginFrame(clearBack);

		render.GetDrawList(dxfw::EDrawListType::BACK)
			->AddRectFilled({100.f, 100.f},
							{200.f, 200.f},
							ImGui::ColorConvertFloat4ToU32(
								{1.f, 0.f, 0.f, 1.f}
							));

		render.EndFrame(false);
	}

	return 0;
}