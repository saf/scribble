#include "LiterakiGame.h"
#include "IdlePlayer.h"
#include "LiterakiBoardPrinter.h"

int main(int argc, const char **argv) {
	IdlePlayer p;
	LiterakiBoardPrinter printer;
	std::vector<Player *> players;

	players.push_back(&p);

	LiterakiGame g(players);
	g.initializeState();

	printer.printBoard(g.getCurrentState()->getBoard());
}
