#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    if (argc > 1) {
        int i = atoi(argv[1]);
        int j = atoi(argv[2]);
        printf("result : %d", i + j);
    } else {
        printf("No data received.\n");
    }
    return 0;
}
