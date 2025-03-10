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

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define START_EVENTTIME (1800) // イベントが始まるまでの時間
#define POSITIONONE_RADIUS (700.0f) // 0番目のイベントの半径

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
	GAMESTATE gamestate = GetGameState();

	if (bEvent == false)
	{
		// インクリメント
		g_nStartTime++;
	}

	// イベントの場所分回す
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// 未使用だったら
		if (g_Event[nCnt].bEvent == true)
		{
			continue;
		}

		// イベントが始まる
		if (g_nStartTime >= START_EVENTTIME)
		{
			// イベントの場所を決める
			int EventPos = rand() % EVENTPOSITION_MAX;

			// イベントの場所を代入
			g_EventPos = EventPos;

			// イベントの場所
			switch (EventPos)
			{
			case EVENTPOSITION_ONE:
				// 音楽再生
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_ONE, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_ONE, CYLINDERTYPE_EVENT,-1, POSITIONONE_RADIUS,COLOR_YELLOW,16,1,0.0f,2000.0f);
				SetBoss(EVENTPOS_ONE, 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_TWO:
				// 音楽再生
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_TWO, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_TWO, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_TWO, 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_THREE:
				// 音楽再生
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_THREE, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_THREE, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_THREE, 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_FOUR:
				// 音楽再生
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_FOUR, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_FOUR, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_FOUR, 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);

				break;
			default:
				break;
			}

			g_nStartTime = 0;

			break;
		}
	}

	// イベントの場所分回す
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// 未使用だったら
		if (g_Event[nCnt].bEvent == false)
		{
			continue;
		}

		bEvent = true;

		if (gamestate == GAMESTATE_NORMAL)
		{
			g_Event[nCnt].nTime--;
		}

		// イベントが終わった
		if (g_Event[nCnt].nTime <= 0)
		{
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);
			g_Event[nCnt].bEvent = false;
			bEvent = false;
		}
	}

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
			g_Event[nCnt].nTime = 0;
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);
			g_Event[nCnt].bEvent = bEvent;
			bEvent = false;

			return;
		}
	}
}