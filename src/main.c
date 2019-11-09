#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <config.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

const int opto = 29;
int fd;

void init_wiringpi(void);
void init_interrupt(void);
void pause_print(void);
void openfile(uint32_t, char*);

int main(int parameter_num, char** values) {
    config_t parametersoutput = config_settings(parameter_num,values);
    if( parametersoutput.printhelp == 1){
        printhelp();
        return EXIT_FAILURE;
    }
    openfile(parametersoutput.baud,parametersoutput.port);
    pause_print();
    init_wiringpi();
    init_interrupt();
    while(1){
        sleep(50000);
    }
    return EXIT_SUCCESS;
}

void init_wiringpi(void) {
    wiringPiSetup();
    pinMode(opto,INPUT);

}

void init_interrupt(void) {
    wiringPiISR(opto, INT_EDGE_RISING, pause_print);
}

void pause_print(void){
    char* string = "G0 Z10\n";
    write(fd,string,strlen(string));
}

void openfile(uint32_t baudrate, char* serialportfile){
    fd = open(serialportfile,O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if(tcgetattr(fd,&tty) != 0){
        perror("tcgetattr");
        exit(1);
    }
    int speedthing;
    switch (baudrate)
    {
    case 0:
        speedthing = B0;
        break;
    case 50:
        speedthing = B50;
        break;
    case 75:
        speedthing = B75;
        break;
    case 110:
        speedthing = B110;
        break;
    case 134:
        speedthing = B134;
        break;
    case 150:
        speedthing = B150;
        break;
    case 200:
        speedthing = B200;
        break;
    case 300:
        speedthing = B300;
        break;
    case 600:
        speedthing = B600;
        break;
    case 1200:
        speedthing = B1200;
        break;
    case 1800:
        speedthing = B1800;
        break;
    case 2400:
        speedthing = B2400;
        break;
    case 4800:
        speedthing = B4800;
        break;
    case 9600:
        speedthing = B9600;
        break;
    case 19200:
        speedthing = B19200;
        break;
    case 38400:
        speedthing = B38400;
        break;
    case 57600:
        speedthing = B57600;
        break;
    case 115200:
        speedthing = B115200;
        break;
    case 230400:
        speedthing = B230400;
        break;
    }
    cfsetospeed(&tty,speedthing);
    cfsetispeed(&tty,speedthing);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= 0;
        tty.c_cflag &= ~CSTOPB;
        
        if(tcsetattr(fd,TCSANOW,&tty) != 0){
            perror("tcsetattr");
            exit(1);
        }
}





