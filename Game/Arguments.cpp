#include "Arguments.h"

#include <cstring>

/* Parser
-------------------- */

Arguments::Arguments(const char* pCmdLine) {
    size_t length = strlen(pCmdLine);

    // Find argc
    size_t argc = 0;
    for (size_t x = 0; x < length; ++x) {
        // Char matches delim, or end of command
        if (pCmdLine[x] == CLI_ARG_DELIM || x == length - 1) {
            ++argc;
        }
    }

    // Parse argv
    char** argv = new char*[argc];
    if (argc > 0) {
        int argIndex = 0; // The argv index to place the next argument parsed in
        int argStart = 0; // The string index just after the end of the previous argument parsed

        for (size_t x = 0; x < length; ++x) {
            if (x == length - 1) ++x; // Simulate an extra delim char on the end

            // Keep going if we're not at the end of an argument
            if (pCmdLine[x] != CLI_ARG_DELIM && x != length) continue;

            int argumentLen = (x - argStart);
            int argumentArrLen = argumentLen + 1;
            char* argument = new char[argumentArrLen];

            // Eg.
            // pCmdLine = |p1 -p 3\0|
            //   indexes: |0123456 7|
            // 
            //      argv index: 0 | 1 | 2
            //                  --+---+--
            //       argStart = 0 | 3 | 6 (arg start char index)
            //              x = 2 | 5 | 7 (arg end char index)
            //    argumentLen = 2 | 2 | 1 (2 - 0 = 2; 5 - 3 = 2; 7 - 6 = 1)
            // argumentArrLen = 3 | 3 | 2 (argumentLen + 1)

            // Copy the argument into its own string
            memcpy(argument, pCmdLine + argStart, argumentLen);
            /*int offset = 0;
            for (; offset < argumentLen; ++offset) {
                argument[offset] = pCmdLine[argStart + offset];
            }*/
            argument[argumentLen] = '\0'; // Ensure the string is null-terminated

            // Put the argument into argv and move to the next argument
            argv[argIndex] = argument;
            argIndex++;
            argStart = x + 1;
        }
    }

    _argc = argc;
    _argv = argv;
}

Arguments::~Arguments() {
    for (size_t arg = 0; arg < _argc; arg++) {
        delete[] _argv[arg];
    }
    delete[] _argv;
}

/* Raw Getters
-------------------- */

const size_t Arguments::argc() { return _argc; }
const char *const * Arguments::argv() { return _argv; }

/* Argument Checkers
-------------------- */

bool Arguments::contains(const char* argument) {
    for (size_t arg = 0; arg < _argc; arg++) {
        if (strcmp(_argv[arg], argument) == 0) {
            return true;
        }
    }
    return false;
}
