//接続待機側
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"
#include "square.h"
#include"player.h"

int NetHandle{ -1 };//ネットワークハンドル
int DataLength{ -1 };//データサイズ
char strBuf[256];//バイナリデータ(通信処理用)

IPDATA ip;//接続してきたPCのIPアドレス

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){

	//接続受付状態
	PreparationListenNetWork(40);//通信ポート指定

    bgHandle = LoadGraph("ipset_image/Back.jpg");

	base.push_back(make_unique<CPlayer>()); // 自分
	base.push_back(make_unique<CPlayer>()); // 相手
    base.push_back(make_unique<CSquare>());

	((CPlayer*)base[0].get())->isLocal = true;
	((CPlayer*)base[1].get())->isLocal = false;

	((CPlayer*)base[0].get())->pos.x = 200;
	((CPlayer*)base[0].get())->pos.y = 400;

	((CPlayer*)base[1].get())->pos.x = 450;
	((CPlayer*)base[1].get())->pos.y = 400;

}
CGame::CGame(CManager* p,ObjList carry) :CScene(p) {
}

//サーバー側
int CGame::UpDate()
{
    scrollX += 3;   // 3px/フレームで右→左に流れる

    if (scrollX > 1280)   // 1920 - 640
    {
        scrollX = 0;
    }
    if (NetHandle == -1)
    {
        NetHandle = GetNewAcceptNetWork();
        return 0;
    }

    StopListenNetWork();
    GetNetWorkIP(NetHandle, &ip);

    // ライアントからの受信
    DataLength = GetNetWorkDataLength(NetHandle);

    if (DataLength >= sizeof(COMDATA))
    {
        COMDATA recvData;
        NetWorkRecv(NetHandle, strBuf, sizeof(COMDATA));
        memcpy(&recvData, strBuf, sizeof(COMDATA));

        base[1]->pos = recvData.pos;
        base[1]->vec = recvData.vec;
        if (base[1]->FLAG)   // 生きている時だけ更新
        {
            base[1]->FLAG = !recvData.dead1;
        }
    }

    // サーバー側のキャラを送信
    COMDATA sendData{};
    sendData.pos = base[0]->pos;
    sendData.vec = base[0]->vec;
    sendData.scrollX = scrollX;

    sendData.dead0 = !base[0]->FLAG;
    sendData.dead1 = !base[1]->FLAG;

    memcpy(strBuf, &sendData, sizeof(COMDATA));
    NetWorkSend(NetHandle, strBuf, sizeof(COMDATA));

    // 更新処理
    ObjList add_list;

    for (auto& obj : base)
    {
        obj->Action(base, add_list);
    }

    return 0;
}



//描画処理
void CGame::Draw()
{
    DrawRectGraph(
        0,
        0,
        (int)scrollX,
        0,
        640,
        480,
        bgHandle,
        TRUE,
        FALSE
    );
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
	
	for (auto& obj : base) obj->Draw(scrollX);
}

CGame::~CGame()
{

}
