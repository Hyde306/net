#pragma once
#include "main.h"
#include "objChar.h"

using namespace std;
class BaseVector;
using ObjList = vector<unique_ptr<BaseVector>>;

//スプライトクラス
//簡易描画クラス
class Sprite {
public:
	//描画元画像
	int img{ -1 };
	//画像サイズ
	int width{ 0 };
	int height{ 0 };
	//切り取り開始位置
	int cutX{ 0 };
	int cutY{ 0 };
	//大きさ
	double scale{ 1.0 };
	//角度
	double angle{ 0.0 };

	void Draw(float x, float y) const
	{
		DrawRectRotaGraph(
			x, y,
			cutX, cutY,
			width, height,
			scale,
			angle,
			img,
			true);
	}
};

//オブジェクトベースクラス(vector)
class BaseVector : public Character {
public:
	int pri{ 0 };//描画の順番
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ
	Sprite sprite;//簡易描画クラス

	//オブジェクトリスト,追加オブジェクト用リスト
	virtual int Action(const ObjList&, ObjList&) = 0;
	virtual void Draw(float scrollX) = 0;
	virtual ~BaseVector() = default;
};

//オブジェクトのソートクラス
class sort
{
public:
	bool operator()(const unique_ptr<BaseVector>& x, const unique_ptr<BaseVector>& y) const
	{
		return x.get()->pri < y.get()->pri;
	}
};

//オブジェクトのソートクラス（y座標でソート)
class sort_position_Y
{
public:
	bool operator()(const unique_ptr<BaseVector>& a, const unique_ptr<BaseVector>& b) const
	{
		return a.get()->pos.y < b.get()->pos.y;
	}
};


//3D用
class BaseVector3D : public Character3D {
public:
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ

	virtual int Action(vector<unique_ptr<BaseVector3D>>&) = 0;
	virtual void Draw() = 0;
};