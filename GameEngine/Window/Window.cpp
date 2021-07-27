#include <sstream>
#include <comdef.h>
#include "Window.hpp"
#include "resource.h"
#include "../Engine.hpp"

Window::WindowClass Window::WindowClass::wndClassInstance;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIconW(hInst, LPWSTR(IDI_GAMEICON));
	wc.hIconSm = LoadIconW(hInst, LPWSTR(IDI_GAMEICON));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name;
	RegisterClassExW(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClassW(name, hInst);
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return name;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClassInstance.hInst;
}

Window::Window(const wchar_t* title, int width, int height): width(width), height(height), inputDevice()
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == NULL)
		throw WND_LAST_EXCEPTION();

	hWnd = CreateWindowW(
		WindowClass::GetName(), title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	if(hWnd == nullptr)
		throw WND_LAST_EXCEPTION();
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	static bool rawInputInitialized = false;
	if (!rawInputInitialized)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;
		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			throw WND_LAST_EXCEPTION();
		}
		rawInputInitialized = true;
	}
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::wstring& title)
{
	if (!SetWindowTextW(hWnd, title.c_str()))
		throw WND_LAST_EXCEPTION();
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		//PM_REMOVE Messages are removed from the queue after processing by PeekMessage.
	{
		if (msg.message == WM_QUIT)
			return static_cast<int>(msg.wParam);;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return {};
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// WM_NCCREATE - message when a window is first created.
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreateStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWindow = static_cast<Window*>(pCreateStruct->lpCreateParams);
		if (pWindow == nullptr)
		{
			assert("Critical Error: pointer to Window is null!(Window::HandleMsgSetup)");
			exit(-1);
		}
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow)); // Set window ptr
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgForwarding));
		return pWindow->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgForwarding(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWindow->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		// Keyboard
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (inputDevice.keyboard.IsKeysAutoRepeat())
		{
			inputDevice.keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				inputDevice.keyboard.OnKeyPressed(keycode);
			}
		}
		break;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		inputDevice.keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		char ch = static_cast<char>(wParam);
		if (inputDevice.keyboard.IsCharsAutoRepeat())
		{
			inputDevice.keyboard.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				inputDevice.keyboard.OnChar(ch);
			}
		}
		break;
	}
	case WM_KILLFOCUS:
	{
		inputDevice.keyboard.ClearKeyState();
		break;
	}
	// Mouse
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnMouseMove(pt.x, pt.y);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			if (!inputDevice.mouse.IsInWindow())
			{
				SetCapture(hWnd);
				inputDevice.mouse.OnMouseEnter();
			}
		}
		else
		{
			if (inputDevice.mouse.IsInWindow())
			{
				ReleaseCapture();
				inputDevice.mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnMiddlePressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		inputDevice.mouse.OnMiddleReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		inputDevice.mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	case WM_INPUT:
	{
		UINT dataSize = 0u;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					inputDevice.mouse.OnMouseRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept : EngineException(line, file), hr(hr) {}

const wchar_t* Window::WindowException::what() const noexcept
{
	_com_error errorInfo(hr);
	std::wostringstream oss;
	oss << GetType() << " - 0x" << std::hex << std::uppercase << hr << std::dec << L"(" << (unsigned long)hr << L")" << std::endl
		<< L"Description: " << errorInfo.ErrorMessage() << std::endl
		<< GetErrorLocation().c_str();
	buffer = oss.str();
	return buffer.c_str();
}

const wchar_t* Window::WindowException::GetType() const noexcept
{
	return L"WindowException";
}

HRESULT Window::WindowException::GetHr() const noexcept
{
	return hr;
}

std::wstring Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	_com_error errorInfo(hr);
	return errorInfo.ErrorMessage();
}