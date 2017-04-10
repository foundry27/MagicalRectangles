#include <string>
#include <map>
#include <type_traits>
#include <functional>
#include <iostream>
#include <experimental/optional>
#include <limits>
#include <locale>

namespace RP {

    template <typename T>
    struct Vector2 {
        const std::enable_if_t<std::is_integral<T>::value, T> x, y;

        const std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };

    template <typename T>
    struct Shape {
        virtual const T getPerimeter() const noexcept = 0;

        virtual const T getArea() const noexcept = 0;
    };

    template <typename T>
    struct Rectangle : Shape<T> {
        const Vector2<T> bottomLeft, topRight;

        const std::string name;

        Rectangle(const Vector2<T>&& bottomLeft, const Vector2<T>&& topRight, const std::string&& name)
                : bottomLeft(bottomLeft), topRight(topRight), name(name) {}

        virtual const T getPerimeter() const noexcept override {
            return 2 * ((topRight.x - bottomLeft.x) + (topRight.y - bottomLeft.y));
        }

        virtual const T getArea() const noexcept override {
            return (topRight.x - bottomLeft.x) * (topRight.y - bottomLeft.y);
        }

        const Rectangle<T> getUnionRectangle(const Rectangle<T>& rect) const noexcept {
            return Rectangle<T> {{std::min(bottomLeft.x, rect.bottomLeft.x), std::min(bottomLeft.y, rect.bottomLeft.y)},
                                 {std::max(topRight.x, rect.topRight.x), std::max(topRight.y, rect.topRight.y)},
                                 "<" + name + " + " + rect.name + ">"};
        }

        const std::experimental::optional<Rectangle<T>> findIntersectionRectangle(const Rectangle<T>& rect) const noexcept {
            const T x5 = std::max(bottomLeft.x, rect.bottomLeft.x);
            const T x6 = std::min(topRight.x, rect.topRight.x);

            if (x5 > x6) return {};

            const T y5 = std::max(bottomLeft.y, rect.bottomLeft.y);
            const T y6 = std::min(topRight.y, rect.topRight.y);

            if (y5 > y6) return {};

            return Rectangle<T> {{x5, y5}, {x6, y6}, "<" + name + " / " + rect.name + ">"};
        }

        const bool containsPoint(const Vector2<T>& point) const noexcept {
            return (point.x >= bottomLeft.x && point.x <= topRight.x) && (point.y >= bottomLeft.y && point.y <= topRight.y);
        }

        const std::string toString() const noexcept {
            return "\"" + name + "\" - " + bottomLeft.toString() + " - " + topRight.toString();
        }
    };

    struct IllegalNameError {
        const std::string what;
    };

    struct IllegalSizeError {
        const std::string what;
    };

    template <typename T>
    class Grid {
    public:
        Grid(const T height, const T width) : height(height), width(width), rects({}) {}

        void forEach(const std::function<void (const Rectangle<T>&)> consumer) const noexcept {
            for (const auto& r : rects) {
                consumer(r.second);
            }
        }

        void addRectangle(const Rectangle<T>&& rect) {
            validateRectangle(rect);
            rects.insert({rect.name, rect});
        }

        const bool removeRectangleByName(const std::string& name) noexcept {
            if (!rects.count(name)) {
                return false;
            } else {
                rects.erase(name);
                return true;
            }
        }

        const std::experimental::optional<Rectangle<T>> findRectangleByName(const std::string& name) const noexcept {
            const auto lookup = rects.find(name);
            if (lookup == rects.end()) {
                return {};
            }
            return lookup -> second;
        }

    private:
        const Rectangle<T>& getRectangleByName(const std::string& name) const {
            const auto lookup = rects.find(name);
            if (lookup == rects.end()) {
                throw IllegalNameError {"No rectangle with name " + name + " exists in this grid"};
            }
            return lookup -> second;
        }

        void validateRectangle(const Rectangle<T>& rect) const {
            if (rects.count(rect.name)) {
                throw IllegalNameError {"A rectangle named " + rect.name + " already exists in this grid"};
            }
            if (rect.bottomLeft.y > rect.topRight.y) {
                throw IllegalSizeError {"Rectangle " + rect.name + " must have a lower left corner with a lower X coordinate than its upper right corner"};
            } else if (rect.bottomLeft.x > rect.topRight.x) {
                throw IllegalSizeError {"Rectangle " + rect.name + " must have a lower left corner with a lower Y coordinate than its upper right corner"};
            }
            if (rect.topRight.x > width) {
                throw IllegalSizeError {"Rectangle " + rect.name + " has width exceeding grid width " + std::to_string(width)};
            } else if (rect.topRight.y > height) {
                throw IllegalSizeError {"Rectangle " + rect.name + " has height exceeding grid height " + std::to_string(height)};
            }
        }

        const T height, width;

        std::map<std::string, Rectangle<T>> rects;
    };
}

inline void clearScreen() {
    system("clear");
}

inline void promptForEnterKeyAndAdvanceLine() {
    std::cout << "Press enter to continue." << std::flush;
    system("read");
    std::cout << std::endl;
}

void printRectangles(RP::Grid<int>& grid) {
    std::cout << "Rectangles currently present in grid\n------------------------------\n";
    grid.forEach([](const RP::Rectangle<int>& r) {
       std::cout << "\"" << r.name << "\" - " << r.bottomLeft.toString() << " - " << r.topRight.toString() << std::endl;
    });
    std::cout << "------------------------------\n";
}

void addRectangleToGrid(RP::Grid<int>& grid) {
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

void removeRectangleFromGrid(RP::Grid<int>& grid) {
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

void getRectangleIntersection(RP::Grid<int>& grid) {
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

void getRectangleUnion(RP::Grid<int>& grid) {
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

void checkIfPointInRectangle(const RP::Grid<int>& grid) {
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

int main() {
    RP::Grid<int> grid{600, 400};
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
                  << "8. Quit\n";
        std::cout << "Enter an option: ";
        int input;
        std::cout << std::endl;
        if (!(std::cin >> input)) {
            std::cout << "\nIllegal input: must be a number between 1 and 8." << std::endl;
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
                    std::cout << "Goodbye.";
                    return 0;
                default:
                    std::cout << "Illegal option '" << input << "': Must be between 1 and 8." << std::endl;
                    break;
            }
        }
        promptForEnterKeyAndAdvanceLine();
        clearScreen();
    }
}