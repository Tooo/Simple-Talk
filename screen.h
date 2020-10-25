#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "list.h"

void Screen_signalNextMessage();
void Screen_init(List * list);
void Screen_shutdown();

#endif