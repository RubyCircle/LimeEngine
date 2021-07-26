/*
* Copyright(C) 2021, by ����� ������(ruby.circles774@gmail.com)
*/
#include "../Engine.hpp"
#include <sstream>
int APIENTRY wWinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPWSTR		lpCmdLine,
	_In_		int			nCmdShow)
{
	try
	{
		Engine engine;

		std::optional<int> exitCode = 0;
		while (true)
		{
			if (exitCode = engine.window.ProcessMessages())
			{
				return *exitCode;
			}
			static std::wstringstream str;
			char c = engine.inputDevice.keyboard.ReadChar();
			if (c)
			{
				str << c;
				engine.window.SetTitle(str.str());
			}
		}
		return 0;
	}
	catch (const EngineException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No info", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}