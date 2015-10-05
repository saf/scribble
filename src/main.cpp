#include "Dictionary.h"
#include <string>

int main(int argc, char **argv) {
	Alphabet polishAlphabet(WORD("ĄĆĘŁŃÓŚŹŻ"));
	Dictionary dict(polishAlphabet);

	setlocale(LC_ALL, "");

	dict.readFromStream(std::wcin);
	std::cout << dict.getSize() << std::endl;
	return 0;
}
