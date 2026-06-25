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

	((CPlayer*)base[0].get())->isLocal = false; 
	((CPlayer*)base[1].get())->isLocal = true;  

	((CPlayer*)base[0].get())->pos.x = 200;
	((CPlayer*)base[0].get())->pos.y = 400;

	((CPlayer*)base[1].get())->pos.x = 450;
	((CPlayer*)base[1].get())->pos.y = 400;
}
CGame::CGame(CManager* p,ObjList carry) :CScene(p) {
}

//クライアント側
int CGame::UpDate()
{
    ObjList add_list;
    bool send_flag = false;

    if (base[1]->Action(base, add_list))
        send_flag = true;

    // サーバーからの受信
    DataLength = GetNetWorkDataLength(NetHandle);

    if (DataLength >= sizeof(COMDATA))
    {
        COMDATA data;
        NetWorkRecv(NetHandle, strBuf, sizeof(COMDATA));
        memcpy(&data, strBuf, sizeof(COMDATA));

        base[0]->pos = data.pos;
        base[0]->vec = data.vec;
    }

    base[0]->Action(base, add_list);

    // 自分のキャラのデータ送信
    if (send_flag)
    {
        COMDATA data{};
        data.pos = base[1]->pos;
        data.vec = base[1]->vec;

        memcpy(strBuf, &data, sizeof(COMDATA));
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
