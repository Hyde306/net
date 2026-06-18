//接続待機側
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include"player.h"

int NetHandle{ -1 };//ネットワークハンドル
int DataLength{ -1 };//データサイズ
char strBuf[256];//バイナリデータ(通信処理用)

IPDATA ip;//接続してきたPCのIPアドレス

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){

	//接続受付状態
	PreparationListenNetWork(40);//通信ポート指定

	base.push_back(make_unique<CPlayer>()); // 自分
	base.push_back(make_unique<CPlayer>()); // 相手

	((CPlayer*)base[0].get())->isLocal = true;
	((CPlayer*)base[1].get())->isLocal = false;

	((CPlayer*)base[0].get())->pos.x = 200;
	((CPlayer*)base[0].get())->pos.y = 400;

	((CPlayer*)base[1].get())->pos.x = 450;
	((CPlayer*)base[1].get())->pos.y = 400;

}
CGame::CGame(CManager* p,ObjList carry) :CScene(p) {
}

//更新処理
int CGame::UpDate(){
	if (NetHandle == -1)
	{
		//新しい接続の確認
		NetHandle = GetNewAcceptNetWork();
		return 0;
	}
	else {
		//接続受付終了
		StopListenNetWork();
		//接続してきたPCのアドレス取得
		GetNetWorkIP(NetHandle, &ip);
	}

	//データ受信
	//指定したネットワークハンドルの通信データを取得
	DataLength = GetNetWorkDataLength(NetHandle);
	if (DataLength > 0) {
		//データ受信処理
		COMDATA data;//受信したデータ
		//データ受信
		NetWorkRecv(NetHandle, strBuf, sizeof(COMDATA));
		//処理可能データに変換
     	memcpy_s(&data, sizeof(COMDATA), strBuf, sizeof(COMDATA));

		//データ更新
		//base[0].get()->pos = data.pos;
		for (auto& b : base) {
			if (b->ID == (int)ObjID::PLAYER)
			{
				//プレイヤーオブジェクトの位置更新
				((CPlayer*)base[0].get())->vec = data.vec;
			}
		}
	}

	//更新処理
	ObjList add_list;//追加処理用オブジェクトリスト
	for (auto& obj : base)
	obj->Action(base,add_list);
	//オブジェクト追加処理
	for (auto& obj : add_list)
		base.push_back(move(obj));

	//削除処理
	erase_if(base, [](const auto& obj) {return !obj->FLAG; });

	//オブジェクトのソート処理(クイックソート)指定したインデックス間
	ObjSort_Quick(base, 0, base.size() - 1);

	return 0;
}

//描画処理
void CGame::Draw()
{
	IPDATA IP;
	//起動端末のIPアドレス取得
	GetMyIPAddress(&IP, 1, NULL);
	DrawFormatString(0, 0, 0xffffff,
		"この端末のIPアドレス %d:%d:%d:%d",
		IP.d1, IP.d2, IP.d3, IP.d4
	);
	DrawFormatString(0, 16, 0xffffff,
		"この端末のIPアドレス %d:%d:%d:%d",
		ip.d1, ip.d2, ip.d3, ip.d4
	);
	////オブジェクト個数
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());
	
	for (auto& obj : base) obj->Draw();
}

CGame::~CGame()
{

}
