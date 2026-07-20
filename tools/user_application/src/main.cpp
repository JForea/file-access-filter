#include <iostream>

#include "Commands.hpp"
#include "Helpers.hpp"
#include "IoctlHelpers.hpp"

const std::string WrongUsage = "Wrong usage: faf [command]\n";

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << WrongUsage;
        return 1;
    }

    Command cmd = ParseCommand(argv[1]);

    try {
        switch (cmd) {
        case Command::Help:
            std::cout << "faf command list:\n" <<
                "help - view command list\n" <<
                "add [mask] - add new mask to driver\n" <<
                "remove [mask] - remove mask from driver\n" <<
                "view - view list of masks in driver\n" <<
                "clear - remove all masks from driver\n";
            return 0;

        case Command::Add:
            if (argc != 3) {
                std::cerr << "Wrong usage: faf add [mask]\n";
                return 1;
            }

            AddMask(argv[2]);

            std::cout << "Added.\n";
            return 0;

        case Command::Remove:
            if (argc != 3) {
                std::cerr << "Wrong usage: faf remove [mask]\n";
                return 1;
            }

            RemoveMask(argv[2]);

            std::cout << "Removed.\n";
            return 0;

        case Command::View:
            std::cout << "PLACEHOLDER\n";

            return 0;

        default:
            std::cerr << "Unknown command. Use 'faf help' for command list.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
