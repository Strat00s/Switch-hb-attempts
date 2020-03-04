#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "init.c"

#define STATUS_POS CONSOLE_ESC(10;28H)
#define CHARGER_POS CONSOLE_ESC(11;29H)

u64 kDown;

int main_menu_pos = 1;
char *main_menu_labels[] = {"Help", "Console Info", "Exit", "Shutdown", "Reboot"};
bool main_menu_aval = true;

int info_menu_pos = 1;
char *info_menu_labels[] = {"System", "Hardware","Button test", "Other"};
bool info_menu_aval = true;

int hardware_menu_pos = 1;
char *hardware_menu_labels[] = {"Battery", "Storage", "Controllers", "LCD", "USB", "Other?"};
bool hardware_menu_aval = true;


// move cursor
int MoveCursor(int *pos, int max, int min){
    if (kDown & KEY_DOWN && *pos < max){
        (*pos)++;
    }
    if (kDown & KEY_UP && *pos > min){
        (*pos)--;
    }
    printf("\x1b[%d;1H%c", *pos, 16);
    return *pos;
}

// print menu items
void PrintItems(char **items, int size){
    for (int i = 0; i < size; i++){
        printf("\x1b[%d;1H  %s", i+1, items[i]);
    }
}

// exit from a menu and reset cursor possition for said menu
bool Exit(int *position){
    if (kDown & KEY_B){
        if (position != NULL){
            *position = 1;
        }
        printf(CONSOLE_ESC(2J));
        return true;
    }
    else {
        return false;
    }
}

int ConfigMenu(){
    return 2;
}

int main(int argc, char *argv[]){

    consoleInit(NULL);

    while(appletMainLoop()){
        
        hidScanInput();
        kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        
        // exit with PLUS
        if (kDown &KEY_PLUS){
            break;
        }

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    return 0;
}
