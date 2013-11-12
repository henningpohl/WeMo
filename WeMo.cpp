#include <Strsafe.h>
#include <regex>
#include "WeMo.h"
#include "win32_exception.h"

const wchar_t * headersTemplate = L"Content-Type: text/xml; charset=\"utf-8\"\r\nSOAPACTION: \"%s\"\r\nHOST: %s";
const char * soapToggleTemplate = 
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
	"  <s:Body>\r\n"
	"    <u:SetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
	"      <BinaryState>%d</BinaryState>\r\n"
	"    </u:SetBinaryState>\r\n"
	"  </s:Body>\r\n"
	"</s:Envelope>\r\n";
const char * soapQueryTemplate = 
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
	"  <s:Body>\r\n"
	"    <u:GetBinaryState xmlns:u=\"urn:Belkin:service:basicevent:1\" />\r\n"
	"  </s:Body>\r\n"
	"</s:Envelope>\r\n";


WeMo::WeMo(const wchar_t * ip, int port) {
	userAgent = L"CyberGarage-HTTP/1.0";
	StringCbPrintfW(host, 100, L"%s:%d", ip, port);

	session = WinHttpOpen(userAgent, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if(!session) {
		throw win32_exception();
	}

	connection = WinHttpConnect(session, L"10.22.22.1", 49152, 0);
	if(!connection) {
		throw win32_exception();
	}
}

WeMo::~WeMo() {
	WinHttpCloseHandle(connection);
	WinHttpCloseHandle(session);
}

void WeMo::turnOn() {
	wchar_t headers[1024];
	char message[1024];

	StringCbPrintfW(headers, 1024, headersTemplate, L"urn:Belkin:service:basicevent:1#SetBinaryState", host);
	StringCbPrintf(message, 1024, soapToggleTemplate, 1);

	sendRequest(L"/upnp/control/basicevent1", headers, message);
}

void WeMo::turnOff() {
	wchar_t headers[1024];
	char message[1024];

	StringCbPrintfW(headers, 1024, headersTemplate, L"urn:Belkin:service:basicevent:1#SetBinaryState", host);
	StringCbPrintf(message, 1024, soapToggleTemplate, 0);

	sendRequest(L"/upnp/control/basicevent1", headers, message);
}

bool WeMo::getState() {
	wchar_t headers[1024];
	StringCbPrintfW(headers, 1024, headersTemplate, L"urn:Belkin:service:basicevent:1#GetBinaryState", host);

	auto response = sendRequest(L"/upnp/control/basicevent1", headers, soapQueryTemplate);
	std::regex reg("<BinaryState>([01])</BinaryState>");

	std::smatch regResult;
	if(std::regex_search(response, regResult, reg) == false) {
		return false;
	}
		
	if(regResult.size() != 2) {
		return false;
	}

	auto resStr = regResult[1].str();
	if(resStr.length() != 1) {
		return false;
	}

	if(resStr.at(0) == '1') {
		return true;
	} else {
		return false;
	}
}

std::string WeMo::sendRequest(const wchar_t * call, const wchar_t * headers, const char * message) {
	auto request = WinHttpOpenRequest(connection, L"POST", call, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	if(!request) {
		throw win32_exception();
	}

	auto whereHeadersAdded = WinHttpAddRequestHeaders(request, headers, (DWORD)wcslen(headers), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);
	if(whereHeadersAdded != TRUE) {
		throw win32_exception();
	}

	size_t messageLen = strlen(message);
	auto couldStartSend = WinHttpSendRequest(request, 0, 0, 0, 0, (DWORD)messageLen, 0);
	if(couldStartSend != TRUE) {
		throw win32_exception();
	}

	DWORD wroteLen;
	auto couldWriteData = WinHttpWriteData(request, message, (DWORD)messageLen, &wroteLen);
	if(couldWriteData != TRUE || wroteLen != messageLen) {
		throw win32_exception();
	}

	std::string response;
	auto gotResponse = WinHttpReceiveResponse(request, nullptr);
	if(gotResponse == TRUE) {
		DWORD lenDataAvailable;
		WinHttpQueryDataAvailable(request, &lenDataAvailable);

		char * resultBuffer = new char[lenDataAvailable];
		DWORD bytesRead;
		auto couldReadResponse = WinHttpReadData(request, resultBuffer, lenDataAvailable, &bytesRead);

		response.append(resultBuffer);
		delete[] resultBuffer;
	}

	WinHttpCloseHandle(request);
	return response;
}


