#pragma once
#include <Windows.h>
#include <string>

#if _DEBUG
#define LOGLINE(msg) \
	OutputDebugStringA((std::string(msg) + "\n").c_str())

#define LOG(msg) \
	OutputDebugStringA((std::string(__FILE__) + "(" + std::to_string(__LINE__)\
		+ "):" + std::string(msg) + "\n").c_str())
#else
#define LOGLINE(msg)
#define LOG(msg)
#endif // _DEBUG

