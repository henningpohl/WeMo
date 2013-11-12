#pragma once

#include <string>

#include <Windows.h>
#include <Winhttp.h>

class WeMo {
public:
	WeMo(const wchar_t * ip = L"10.22.22.1", int port = 49152);
	~WeMo();

	void turnOn();
	void turnOff();
	bool getState();
private:
	HINTERNET session;
	HINTERNET connection;

	const wchar_t * userAgent;
	wchar_t host[100];

	std::string sendRequest(const wchar_t * call, const wchar_t * headers, const char * message);
};