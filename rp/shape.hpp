/*
* Created by Mark on 4/11/2017.
*/

#ifndef DT1_SHAPE_H
#define DT1_SHAPE_H

namespace RP {
    template <typename T>
    struct Shape {
        virtual const T getPerimeter() const noexcept = 0;

        virtual const T getArea() const noexcept = 0;
    };
}

#endif //DT1_SHAPE_H
