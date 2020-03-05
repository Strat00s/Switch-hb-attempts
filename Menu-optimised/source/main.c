#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/init.h"

// Include the main libnx system header, for Switch development
#include <switch.h>

// Macros
#define kDown hidKeysDown(CONTROLLER_P1_AUTO)
#define IS_PRESSED(KEY) kDown & KEY
#define CLEAR() printf(CONSOLE_ESC(2J))
#define PRINT(row, col, fmt, ...) printf("\x1b["#row #col"H", fmt, ...)

void PrintEntries(Entry **menu){
    if ((*menu)->data != NULL){
        printf("%s", (*menu)->data);
    }
    if ((*menu)->func != NULL){
        (*menu)->ret = (*menu)->func();
    }
    if (!(*menu)->is_item){
        for(int i = (*menu)->min; i < (*menu)->max+1; i++){    // print (*menu) items
            printf("\x1b[%d;1H  %s", i, (*menu)->labels[i-(*menu)->min]->name);
        }
    }
}

void MoveCursor(Entry **menu){
    if ((*menu)->pos != 0){
        if (IS_PRESSED(KEY_UP) && (*menu)->pos > (*menu)->min){
            ((*menu)->pos)--;
        }
        if (IS_PRESSED(KEY_DOWN) && (*menu)->pos < (*menu)->max){
            ((*menu)->pos)++;
        }
        printf("\x1b[%d;1H%c", (*menu)->pos, 16);// print cursor
    }
}

void Select(Entry **menu){
    if ((IS_PRESSED(KEY_A)) && (*menu)->labels != NULL){
        if ((*menu)->data == NULL && (*menu)->func == NULL){
            printf("\x1b[%d;%ldHThis entry is empty!", (*menu)->pos, strlen((*menu)->labels[(*menu)->pos-(*menu)->min]->name)+7);
        }
        else {
            *menu = (*menu)->labels[(*menu)->pos-(*menu)->min];
            CLEAR();
        }
    }
}

void GoBack(Entry **menu){
    if (IS_PRESSED(KEY_B) && (*menu)->prev != NULL){
        (*menu)->pos = (*menu)->min;
        *menu = (*menu)->prev;
        CLEAR();
    }
}

int DefaultExitFunction(){
    return 1;
}

int GoToExit(){
    if (IS_PRESSED(KEY_B)){
        default_menu.pos = default_menu.max;
    }
    return 0;
}

int main(){

    // initialize and set our menus and items
    // default_menu defined in init.h for it's specific function to work
    default_menu = InitStructMenu("default menu", 4, 2, NULL);
    Entry help_item = InitStructItem("Help", "\x1b[20;20HThis is a simple help.", &default_menu);
    Entry other_menu = InitStructMenu("Other submenu", 5, 1, &default_menu);
    Entry exit_item = InitStructItem("Exit", NULL, &default_menu);
    Entry useless1_item = InitStructItem("This", NULL, &other_menu); 
    Entry useless2_item = InitStructItem("menu", NULL, &other_menu);
    Entry useless3_item = InitStructItem("does", NULL, &other_menu);
    Entry useless4_item = InitStructItem("absolutely", NULL, &other_menu);
    Entry useless5_item = InitStructItem("nothing!", NULL, &other_menu);

    other_menu.labels[0] = &useless1_item;
    other_menu.labels[1] = &useless2_item;
    other_menu.labels[2] = &useless3_item;
    other_menu.labels[3] = &useless4_item;
    other_menu.labels[4] = &useless5_item;

    exit_item.func = DefaultExitFunction;

    default_menu.labels[0] = &help_item;
    default_menu.labels[1] = &other_menu;
    default_menu.labels[2] = &exit_item;

    default_menu.data = "\x1b[1;1HMain Menu";

    default_menu.func = GoToExit;    

    // pointer to menus and items
    Entry *mover = &default_menu;

    consoleInit(NULL);

    while(appletMainLoop()){

        hidScanInput();
        
        // exit with PLUS or when we chose exit from menu
        if (IS_PRESSED(KEY_PLUS) || exit_item.ret){
            break;
        }

        PrintEntries(&mover);
        MoveCursor(&mover);
        Select(&mover);
        GoBack(&mover);

        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    free(default_menu.labels);
    free(other_menu.labels);
    return 0;
}