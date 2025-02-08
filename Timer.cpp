//================================================================================================================
//
// タイマー処理 [Timer.cpp]
// Author:Asuma Nishio
//
//================================================================================================================

//******************************
// インクルードファイル宣言
//******************************
#include "Timer.h"

//******************************
// マクロ定義
//******************************
#define MAX_TIMER (3)	// 最大テクスチャ数

//******************************
// 構造体宣言
//******************************
typedef struct
{
	bool bUse;		// 使用判定
}Timer;

//******************************
// グローバル変数宣言
//******************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimer[MAX_TIMER] = {};	// テクスチャのポインタ
Timer g_Timer[MAX_TIMER];							// タイマー構造体
int g_nTimer = 0;									// カウント用の変数1
int g_nTimerCount = 10;								// 最大時間

//=======================================================================================================
// タイマーの初期化処理
//=======================================================================================================
void InitTimer(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;				

	for (int nCnttime = 0;nCnttime < MAX_TIMER;nCnttime++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimer[nCnttime]);
	}

	// 構造体初期化
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		g_Timer[nCnt].bUse = true; // 使用判定
	}

	// グローバル変数の初期化
	g_nTimer = 0;	
	g_nTimerCount = 100; // メインのゲーム時間

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_TIMER; nCnt1++)
	{
		//頂点座標1の設定
		pVtx[0].pos = D3DXVECTOR3(1050.0f +(60.0f* nCnt1),10.0f,0.0f);	// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(1100.0f +(60.0f* nCnt1),10.0f,0.0f);	// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(1050.0f +(60.0f* nCnt1),80.0f,0.0f);	// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(1100.0f +(60.0f* nCnt1),80.0f,0.0f);	// 4つ目の頂点情報

		// rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// ポインタをずらす
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTimer->Unlock();

}
//=======================================================================================================
// タイマーの終了処理
//=======================================================================================================
void UninitTimer(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (g_pTextureTimer[nCnt] != NULL)
		{
			g_pTextureTimer[nCnt]->Release();
			g_pTextureTimer[nCnt] = NULL;
		}

	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}
//=========================================================================================================
// タイマーの更新処理
//=========================================================================================================
void UpdateTimer(void)
{
	// ローカル変数
	int g_nData[MAX_TIMER] = {};

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// タイマーを加算
	g_nTimer++;				

	if (g_nTimer >= 60)
	{// 60を超えたら
		g_nTimerCount--;	// 時間を1秒減らす
		g_nTimer = 0;		// 初期値に戻す
	}

	g_nData[0] = g_nTimerCount % 1000 / 100;		// 百の位
	g_nData[1] = g_nTimerCount % 100 / 10;			// 十の位
	g_nData[2] = g_nTimerCount % 10 / 1;			// 一の位

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_nData[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nData[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_nData[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nData[nCnt]), 1.0f);

		// ポインタをずらす
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTimer->Unlock();

}
//=========================================================================================================
//タイマーの描画処理
//=========================================================================================================
void DrawTimer(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;		

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTimer[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

}
//======================================================================================================
//タイマーの取得
//======================================================================================================
int GetTimer(void)
{
	return g_nTimerCount;
}
