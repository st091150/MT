#include "MT_commands.h"

int main()
{
    try {
        MT_Commands Array("input.txt");
        Array.DownloadCommandsFromFile("Command.txt");
        Array.showCommands();std::cout << std::endl;
        Array.doCommmand();
        Array.showTape();
        Array.resultInFile("output.txt");
    }
    catch (Errors& Y) {
        std::cout << Y.what();
    }
}

