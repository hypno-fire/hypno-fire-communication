#include "hypnoPulse.h"

double easeOutExpo(double x) {
    return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

double easeInExpo(double x) {
    return x == 0 ? 0 : pow(2, 10 * x - 10);
}

hypnoPulse::hypnoPulse()
    : hypnoPattern()
{

}

void hypnoPulse::fire(int fadeInTicks, int durationTicks, int fadeOutTicks, int colorIntensity)
{
    _currentTick = 0;
    _fadeInTicks = fadeInTicks,
    _durationTicks = durationTicks,
    _fadeOutTicks = fadeOutTicks;
    _colorIntensity = colorIntensity;

    _fadeInStop = _fadeInTicks;
    _durationStop = _fadeInStop + _durationTicks;
    _fadeOutStop = _durationStop + _fadeOutTicks;

    if(_fadeInTicks > 0) {
        _fadeInFactor = 1.0 / (_fadeInTicks + 1);
    }
    if(_fadeOutTicks > 0) {
        _fadeOutFactor = 1.0 / (_fadeOutTicks + 1); 
    }

    _stopTrigger = false;
    activate();
}

void hypnoPulse::draw()
{
    if(_stopTrigger)
    {
        fill_solid(_targetLeds, _numLeds, CRGB::Black);
        _stopTrigger = false;
        deactivate();
        return;
    }

    CRGB color = _getColor();
    color = color.fadeToBlackBy(255 - _colorIntensity);

    if (_currentTick < _fadeInStop)
    {
        int step = (_currentTick + 1);
        double progress = _fadeInFactor * step;
        progress = easeOutExpo(progress);
        int value = progress * 255;
        color = color.fadeToBlackBy(255 - value);
    } else if (_currentTick < _durationStop)
    {
        
    } else if (_currentTick < _fadeOutStop) {
        int step = (_fadeOutStop - _currentTick);
        double progress = _fadeOutFactor * step;
        progress = easeInExpo(progress);
        int value = progress * 255;
        color = color.fadeToBlackBy(255 - value);
    } else {
        deactivate();
        for(int x = 0; x < _numLeds; x++) {
            _targetLeds[x] = CRGB::Black;
        }
        return;
    }
    
    for(int x = 0; x < _numLeds; x++) {
        _targetLeds[x] = color;
    }
    
    _currentTick++;
}

void hypnoPulse::stop()
{
    _stopTrigger = true;
}