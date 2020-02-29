// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* const months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* const weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Include the main libnx system header, for Switch development
#include <switch.h>

//printf("\e[1;1H\e[2J"); clear screen

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("This a simple demo.\nPress - to show help.");

    // Main loop
    bool A_but = false;
    bool M_but = false;
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu

        if (kDown & KEY_MINUS || M_but == true){
            M_but = true;
            A_but = false;
            printf( CONSOLE_ESC(2J) );
            printf(CONSOLE_ESC(10;20H)"This is a simple help.\n");
            printf(CONSOLE_ESC(11;20H)"Just press buttons!:\n");
            printf(CONSOLE_ESC(12;20H)"--------------------------------\n");
            printf(CONSOLE_ESC(13;20H)"Press - to show this help.\n");
            printf(CONSOLE_ESC(14;20H)"Press A to show time.\n");
            printf(CONSOLE_ESC(15;20H)"Press B to set alarm.\n");
            printf(CONSOLE_ESC(16;20H)"Press + to exit.");
        }
        if (kDown & KEY_A || A_but == true){
            A_but = true;
            M_but = false;
            printf( CONSOLE_ESC(2J) );
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
        }
        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}