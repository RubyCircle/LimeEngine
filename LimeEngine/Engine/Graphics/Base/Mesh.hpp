#pragma once
#include <vector>
#include "Material.hpp"
#include "../../Base/Vertex.hpp"
#include "../Systems/DX11/MeshDX11.hpp"

namespace LimeEngine
{
	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, size_t id);
		void SetMaterial(Material* material) noexcept;
		Material* GetMaterial() const noexcept;
		const std::vector<Vertex>& GetVertices() const noexcept;
		const std::vector<DWORD>& GetIndices() const noexcept;
		const size_t& GetId() const noexcept;

	private:
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		size_t id;
		Material* material = nullptr;
	};
}