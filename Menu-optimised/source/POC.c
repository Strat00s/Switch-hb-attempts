#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NL printf("\n");

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

int ExitFCE(){
    return 99;
}

int main(){
    Menu help;
    Menu config_menu;
    Menu main_menu;
    Menu eexit;
    
    Menu cfg1;
    Menu cfg2;
    Menu cfg3;
    Menu cfg4;
    Menu cfg5;
    
    cfg1.name = "Config1";
    cfg2.name = "Config2";
    cfg3.name = "Config3";
    cfg4.name = "Config4";
    cfg5.name = "Config5";

    cfg1.is_item = true;
    cfg2.is_item = true;
    cfg3.is_item = true;
    cfg4.is_item = true;
    cfg5.is_item = true;

    cfg1.prev = &config_menu;
    cfg2.prev = &config_menu;
    cfg3.prev = &config_menu;
    cfg4.prev = &config_menu;
    cfg5.prev = &config_menu;

    cfg1.data = NULL;
    cfg2.data = NULL;
    cfg3.data = NULL;
    cfg4.data = NULL;
    cfg5.data = NULL;
    
    cfg1.func = NULL;
    cfg2.func = NULL;
    cfg3.func = NULL;
    cfg4.func = NULL;
    cfg5.func = NULL;

    eexit.name = "Exit";
    eexit.is_item = true;
    eexit.func = ExitFCE;
    
    help.is_item = true;
    help.name = "Help";
    help.data = "This is a help";
    help.prev = &main_menu;
    help.func = NULL;

    config_menu.is_item = false;
    config_menu.name = "Config Menu";
    config_menu.data = NULL;
    config_menu.available = true;
    config_menu.pos = 1;
    config_menu.size = 5;
    config_menu.func = NULL;
    config_menu.prev = &main_menu;
    config_menu.labels = malloc(sizeof(struct Menu *)*5);
    config_menu.labels[0] = &cfg1;
    config_menu.labels[1] = &cfg2;
    config_menu.labels[2] = &cfg3;
    config_menu.labels[3] = &cfg4;
    config_menu.labels[4] = &cfg5;

    main_menu.is_item = false;
    main_menu.name = "Main Menu";
    main_menu.data = NULL;
    main_menu.available = true;
    main_menu.pos = 1;
    main_menu.size = 3;
    main_menu.func = NULL;
    main_menu.prev = NULL;
    main_menu.labels = malloc(sizeof(struct Menu *)*3);
    main_menu.labels[0] = &help;
    main_menu.labels[1] = &config_menu;
    main_menu.labels[2] = &eexit;

    Menu holder;
    holder = main_menu;

    for (int z = 0; z < 5;){
        if (holder.is_item == false){
            printf("Current menu: %s\n", holder.name);
            printf("Size: %d\n", holder.size);
            printf("Items: \n");
            for (int i = 0; i < holder.size; i++){
                printf("  %d", holder.pos);
                holder.pos++;
                printf(" %s\n", holder.labels[i]->name);
            }
        }
        else{
            printf("Current item name: %s\n", holder.name);
        }
        if (holder.prev != NULL){
            printf("previous menu: %s\n", holder.prev->name);
        }
        if (holder.data != NULL){
            printf("Data:\n %s", holder.data);
        }
        if (holder.func != NULL){
            printf("Exit of called function is: %d\n", holder.func());
        }
        if (holder.labels != NULL && holder.is_item == false){
            holder = *holder.labels[z];
            printf("\nGOING UP\n\n");
        }
        else{
            holder = *holder.prev;
            printf("\nGOING DOWN\n\n");
            z++;
        }
    }

    //printf("main_menu 1: %s\n", main_menu.labels[0]->name);
    //printf("main_menu 2: %s\n", main_menu.labels[1]->name);
    //printf("main_menu 3: %s\n", main_menu.labels[2]->name);
    //printf("config_menu 1: %s\n", config_menu.labels[0]->name);
    //printf("config_menu 2: %s\n", config_menu.labels[1]->name);
    //printf("config_menu 3: %s\n", config_menu.labels[2]->name);
    //printf("config_menu 4: %s\n", config_menu.labels[3]->name);
    //printf("config_menu 5: %s\n", config_menu.labels[4]->name);
    free(main_menu.labels);
    free(config_menu.labels);
    return 0;
}