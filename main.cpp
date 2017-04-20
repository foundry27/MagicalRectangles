#include <iostream>
#include <limits>
#include <locale>
#include <fstream>
#include <vector>
#include <sstream>
#include <exception>
#include <ctime>
#include "rp/vector2.hpp"
#include "rp/shape.hpp"
#include "rp/rectangle.hpp"
#include "rp/grid.hpp"
#include "rp/rect_generator.hpp"

static inline void clearScreen() {
    system("clear");
}

static inline void promptForEnterKeyAndAdvanceLine() {
    std::cout << "Press enter to continue." << std::flush;
    system("read");
    std::cout << std::endl;
}

static void printRectangles(RP::Grid<int>& grid) {
    std::cout << "Rectangles currently present in grid\n------------------------------\n";
    grid.forEach([](const RP::Rectangle<int>& r) {
       std::cout << "\"" << r.name << "\" - " << r.bottomLeft.toString() << " - " << r.topRight.toString() << std::endl;
    });
    std::cout << "------------------------------\n";
}

static void addRectangleToGrid(RP::Grid<int>& grid) {
    int lowerLeftX, lowerLeftY, upperRightX, upperRightY;
    std::string name;

    for (;;) {
        for (;;) {
            std::cout << "Enter the X coordinate of the lower left corner of the rectangle: ";
            if (!(std::cin >> lowerLeftX)) {
                std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        for (;;) {
            std::cout << "Enter the Y coordinate of the lower left corner of the rectangle: ";
            if (!(std::cin >> lowerLeftY)) {
                std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        for (;;) {
            std::cout << "Enter the X coordinate of the upper right corner of the rectangle: ";
            if (!(std::cin >> upperRightX)) {
                std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        for (;;) {
            std::cout << "Enter the Y coordinate of the upper right corner of the rectangle: ";
            if (!(std::cin >> upperRightY)) {
                std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                break;
            }
        }
        for (;;) {
            std::cout << "Enter the name of this rectangle: ";
            std::cin >> name;

            if (name.length() != 4) {
                std::cout << "Rectangle name length must be four characters long." << std::endl;
                promptForEnterKeyAndAdvanceLine();
                continue;
            }
            for (const auto& c : name) {
                if (!std::islower(c, {})) {
                    std::cout << "Character '" << c << "' in rectangle name \"" << name << "\" must be a lowercase letter." << std::endl;
                    promptForEnterKeyAndAdvanceLine();
                    goto LOOP_EXIT;
                }
            }

            try {
                grid.addRectangle(RP::Rectangle<int> {{lowerLeftX, lowerLeftY}, {upperRightX, upperRightY}, std::move(name)});
            } catch (const RP::IllegalSizeError& e) {
                std::cout << "Illegal size specified: " + e.what + "\n";
                promptForEnterKeyAndAdvanceLine();
                break;
            } catch (const RP::IllegalNameError& e) {
                std::cout << "Illegal name specified: " + e.what + "\n";
                promptForEnterKeyAndAdvanceLine();
                break;
            }
            std::cout << "Successfully added rectangle \"" << name << "\" to grid!" << std::endl;
            promptForEnterKeyAndAdvanceLine();
            printRectangles(grid);
            return;

            LOOP_EXIT:;
        }
    }
}

static void removeRectangleFromGrid(RP::Grid<int>& grid) {
    std::cout << "Enter the name of this rectangle: ";
    std::string name;
    std::cin >> name;
    if (!grid.removeRectangleByName(name)) {
        std::cout << "No rectangle with name \"" + name + "\" exists in the grid.\n";
    } else {
        std::cout << "Removed rectangle \"" + name +"\".";
    }
    promptForEnterKeyAndAdvanceLine();
    printRectangles(grid);
}

static void getRectangleIntersection(RP::Grid<int>& grid) {
    if (grid.size() < 2) {
        std::cout << "Need at least 2 rectangles to perform this action." << std::endl;
        return;
    }
    for (;;) {
        std::cout << "Enter the name of the first rectangle: ";
        std::string firstRectName;
        std::cin >> firstRectName;
        if (auto firstRect = grid.findRectangleByName(firstRectName)) {
            for (;;) {
                std::cout << "Enter the name of the second rectangle: ";
                std::string secondRectName;
                std::cin >> secondRectName;
                if (auto secondRect = grid.findRectangleByName(secondRectName)) {
                    if (auto intersection = firstRect -> findIntersectionRectangle(*secondRect)) {
                        std::cout << "Found intersection: " << intersection -> toString() << "\n";
                    } else {
                        std::cout << "No intersection found!" << "\n";
                    }
                    return;
                } else {
                    std::cout << "No rectangle with name \"" << secondRectName << "\" exists in the grid.\n";
                    promptForEnterKeyAndAdvanceLine();
                    clearScreen();
                    continue;
                }
            }
        } else {
            std::cout << "No rectangle with name \"" << firstRectName << "\" exists in the grid.\n";
            promptForEnterKeyAndAdvanceLine();
            clearScreen();
            continue;
        }
    }
}

static void getRectangleUnion(RP::Grid<int>& grid) {
    if (grid.size() < 2) {
        std::cout << "Need at least 2 rectangles to perform this action." << std::endl;
        return;
    }
    for (;;) {
        std::cout << "Enter the name of the first rectangle: ";
        std::string firstRectName;
        std::cin >> firstRectName;
        if (auto firstRect = grid.findRectangleByName(firstRectName)) {
            for (;;) {
                std::cout << "Enter the name of the second rectangle: ";
                std::string secondRectName;
                std::cin >> secondRectName;
                if (auto secondRect = grid.findRectangleByName(secondRectName)) {
                    auto intersection = firstRect -> getUnionRectangle(*secondRect);
                    std::cout << "Found union: " << intersection.toString() << "\n";
                    return;
                } else {
                    std::cout << "No rectangle with name \"" << secondRectName << "\" exists in the grid.\n";
                    promptForEnterKeyAndAdvanceLine();
                    clearScreen();
                    continue;
                }
            }
        } else {
            std::cout << "No rectangle with name \"" << firstRectName << "\" exists in the grid.\n";
            promptForEnterKeyAndAdvanceLine();
            clearScreen();
            continue;
        }
    }
}

static void checkIfPointInRectangle(const RP::Grid<int>& grid) {
    if (grid.size() < 1) {
        std::cout << "Need at least 1 rectangle to perform this action." << std::endl;
        return;
    }
    for (;;) {
        std::cout << "Enter the name of the rectangle to check: ";
        std::string rectName;
        std::cin >> rectName;
        if (auto rectLookup = grid.findRectangleByName(rectName)) {
            int posX, posY;
            for (;;) {
                std::cout << "Enter the X coordinate of the point to check: ";
                if (!(std::cin >> posX)) {
                    std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
                }
            }
            for (;;) {
                std::cout << "Enter the Y coordinate of the point to check: ";
                if (!(std::cin >> posY)) {
                    std::cout << "Illegal input! Input must be a positive integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
                }
            }
            if (rectLookup -> containsPoint({posX, posY})) {
                std::cout << "Point was within rectangle!" << std::endl;
            } else {
                std::cout << "Point was not within rectangle!" << std::endl;
            }
            return;
        } else {
            std::cout << "No rectangle with name \"" << rectName << "\" exists in the grid.\n";
            promptForEnterKeyAndAdvanceLine();
            clearScreen();
            continue;
        }
    }
}

static void createRandomRectangles(RP::Grid<int>& grid) {
    int input;
    while (true) {
        std::cout << "Please enter the amount of rectangles to be randomly created: ";
        if (!(std::cin >> input)) {
            std::cout << "Illegal input: must be a number between 0 and 25" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (input < 0 || input > 25) {
            std::cout << "Illegal input: " << input << " is not a number between 0 and 25" << std::endl;
        }
        else {
            break;
        }
    }
    RP::RectGenerator<int> rectGenerator;
    for (unsigned int i = 0; i < input; i++) {
        rectGenerator.addRandomRectangleToGrid(grid);
    }
}

namespace {

    struct IllegalFormatError {
        const std::string what;
    };

}

std::vector<std::string> stringSplit(const std::string& str, const char delim) {
    std::vector<std::string> strings;
    std::istringstream iss(str);

    for (std::string s; std::getline(iss, s, delim);) {
        strings.push_back(s);
    }
    return strings;
}

std::string trimString(const std::string& str, char c) {
    std::string trimmed;
    std::string::const_iterator i;
    for (i = str.begin(); i != str.end() && *i == c; i++);
    for (; i != str.end() && *i != c; i++) trimmed += *i;
    return trimmed;
}

RP::Vector2<int> stringToPoint(const std::string& str) {
    std::string trimmed = trimString(trimString(str, '('), ')');
    std::vector<std::string> coordinates = stringSplit(trimmed, ',');
    try {
        return { std::stoi(coordinates[0], nullptr, 10), std::stoi(coordinates[1], nullptr, 10) };
    } catch (const std::invalid_argument& e) {
        throw IllegalFormatError();
    } catch (const std::out_of_range& e) {
        throw IllegalFormatError();
    }
}

static RP::Rectangle<int> parseRectString(const std::string& str) {
    std::vector<std::string> splitString = stringSplit(str, ';');
    if (splitString.size() != 3) {
        throw IllegalFormatError();
    }
    if (splitString.at(0).size() != 4) {
        throw IllegalFormatError();
    }
    RP::Vector2<int> bottomLeft = stringToPoint(splitString.at(1));
    RP::Vector2<int> topRight = stringToPoint(splitString.at(2));

    return { std::move(bottomLeft), std::move(topRight), std::move(splitString.at(0)) };
}

static std::vector<RP::Rectangle<int>> readRectanglesFromStream(std::istream& in) {
    std::vector<RP::Rectangle<int>> rectangles;
    for (std::string input; std::getline(in, input, '\n');) {
        try {
            rectangles.push_back(parseRectString(input));
        } catch (const IllegalFormatError& e) {
            std::cout << "Line " << input << " ignored due to illegal format." << std::endl;
        }
    }
    return rectangles;
}

static void readRectanglesFromStreamToGrid(std::istream&& in, RP::Grid<int>& grid) {
    std::vector<RP::Rectangle<int>> rectangles = readRectanglesFromStream(in);

    for (const auto& rect : rectangles) {
        try {
            grid.addRectangle(std::move(rect));
        } catch (const RP::IllegalNameError& e) {
            std::cout << "Rectangle " << rect.toString() << " was ignored due to a name conflict." << std::endl;

        } catch (const RP::IllegalSizeError& e) {
            std::cout << "Rectangle " << rect.toString() << " was ignored because it has illegal size:\n" << e.what << std::endl;
        }
    }
}

static void readRectanglesFromUserFile(RP::Grid<int>& grid) {
    while (true) {
        std::string input;
        std::cout << "Please enter a valid file name: ";
        std::cin >> input;
        std::ifstream file(input);
        if (!file) {
            std::cout << "File name \"" << input << "\" invalid: No such file exists." << std::endl;
        }
        else {
            readRectanglesFromStreamToGrid(std::move(file), grid);
            break;
        }
    }
}

int main() {
    srand(time(nullptr));
    RP::Grid<int> grid{600, 400};
    clearScreen();
    createRandomRectangles(grid);
    clearScreen();
    for (;;) {
        std::cout << "Rectangle Program Menu Options\n------------------------------\n"
                  << "1. Print rectangles\n"
                  << "2. Add Rectangle\n"
                  << "3. Remove Rectangle\n"
                  << "4. Find rectangle intersection\n"
                  << "5. Find rectangle union\n"
                  << "6. Sort rectangles by name\n"
                  << "7. Check if point in rectangle\n"
                  << "8. Load rectangles from file name\n"
                  << "9. Quit\n";
        std::cout << "Enter an option: ";
        int input;
        if (!(std::cin >> input)) {
            std::cout << "Illegal input: must be a number between 1 and 9." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            switch (input) {
                case 1:
                    printRectangles(grid);
                    break;
                case 2:
                    addRectangleToGrid(grid);
                    break;
                case 3:
                    removeRectangleFromGrid(grid);
                    break;
                case 4:
                    getRectangleIntersection(grid);
                    break;
                case 5:
                    getRectangleUnion(grid);
                    break;
                case 6:
                    std::cout << "All rectangles sorted." << std::endl;
                    break;
                case 7:
                    checkIfPointInRectangle(grid);
                    break;
                case 8:
                    try {
                        readRectanglesFromUserFile(grid);
                    } catch (...) {
                        const auto e = std::current_exception();
                        try {
                            std::rethrow_exception(e);
                        } catch (const std::exception& e2) {
                            std::cout << e2.what() << std::endl;
                        }
                    }
                    break;
                case 9:
                    std::cout << "Goodbye.";
                    return 0;
                default:
                    std::cout << "Illegal option '" << input << "': Must be between 1 and 9." << std::endl;
                    break;
            }
        }
        promptForEnterKeyAndAdvanceLine();
        clearScreen();
    }
}
