#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

//#define M_E 2.71828182845904523536028747135266249775724709369995  // uncomment if M_E is undefined. Works totally randomoly.
#define I0 1e-12       // A
#define Ut 0.0258563    // V
#define Umin 0.0        // V

double diode(double u0, double r, double eps, bool debug){
    double AmpMax, AmpMin, AmpPiv, pivot;
    double x = Umin;
    double y = u0;
    do{
        pivot = (x + y)/2;
        AmpMax = I0*(pow(M_E, y/Ut)-1) - (u0 - y)/r;
        AmpMin = I0*(pow(M_E, x/Ut)-1) - (u0 - x)/r;
        AmpPiv = I0*(pow(M_E, pivot/Ut)-1) - (u0 - pivot)/r;

        if (AmpMax*AmpPiv > 0){
            y = pivot;
        }
        else {
            x = pivot;
        }
        if (debug == true){
            printf("AmpMin: %f\n", AmpMin);
            printf("Pivot: %f\n", pivot);
            printf("AmpMax: %f\n", AmpMax);
            printf("-----------------------------\n");
        }
    } while(fabs((AmpMax + AmpMin)/2) >= eps);
    return (pivot);
}

int main(int argc, char *argv[]){
    //Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    consoleInit(NULL);

    //Move the cursor to row 16 and column 20 and then prints "Hello World!"
    //To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
    //the row and column where you want your cursor to move

    printf( CONSOLE_ESC(2J) );
    printf(CONSOLE_ESC(10;20H)"This is an LED calculator.\n");
    printf(CONSOLE_ESC(11;20H)"Just press buttons!:\n");
    printf(CONSOLE_ESC(12;20H)"--------------------------------\n");
    printf(CONSOLE_ESC(13;20H)"Press - to show this help.\n");
    printf(CONSOLE_ESC(14;20H)"Press A to run with defautl values.\n");
    printf(CONSOLE_ESC(15;20H)"Press B to run with random values.\n");
    printf(CONSOLE_ESC(16;20H)"Press X to show debug values (default values).\n");
    printf(CONSOLE_ESC(17;20H)"Press Y to show debug values (default values).\n");
    printf(CONSOLE_ESC(18;20H)"Press + to exit.");


    while(appletMainLoop()){
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A){
            printf( CONSOLE_ESC(2J) );
            double Up = diode(100, 200, 300, false);
            double Ip = ((100 - Up)/200);
            printf("Ip:  %gA\nUp:  %gV\n", Ip, Up);
        }
        if (kDown & KEY_B){
            printf( CONSOLE_ESC(2J) );
            int first = rand()%1000;
            int second = rand()%1000;
            int third = rand()%1000;
            double Up = diode(first, second, third, false);
            double Ip = ((first - Up)/second);
            printf("Ip:  %gA\nUp:  %gV\n", Ip, Up);
        }
        if (kDown & KEY_X){
            printf( CONSOLE_ESC(2J) );
            double Up = diode(100, 200, 300, true);
            double Ip = ((100 - Up)/200);
            printf("\nIp:  %gA\nUp:  %gV\n", Ip, Up);
        }
        if (kDown & KEY_Y){
            printf( CONSOLE_ESC(2J) );
            int first = rand()%1000;
            int second = rand()%1000;
            int third = rand()%1000;
            double Up = diode(first, second, third, true);
            double Ip = ((first - Up)/second);
            printf("\nIp:  %gA\nUp:  %gV\n", Ip, Up);
        }

        if (kDown & KEY_MINUS){
            printf( CONSOLE_ESC(2J) );
            printf(CONSOLE_ESC(10;20H)"This is an LED calculator.\n");
            printf(CONSOLE_ESC(11;20H)"Just press buttons!:\n");
            printf(CONSOLE_ESC(12;20H)"--------------------------------\n");
            printf(CONSOLE_ESC(13;20H)"Press - to show this help.\n");
            printf(CONSOLE_ESC(14;20H)"Press A to run with defautl values.\n");
            printf(CONSOLE_ESC(15;20H)"Press B to run with random values.\n");
            printf(CONSOLE_ESC(16;20H)"Press X to show pivot values (default values).\n");
            printf(CONSOLE_ESC(17;20H)"Press Y to show pivot values (default values).\n");
            printf(CONSOLE_ESC(18;20H)"Press + to exit.");
        }

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}