#pragma once
#include <windows.h>

namespace Hooks
{
	namespace YYError
	{
		void Function(const char* pFormat, ...);
		void* GetTargetAddress();

		inline decltype(&Function) pfnOriginal;
	}
}