#include <fstream>
#include <iostream>
#include <string>

#include "Decision.h"
#include "Dictionary.h"
#include "ESPlayer.h"
#include "GameState.h"
#include "LiterakiBoardPrinter.h"
#include "LiterakiGame.h"

int main(int argc, char **argv) {
	Alphabet polishAlphabet(WORD("ąćęłńóśźż"));
	ESDictionary dict(polishAlphabet);
	LiterakiBoardPrinter printer;

	setlocale(LC_ALL, "");
	std::locale locale("");
	std::wcout.imbue(locale);

	std::wifstream dictFs;
	dictFs.open(argv[1], std::fstream::in);
	dictFs.imbue(locale);
	if (!dictFs.is_open()) {
		std::cerr << "Failed to open " << argv[1] << std::endl;
	}
	dict.readFromStream(dictFs);
	ESPlayer player(dict);

	std::cerr << "Read dictionary (" << dict.getSize() << ") entries." << std::endl;

	std::wifstream fs;
	fs.open(argv[2], std::fstream::in);
	if (!fs.is_open()) {
		std::cerr << "Failed to open " << argv[2] << std::endl;
		exit(1);
	}

	fs.imbue(locale);
	LiterakiGame g = LiterakiGame::readFromStream(fs);

	for (auto& state : g.getStateHistory()) {
		printer.printBoard(g.getBoard(),
				state->getTiles(),
				state->getBlankAssignments());
		PlayerState playerState(state, state->getTurn());
		auto decision = player.makeDecision(playerState);
		std::wcout << decision->toString() << std::endl;
	}

	return 0;
}
