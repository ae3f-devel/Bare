#include <Svr/Main.h> 
#include <stdio.h>
#include "port.auto.h"


int main() {
    int a = SvrMain(TEST_PORT);

    if(a) {
        printf("[SvrMain] error occured: %d\n", a);
        return a;
    }

    puts("[SvrMain] started.");
    puts("[SvrMain] Press \'q\' to kill this.");

    while(1) {
        a = getc(stdin);
        if(a == 'q') {
            break;
        }
    }

    puts("[SvrMain] quitted gracefully.");

    SvrExit();
    return 0;
}