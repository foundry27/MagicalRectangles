/*
* Created by Mark on 4/11/2017.
*/

#ifndef DT1_GRID_H
#define DT1_GRID_H

#include <functional>
#include <map>
#include "rectangle.hpp"
#include "gridexceptions.hpp"

namespace RP {
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

#endif //DT1_GRID_H
