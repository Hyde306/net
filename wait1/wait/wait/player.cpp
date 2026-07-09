//player.cpp 接続待機側
#pragma once
#include"player.h"

CPlayer::CPlayer()
{
	ID = (int)ObjID::PLAYER;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
    vec.y += 0.4f;
    pos.y += vec.y;

    if (isLocal)
    {
        // ジャンプは「落下していないとき」だけ許可
        if (CheckHitKey(KEY_INPUT_SPACE) && vec.y >= 0)
        {
            vec.y = -8.0f;
        }
    }

    if (pos.y > 480)
    {
        FLAG = false;
    }

    return 1;
}

void CPlayer::Draw(float scrollX)
{
    DrawCircle(pos.x, pos.y, 32, 0xff0000, TRUE);
}