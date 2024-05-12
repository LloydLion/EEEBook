#ifndef GUI_ALIGNMENT_H
#define GUI_ALIGNMENT_H

enum class VerticalAlignment
{
    Top,
    Center,
    Bottom
};

enum class HorizontalAlignment
{
    Left,
    Center,
    Right
};

struct Alignment
{
    VerticalAlignment vertical;
    HorizontalAlignment horizontal;
    Alignment(): vertical(VerticalAlignment::Center), horizontal(HorizontalAlignment::Center) {}
    Alignment(VerticalAlignment vertical, HorizontalAlignment horizontal): vertical(vertical), horizontal(horizontal) {}
};


#endif
