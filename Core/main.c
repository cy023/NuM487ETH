#include <stdio.h>
#include "NuMicro.h"
#include "system.h"

int main()
{
    system_init();
    printf("Hello World! Hello Nuvoton!\n");
    printf("Here is Nuvoton M487KMCAN.\n");

    while (1)
        ;
}
