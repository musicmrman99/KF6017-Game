#pragma once

#include <cstdlib>

class Arguments {
private:
    constexpr static const char CLI_ARG_DELIM = ' ';

    size_t _argc;
    char** _argv;



public:
    Arguments(const char* command);
    Arguments(const Arguments&) = delete;
    Arguments& operator=(const Arguments&) = delete;
    ~Arguments();

    const size_t argc();
    const char *const * argv();

    /**
     * Return true if this set of Arguments contains the given
     * argument, or false otherwise.
     */
    bool contains(const char* argument);
};
