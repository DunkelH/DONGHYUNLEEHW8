#pragma once

void init_timer();
void start_timing();
float stop_timing();

extern float gTotalTimeElapsed;
extern int gTotalFrames;
