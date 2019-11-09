#include <config.h>
#include <stdio.h>
#include <string.h>

config_t config_settings(int parameter_num, char** values){
    config_t parameters;
    for(int i = 1; i < parameter_num; i++){
        if(strcmp(values[i],"--baud") == 0) {
            sscanf(values[i+1],"%d",&parameters.baud);
            i++;
        }
        else if(strcmp(values[i],"--port") == 0) {
            sscanf(values[i+1],"%s",parameters.port);
            i++;
        }
        else if(strcmp(values[i],"--gcode") == 0) {
            sscanf(values[i+1],"%s",parameters.gcode_file);
            i++;
        }
        else if(strcmp(values[i],"--help") == 0) {
            parameters.printhelp = 1;
        }
        else{
            printf("Can not handle input: %s.\n",values[i]);
            parameters.printhelp = 1;
        }
    }
    return parameters;
}

void printhelp(void){
    printf("Inputs needed for correct configuration:\n"
    "--baud , Baud Rate, The bod rate of the system \n"
    "--port, Port Configuration, The name of the Serial Port.\n"
    "--gcode, GCODE File, The name of the GCode file to impliment.\n");
}