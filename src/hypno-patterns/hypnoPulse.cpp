#include "hypnoPulse.h"

hypnoPulse::hypnoPulse()
    : hypnoPattern()
{

}

void hypnoPulse::fire(int fadeInTicks, int durationTicks, int fadeOutTicks)
{
    _currentTick = 0;
    _fadeInTicks = fadeInTicks,
    _durationTicks = durationTicks,
    _fadeOutTicks = fadeOutTicks;

    _fadeInStop = _fadeInTicks;
    _durationStop = _fadeInStop + _durationTicks;
    _fadeOutStop = _durationStop + _fadeOutTicks;

    if(_fadeInTicks > 0) {
        _fadeInFactor = 1.0 / (_fadeInTicks + 1);
    }
    if(_fadeOutTicks > 0) {
        _fadeOutFactor = 1.0 / (_fadeOutTicks + 1); 
    }

    this->activate();
}

void hypnoPulse::draw()
{
    CRGB color = _getColor();
    if (_currentTick < _fadeInStop)
    {
        int step = (_currentTick + 1);
        int value = _fadeInFactor * step * 255;
        color = color.fadeToBlackBy(255 - value);
    } else if (_currentTick < _durationStop)
    {
        
    } else if (_currentTick < _fadeOutStop) {
        int step = (_fadeOutStop - _currentTick);
        int value = _fadeOutFactor * step * 255;
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