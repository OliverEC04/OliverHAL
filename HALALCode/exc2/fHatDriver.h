#pragma once

void initLed(char gpio);
void setLed(char state, char gpio);
void initSw(char gpio);
char getSw(char gpio);