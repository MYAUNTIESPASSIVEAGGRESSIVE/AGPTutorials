#pragma once
#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>

class Camera;

class Window
{
// private vars
private:
	HWND handle = NULL;
	HINSTANCE instance = NULL;
	int height = 32, width = 32;

	DirectX::Keyboard keyboard;
	DirectX::Mouse mouse;

// private funcs
private:
	// static window proc func
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// public funcs
public:
	bool DoesExist() { return handle != NULL; }
	HWND GetHandle() { return handle; }
	int GetHeight() { return height; }
	int GetWidth() { return width; }

	DirectX::Keyboard::KeyboardStateTracker kbTracker;

	// constructor
	Window(int width, int height, HINSTANCE instance, int nCmdShow);
	void HandleInput(Camera cam);
};

