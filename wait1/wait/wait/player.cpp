//player.cpp 接続待機側
#pragma once
#include"player.h"

CPlayer::CPlayer()
{
	ID = (int)ObjID::PLAYER;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
    // 重力
    vec.y += 0.4f;
    pos.y += vec.y;

    // ジャンプ
    if (isLocal)
    {
        if (CheckHitKey(KEY_INPUT_SPACE) && vec.y >= 0)
        {
            vec.y = -8.0f;
        }
    }

    // 柱との当たり判定
    for (auto& obj : base)
    {
        if (obj->ID != 100)
            continue;

        int left = obj->pos.x;
        int right = obj->pos.x + 80;

        if (pos.x + 32 > left && pos.x - 32 < right)
        {
            // 上の柱
            if (pos.y - 32 < 180)
            {
                FLAG = false;
            }

            // 下の柱
            if (pos.y + 32 > 300)
            {
                FLAG = false;
            }
        }
    }

    // 画面外
    if (pos.y > 480)
    {
        FLAG = false;
    }

    return 1;
}

void CPlayer::Draw(float scrollX)
{
    if (!FLAG) return;

    DrawCircle(pos.x, pos.y, 32, 0xff0000, TRUE);
}