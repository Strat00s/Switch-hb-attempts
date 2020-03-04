#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Entry{
    bool is_item;
    char *name;
    char *data;
    bool available;
    int pos;
    int size;
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

Entry InitStructMenu(char *name, int size, Entry *prev){
    Entry new_menu = {
        .name = name,           // name to print
        .size = size,           // number of items
        .pos = 1,               // cursor starting position (99% is 1)
        .data = NULL,           // printable data. use "\x1b[x;yH" to set coords
        .is_item = false,       // is it a menu or item. items usually have data or call function
        //.available = true,
        .prev = prev,           // pointer to previous menu
        .labels = malloc(sizeof(Entry *)*size) // array of pointers to individual menu items
    };
    return new_menu;
}

Entry InitStructItem(char *name, char *data, Entry *prev){
    Entry new_menu = {
        .name = name,
        .size = 0,
        .pos = 0,
        .data = data,
        .is_item = true,    
        //.available = false,
        .prev = prev,
        .labels = NULL //*
    };
    return new_menu;
}