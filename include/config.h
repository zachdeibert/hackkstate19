#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <linux/limits.h>

typedef struct {
    uint32_t baud;
    char port [PATH_MAX];
    char gcode_file [PATH_MAX];
    int printhelp;
    char account_sid [35];
    char auth_token [33];
    char from_number [13];
    char to_number [13];
} config_t;

/* config_settings - configures the linux commands to move over for easier use in C
    Parameters:
        parameter_num - number of parameters entered on the linux machine
        values - independent linux commands
        */
config_t config_settings(int parameter_num, char** values);

/* printhelp - prints a list of helpful commands for the user */
void printhelp(void);

#endif