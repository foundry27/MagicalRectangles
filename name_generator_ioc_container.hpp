#ifndef NAME_GENERATOR_IOC
#define NAME_GENERATOR_IOC
#include "name_generator.hpp"

namespace RP {
    class NameGeneratorIOC {
        static NameGeneratorIOC instance;
        NameGeneratorIOC();
    public:
        static NameGeneratorIOC& getInstance() noexcept;
        NameGenerator constructNameGenerator() const;
    };
}

#endif
