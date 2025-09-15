#pragma once

#include "raylib.h"

class Images_t
{
private:
    // data
    Texture2D image_container[58];

public:
    // Constructor
    Images_t();
    // destructor
    ~Images_t();
    // draw function
    Texture2D GetImage(int id);
}; // end class Images_t
