// square.cpp
#include "square.h"
#include "DxLib.h"

CSquare::CSquare()
{
    ID = 100;

    pos.x = 900;
    pos.y = 0;
}

int CSquare::Action(const ObjList&, ObjList&)
{
    return 0;
}

void CSquare::Draw(float scrollX)
{
    DrawBox(
        pos.x - scrollX,
        0,
        pos.x + 80 - scrollX,
        180,
        GetColor(0, 200, 0),
        TRUE);

    DrawBox(
        pos.x - scrollX,
        300,
        pos.x + 80 - scrollX,
        480,
        GetColor(0, 200, 0),
        TRUE);
}