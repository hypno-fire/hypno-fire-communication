#ifndef HYPNO_PULSE_H
#define HYPNO_PULSE_H

#include "hypno-patterns/hypnoPattern.h"

class hypnoPulse : public hypnoPattern
{
private:
    int _fadeInTicks = 0;
    int _durationTicks = 5;
    int _fadeOutTicks = 0;
    int _colorIntensity = 255;
    int _currentTick = 0;
    int _fadeInStop = 0;
    int _durationStop = 0;
    int _fadeOutStop = 0;

    double _fadeInFactor = 0.0;
    int _fadeInStep = 0;
    double _fadeOutFactor = 0.0;
    int _fadeOutStep = 0;

    bool _stopTrigger = false;

public:
    hypnoPulse();
    void fire(int fadeInTicks, int durationTicks, int fadeOutTicks, int colorIntensity);
    void draw();
    void stop();
};

#endif