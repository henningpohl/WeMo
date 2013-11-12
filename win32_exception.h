#pragma once

#include <stdexcept>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef _MSC_VER
	#define NOEXCEPT noexcept
#else
	#if _MSC_VER > 1700 // Visual Studio 2013 or later
		#define NOEXCEPT noexcept
	#else
		#define NOEXCEPT throw()
	#endif
#endif

class win32_exception : public std::runtime_error {
public:
	win32_exception() : std::runtime_error(MAKE_ERROR_MESSAGE(errorCode)) { }
	virtual ~win32_exception() { }

	DWORD get_error_code() const NOEXCEPT {
		return errorCode;
	}
private:
	DWORD errorCode;

	static std::string MAKE_ERROR_MESSAGE(DWORD & errorCode) {
		errorCode = GetLastError();

		void * messageBuffer;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer,
			0, nullptr);
		
		if(messageBuffer != nullptr) {
			std::string errorMessage((const char *)messageBuffer);
			LocalFree(messageBuffer);
			return errorMessage;
		} else {
			return std::string("No error message available.");
		}
	}
};