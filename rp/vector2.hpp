/*
* Created by Mark on 4/11/2017.
*/

#ifndef DT1_VECTOR2_H
#define DT1_VECTOR2_H

#include <string>

namespace RP {
    template <typename T>
    struct Vector2 {
        const std::enable_if_t<std::is_integral<T>::value, T> x, y;

        const std::string toString() const noexcept {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };
}

#endif //DT1_VECTOR2_H
