#include "app/app.h"

int main() {
    App app;
    app.init("SuperVK", WINDOWED);
    app.run();
    app.destroy();

    return 0;
}
