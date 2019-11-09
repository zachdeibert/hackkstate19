#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

const int opto = 29;

void init_wiringpi(void);
void init_interrupt(void);
void print_val(void);

int main(void) {
    // printf("Hello World \n");
    init_wiringpi();
    init_interrupt();
    return EXIT_SUCCESS;
}

void init_wiringpi(void) {
    wiringPiSetup();
    pinMode(opto,INPUT);

}

void init_interrupt(void) {
    wiringPiISR(opto, INT_EDGE_FALLING, print_val);
}

void print_val(void){
    int value = digitalRead(opto);
    printf("The current values is: %d\n",value);

}



