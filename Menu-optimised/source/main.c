#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

typedef struct Menu{
    struct Menu *parrent;
    int max;
    int current;
    struct Menu *submenus[];
};

// names (straight from switch-example)
const char* const months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* const weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// main menu array
const char MainMenu[][15] = {"Help", "Submenu","Time","Exit"};
const char SubmenuMenu[][20] = {"Submenu option 1", "Submenu option 2", "Submenu option 3"};

// variables
int main_pos = 1;  // main menu
int sub1_pos = 1;  // 1st layer submenus

bool Main = true;
bool sub1_on = true;


bool submenu1(u64 kDown){
    // print the help
    printf(CONSOLE_ESC(10;10H) "********************Submenu option 1********************");
    // exit with B, reset and clear
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}
bool submenu2(u64 kDown){
    printf(CONSOLE_ESC(10;10H) "********************Submenu option 2********************");
    // exit with B, reset and clear
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}
bool submenu3(u64 kDown){

    printf(CONSOLE_ESC(10;10H) "********************Submenu option 3********************");
    // exit with B, reset and clear
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

// individual submenus
bool Submenu(u64 kDown){
    if (sub1_on == true){
        if (kDown & KEY_DOWN && sub1_pos < 3){
            sub1_pos++;
        }
        if (kDown & KEY_UP && sub1_pos > 1){
            sub1_pos--;
        }
        printf(CONSOLE_ESC(1;1H)"  %s", SubmenuMenu[0]);
        printf(CONSOLE_ESC(2;1H)"  %s", SubmenuMenu[1]);
        printf(CONSOLE_ESC(3;1H)"  %s", SubmenuMenu[2]);
        printf("\x1b[%d;1H%c", sub1_pos, 16);

        if (kDown & KEY_B){
            sub1_pos = 1;
            sub1_on = true;
            printf(CONSOLE_ESC(2J));
            return true;
        }
    }
    if ((kDown & KEY_A && sub1_on == true) || sub1_on == false){
            // clear and reset everything
            //sub1_on = false;
            switch (sub1_pos) {
            case 1: printf(CONSOLE_ESC(2J)); sub1_on = submenu1(kDown); break;
            case 2: printf(CONSOLE_ESC(2J)); sub1_on = submenu2(kDown); break;
            case 3: printf(CONSOLE_ESC(2J)); sub1_on = submenu3(kDown); break;
            }
        }
    return false;
}

// individual suboptions
bool Help(u64 kDown){
    // print the help
    printf(CONSOLE_ESC(9;20H) "+-------------------------------------------+");
    printf(CONSOLE_ESC(10;20H)"|This is a simple help.                     |");
    printf(CONSOLE_ESC(11;20H)"+-------------------------------------------+");
    printf(CONSOLE_ESC(12;20H)"|To select items, press A.                  |");
    printf(CONSOLE_ESC(13;20H)"|Press B to go back.                        |");
    printf(CONSOLE_ESC(14;20H)"|To exit, go to main menu and select 'Exit'.|");
    printf(CONSOLE_ESC(15;20H)"+-------------------------------------------+");
    // exit with B, reset and clear
    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

bool Time(u64 kDown){
    time_t unixTime = time(NULL);
    struct tm* timeStruct = gmtime((const time_t *)&unixTime);//Gets UTC time. If you want local-time use localtime().
    int hours = timeStruct->tm_hour;
    int minutes = timeStruct->tm_min;
    int seconds = timeStruct->tm_sec;
    int day = timeStruct->tm_mday;
    int month = timeStruct->tm_mon;
    int year = timeStruct->tm_year +1900;
    int wday = timeStruct->tm_wday;
    printf("\x1b[20;35H%02i:%02i:%02i", hours, minutes, seconds);
    printf("\n\x1b[21;30H%s %s %i %i", weekDays[wday], months[month], day, year);

    if (kDown & KEY_B){
        printf(CONSOLE_ESC(2J));
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){

    consoleInit(NULL);

    while(appletMainLoop()){

        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);



        // printf the main menu
        if (Main == true){
            if (kDown & KEY_DOWN && main_pos < sizeof(MainMenu)/sizeof(MainMenu[0])){
                main_pos++;
            }
            if (kDown & KEY_UP && main_pos > 1){
                main_pos--;
            }

            // go to exit option
            if (kDown & KEY_B){
                main_pos = sizeof(MainMenu)/sizeof(MainMenu[0]);
            }
            printf(CONSOLE_ESC(1;1H)"  %s\n",MainMenu[0]);
            printf(CONSOLE_ESC(2;1H)"  %s\n",MainMenu[1]);
            printf(CONSOLE_ESC(3;1H)"  %s\n",MainMenu[2]);
            printf(CONSOLE_ESC(4;1H)"  %s\n",MainMenu[3]);

            // print ASCII 16 on "x" row
            printf("\x1b[%d;1H%c", main_pos, 16);
        }

        // when we are in main menu, we can press A to select others depending on cursor position
        if (kDown & KEY_A || Main == false){
            // clear and reset everything
            Main = false;
            switch (main_pos) {
            case 1: printf(CONSOLE_ESC(2J)); Main = Help(kDown); break;
            case 2: printf(CONSOLE_ESC(2J)); Main = Submenu(kDown); break;
            case 3: printf(CONSOLE_ESC(2J)); Main = Time(kDown); break;
            case 4: consoleExit(NULL); return 0;
            }
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
