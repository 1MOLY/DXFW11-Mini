//
// Created by Lukas on 11 Nov. 2026.
//
#include <dxfw11Mini.hpp>


int main()
{
	if (!dxfw::CRender::Get().InitWindow({ 1920, 1080 }, {}, "TestWindow"))
		return -1;

	while (!dxfw::CRender::Get().WindowShouldClose())
	{
		dxfw::CRender::Get().BeginFrame({ 0.1F, 0.1F, 0.1F, 1.0F });



		dxfw::CRender::Get().EndFrame(false);
	}

	return 0;
}