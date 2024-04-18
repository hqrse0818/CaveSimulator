#pragma once
#include "Com_Sprite.h"
class Com_ScreenTexture :
    public Com_Sprite
{
private:
    int iIndex = 0;

public:
    void Draw();
};
