#include "PostfixConverter.h"

map<string, int> PostfixConverter::priority = {
    {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"div", 2}, {"mod", 2}, {"_", 3}
};

// test for commit