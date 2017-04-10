#ifndef NAME_GENERATOR
#define NAME_GENERATOR
#include <string>
#include <functional>

class NameGenerator {
    std::function<char ()> charProvider;
    size_t len;
public:
    NameGenerator(const std::function<char ()>& charProvider, size_t len);
    virtual std::string generateName();
};

#endif
