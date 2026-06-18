//player.cpp Ú‘±‚·‚é‘¤
#pragma once
#include"player.h"

CPlayer::CPlayer()
{
    pos.x = 300;
    pos.y = 400;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
    if (!isLocal)
    {
        return 0;
    }

    vec.y += 0.4f;

    if (CheckHitKey(KEY_INPUT_SPACE) && onGround)
    {
        vec.y = -8.0f;
        onGround = false;
    }

    pos.y += vec.y;

    if (pos.y >= 400)
    {
        pos.y = 400;
        vec.y = 0;
        onGround = true;
    }

    return 1;
}

void CPlayer::Draw()
{
    DrawCircle(pos.x, pos.y, 32, 0xff0000, true);
    DrawFormatString(
        pos.x - 20,
        pos.y - 50,
        0xffffff,
        "%d",
        isLocal
    );
}