#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "init.h"

void CreateDefaultMenus(){
    main_menu = InitStructMenu("Main Menu", 3, NULL);
    help = InitStructItem("Help", "This is simple help", &main_menu);
    main_menu.labels[0] = &help;
}