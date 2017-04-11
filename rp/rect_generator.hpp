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

        void addRandomRectangleToGrid(Grid<T>& grid) {
            std::string name;
            while (true) {
                name = nameGenerator.generateName();
                bool nameAvailable = !grid.findRectangleByName(name);
                if (nameAvailable) {
                    break;
                }
            }

            Vector2<T> topRight{rand() % (grid.getWidth() + 1), rand() % (grid.getHeight() + 1)};
            Vector2<T> bottomLeft{rand() % (topRight.x), rand() % (topRight.y)};

            grid.addRectangle( Rectangle<T>{
                std::move(bottomLeft), std::move(topRight), std::move(name) } );
        }

    };
}
#endif
