#pragma once
#include "Window/Window.hpp"
#include "Input/InputDevice.hpp"
#include "Helpers/Timer.hpp"

class Engine
{
public:
	Engine(const Engine&) = delete;
	Engine(Engine&&) noexcept = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) noexcept = delete;
	Engine();
public:
	Window window;
	Timer timer;
};