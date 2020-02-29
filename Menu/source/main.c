#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// names (straight from switch-example)
const char* const months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* const weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int main(int argc, char *argv[]){
    
    // variables
    int x = 1;
    bool Main = true;
    bool Help = false;
    bool Time = false;

    // main menu array
    char MainMenu[][10] = {"Help","Time","Exit"};

    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // main loop
    while(appletMainLoop()){

        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // Your code goes here

        // move when a button is pressed as long as we are on the screen
        if (kDown & KEY_DOWN && x < sizeof(MainMenu)/sizeof(MainMenu[0])){
            x++;
        }
        if (kDown & KEY_UP && x > 1){
            x--;
        }

        // printf the main menu
        if (Main == true){
            printf(CONSOLE_ESC(1;1H)"  %s\n",MainMenu[0]);
            printf(CONSOLE_ESC(2;1H)"  %s\n",MainMenu[1]);
            printf(CONSOLE_ESC(3;1H)"  %s\n",MainMenu[2]);

            printf("\x1b[%d;1H->", x); // print "_" on "x" row and "y" collum
        }

        // help submenu
        else if (Help == true) {
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
                x = 1;
                Main = true;
                Help = Time = false;
                printf(CONSOLE_ESC(2J));
            }
        }

        // time submenu
        else if (Time == true) {
            // show time (straight from switch-example)
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
            printf("\n\x1b[21;28H%s %s %i %i", weekDays[wday], months[month], day, year);
            // exit with B, reset and clear
            if (kDown & KEY_B){
                x = 1;
                Main = true;
                Help = Time = false;
                printf(CONSOLE_ESC(2J));
            }
        }

        // when we are in main menu, we can press A to select others depending on cursor position
        if (kDown & KEY_A && Main == true){
            // clear and reset everything
            switch (x) {
            case 1: printf(CONSOLE_ESC(2J)); x = 1; Help = true; Main = Time = false; break;
            case 2: printf(CONSOLE_ESC(2J)); x = 1; Time = true; Main = Help = false; break;
            case 3: consoleExit(NULL); return 0;
            }
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
