#pragma once
#include "io/keypad.h"
#include "io/display.h"
#include "io/timers.h"

class IO {
public:
public:
    IO();
    ~IO();
    Keypad keypad;
    Display display;
    Timers timers;

};