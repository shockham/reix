// Quick test program to list SDL compatible joysticks that are detected.
// Based on sample code found here: http://wiki.libsdl.org/moin.cgi/SDL_JoystickOpen

/* You need XCode installed and/or the compiler command line tools.
 * If you have SDL installed from macports (which I recommend) then compile from the terminal as:
 *   g++ -I/opt/local/include/SDL/ -L/opt/local/lib/ -lSDL -lSDLmain -framework Foundation -framework AppKit -o ListSDLJoysticks ListSDLJoysticks.cpp
 */

#include <stdio.h>
#include <SDL/SDL.h>

extern C_LINKAGE int main(int argc, char *argv[])
{
    SDL_Joystick *joy;

    // Initialize the joystick subsystem
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    int joyCount = SDL_NumJoysticks();

    if(joyCount == 0) printf("No SDL joysticks found.\n");
    else
    {
        for(int i=0; i<joyCount; i++)
        {
            // Try to open joystick
            joy = SDL_JoystickOpen(i);
            if(joy != NULL)
            {
                printf("Opened Joystick %i:\n", i);
                printf("\tName: %s\n", SDL_JoystickName(i));
                printf("\tNumber of Axes: %d\n", SDL_JoystickNumAxes(joy));
                printf("\tNumber of Buttons: %d\n", SDL_JoystickNumButtons(joy));
                printf("\tNumber of Balls: %d\n", SDL_JoystickNumBalls(joy));
            }
              else printf("Failed to open Joystick %i\n", i);

            // Close if opened
            if(SDL_JoystickOpened(i)) SDL_JoystickClose(joy);

            printf("\n");
        }
    }

    fflush(stdout);
    return 0;
} 