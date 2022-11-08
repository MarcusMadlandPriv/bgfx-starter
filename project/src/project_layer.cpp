#include <fstream>
#include <iostream>
#include <cstdio>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui.h>
#include <string>

#include "project_layer.hpp"
#include "app/app.hpp"

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	5, 6, 7,
	0, 2, 4,
	4, 2, 6,
	1, 5, 3,
	5, 7, 3,
	0, 4, 1,
	4, 5, 1,
	2, 3, 6,
	6, 3, 7,
};

bgfx::ShaderHandle loadShader(const std::string& filename)
{
	FILE* file = fopen(filename.c_str(), "rb");
	if (!file)
	{
		std::cout << "ERROR LOADIN SHADER" << std::endl;
		return bgfx::ShaderHandle();
	}
		
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

bgfx::VertexBufferHandle vbh;
bgfx::IndexBufferHandle ibh;
bgfx::ProgramHandle program;
unsigned int counter = 0;

void ProjectLayer::OnAttach()
{
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);

	bgfx::VertexLayout pcvLayout;
	pcvLayout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();
	vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvLayout);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

	bgfx::ShaderHandle vsh = loadShader("../shaders/compiled/simple-vert.bin");
	bgfx::ShaderHandle fsh = loadShader("../shaders/compiled/simple-frag.bin");
	program = bgfx::createProgram(vsh, fsh, true);
}

void ProjectLayer::OnDetach()
{
	bgfx::destroy(ibh);
	bgfx::destroy(vbh);
}

void ProjectLayer::OnEvent(Core::Event& event)
{

}

void ProjectLayer::OnUpdate(const float& dt)
{
	#ifdef _DEBUG
		bgfx::setDebug(BGFX_DEBUG_TEXT);
		bgfx::dbgTextClear(0, false);
	#endif

	uint32_t width = Core::App::Instance().GetWindow().GetWidth();
	uint32_t height = Core::App::Instance().GetWindow().GetHeight();
	bgfx::setViewRect(0, 0, 0, width, height);

	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	float view[16];
	bx::mtxLookAt(view, eye, at);
	float proj[16];
	bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(0, view, proj);

	float mtx[16];
	bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
	bgfx::setTransform(mtx);

	bgfx::setVertexBuffer(0, vbh);
	bgfx::setIndexBuffer(ibh);

	bgfx::submit(0, program);

	bgfx::dbgTextPrintf(1, 1, 0x0f, "Hello World!");

	bgfx::touch(0);
	counter++;
}

void ProjectLayer::OnImGuiRender()
{
	ImGui::ShowDemoWindow();
}
