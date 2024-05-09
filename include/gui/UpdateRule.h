#ifndef GUI_UPDATE_RULE
#define GUI_UPDATE_RULE

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
    int _update_delay, _update_count;
    unsigned long _time = 0;
    int _count = 0;
public:
    UpdateRule_(UpdateType type, int delay = 0, int count = 0);
    bool is_partial_update();
};
const UpdateRule FullRule = new UpdateRule_(UpdateType::PartialUpdate);
const UpdateRule PartialRule = new UpdateRule_(UpdateType::PartialUpdate);
#endif
