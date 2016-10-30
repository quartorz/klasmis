#define NOMINMAX

#include <quote/quote.hpp>
#include <quote/win32/message_loop.hpp>

#include "main_window.hpp"
#include "tetrimino.hpp"

int run()
{
	if (!main_window::register_class()) {
		return 0;
	}

	main_window w;

	if (!w.create(nullptr, L"Klasmis", CW_USEDEFAULT, CW_USEDEFAULT, 550, 670)) {
		return 0;
	}

	w.show();
	
	tetrimino t(tetrimino::shape::I);

	return quote::win32::message_loop();
}

QUOTE_DEFINE_MAIN
