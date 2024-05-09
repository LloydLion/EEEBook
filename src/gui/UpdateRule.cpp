#include "gui/UpdateRule.h"
#include <Arduino.h>

UpdateType inverse_update(UpdateType type)
{
    if (type == PartialUpdate)
    {
        return UpdateType::FullUpdate;
    }
    return UpdateType::PartialUpdate;
}

UpdateRule_::UpdateRule_(UpdateType type, int delay, int count): _default_type(type), _update_delay(delay), _update_count(count){}

unsigned long abs(unsigned long a, unsigned long b) {
    return (a > b) ? (a - b) : (b - a);
}

bool UpdateRule_::is_partial_update()
{
    if (_update_delay == 0);
    // min(millis() - _time, _time - millis())
    else if (_update_delay <= abs(millis(), _time))
        _count = 0;
        _time = millis();
        return inverse_update(_default_type);
    if (_update_count == 0);
    else if (_count <= _update_count)
    {
        _count = 0;
        _time = millis();
        return inverse_update(_default_type);
    }
    _count ++;
    _time = millis();
    return _default_type;
}


