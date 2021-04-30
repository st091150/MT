#ifndef _MT_COMMAND_
#define _MT_COMMAND_

#include "Tape.h"
#include "Command.h"
#include "Errors.h"
#include <iostream>



class TapeIterator {
	bool* current;
public:
	TapeIterator(bool* p = nullptr) : current(p) {}
	~TapeIterator() {
		current = nullptr;
	}
	bool& operator*() { return *current; }
    bool& operator*() const { return *current; }
	bool operator==(const TapeIterator& p) const { return current == p.current; }
	bool operator!=(const TapeIterator& p) const { return current != p.current; }
	TapeIterator& operator=(bool* X) {
		current = X;
		return *this;
	}
	TapeIterator& operator++() { ++current; return *this; }
	TapeIterator operator++(int) { bool* tmp = current; ++current; return TapeIterator(tmp); }
	friend class MT_Commands;
	//friend std::ostream& operator<<(std::ostream& out, const TapeIterator& X);
};

class MT_Commands {
	Command* commands;
	Tape tape;
	TapeIterator begin, now, end;
	unsigned int MaxCountOfCommands, CountOfCommands;
public:
	MT_Commands(const char* FileWord = "",unsigned int Count = 0) :tape(), CountOfCommands(0), MaxCountOfCommands(Count) {
		if (FileWord != "") { tape.setWordFromFile(FileWord); }
		if (MaxCountOfCommands > 0) {
			commands = new Command[MaxCountOfCommands];
			if (!commands)throw FailOutOfMemory();
		}
		now = begin = &tape[tape.getStartPosition()];
		end = &tape[tape.getStartPosition() + tape.getLengthOfWord()];
	}
	~MT_Commands() {
		if (commands)delete[] commands;
		
		commands = nullptr;
		MaxCountOfCommands = 0;
		CountOfCommands = 0;
	}

	unsigned int size() const { return CountOfCommands; }
	unsigned int capacity() const { return MaxCountOfCommands; }

	Command& operator[] (unsigned int i) const {
		if (i >= CountOfCommands) throw "Out of array's bounds";
		return commands[i];
	}

	Command& find(const char* command, const bool& ch)const {
		for (int i = 0; i < CountOfCommands; i++) {
			if (!strcmp(command, commands[i].getCurrentCommand()) && ch == bool(commands[i].getcurrentValue() - '0')) {
				return commands[i];
			}
		}
		throw FailNextCommand(CountCommands,command, char(ch + '0'));
	}
	void push_back(const Command&);
	void DownloadCommandsFromFile(const char* FileName) {
		std::ifstream File(FileName);
		Command V;
		for (int i = 0; File.peek() != EOF; i++) {
			File >> V;
			push_back(V);
		}
		File.close();
	}
	void showCommands() {
		for (unsigned int i = 0; i < CountOfCommands; i++) {
			std::cout << commands[i];
		}
	}
	void showTape() {
		TapeIterator newBegin = begin, newEnd = end;
		while (*newBegin != 1 && newBegin != now && newBegin != end) {
			newBegin.current++;
		}
		while (*newEnd != 1 && newEnd != now && newEnd != begin) {
			newEnd.current--;
		}
		TapeIterator Position = newBegin;
		int sup = 0;
		for(int i = 0;Position != newEnd.current+1;i++) {
			if (Position == now) {
				sup = i;
			}
			std::cout << *Position.current++;
		}
		std::cout << std::endl;
		for (int j = 0; j < sup; j++)std::cout << ' ';
		std::cout << '^' << std::endl;
	}
	void setWord(const char* FileName) {
		tape.setWordFromFile(FileName);
	}
	void doCommmand(){
		Command& X = commands[0];
		for(unsigned int i = 0 ;i < 10000000;i++){
			//showTape(); ïîêàçàòü êàæäóþ èòåðàöèþ
			for (int j = 0; j < 2; j++) {
				switch (j) {
				case 0: {
					if (X.getChangeValue() == '1') {
						*now.current = 1;
					}
					else {
						*now.current = 0;
					}
					break;
				}
				case 1: {
					if (X.getShift() == 'R') {
						
						now.current++;
						if (now.current - 1 == end.current) {
							end.current = now.current;
						}
						 if (now.current == &tape[tape.getLength()]) {
							std::ofstream File("output.txt");
							if (!File) {
								throw FailOpenFile("output.txt");
							}
							File << "out of memmory";
							File.close();
							throw FailOutOfMemory();
						}
					}
					else if (X.getShift() == 'L') {
						now.current--;
						if (now.current + 1 == begin.current) {
							begin.current = now.current;
						}
						 if (now.current == &tape[0]) {
							std::ofstream File("output.txt");
							if (!File) {
								throw FailOpenFile("output.txt");
							}
							File << "out of memmory";
							File.close();
							throw FailOutOfMemory();
						}
					}
					break;
				}
				}
			}
			if (!strcmp(X.getNextCommand(), "q0")) {
				break;
			}
			else if (i == 10000000 - 1) {
				std::ofstream File("output.txt");
				if (!File) {
					throw FailOpenFile("output.txt");
				}
				File << "not applicable";
				File.close();
				throw FailNotApplicable();
			}
			X = find(X.getNextCommand(), *now.current);
		}

	}
	void resultInFile(const char* X) {
		std::ofstream File(X);
		if (!File) {
			throw FailOpenFile(X);
		}
		TapeIterator newBegin = begin, newEnd = end;
		while (*newBegin != 1 && newBegin != now && newBegin != end) {
			newBegin.current++;
		}
		while (*newEnd != 1 && newEnd != now && newEnd != begin) {
			newEnd.current--;
		}
		TapeIterator Position = newBegin;
		int sup = 0;
		for (int i = 0; Position != newEnd.current + 1; i++) {
			if (Position == now) {
				sup = i;
			}
			File << *Position.current++;
		}
		File << std::endl;
		for (int j = 0; j < sup; j++)File << ' ';
		File << '^' << std::endl;
		File.close();
	}
};

void MT_Commands::push_back(const Command& X) {
	if (CountOfCommands + 1 > MaxCountOfCommands) {
		unsigned int newLen = MaxCountOfCommands + 1;
		newLen = MaxCountOfCommands + 1;
		//else
		//newLen = (unsigned int)(maxLen * 1.1);
		//newLen = MaxCountOfCommands + 5;
		//newLen = 2*maxLen;

		Command* tmp = new Command[newLen];
		if (!tmp) throw FailOutOfMemory();
		for (unsigned int i = 0; i < CountOfCommands; ++i)
			tmp[i] = std::move(commands[i]);
		delete[] commands;
		commands = tmp;
		MaxCountOfCommands = newLen;
	}
	// Äîáàâèòü íîâûé ýëåìåíò
	commands[CountOfCommands++] = X;
}

#endif 
