#ifndef STD_PROPERTIES_H
#define STD_PROPERTIES_H

#define PROPERTY(TYPE, NAME) void NAME(TYPE value); TYPE NAME()

#define AUTO_GET(FIELD) { return FIELD; }

#define AUTO_PROPERTY(TYPE, NAME, FIELD) void NAME(TYPE value) { FIELD = value; } TYPE NAME() { return FIELD; }

#endif
