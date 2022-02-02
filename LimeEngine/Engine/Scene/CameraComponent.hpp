#pragma once
#include "SceneComponent.hpp"

namespace LimeEngine
{
	class CameraComponent : public SceneComponent
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};
		CameraComponent(Engine* engine, Transform transform, bool autoActivate, float width, float height, ProjectionType projectionType = ProjectionType::Perspective, float fovDegrees = 90.0f, float nearZ = 0.001f, float farZ = 10000.0f) noexcept;
		void MakeActive() noexcept;
		void SetPerspective();
		void SetOrthographic();

		void UpdateViewMatrix() const noexcept;
		const XMMATRIX& GetViewMatrix() const noexcept;
		const XMMATRIX& GetProjectionMatrix() const noexcept;
		XMMATRIX GetViewProjectionMatrix() const noexcept;

	private:
		mutable XMMATRIX viewMatrix = XMMatrixIdentity();
		mutable XMMATRIX projectionMatrix = XMMatrixIdentity();
		ProjectionType projectionType = ProjectionType::Perspective;
	public:
		float width;
		float height;
		float fovRadians;
		float aspectRatio;
		float nearZ;
		float farZ;
	};
}