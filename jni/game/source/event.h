
#ifndef __EVENT_H
#define __EVENT_H

enum {IN_STARTSCREEN = 0, IN_GAME = 1, IN_ENDSCREEN = 2, 
            IN_DEMOSCREEN = 3,START_TO_GAMESCREEN = 4, END_TO_STARTSCREEN = 5};
extern int state;

void startScreen();
void endScreen();

#endif
