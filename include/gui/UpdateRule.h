#ifndef GUI_UPDATE_RULE
#define GUI_UPDATE_RULE
#include "std/iterator.h"
#include "cordinates.h"

class UpdateRule_;

typedef UpdateRule_ *UpdateRule;

enum UpdateType : bool
{
    PartialUpdate = true,
    FullUpdate = false
};
UpdateType inverse_update(UpdateType type);

class UpdateRule_
{
private:
    UpdateType _default_type;
    int _update_delay, _update_count, _count = 0, _percentage;
    unsigned long _time = 0;
public:
    UpdateRule_(UpdateType type, int delay = 0, int count = 0, int _percentage = 0);
    bool is_partial_update(Size size, Iterator<Bounds> *regions);
    static UpdateRule get_full_update();
    static UpdateRule get_partial_update();
};
#endif
