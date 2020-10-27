#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "list.h"

void Screen_signalNextMessage();
void Screen_init();
void Screen_waitForShutdown();
void Screen_clean();

#endif