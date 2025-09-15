#pragma once

#include <raylib.h>

class Images
{
private:
	Texture2D *texture;

public:
	Images();
	~Images();
	Texture2D Get(int imagetype, int index);
};

extern Images *images;
