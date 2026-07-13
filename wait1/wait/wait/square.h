#pragma once
#include"objBase.h"

class CSquare : public BaseVector
{
public:
    CSquare();

    int Action(const ObjList&, ObjList&) override;
    void Draw(float scrollX) override;
};