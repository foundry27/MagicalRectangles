#ifndef NAME_GENERATOR_IOC
#define NAME_GENERATOR_IOC
#include "name_generator"

namespace RP {
    class NameGeneratorIOC {
        static const NameGeneratorIOC instance;
    public:
        static NameGenerator getInstance();
        NameGenerator constructNameGenerator() const;
    };
}

#endif
