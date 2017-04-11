/*
* Created by Mark on 4/11/2017.
*/

#ifndef DT1_RECTANGLE_H
#define DT1_RECTANGLE_H

#include <experimental/optional>
#include "shape.hpp"
#include "vector2.hpp"

namespace RP {
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
}

#endif //DT1_RECTANGLE_H
