#pragma once
//player.h
#include"objBase.h"

class CPlayer :public BaseVector
{
public:
	CPlayer();

	int Action(const ObjList&, ObjList&);
	void Draw(float scrollX);

	bool isLocal = false;
	bool onGround = false;
};