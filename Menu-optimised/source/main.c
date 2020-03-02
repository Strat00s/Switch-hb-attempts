#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#define STATUS_POS CONSOLE_ESC(10;28H)
#define CHARGER_POS CONSOLE_ESC(11;29H)

u64 kDown;

int main_menu_pos = 1;
char main_menu_labels[][15] = {"Help", "Console Info", "Exit"};
bool main_menu_aval = true;

int info_menu_pos = 1;
char info_menu_labels[][15] = {"System", "Power", "Storage", "Joy-cons", "LCD", "Other"};
bool info_menu_aval = true;

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

// possibility to print text items with one function
//bool PrintItems(){
//    for (int i = 0; i < sizeof(*text); i++){
//        printf("\x1b[%d;1H  %s", i+1, text[i]);
//    }
//    if (kDown & KEY_B){
//        printf(CONSOLE_ESC(2J));
//        return true;
//    }
//    return false;
//}




bool PowerMenu(){
    u32 bat_percentage;
    ChargerType charger;
    HidPowerInfo bat_info;
    psmInitialize();
    psmGetBatteryChargePercentage(&bat_percentage);
    psmGetChargerType(&charger);
    psmExit();
    hidGetControllerPowerInfo(8, &bat_info, 1);

    printf(CONSOLE_ESC(9;20H) "Charge: %d%%", bat_percentage);
    printf(CONSOLE_ESC(10;20H) "Status:");
    if (bat_info.powerConnected){
        if (bat_info.isCharging){
            printf(STATUS_POS "Charging                  ");    
        }
        else {
            printf(STATUS_POS "Connected but not charging");
        }
    }
    else{
        printf(STATUS_POS "Disconnected              ");
    }
    printf(CONSOLE_ESC(11;20H) "Charger: ");
    switch(charger){
        case 0: printf(CHARGER_POS "None    "); break;
        case 1: printf(CHARGER_POS "Official"); break;
        case 2: printf(CHARGER_POS "Other   "); break;
    }

    // exit
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

bool JoyConsMenu(){
    u8 interface;
    hidGetNpadInterfaceType(8, &interface);
    printf(CONSOLE_ESC(10;20H) "Handheld connection type: ");
    switch(interface){
        case 1: printf(CONSOLE_ESC(10;46H) "Bleutooth"); break;
        case 2: printf(CONSOLE_ESC(10;46H) "Rail     "); break;
        case 3: printf(CONSOLE_ESC(10;46H) "USB      "); break;
        case 4: printf(CONSOLE_ESC(10;46H) "Other    "); break;
        default: printf(CONSOLE_ESC(10;46H) "Not connected"); break;
    }
    hidGetNpadInterfaceType(0, &interface);
    printf(CONSOLE_ESC(12;20H) "Player 1 connection type: ");
    switch(interface){
        case 1: printf(CONSOLE_ESC(12;46H) "Bleutooth"); break;
        case 2: printf(CONSOLE_ESC(12;46H) "Rail     "); break;
        case 3: printf(CONSOLE_ESC(12;46H) "USB      "); break;
        case 4: printf(CONSOLE_ESC(12;46H) "Other    "); break;
        default: printf(CONSOLE_ESC(12;46H) "Not connected"); break;
    }
    printf(CONSOLE_ESC(4;60H)"con_interface: %d", interface);   //debug
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

// each new function = new menu/item
bool ConsoleInfoMenu(){
    if (info_menu_aval){
        for (int i = 0; i < 6; i++){
            printf("\x1b[%d;1H  %s", i+1, info_menu_labels[i]);
        }
        MoveCursor(&info_menu_pos, 6, 1);
        if (kDown & KEY_B){
            info_menu_pos = 1;
            printf(CONSOLE_ESC(2J));
            return true;
        }
    }
    if (kDown & KEY_A || !info_menu_aval){
        switch (info_menu_pos) {
        case 2: printf(CONSOLE_ESC(2J)); info_menu_aval = PowerMenu();break;
        case 4: printf(CONSOLE_ESC(2J)); info_menu_aval = JoyConsMenu();break;
        }
    }
    return false;
}

bool HelpMenu(){
    printf(CONSOLE_ESC(9;20H) "+-------------------------------------------+");
    printf(CONSOLE_ESC(10;20H)"|This is a simple help.                     |");
    printf(CONSOLE_ESC(11;20H)"+-------------------------------------------+");
    printf(CONSOLE_ESC(12;20H)"|To select items, press A.                  |");
    printf(CONSOLE_ESC(13;20H)"|Press B to go back.                        |");
    printf(CONSOLE_ESC(14;20H)"|To exit, go to main menu and select 'Exit'.|");
    printf(CONSOLE_ESC(15;20H)"+-------------------------------------------+");
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

bool MainMenu(){
    if (main_menu_aval){
        for (int i = 0; i < sizeof(main_menu_labels)/sizeof(main_menu_labels[0]); i++){
            printf("\x1b[%d;1H  %s", i+1, main_menu_labels[i]);
        }
        MoveCursor(&main_menu_pos, 3, 1);
        if (kDown & KEY_B){
            main_menu_pos = 3;
        }
    }
    if (kDown & KEY_A || !main_menu_aval){
        switch (main_menu_pos) {
        case 1: printf(CONSOLE_ESC(2J)); main_menu_aval = HelpMenu();break;
        case 2: printf(CONSOLE_ESC(2J)); main_menu_aval = ConsoleInfoMenu(); break;
        case 3: return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]){

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
