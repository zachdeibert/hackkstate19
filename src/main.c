#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

const int opto = 21;

void init_wiringpi(void);

int main(void) {
    // printf("Hello World \n");
    init_wiringpi();

    while(1)
    {
        int value = digitalRead(opto);
        printf("The current values is: %d\n",value);
    }
    return EXIT_SUCCESS;
}

void init_wiringpi(void) {
    wiringPiSetup();
    pinMode(opto,INPUT);

}



