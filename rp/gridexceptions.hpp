/*
* Created by Mark on 4/11/2017.
*/

#ifndef DT1_EXCEPTIONS_H
#define DT1_EXCEPTIONS_H

#include <string>

namespace RP {

    struct IllegalNameError {
        const std::string what;
    };

    struct IllegalSizeError {
        const std::string what;
    };
}

#endif //DT1_EXCEPTIONS_H
