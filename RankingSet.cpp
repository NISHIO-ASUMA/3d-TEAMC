//=========================================
//
// ランキング(順位) [rankingset.cpp]
// Author: TEAM_C
//
//=========================================

//****************************
//インクルードファイル
//****************************
#include "RankingSet.h"
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"Score.h"

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureRankSet = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankSet = NULL;//頂点バッファへのポインタ

//=============================
//ランキング(順位)の初期化処理
//=============================
void InitRankingSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み(順位)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ranking.png",
		&g_pTextureRankSet);

	//頂点バッファの生成・頂点情報の設定///順位
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankSet,
		NULL);

	//頂点バッファのロック
	g_pVtxBuffRankSet->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffRankSet->Unlock();
}
//=============================
//ランキング(順位)の終了処理
//=============================
void UninitRankingSet(void)
{
	//テクスチャの破棄
	if (g_pTextureRankSet != NULL)
	{
		g_pTextureRankSet->Release();
		g_pTextureRankSet = NULL;
	}
	if (g_pVtxBuffRankSet != NULL)
	{
		g_pVtxBuffRankSet->Release();
		g_pVtxBuffRankSet = NULL;
	}
}
//=============================
//ランキング(順位)の更新処理
//=============================
void UpdateRankingSet(void)
{
	// 無し
}
//=============================
//ランキング(順位)の描画処理
//=============================
void DrawRankingSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの習得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankSet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankSet);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//プリミティブの種類
}
