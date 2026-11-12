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

	if (!dxfw::CRender::Get().InitWindow({ 1920, 1080 }, {}, "TestWindow"))
		return -1;

	std::array clearBack { 0.1F, 0.1F, 0.1F, 1.0F };

	while (!dxfw::CRender::Get().WindowShouldClose())
	{
		dxfw::CRender::Get().BeginFrame(clearBack);



		dxfw::CRender::Get().EndFrame(false);
	}

	return 0;
}