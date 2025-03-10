//==============================================================================================================
//
// SPゲージ [spgage.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"result.h"
#include"fade.h"
#include"input.h"
#include "time.h"
#include "camera.h"
#include "resultscore.h"
#include "sound.h"
#include "player.h"
#include "Timer.h"
#include"spgauge.h"
#include "meshsword.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define SPGAUGE_LENGTH (390.0f) // スペシャルゲージの横幅

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SetChargeGage(int nCnt); // ゲージ0の更新

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSPgauge[SPGAUGE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSPgauge = NULL;       // 頂点バッファへのポインタ
SPgauge g_SPgauge[SPGAUGE_MAX];							// 構造体変数

//=======================================================================================================
//リザルトの初期化処理
//=======================================================================================================
void InitSPgauge(void)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\boss_lifeframe.png",
		&g_pTextureSPgauge[0]);

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSPgauge,
		NULL);

	// 頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	g_SPgauge[SPGAUGE_GAUGE_0].col = COLOR_AQUA;
	g_SPgauge[SPGAUGE_GAUGE_1].col = COLOR_YELLOW;
	g_SPgauge[SPGAUGE_GAUGE_2].col = COLOR_RED;

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		g_SPgauge[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 位置を初期化
		g_SPgauge[nCnt].SpGauge = 0.0f;						 // 大きさ
		g_SPgauge[nCnt].nType = nCnt;						 // 種類

		if (nCnt == 1)
		{
			g_SPgauge[nCnt].state = SPGAUGESTATE_CHARGE;
		}
		else
		{
			g_SPgauge[nCnt].state = SPGAUGESTATE_NONE;
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
//リザルトの終了処理
//=======================================================================================================
void UninitSPgauge(void)
{
	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureSPgauge[nCnt] != NULL)
		{
			g_pTextureSPgauge[nCnt]->Release();
			g_pTextureSPgauge[nCnt] = NULL;
		}
	}
	// バッファの破棄
	if (g_pVtxBuffSPgauge != NULL)
	{
		g_pVtxBuffSPgauge->Release();
		g_pVtxBuffSPgauge = NULL;
	}
}
//=======================================================================================================
//リザルトの更新処理
//=======================================================================================================
void UpdateSPgauge(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	pPlayer->SpMode = false; // フィーバーモードをオフにする

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		if (g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			SetChargeGage(nCnt);
		}
		
		if (g_SPgauge[nCnt].nType == SPGAUGE_FRAME)
		{
			pVtx += 4 * nCnt;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SPGAUGE_LENGTH, 80.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SPGAUGE_LENGTH, 105.0f, 0.0f);
		}
	}
	// 頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
//リザルトの描画処理
//=======================================================================================================
void DrawSPgauge(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSPgauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ゲージバーの描画----------------------------------
	// テクスチャの設定	

	for (int nCnt = 1; nCnt < SPGAUGE_MAX; nCnt++)
	{
		int nType = g_SPgauge[nCnt].nType;

		if (nType != SPGAUGE_FRAME)
		{
			// ゲージバーの描画----------------------------------
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
		//---------------------------------------------------
	}

	pDevice->SetTexture(0, g_pTextureSPgauge[0]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類

	//--------------------------------------------------
}
//=======================================================================================================
// ゲージの加算処理
//=======================================================================================================
void AddSpgauge(float fValue)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		if (g_SPgauge[nCnt].SpGauge <= 100.0f && pPlayer->AttackSp == false && g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			// 加算処理
			g_SPgauge[nCnt].SpGauge += fValue;
		}
	}
}
//=======================================================================================================
// ゲージの減算処理
//=======================================================================================================
void DecSpgauge(float fValue)
{
	int NowChargeGage = 0;
	static float Dec = 0.0f;

	for (int nCnt = SPGAUGE_MAX - 1; nCnt >= SPGAUGE_GAUGE_0; nCnt--)
	{
		if (g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			NowChargeGage = nCnt;
			break;
		}
	}

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	if (NowChargeGage != 1)
	{
		// 現在のゲージから減らせる分を消費
		Dec = g_SPgauge[NowChargeGage].SpGauge;
		g_SPgauge[NowChargeGage].SpGauge -= Dec;

		float fRateLength = g_SPgauge[NowChargeGage].SpGauge / 100.0f;
		float fLength = fRateLength * SPGAUGE_LENGTH;

		pVtx += 4 * NowChargeGage;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(10.0f, 125.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fLength, 125.0f, 0.0f);

		float Dec2 = fValue - Dec;

		g_SPgauge[NowChargeGage - 1].SpGauge -= Dec2;

		float fRateLength2 = g_SPgauge[NowChargeGage - 1].SpGauge / 100.0f;
		float fLength2 = fRateLength2 * SPGAUGE_LENGTH;

		pVtx += 4 * (NowChargeGage - 1);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fLength, 105.0f, 0.0f);

	}
	else
	{
		g_SPgauge[NowChargeGage].SpGauge -= fValue;
	}

	// 頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
// ゲージ0の更新
//=======================================================================================================
void SetChargeGage(int nCnt)
{
	Player* pPlayer = GetPlayer();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	float fRateLength = g_SPgauge[nCnt].SpGauge / 100.0f;
	float fLength = fRateLength * SPGAUGE_LENGTH;

	pVtx += 4 * nCnt;

	if (g_SPgauge[nCnt].SpGauge >= 100.0f)
	{
		pPlayer->SpMode = true;
	}

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fLength, 105.0f, 0.0f);

	switch (g_SPgauge[nCnt].nType)
	{
	case SPGAUGE_GAUGE_0:

		// ゲージがMAXだったら
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_1].state = SPGAUGESTATE_CHARGE;
			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// ゲージが最大じゃなかったら
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;
			g_SPgauge[SPGAUGE_GAUGE_1].state = SPGAUGESTATE_NONE;
		}
		break;
	case SPGAUGE_GAUGE_1:

		// ゲージがMAXだったら
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_2].state = SPGAUGESTATE_CHARGE;
			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// ゲージが最大じゃなかったら
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_2].state = SPGAUGESTATE_NONE;
		}
		break;
	case SPGAUGE_GAUGE_2:

		// ゲージがMAXだったら
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// ゲージが最大じゃなかったら
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// 頂点カラーの設定
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;
		}
		break;
	default:
		break;
	}
	// 頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
