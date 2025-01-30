//============================
//
// リザルト [result.cpp]
// Author:TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
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

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureSPgauge[SPGAUGE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSPgauge = NULL;//頂点バッファへのポインタ
SPgauge g_SPgauge[SPGAUGE_MAX];

//=====================
//リザルトの初期化処理
//=====================
void InitSPgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SP_frame.png",
		&g_pTextureSPgauge[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SP_gage.png",
		&g_pTextureSPgauge[1]);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSPgauge,
		NULL);

	//頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	g_SPgauge[0].nType = SPGAUGE_FRAME;
	g_SPgauge[1].nType = SPGAUGE_GAUGE;

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		g_SPgauge[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 位置を初期化
		g_SPgauge[nCnt].SpGauge = 0.0f; // 大きさ

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
//=====================
//リザルトの終了処理
//=====================
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
	if (g_pVtxBuffSPgauge != NULL)
	{
		g_pVtxBuffSPgauge->Release();
		g_pVtxBuffSPgauge = NULL;
	}
}
//=====================
//リザルトの更新処理
//=====================
void UpdateSPgauge(void)
{
	VERTEX_2D* pVtx;

	//頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		if (g_SPgauge[nCnt].nType == SPGAUGE_FRAME)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
		}
		else if (g_SPgauge[nCnt].nType == SPGAUGE_GAUGE)
		{
			// 最大の値
			float fDest = g_SPgauge[nCnt].SpGauge / 100.0f;

			// 横幅
			float fWidth = fDest * 1000.0f;

			if (g_SPgauge[nCnt].SpGauge >= 100.0f)
			{
				g_SPgauge[nCnt].SpGauge = 100.0f;
			}
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 100.0f, 0.0f);
		}

		pVtx += 4;
	}
	//頂点ロック解除
	g_pVtxBuffSPgauge->Unlock();
}
//=====================
//リザルトの描画処理
//=====================
void DrawSPgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの習得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSPgauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ゲージバーの描画----------------------------------
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[1].nType]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);//プリミティブの種類
	//---------------------------------------------------

	// ゲージの枠の描画-------------------------------
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[0].nType]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//プリミティブの種類
	//--------------------------------------------------
}
//=====================
// ゲージの加算処理
//=====================
void AddSpgauge(float fValue)
{
	if (g_SPgauge[1].SpGauge <= 100.0f)
	{
		g_SPgauge[1].SpGauge += fValue;
	}
}
