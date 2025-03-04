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
//グローバル変数
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
		"data\\TEXTURE\\SP_frame.png",
		&g_pTextureSPgauge[0]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SP_gage.png",
		&g_pTextureSPgauge[4]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha01.png", &g_pTextureSPgauge[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha02.png", &g_pTextureSPgauge[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha03.png", &g_pTextureSPgauge[3]);

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSPgauge,
		NULL);

	// 頂点ロック
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	// 種類を設定
	g_SPgauge[0].nType = SPGAUGE_FRAME0;
	g_SPgauge[1].nType = SPGAUGE_GAUGE;

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		g_SPgauge[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 位置を初期化
		g_SPgauge[nCnt].SpGauge = 0.0f;						 // 大きさ

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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
		if (g_SPgauge[nCnt].nType == SPGAUGE_GAUGE)
		{
			// 最大の値
			float fDest = g_SPgauge[nCnt].SpGauge / 300.0f;

			// 横幅
			float fWidth = fDest * 900.0f;

			// 100.0fたまったら発動可能
			if (g_SPgauge[nCnt].SpGauge >= 100.0f)
			{
				pPlayer->SpMode = true; // 発動可能
			}

			// 300.0f以上になったら
			if (g_SPgauge[nCnt].SpGauge >= 300.0f)
			{
				// 300.0f以上にならないようにする
				g_SPgauge[nCnt].SpGauge = 300.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 70.0f, 0.0f);
		}
		else
		{
			// フレームの溜まったゲージの数値を決定
			g_SPgauge[nCnt].nType = g_SPgauge[1].SpGauge / 100.0f;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(900.0f, 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(900.0f, 70.0f, 0.0f);

			if (g_SPgauge[1].SpGauge >= 100)
			{
				// フレーム用変数
				static int nFrame = 0;

				// フレームが2になったら
				if (nFrame == 2)
				{
					nFrame = 0; // フレームを初期化
					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
				}
				else
				{
					// インクリメント
					nFrame++;
				}
			}
		}
		pVtx += 4;
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
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[1].nType]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2); // プリミティブの種類
	//---------------------------------------------------

	// ゲージの枠の描画-------------------------------
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[0].nType]);

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

	if (g_SPgauge[1].SpGauge < 300.0f && !pPlayer->AttackSp)
	{
		// 加算処理
		g_SPgauge[1].SpGauge += fValue;
	}
}
