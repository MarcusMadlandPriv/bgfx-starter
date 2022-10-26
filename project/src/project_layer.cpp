#include <fstream>
#include <iostream>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui.h>

#include "project_layer.hpp"
#include "app/app.hpp"

bgfx::ShaderHandle loadShader(const char* fileName)
{
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(fileName);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }
    else
    {
        std::cout << "ERROR LOADING SHADER" << std::endl;
        return bgfx::ShaderHandle();
    }
        
    const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
    mem->data[mem->size - 1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, fileName);
    return handle;
}


struct Vertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static Vertex cubeVertices[] =
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

bgfx::ProgramHandle program;
bgfx::VertexBufferHandle vbh;
bgfx::IndexBufferHandle ibh;

void ProjectLayer::OnAttach()
{
    // Cube Test
    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), vertexLayout);
    ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    bgfx::ShaderHandle vsh = loadShader("../shaders/simple-vert.bin");
    bgfx::ShaderHandle fsh = loadShader("../shaders/simple-frag.bin");
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
	// Clear Color Buffer 
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0xff00ffff);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

   // Camera
    const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
    const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };
    float view[16];
    bx::mtxLookAt(view, eye, at);
    float proj[16];
    bx::mtxProj(proj, 60.0f,
        float(Core::App::Instance().GetWindow().GetWidth()) / float(Core::App::Instance().GetWindow().GetHeight()),
        0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    bgfx::setViewRect(0, 0, 0, Core::App::Instance().GetWindow().GetWidth(), Core::App::Instance().GetWindow().GetHeight()); // TEST

    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    // RENDERING BEGIN
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);

    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, program);
    // RENDERING END

    // Enable Debug Text
    bgfx::dbgTextClear();
	bgfx::setDebug(BGFX_DEBUG_TEXT);
 
    // Debug Text
	const bgfx::Stats* stats = bgfx::getStats();
	bgfx::dbgTextPrintf(2, 2, 0x0f, "Window size is %dW x %dH", Core::App::Instance().GetWindow().GetWidth(), Core::App::Instance().GetWindow().GetHeight());
	bgfx::dbgTextPrintf(2, 4, 0x0f, "Debug text, will stay on top");
}

void ProjectLayer::OnImGuiRender()
{
	ImGui::ShowDemoWindow();
}
