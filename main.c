#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include "tests.h"

int main(void) {
    SetConsoleOutputCP(65001);
    run_automatic_tests();
    run_demo_tests();
    return 0;
}