#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

int main(int argc, char *argv[]){
    
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Your non-loop code goes here

    // main loop
    while(appletMainLoop()){

        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu
        printf("\x1b[0;0HPress + to exit.\n");  // print exit message

        // print all ascii characters
        for (int i = 0; i < 255; i++){
            printf("%d %c ", i, i);
            if (i%10 == 0){
                printf("\n");
            }
        }

        // Your code goes here

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
