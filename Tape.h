#ifndef _TAPE_OF_MT_
#define _TAPE_OF_MT_
#include "Errors.h"
#include <string>


class Tape {
	bool* tape;
	char* Word;
	unsigned int LenOfTape,LengthOfWord;
public:
	
	Tape(const char* Word = "",unsigned int LengthOfTape = 2000000):Word(nullptr),LenOfTape(LengthOfTape), LengthOfWord(0){

		tape = new bool[LengthOfTape];
		for (int i = 1; i < LengthOfTape-1; i++) {
			tape[i] = 0;
		}

		if (Word != "") {
			setWord(Word);
		}

	}
	void setWordFromFile(const char*);
	void setWord(const char* Word) {

		for (int i = 0; i < strlen(Word); i++) {
			if (Word[i] != '0' && Word[i] != '1') {
				throw FailWord(Word);
			}
		}

		this->Word = new char[strlen(Word)+1];
		memcpy(this->Word, Word, strlen(Word)+1);
		
		LengthOfWord = strlen(this->Word);

		for (int i = LenOfTape/2 ; i <= LengthOfWord + LenOfTape/2; i++) {
			if (this->Word[i - LenOfTape / 2] == '0' || this->Word[i - LenOfTape / 2] == '\0')tape[i] = 0;
			else tape[i] = 1;
		}
	}
	unsigned int getStartPosition()const { return LenOfTape/2; }
	unsigned int getLength() { return LenOfTape; }
	unsigned int getLengthOfWord() { return strlen(Word); }
	bool& operator[](unsigned int X) { return tape[X]; }

	~Tape() {
		if (tape)delete[] tape;
		if (Word)delete[] Word;
		tape = nullptr; Word = nullptr;
		LenOfTape = LengthOfWord = 0;
	}
};

#include <fstream>
void Tape::setWordFromFile(const char* FileName) {
	std::ifstream File(FileName);
	if (!File) {
		throw FailOpenFile(FileName);
	}
	std::string  sup = "";
	File >> sup;
	File.close();
	if (sup.length() >= 1000) {
		throw FailWord("The word must not be 1000 characters");
	}
	if (!sup.empty()) {
		char* supWord = new char[sup.length()];
		for (int i = 0; i < sup.length(); i++) {
			supWord[i] = sup[i];
		}
		supWord[sup.length()] = '\0';
		setWord(supWord);
	}
}
#endif
