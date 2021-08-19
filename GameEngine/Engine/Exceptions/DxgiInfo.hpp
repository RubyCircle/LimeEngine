#pragma once
#include "../Graphics/DirectXDef.hpp"
#include <vector>
#include <string>
#include <dxgidebug.h>

class DxgiInfo
{
public:
	DxgiInfo();
	DxgiInfo(const DxgiInfo&) = delete;
	DxgiInfo& operator=(const DxgiInfo&) = delete;
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;
private:
	unsigned long long next = 0ull;
	com_ptr<IDXGIInfoQueue> pDxgiInfoQueue;
};