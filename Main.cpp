#include <iostream>
#include "WeMo.h"
#include "win32_exception.h"

int main(int argc, const char * argv[]) {
	WeMo wemo;
	
	try {
		auto s0 = wemo.getState();

		wemo.turnOn();

		auto s1 = wemo.getState();

		Sleep(2000);

		wemo.turnOff();

		auto s2 = wemo.getState();


	} catch(win32_exception & e) {
		std::cerr << e.what() << std::endl;
	}
}