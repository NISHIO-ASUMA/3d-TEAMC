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

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define START_EVENTTIME (900) // イベントが始まるまでの時間
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
	g_Event[EVENTPOSITION_ONE].Eventpos = EVENTPOS_ONE; // 位置の初期化

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
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT,-1, POSITIONONE_RADIUS,COLOR_YELLOW,8,1,0.0f,2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_TWO:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_THREE:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_FOUR:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // ボスをセット
				SetMovie(300);
				EnableMovie(true);
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

		g_Event[nCnt].nTime--;

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