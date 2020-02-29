#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// screen rows and cols
#define MIN_ROW 1
#define MAX_ROW 45
#define MIN_COL 1
#define MAX_COL 80

int main(int argc, char *argv[]){
    
    // variables for coordinates
    int x = MIN_ROW;
    int y = MIN_COL;

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

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu
        printf(CONSOLE_ESC(45;65H)"Press + to exit.");  // print exit message in the right bottom corner

        // Your code goes here
        printf(CONSOLE_ESC(21;20H)"Use D-Pad or joystick to move the cursor");  // Show controls in the middle
        printf(CONSOLE_ESC(23;30H)"Row: %d        Col: %d", x, y);  // show coords in the middle
        printf("\x1b[%d;%dH_", x, y, 16);  // print "_" on "x" row and "y" collum

        // move when a button is pressed as long as we are on the screen
        if (kDown & KEY_DOWN && x < MAX_ROW){
            x++;
            printf( CONSOLE_ESC(2J) );
        }
        if (kDown & KEY_UP && x > MIN_ROW){
            x--;
            printf( CONSOLE_ESC(2J) );
        }
        if (kDown & KEY_RIGHT && y < MAX_COL){
            y++;
            printf( CONSOLE_ESC(2J) );
        }

        if (kDown & KEY_LEFT && y > MIN_COL){
            y--;
            printf( CONSOLE_ESC(2J) );
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
