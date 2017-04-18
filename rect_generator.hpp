#ifndef RECT_GENERATOR
#define RECT_GENERATOR
#include "name_generator_ioc_container.hpp"
#include "grid.hpp"
namespace RP {
    template <class T>
    class RectGenerator {
        NameGenerator nameGenerator;
    public:
        RectGenerator() : nameGenerator(NameGeneratorIOC::getInstance().constructNameGenerator()) {}

        void addRandomRectangleToGrid(const Grid<T>& grid) {
            std::string name;
            while (true) {
                name = nameGenerator.generateName();
                bool nameAvailable = !grid.findRectangleByName(name).has_value();
                if (nameAvailable) {
                    break;
                }
            }

            Vector2<T> topRight;
            Vector2<T> bottomLeft;

            topRight.x = rand() % (grid.width + 1);
            topRight.y = rand() % (grid.height + 1);

            bottomLeft.x = rand() % (topRight.x);
            bottomLeft.y = rand() % (topRight.y);
            grid.addRectangle( { bottomLeft, topRight, name } );
        }

    };
}
#endif
