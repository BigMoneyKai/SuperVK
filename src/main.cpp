#include "defines.h"
#include "core/container/string/string.hpp"
#include "debug/debugger.h"

#include <glm/glm.hpp>
#include <cstdio>
#include <format>
#include <string>
#include <iostream>

int main() {
    // quick smoke test

    String string = "Hello";

    std::cout << string << std::endl;

    return 0;
}
