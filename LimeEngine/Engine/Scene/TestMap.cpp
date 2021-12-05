#include "TestMap.hpp"
#include "../Engine.hpp"

#include "MeshObject.hpp"

namespace LimeEngine
{
	void TestMap::Initialize(Engine* engine)
	{
		this->engine = engine;
		// Loading
		static VertexShader vertexShader;
		static PixelShader pixelShader;
		D3D11_INPUT_ELEMENT_DESC loyout[]
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(loyout);
		vertexShader.Initalize(engine->window.graphics.device, Paths::ShaderFolder + L"VertexShader.cso", loyout, numElements);
		pixelShader.Initalize(engine->window.graphics.device, Paths::ShaderFolder + L"PixelShader.cso");



		static std::vector<Vertex> vertices
		{
			{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f }, // front
			{ -0.5f,  0.5f, -0.5f, 0.0f, 0.0f },
			{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f },
			{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f },

			{ -0.5f, -0.5f, 0.5f, 0.0f, 1.0f }, // back
			{ -0.5f,  0.5f, 0.5f, 0.0f, 0.0f },
			{ 0.5f,  0.5f, 0.5f, 1.0f, 0.0f },
			{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
		};
		static std::vector<DWORD> indices =
		{
			0, 1, 2, 0, 2, 3, // front
			4, 7, 6, 4, 6, 5, // back
			3, 2, 6, 3, 6, 7, // right
			4, 5, 1, 4, 1, 0, // left
			1, 5, 6, 1, 6, 2, // top
			0, 3, 7, 0, 7, 4, // bottom
		};
		auto mesh = engine->gameDataManager.meshes.Create(0, engine->window.graphics.device.Get(), engine->window.graphics.deviceContext.Get(), vertices, indices);
		auto material = engine->gameDataManager.materials.Create(0, engine->window.graphics.deviceContext.Get(), &vertexShader, &pixelShader);
		auto texture = engine->gameDataManager.textures2D.Create(0, engine->window.graphics.device.Get(), L"Data\\Textures\\cat.jpg", TextureType::Diffuse);

		material->AddTexture(texture);
		mesh->SetMaterial(material);

		// Make Scene
		AttachObject(std::make_unique<MeshObject>(0));
		objects[0]->AttachComponent(std::make_unique<MeshComponent>(0));
		objects[0]->SetLocation(5, 5, 10);
		objects[0]->SetRotation(XMConvertToRadians(45), XMConvertToRadians(45), XMConvertToRadians(45));

		objects[0]->components[0]->SetLocation(10, 10, 12);
		objects[0]->components[0]->SetRotation(XMConvertToRadians(30), XMConvertToRadians(60), XMConvertToRadians(90));
		objects[0]->components[0]->SetScale(0.5, 0.5, 0.5);
	}
}