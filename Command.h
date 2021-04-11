#ifndef _COMMAND_OF_MT_
#define _COMMAND_OF_MT_

#include <string>
#include "Errors.h"

const int ArgumentsInCommand = 7;
class Command {
	char *currentCommand, *nextCommand;
	char currentValue, changeValue ,shift;
public:
	Command(const char* currentCommand = "q0", char currentValue = '0',
		char changeValue = '0',char shift = 'N', const char* nextCommand = "q0"):
		currentValue(currentValue), changeValue(changeValue),shift(shift) {

		for (unsigned int i = 1; i < strlen(currentCommand); i++) {
			if (currentCommand[i] < '0' || currentCommand[i] > '9') {
				throw FailCommand(CountCommands ,currentCommand);
			}
		}
		for (unsigned int i = 1; i < strlen(nextCommand); i++) {
			if (nextCommand[i] < '0' || nextCommand[i] > '9') {
				throw FailCommand(CountCommands, nextCommand);
			}
		}
		if (currentCommand[0] != 'q') { throw FailCommand(CountCommands,currentCommand); }
		else if (nextCommand[0] != 'q') { throw FailCommand(CountCommands, nextCommand); }
		else if (currentValue != '0' && currentValue != '1') { throw FailCurrentValue(CountCommands, currentValue); }
		else if (changeValue != '0' && changeValue != '1') { throw FailChangeValue(CountCommands, changeValue); }
		else if (shift != 'N' && shift != 'R' && shift != 'L') { throw FailShift(CountCommands, shift); }
		else {
			int Length = strlen(currentCommand) + 1;
			this->currentCommand = new char[Length];
			memcpy(this->currentCommand, currentCommand, Length);

			Length = strlen(nextCommand) + 1;
			this->nextCommand = new char[Length];
			memcpy(this->nextCommand, nextCommand, Length);
		}
	}
	Command(Command&&);
	Command(const Command&);
	Command& operator=(Command&&);
	Command& operator=(const Command&);

	char* getCurrentCommand() { return currentCommand; }
	char* getNextCommand() { return nextCommand; }
	char  getcurrentValue() { return currentValue; }
	char  getChangeValue() { return changeValue; }
	char  getShift() { return shift; }
	
	bool operator==(const Command& X)const;
	bool operator!=(const Command& X)const { return (!(this == &X)); }
	~Command() {
		if (currentCommand) delete[] currentCommand;
		if (nextCommand) delete[] nextCommand;

		currentCommand = nextCommand = nullptr;
		currentValue = changeValue = 0;
	}

	friend std::ostream& operator<< (std::ostream& out, const Command& X);
};

Command::Command(Command&& X) {
	currentCommand = X.currentCommand;
	nextCommand = X.nextCommand;
	currentValue = X.currentValue;
	changeValue = X.changeValue;
	shift = X.shift;

	X.currentCommand = X.nextCommand = nullptr;
	X.currentValue = X.changeValue = 0;
}
Command::Command(const Command& X) {
	currentCommand = new char[strlen(X.currentCommand)+1];
	nextCommand = new char[strlen(X.nextCommand)+1];

	memcpy(currentCommand, X.currentCommand, strlen(X.currentCommand)+1);
	memcpy(nextCommand, X.nextCommand, strlen(X.nextCommand)+1);

	currentCommand[strlen(X.currentCommand)+1] = '\0';
	nextCommand[strlen(X.nextCommand)+1] = '\0';

	currentValue = X.currentValue;
	changeValue = X.changeValue;
	shift = X.shift;
}
Command& Command::operator=(Command&&  X) {
	if (this != &X) {
		if (currentCommand)delete[] currentCommand;
		if (nextCommand)delete[] nextCommand;

		currentCommand = X.currentCommand;
		nextCommand = X.nextCommand;
		currentValue = X.currentValue;
		changeValue = X.changeValue;
		shift = X.shift;

		X.currentCommand = X.nextCommand = nullptr;
		X.currentValue = X.changeValue = 0;
	}
	return *this;
}
Command& Command::operator=(const Command& X) {
	if (this != &X) {
		if (currentCommand)delete[] currentCommand;
		if (nextCommand)delete[] nextCommand;

		currentCommand = new char[strlen(X.currentCommand) + 1];
		nextCommand = new char[strlen(X.nextCommand) + 1];

		memcpy(currentCommand, X.currentCommand, strlen(X.currentCommand) + 1);
		memcpy(nextCommand, X.nextCommand, strlen(X.nextCommand) + 1);

		currentValue = X.currentValue;
		changeValue = X.changeValue;
		shift = X.shift;
	}
	return *this;
}

bool Command::operator==(const Command& X)const  {
	bool FirstStr = (strcmp(currentCommand, X.currentCommand) == 0);
	bool CurrentBool = (currentValue == X.currentValue);

	if (FirstStr && CurrentBool) {
		return true;
	}
	else {
		return false;
	}
}

#include <sstream>
#include <fstream>
std::ifstream& operator>> (std::ifstream& in, Command& X) {
	char buf[1024];
	in.getline(buf, 1024);
	bool allGood = false;
	char* currentCommand = nullptr, *nextCommand = nullptr;
	char currentValue = 'a', changeValue = 'a';
	char shift = 'p';
	std::stringstream Str(buf);
	std::string sup = "";
	Str >> sup;
	for (int i = 0; i < ArgumentsInCommand && Str; i++) {
		switch (i) {
		case 0: {
			currentCommand = new char[sup.size() + 1];
			std::copy(sup.begin(), sup.end(), currentCommand);
			currentCommand[sup.size()] = '\0';
			break;
		}
		case 1: {
			currentValue = sup[0];
			break;
		}
		case 2: {
			break;
		}
		case 3:{
			changeValue = sup[0];
			break;
		}
		case 4:{
			shift = sup[0];
			break;
		}
		case 5: {
			allGood = true;
			nextCommand = new char[sup.size() + 1];
			std::copy(sup.begin(), sup.end(), nextCommand);
			nextCommand[sup.size()] = '\0';
			break;
		}
		}
		Str >> sup;
	}
	if (allGood) {
		X = std::move(Command(currentCommand, currentValue, changeValue, shift, nextCommand));
		return in;
	}
	else {
		throw FailArguments();
	}
	
}

#include <iostream>
std::ostream& operator<< (std::ostream& out, const Command& X) {
	out << X.currentCommand << ' ' << X.currentValue << " -> " <<
		X.changeValue << ' ' << X.shift << ' ' << X.nextCommand << std::endl;
	return out;
}

#endif // !_COMMAND_OF_MT_