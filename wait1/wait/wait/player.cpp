//player.cpp 接続待機側
#pragma once
#include"player.h"

CPlayer::CPlayer()
{
	ID = (int)ObjID::PLAYER;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
    // --- 重力 ---
    vec.y += 0.4f;
    pos.y += vec.y;

    // --- 自分のキャラだけ入力処理 ---
    if (isLocal)
    {
        // ジャンプは「落下していないとき」だけ許可
        if (CheckHitKey(KEY_INPUT_SPACE) && vec.y >= 0)
        {
            vec.y = -8.0f;
        }
    }

    // --- アウト判定 ---
    if (pos.y > 480)
    {
        FLAG = false;
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