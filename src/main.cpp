#include "app/app.h"

int main() {
    App app;
    app.init();
    app.run();
    app.destroy();
    return 0;
}
