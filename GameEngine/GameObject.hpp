#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class GameObject
{
public:
	const XMVECTOR& GetPositionVector() const noexcept;
	const XMFLOAT3& GetPositionFloat3() const noexcept;
	const XMVECTOR& GetRotationVector() const noexcept;
	const XMFLOAT3& GetRotationFloat3() const noexcept;
	const XMVECTOR& GetScaleVector() const noexcept;
	const XMFLOAT3& GetScaleFloat3() const noexcept;

	void SetPosition(const XMVECTOR& pos) noexcept;
	void SetPosition(const XMFLOAT3& pos) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	void AddPosition(const XMVECTOR& pos) noexcept;
	void AddPosition(const XMFLOAT3& pos) noexcept;
	void AddPosition(float x, float y, float z) noexcept;

	void SetRotation(const XMVECTOR& rot) noexcept;
	void SetRotation(const XMFLOAT3& rot) noexcept;
	void SetRotation(float x, float y, float z) noexcept;
	void AddRotation(const XMVECTOR& rot) noexcept;
	void AddRotation(const XMFLOAT3& rot) noexcept;
	void AddRotation(float x, float y, float z) noexcept;

	void SetScale(const XMVECTOR& scale) noexcept;
	void SetScale(const XMFLOAT3& scale) noexcept;
	void SetScale(float x, float y, float z) noexcept;
	void AddScale(const XMVECTOR& scale) noexcept;
	void AddScale(const XMFLOAT3& scale) noexcept;
	void AddScale(float x, float y, float z) noexcept;

protected:
	bool isChanged = false;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
};