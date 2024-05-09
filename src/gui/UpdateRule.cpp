#include "gui/UpdateRule.h"
#include <Arduino.h>

UpdateRule_ FullRule = UpdateRule_(UpdateType::FullUpdate);
UpdateRule_ PartialRule = UpdateRule_(UpdateType::FullUpdate);

UpdateType inverse_update(UpdateType type)
{
    if (type == PartialUpdate)
    {
        return UpdateType::FullUpdate;
    }
    return UpdateType::PartialUpdate;
}

UpdateRule_::UpdateRule_(UpdateType type, int delay, int count, int percentage): _default_type(type), _update_delay(delay), _update_count(count), _percentage(percentage){}

int get_percentage(Size size, Iterator<Bounds> *regions)
{
    int total_area, regions_area = 0;
    total_area = size.width * size.height;
    while(regions->next())
    {
        Size region_size = regions->current().size;
        regions_area += region_size.width * region_size.height;
    }
    return (regions_area*100)/total_area;
}


bool UpdateRule_::is_partial_update(Size size, Iterator<Bounds> *regions)
{
    bool is_inverse_update = (_update_delay != 0 && _update_delay <=millis() - _time) || //time condition 
                            (_update_count != 0 && _count % _update_count == 0) || //count condition
                            (_percentage != 0 && _percentage <= get_percentage(size,regions)); //regions condition 
    //update time and time and count
    _time = millis();
    _count ++;
    //returning inverse update if all conditions passed and returning default update type if not 
    return is_inverse_update ? 
        inverse_update(_default_type) : _default_type;
}

UpdateRule UpdateRule_::get_full_update()
{
    return &FullRule;
}

UpdateRule UpdateRule_::get_partial_update()
{
    return &PartialRule;
}

