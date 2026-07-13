#include "square.h"
#include "DxLib.h"

CSquare::CSquare()
{
    ID = 100;

    pos.x = 500;
    pos.y = 0;
}

int CSquare::Action(const ObjList&, ObjList&)
{
    pos.x -= 3;

    if (pos.x < -80)
    {
        pos.x = 900;
    }

    return 0;
}

void CSquare::Draw(float)
{
    // ã‚Ì’Œ
    DrawBox(
        pos.x,
        0,
        pos.x + 80,
        180,
        GetColor(0, 255, 0),
        TRUE);

    // ‰º‚Ì’Œ
    DrawBox(
        pos.x,
        300,
        pos.x + 80,
        480,
        GetColor(0, 255, 0),
        TRUE);
}