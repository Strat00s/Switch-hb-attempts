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

void PrintEntries(Entry **menu){
    if ((*menu)->data != NULL){
        printf("%s", (*menu)->data);
    }
    if ((*menu)->func != NULL){
        (*menu)->ret = (*menu)->func();
    }
    else if (!(*menu)->is_item){
        for(int i = (*menu)->pos; i < (*menu)->max+1; i++){    // print (*menu) items
            printf("\x1b[%d;1H  %s", i, (*menu)->labels[i-(*menu)->min]->name);
        }
    }
}

void MoveCrusor(Entry **menu){
    if ((*menu)->pos != 0){
        if (kDown & KEY_UP && (*menu)->pos > (*menu)->min){
            ((*menu)->pos)--;
        }
        if (kDown & KEY_DOWN && (*menu)->pos < (*menu)->max){
            ((*menu)->pos)++;
        }
        printf("\x1b[%d;1H%c", (*menu)->pos, 16);// print cursor
    }
}

void Select(Entry **menu){
    if (kDown & KEY_A && (*menu)->labels != NULL){
        *menu = (*menu)->labels[(*menu)->pos-(*menu)->min];
        printf(CONSOLE_ESC(2J));
    }
}

void GoBack(Entry **menu){
    if (kDown & KEY_B && (*menu)->prev != NULL){
        *menu = (*menu)->prev;
        printf(CONSOLE_ESC(2J));
    }
}

int DefaultExitFunction(){
    return 1;
}


int main(){
    Entry default_menu = InitStructMenu("default menu", 4, 2, NULL);
    Entry help_item = InitStructItem("Help", "\x1b[20;35HThis is a simple help.", &default_menu);
    Entry other_menu = InitStructMenu("Other submenu", 5, 1, &default_menu);
    Entry exit_item = InitStructItem("Exit", NULL, &default_menu);

    exit_item.func = DefaultExitFunction;

    default_menu.labels[0] = &help_item;
    default_menu.labels[1] = &other_menu;
    default_menu.labels[2] = &exit_item;

    default_menu.data = "\x1b[1;1HMain Menu";

    Entry *mover = &default_menu;

    consoleInit(NULL);

    while(appletMainLoop()){

        hidScanInput();

        kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        
        // exit with PLUS
        if (kDown & KEY_PLUS || exit_item.ret){
            break;
        }

        PrintEntries(&mover);
        MoveCrusor(&mover);
        Select(&mover);
        GoBack(&mover);

        printf(CONSOLE_ESC(20;20H)"%d", rand()%1000);
        printf(CONSOLE_ESC(21;20H)"done: %d", done);
        printf(CONSOLE_ESC(22;20H)"pos: %d", default_menu.pos);

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    free(default_menu.labels);
    return 0;
}
