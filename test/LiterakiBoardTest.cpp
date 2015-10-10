#include "GameState.h"
#include "IdlePlayer.h"
#include "LiterakiGame.h"
#include "LiterakiBoardPrinter.h"

#include <iostream>
#include <fstream>

int main(int argc, const char **argv) {
	IdlePlayer p;
	LiterakiBoardPrinter printer;

	setlocale(LC_ALL, "");
	std::locale locale("");

	std::wifstream fs;
	std::cerr << "Opening " << argv[1] << std::endl;
	std::wcout.imbue(locale);
	fs.open(argv[1], std::fstream::in);
	if (fs.is_open()) {
		fs.imbue(locale);
		LiterakiGame g = LiterakiGame::readFromStream(fs);
		printer.printBoard(g.getCurrentState().getBoard());
	} else {
		std::cerr << "Failed to open " << argv[1] << std::endl;
	}
}
