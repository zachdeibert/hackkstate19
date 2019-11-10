#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <config.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <twilio.h>

config_t parametersoutput;
const int opto = 29;
const int timerinterval = 300;
char* readfile;
int fd;
time_t lasttime;

/* init_wiringpi - initializes the wiringpi library on the optointerruptor pin (Pin 40 on the Pi) */
void init_wiringpi(void);

/* init_interrupt - enables the wiringpi interrupt service routine on the rising edge of the optointerruptor data pin*/
void init_interrupt(void);

/*  pause_print - check the local time versus the last time the interrupt fired in intervals of 5 minutes
    this makes sure the Gcode commands do not pause and unpause every time the ISR fires once the filament is removed. */
void pause_print(void);

/* openfile - opens the serial port communication between the RPi and 3D Printer, 
    Parameters:
        baudrate - the baud rate of the printer
        serialportfile - the port file to open
*/
void openfile(uint32_t, char*);

/* readthefile - reads the Gcode file to run commands in the event of a pause.
    Parameters:
        readthefile - Gcode file 
*/
void readthefile(char*);

int main(int parameter_num, char** values) {
    parametersoutput = config_settings(parameter_num,values);
    if( parametersoutput.printhelp == 1){
        printhelp();
        return EXIT_FAILURE;
    }
    readthefile(parametersoutput.gcode_file);
    openfile(parametersoutput.baud,parametersoutput.port);
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
    time_t t = time(NULL);
    if((t - lasttime) >= timerinterval)
    {
        write(fd,readfile,strlen(readfile));
        printf("Paused.");
        twilio_send_message(parametersoutput.account_sid,parametersoutput.auth_token,"SOS! Your 3D printer has run out of filament!",parametersoutput.from_number,parametersoutput.to_number,NULL,FALSE);
    }
    lasttime = t;
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

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;                
                                        
    tty.c_oflag = 0;                
    tty.c_cc[VMIN]  = 0;            
    tty.c_cc[VTIME] = 5;            

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); 

    tty.c_cflag |= (CLOCAL | CREAD);
                                    
    tty.c_cflag &= ~(PARENB | PARODD);      
    tty.c_cflag |= 0;
    tty.c_cflag &= ~CSTOPB;
    
    if(tcsetattr(fd,TCSANOW,&tty) != 0){
        perror("tcsetattr");
        exit(1);
    }
}

void readthefile(char* gcode_file){
    FILE *fptr = fopen(gcode_file,"r");
    if(fptr == NULL){
        perror("fopen");
        exit(1);
    }
    fseek(fptr,0,SEEK_END);
    int filesize = ftell(fptr);

    fseek(fptr,0,SEEK_SET);
    readfile = malloc(filesize+1);

    fread(readfile,1,filesize,fptr);
    readfile[filesize] = 0;
    fclose(fptr);
}




