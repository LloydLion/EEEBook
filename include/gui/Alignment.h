#ifndef GUI_ALIGNMENT_H
#define GUI_ALIGNMENT_H

enum VerticalAlignment
{
    Top,
    Center,
    Bottom
};

enum HorizontalAlignment
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
    Alignment(VerticalAlignment, HorizontalAlignment): vertical(vertical), horizontal(horizontal) {}
};


#endif
