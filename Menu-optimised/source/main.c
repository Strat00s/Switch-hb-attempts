#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include "../include/init.h"
//#include "../include/init.c"


u64 kDown;
int done = 0;

//int main_menu_pos = 1;
//char *main_menu_labels[] = {"Help", "Console Info", "Exit", "Shutdown", "Reboot"};
//bool main_menu_aval = true;
//
//int info_menu_pos = 1;
//char *info_menu_labels[] = {"System", "Hardware","Button test", "Other"};
//bool info_menu_aval = true;
//
//int hardware_menu_pos = 1;
//char *hardware_menu_labels[] = {"Battery", "Storage", "Controllers", "LCD", "USB", "Other?"};
//bool hardware_menu_aval = true;

void PrintEntries(Entry *menu){
    for(int i = menu->pos; i < menu->size+1; i++){    // print menu items
        printf("\x1b[%d;1H  %s", i, menu->labels[i-1]->name);
    }
}

void MoveCrusor(Entry *menu){
    if (menu->pos != 0){
        if (kDown & KEY_UP && menu->pos > 1){
            (menu->pos)--;
        }
        if (kDown & KEY_DOWN && menu->pos < menu->size){
            (menu->pos)++;
        }
        printf("\x1b[%d;1H%c", menu->pos, 16);// print cursor
    }
}

int DefaultExitFunction(){
    if (kDown & KEY_A){
        done = 1;
    }
    done = 0;
    return 0;
}


int main(int argc, char *argv[]){
    Entry default_menu = InitStructMenu("default menu", 3, NULL);
    Entry help_item = InitStructItem("Help", "\x1b[20;35HThis is a simple help.", &default_menu);
    Entry other_menu = InitStructMenu("Other submenu", 5, &default_menu);
    Entry exit_item = InitStructItem("Exit", NULL, &default_menu);

    exit_item.func = DefaultExitFunction;

    default_menu.labels[0] = &help_item;
    default_menu.labels[1] = &other_menu;
    default_menu.labels[2] = &exit_item;

    Entry mover = default_menu;

    consoleInit(NULL);

    while(appletMainLoop()){

        hidScanInput();

        kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        
        // exit with PLUS
        if (kDown & KEY_PLUS || done){
            break;
        }

        /*
        if(mover.is_item)   // is item
            if(mover.data != NULL)  // has data -> print them
            if(mover.func != NULL)  // has function -> execute
            if(kDown & KEY_B)   // go back mover = *mover.prev
        else(!mover.is_item)    // is menu
            if(mover.data != NULL)  // has data -> print them
            print entrie (done)
            move cursor (done)

            if(mover.func != NULL)  // has function -> execute
            if
        */
        if (!mover.is_item){
            PrintEntries(&mover);
            MoveCrusor(&mover);
            if (kDown & KEY_A && mover.labels != NULL){
                mover = *mover.labels[mover.pos-1];
                printf(CONSOLE_ESC(2J));
            }
        }
        else {
            printf("%s", mover.data);
            if (kDown & KEY_B){
                mover = *mover.prev;
                printf(CONSOLE_ESC(2J));
            }
        }

        printf(CONSOLE_ESC(20;20H)"%d", rand()%1000);

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    free(default_menu.labels);
    return 0;
}
