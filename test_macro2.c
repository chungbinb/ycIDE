#include <windows.h>
#include <stdio.h>

int main() {
#ifdef UNKNOWN
    printf("UNKNOWN defined: %s\n", "UNKNOWN");
#endif
#ifdef IDENTIFIER
    printf("IDENTIFIER defined\n");
#endif
#ifdef NUMBER
    printf("NUMBER defined\n");
#endif
#ifdef STRING
    printf("STRING defined\n");
#endif
#ifdef TRANSPARENT
    printf("TRANSPARENT defined\n");
#endif
    return 0;
}
