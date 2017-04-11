#include "name_generator.hpp"

namespace RP {
    NameGenerator::NameGenerator(const std::function<char ()>& charProvider, size_t len) : charProvider(charProvider), len(len) {}

    std::string NameGenerator::generateName() {
        std::string name;
        for (size_t i = 0; i < len; i++) {
            name.push_back(charProvider());
        }
        return name;
    }
}
