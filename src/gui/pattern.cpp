#include"gui/pattern.h"

bool fill(int x, int y)
{
    return true;    
}
bool chess_order(int x, int y)
{
    return x % 2 == y % 2;
}

Pattern FillPattern = Pattern(fill);
Pattern ChessOrderPattern = Pattern(chess_order);

Pattern::Pattern(bool (*func)(int x , int y)) : _pattern_function(func) {};

bool Pattern::executePattern(int x, int y) 
{
    return _pattern_function(x, y) ^ _is_inverse;
}

Pattern Pattern::get_chess_order()
{
    return ChessOrderPattern;
}

Pattern Pattern::get_fill_pattern()
{
    return FillPattern;
}
Pattern Pattern::inverse() 
{
    Pattern pattern = Pattern(*this);
    pattern._is_inverse = !_is_inverse;
    return pattern;
}