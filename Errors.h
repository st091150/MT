#ifndef _ERRORS_OF_MT_
#define _ERRORS_OF_MT_
#include <string>
unsigned int CountCommands = 1;

class Errors {
public:
	virtual std::string what() const { return std::string("Unknown error"); }
};

class FailOpenFile :public Errors {
	char* FileName;
public:
	FailOpenFile(const char * X) {
		int Len = strlen(X) + 1;
		FileName = new char[Len];
		memcpy(FileName, X, Len - 1);
		FileName[strlen(X)] = '\0';
	}
	std::string what()const {
		return "Can't open File : " + std::string(FileName);
	}
};

class FailCommand :public Errors {
	std::string currentCommand;
	int i;
public:
	FailCommand(int i ,const char* X) :currentCommand(X),i(i) {}

	std::string what()const { return "ERROR in " + std::to_string(i) + " Line ! It can't be Command : " +
		currentCommand + " First character must be 'q' and after 'q' decimal digits";
	}
};
class FailCurrentValue :public Errors {
	char currentValue;
	int i;
public:
	FailCurrentValue(int i ,const char &X) :currentValue(X),i(i) {}

	std::string what()const {
		return "ERROR in " + std::to_string(i) + " Line! It can't be Command : current Value = " +
			std::string(currentValue + std::string(": Value must be '0' or '1' !"));
	}
};
class FailChangeValue :public Errors {
	char changeValue;
	int i;
public:
	FailChangeValue(int i, const char& X) :changeValue(X),i(i) {}

	std::string what()const {
		return "ERROR in " + std::to_string(i) + " Line! It can't be Command : change Value = " +
			std::string(changeValue + std::string(": Value must be '0' or '1' !"));
	}
};
class FailShift :public Errors {
	char shift;
	int i;
public:
	FailShift(int i ,const char& X) : shift(X),i(i) {}

	std::string what()const {
		return "Error in " + std::to_string(i) + " Line It can't be Command : Shift " +
			std::string(shift + std::string(" Shift must be 'N' , 'R' or 'L'"));
	}
};
class FailNextCommand:public Errors {
	std::string Word;
	char ch;
	int i;
public:
	FailNextCommand(int i ,const char* X,char C):ch(C) {
		for (int i = 0; i < strlen(X); i++) {
			Word += X[i];
		}
	}
	std::string what()const {
		return std::string("Error in " + std::to_string(i) + " Line! There is no command as :" + Word + " with " + ch + " current value");
	}
};

class FailArguments :public Errors {
public:
	FailArguments() {};
	std::string what()const  {
		return std::string("Not enough arguments to build the command!");
	}
};

class FailOutOfMemory :public Errors {
public:
	std::string what()const {
		return "Out Of Memory!";
	}
};

class FailNotApplicable :public Errors {
public:
	std::string what()const {
		return "Not applicable!";
	}
};

class FailWord: public Errors {
	std::string Fail;
public:
	FailWord(const char* X){
		for (int i = 0; i < strlen(X); i++) {
			Fail += X[i];
		}
	}
	std::string what()const  {
		return std::string ("Fail Word :" + Fail);
	}
};


#endif