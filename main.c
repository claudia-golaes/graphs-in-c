#include <stdio.h>
#include <stdlib.h>
#include "cerinta1.h"
#include "cerinta2.h"

int main(int argc, char **argv)
{
    if(atoi(argv[1])==1)
    {
        main_cerinta1();
    }
    if(atoi(argv[1])==2)
    {
        main_cerinta2();
    }
    return 0;
}