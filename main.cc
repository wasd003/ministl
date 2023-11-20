#include <ministl/log.h>

int main() {
    debug("nothing");
    int *a = new int();
    debug("nothing:", 1, " ", a);
    return 0;
}
