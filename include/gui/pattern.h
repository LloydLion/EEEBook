#ifndef PATTERN
#define PATTERN
class Pattern   
{
private:
    bool (*_pattern_function)(int x, int y);
    bool _is_inverse = false;
public:

    Pattern(bool (*func)(int x, int y));
    Pattern();

    Pattern inverse();
    bool executePattern(int pixel_count, int layer);
    static Pattern get_fill_pattern();
    static Pattern get_chess_order();

};
#endif