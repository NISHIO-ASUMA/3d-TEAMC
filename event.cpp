//==============================================================================================================
//
// イベント [event.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "event.h"
#include "mark.h"
#include "meshcylinder.h"
#include "boss.h"
#include "game.h"
#include "gameui.h"
#include "count.h"
#include "sound.h"
#include "time.h"
#include "camera.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define START_EVENTTIME (9999) // イベントが始まるまでの時間
#define POSITIONONE_RADIUS (700.0f) // 0番目のイベントの半径

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
Event g_Event[EVENTPOSITION_MAX]; // イベントの情報
int g_nStartTime = 0;
bool bEvent = false;
int g_EventPos = 0;

//==============================================================================================================
// イベントの初期化処理
//==============================================================================================================
void InitEvent(void)
{
	// イベントの場所分回す
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		g_Event[nCnt].IventPosition = nCnt;   // イベントの場所
		g_Event[nCnt].nTime = 0;              // イベントの時間
		g_Event[nCnt].bEvent = false;         // イベントが発生しているか
	}

	// イベントが始まるまでの時間
	g_nStartTime = 0;
	bEvent = false;
	g_EventPos = 0;
}

//==============================================================================================================
// イベントの設定処理
//==============================================================================================================
void SetEvent(D3DXVECTOR3 Eventpos, int IventPosition, int nTime)
{
	// イベントの場所分回す
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// 未使用だったら
		if (g_Event[nCnt].bEvent == false)
		{
			g_Event[nCnt].Eventpos = Eventpos; // 位置
			g_Event[nCnt].IventPosition = IventPosition;   // イベントの場所
			g_Event[nCnt].nTime = nTime;				   // イベントの時間
			g_Event[nCnt].bEvent = true;				   // イベントが発生

			break;
		}
	}
}
//==============================================================================================================
// イベントの更新処理
//==============================================================================================================
void UpdateEvent(void)
{
	//GAMESTATE gamestate = GetGameState();

	//if (bEvent == false)
	//{
	//	// インクリメント
	//	g_nStartTime++;
	//}

	//int Minute = GetTimeMinute();
	//int Second = GetTimeSecond();

	//// 10秒いかだったら
	//if (Minute <= 0 && Second <= 10)
	//{
	//	// 関数を抜ける
	//	return;
	//}

	//// イベントの場所分回す
	//for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	//{
	//	// 使用中だったら
	//	if (g_Event[nCnt].bEvent == true)
	//	{
	//		continue;
	//	}

	//	// イベントが始まる
	//	if (g_nStartTime >= START_EVENTTIME)
	//	{
	//		// イベントの場所を決める
	//		int EventPos = rand() % EVENTPOSITION_MAX;

	//		// イベントの場所を代入
	//		g_EventPos = EventPos;

	//		// イベントの場所
	//		switch (EventPos)
	//		{
	//		case EVENTPOSITION_ONE:
	//			// イベントのパラメーターの設定
	//			SetEventParam(EVENTPOS_ONE, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_TWO:
	//			// イベントのパラメーターの設定
	//			SetEventParam(EVENTPOS_TWO, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_THREE:
	//			// イベントのパラメーターの設定
	//			SetEventParam(EVENTPOS_THREE, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_FOUR:
	//			// イベントのパラメーターの設定
	//			SetEventParam(EVENTPOS_FOUR, EventPos, nCnt);
	//			break;
	//		default:
	//			break;
	//		}

	//		g_nStartTime = 0;

	//		break;
	//	}
	//}

	//// イベントの場所分回す
	//for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	//{
	//	// 未使用だったら
	//	if (g_Event[nCnt].bEvent == false)
	//	{
	//		continue;
	//	}

	//	bEvent = true;

	//	if (gamestate == GAMESTATE_NORMAL)
	//	{
	//		g_Event[nCnt].nTime--;
	//	}

	//	// イベントが終わった
	//	if (g_Event[nCnt].nTime <= 0)
	//	{
	//		DeleteCylinder(g_Event[nCnt].nCylinderIdx);
	//		g_Event[nCnt].bEvent = false;
	//		bEvent = false;
	//	}
	//}

}
//==============================================================================================================
// イベントの状態取得
//==============================================================================================================
bool EnableEvent(void)
{
	return bEvent;
}
//==============================================================================================================
// イベントの発生地点の取得
//==============================================================================================================
int GetEventPos(void)
{
	return g_EventPos;
}
//==============================================================================================================
// イベントの強制終了
//==============================================================================================================
void SetEndEvent(bool bEnableEvent)
{
	// イベントの場所分回す
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// 使用中だったら
		if (g_Event[nCnt].bEvent == true)
		{
			// タイムカウントをリセット
			g_Event[nCnt].nTime = 0;

			// シリンダーを消去
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);

			// 使用状態を入力
			g_Event[nCnt].bEvent = bEvent;

			// イベントを終わらせる
			bEvent = false;

			return;
		}
	}
}
//==============================================================================================================
// イベントのパラメーターの設定
//==============================================================================================================
void SetEventParam(D3DXVECTOR3 pos)
{
	// 音楽再生
	PlaySound(SOUND_LABEL_EVENTSE);
	
	//// イベントの設定
	//SetEvent(pos, eventpos, 2700);

	//// シリンダーの設定
	//g_Event[nCnt].nCylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);

	// ボスを設定
	SetBoss(pos, 3.0f, 10000);

	// ムービーを設定
	SetMovie(300);

	// ムービーを有効にする
	EnableMovie(true);

	// UIを設定
	SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);

	//// タイマーを設定
	//SetCounter(D3DXVECTOR3(1080.0f, 670.0f, 0.0f), COUNTER_COUNTDOWN, 45, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
}
