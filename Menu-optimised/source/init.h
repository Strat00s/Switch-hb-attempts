#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Menu{
    bool is_item;
    char *name;
    char *data;
    bool available;
    int pos;
    int size;
    struct Menu *prev;
    struct Menu **labels;
    int (*func)();
} Menu;

//Menu InitStructMenu(char *name, bool is_item, int size){
//    Menu new_menu = {
//        .name = name,
//        .size = size,
//        .data = NULL,
//        .is_item = is_item,
//        .available = true,
//        .size = size,
//        .prev = NULL,
//        .labels = malloc(sizeof(Menu *)*size)
//    };
//    return new_menu;
//}

Menu InitStructMenu(char *name, int size, Menu *prev){
    Menu new_menu = {
        .name = name,
        .size = size,
        .pos = 1,
        .data = NULL,
        .is_item = false,
        //.available = true,
        .prev = prev,
        .labels = malloc(sizeof(Menu *)*size)
    };
    return new_menu;
}

Menu InitStructItem(char *name, char *data, Menu *prev){
    Menu new_menu = {
        .name = name,
        .size = 0,
        .pos = 0,
        .data = data,
        .is_item = true,
        //.available = false,
        .prev = prev,
        .labels = NULL
    };
    return new_menu;
}

//void CreateDefaultMenus();
Menu hardware;
Menu system;
Menu battery;
Menu console_info;
Menu help;
Menu eexit;
Menu main_menu;