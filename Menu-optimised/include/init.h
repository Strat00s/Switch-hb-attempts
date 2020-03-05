#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Entry{
    bool is_item;
    char *name;
    char *data;
    bool available;
    unsigned int pos;
    unsigned int min;
    unsigned int max;
    int ret;
    struct Entry *prev;
    struct Entry **labels;
    int (*func)();
} Entry;

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

Entry InitStructMenu(char *name, unsigned int max, unsigned int min, Entry *prev){
    Entry new_menu = {
        .name = name,           // name to print
        .min = min,             // default cursor position (99% is 1). Change if you print something above menu entire
        .max = max,             // number of items
        .pos = min,             // cursor position
        .data = NULL,           // printable data. use "\x1b[x;yH" to set coords
        .is_item = false,       // is it a menu or item. items usually have data or call function
        //.available = true,
        .prev = prev,           // pointer to previous menu. If none, enter NULL
        .labels = malloc(sizeof(Entry *)*max) // array of pointers to individual menu items
    };
    return new_menu;
}

Entry InitStructItem(char *name, char *data,/* int (*func)(),*/Entry *prev){
    Entry new_menu = {
        .name = name,
        .min = 0,
        .max = 0,
        .pos = 0,
        .data = data,
        .is_item = true,    
        //.available = false,
        .prev = prev,
        .labels = NULL //*
    };
    return new_menu;
}

Entry default_menu;