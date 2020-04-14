#include "logger.h"

int main() {

    infof("testing infof: there are %d melons", 3);
    warnf("testing warnf: there are %d pineapples", 4);
    errorf("testing errorf: there are %d apples", 10);
    panicf("testing panicf: there are %d grapes", 500);

    return 0;
}
