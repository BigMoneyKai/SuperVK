#include "defines.h"
#include "debug/debugger.h"
#include "core/container/array.h"

int main() {
    Array<i32> array;
    for(i32 i = 0; i < 32; i++) {
        array.push_back(i);
    }
    for(auto element : array) {
        INFO("Element %d", element);
    }

    return 0;
}
