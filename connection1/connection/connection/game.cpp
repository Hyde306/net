//接続する側
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include"player.h"

IPDATA IP_set();//IPアドレス入力関数
IPDATA ip;//接続先のIPアドレス

int DataLength{ -1 };//データサイ
int NetHandle{ -1 };//ネットワークハンドル
char strBuf[256];

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){
	ip = IP_set();
	//指定したIPアドレス＆ポートに接続
	NetHandle = ConnectNetWork(ip, 40);

	base.push_back(make_unique<CPlayer>()); // 相手
	base.push_back(make_unique<CPlayer>()); // 自分

	((CPlayer*)base[0].get())->isLocal = true;
	((CPlayer*)base[1].get())->isLocal = false;

	((CPlayer*)base[0].get())->pos.x = 200;
	((CPlayer*)base[0].get())->pos.y = 400;

	((CPlayer*)base[1].get())->pos.x = 450;
	((CPlayer*)base[1].get())->pos.y = 400;
}
CGame::CGame(CManager* p,ObjList carry) :CScene(p) {
}

//クライアント側
//更新処理
int CGame::UpDate(){
	//更新処理
	ObjList add_list;//追加処理用オブジェクトリスト

	bool send_flag{ false };//送信するしないフラグ
	//オブジェクト更新処理
	for (auto& obj : base)
	{
		if (obj->Action(base, add_list))
			send_flag = true;
	}

	//オブジェクト追加処理
	for (auto& obj : add_list)
		base.push_back(move(obj));

	//削除処理
	erase_if(base, [](const auto& obj) {return !obj->FLAG; });

	//オブジェクトのソート処理(クイックソート)指定したインデックス間
	
	// サーバーからのデータ受信
	DataLength = GetNetWorkDataLength(NetHandle);

	if (DataLength >= sizeof(COMDATA))
	{
		COMDATA data;

		NetWorkRecv(
			NetHandle,
			strBuf,
			sizeof(COMDATA));

		memcpy_s(
			&data,
			sizeof(COMDATA),
			strBuf,
			sizeof(COMDATA));

		// 相手プレイヤー更新
		((CPlayer*)base[0].get())->vec = data.vec;
	}

	//何か動きがあった場合のみ通信
	if (send_flag)
	{
		//通信処理
		//送信データの作成
		COMDATA data{ 0 };
		data.vec = base[1].get()->vec;		//送信バイナリデータの作成
		memcpy_s(strBuf, sizeof(COMDATA), &data, sizeof(COMDATA));

		//データ送信
		NetWorkSend(NetHandle, strBuf, sizeof(COMDATA));
	}

	return 0;
}

//描画処理
void CGame::Draw()
{
	//オブジェクト個数
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());
	
	for (auto& obj : base) obj->Draw();
}

CGame::~CGame()
{

}
