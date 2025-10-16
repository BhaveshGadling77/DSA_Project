#include <stdio.h>
int main(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Received from Node.js: %s\n", argv[1]);
    } else {
        printf("No data received.\n");
    }
    return 0;
}
