#include "name_generator_ioc_container.hpp"

namespace RP {
    NameGeneratorIOC NameGeneratorIOC::instance;
    NameGeneratorIOC::NameGeneratorIOC() {}
    NameGeneratorIOC& NameGeneratorIOC::getInstance() noexcept {
        return instance;
    }
    NameGenerator NameGeneratorIOC::constructNameGenerator() const {
        auto charProvider = [] ()->char { return static_cast<char>( rand() % ('z' - 'a' + 1) + 'a' ); };
        return NameGenerator(charProvider, 4);
    }
}
