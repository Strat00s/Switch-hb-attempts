#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#define STATUS_POS CONSOLE_ESC(10;28H)
#define CHARGER_POS CONSOLE_ESC(11;29H)

u64 kDown;

typedef struct Menu{
    int position;
    bool available;
    int size;
    char *labels[];
} Menu;

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


// each new function = new menu/item
bool ConsoleInfoMenu(){
    if (info_menu_aval){
        PrintItems(info_menu_labels, 4);
        MoveCursor(&info_menu_pos, 4, 1);
        return Exit(&info_menu_pos);
    }
    return false;
}

bool HelpMenu(){
    printf(CONSOLE_ESC(9;20H)  "+-------------------------------------------+");
    printf(CONSOLE_ESC(10;20H) "|This is a simple help.                     |");
    printf(CONSOLE_ESC(11;20H) "+-------------------------------------------+");
    printf(CONSOLE_ESC(12;20H) "|To select items, press A.                  |");
    printf(CONSOLE_ESC(13;20H) "|Press B to go back.                        |");
    printf(CONSOLE_ESC(14;20H) "|To exit, go to main menu and select 'Exit'.|");
    printf(CONSOLE_ESC(15;20H) "+-------------------------------------------+");

    return Exit(NULL);
}

bool MainMenu(){
    if (main_menu_aval){
        PrintItems(main_menu_labels, 5);
        MoveCursor(&main_menu_pos, 5, 1);
        if (kDown & KEY_B){
            main_menu_pos = 3;
        }
    }
    if (kDown & KEY_A || !main_menu_aval){
        switch (main_menu_pos) {
        case 1: printf(CONSOLE_ESC(2J)); main_menu_aval = HelpMenu();break;
        case 2: printf(CONSOLE_ESC(2J)); main_menu_aval = ConsoleInfoMenu(); break;
        case 3: return true;
        case 4: bpcInitialize(); bpcShutdownSystem(); bpcExit();return true;
        case 5: bpcInitialize(); bpcRebootSystem(); bpcExit(); return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]){
    Menu main_menu;
    Menu info_menu;
    Menu hardware_menu;

    consoleInit(NULL);

    while(appletMainLoop()){
        
        hidScanInput();
        kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        
        // exit with PLUS
        if (kDown &KEY_PLUS || MainMenu()){
            break;
        }
        printf(CONSOLE_ESC(1;60H)"DEBUG:");//debug
        printf(CONSOLE_ESC(2;60H)"main_menu_pos: %d", main_menu_pos);//debug
        printf(CONSOLE_ESC(3;60H)"info_menu_pos: %d", info_menu_pos);//debug

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    return 0;
}
