#include "gui/UpdateRule.h"
#include "platform/time.h"

UpdateRule_ FullRule = UpdateRule_(UpdateType::FullUpdate);
UpdateRule_ PartialRule = UpdateRule_(UpdateType::PartialUpdate);

UpdateType inverse_update(UpdateType type)
{
    return (UpdateType)!type;
}

UpdateRule_::UpdateRule_(UpdateType type, int delay, int count): _default_type(type), _update_delay(delay), _update_count(count)
{

}

bool UpdateRule_::is_partial_update()
{
    bool is_inverse_update =
        (_update_delay != 0 && _update_delay <= current_time() - _time) || //time condition 
        (_update_count != 0 && _count % _update_count == 0) //count condition
    ;

    _time = current_time();
    _count++;
    
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
