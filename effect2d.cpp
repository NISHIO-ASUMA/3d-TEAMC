//===================================================================================================================
//
// エフェクト2d [effect2d.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "effect2d.h"
#include <cassert>

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_EFFECT2D (4096) // エフェクトの最大数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect2d = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2d = NULL;//頂点バッファへのポインタ
Effect2D  g_aEffect2D[MAX_EFFECT2D];//弾の情報

//===================================================================================================================
// エフェクト2Dの初期化処理
//===================================================================================================================
void InitEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイスの取得	

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect2d);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2d,
		NULL);

	//頂点バッファのロック
	g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

	// エフェクトの最大数分回す
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		g_aEffect2D[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aEffect2D[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aEffect2D[nCntEffect].nLife = 0;									// 寿命
		g_aEffect2D[nCntEffect].fRadius = 0.0f;								// 半径
		g_aEffect2D[nCntEffect].fWidth = 0.0f;								// 横幅
		g_aEffect2D[nCntEffect].fHeight = 0.0f;								// 高さ
		g_aEffect2D[nCntEffect].DecAlv = 0.0f;								// アルファ値の減少スピード
		g_aEffect2D[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_aEffect2D[nCntEffect].bUse = false;								// 使用していない状態にする

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

		//頂点カラーの設定bh
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分増やす

		g_pVtxBuffEffect2d->Unlock();
	}
}
//===================================================================================================================
// エフェクト2Dの終了処理
//===================================================================================================================
void UninitEffect2D(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect2d != NULL)
	{
		g_pTextureEffect2d->Release();
		g_pTextureEffect2d = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect2d != NULL)
	{
		g_pVtxBuffEffect2d->Release();
		g_pVtxBuffEffect2d = NULL;
	}

#ifdef _DEBUG

	// テクスチャの破棄ができているか確認
	assert(g_pTextureEffect2d == NULL && "effect2d");

	// 頂点バッファの破棄ができているか確認
	assert(g_pVtxBuffEffect2d == NULL && "effect2d");

#endif

}
//===================================================================================================================
// エフェクト2Dの更新処理
//===================================================================================================================
void UpdateEffect2D(void)
{
	// 頂点バッファのポインタ
	VERTEX_2D* pVtx;

	// エフェクトの最大数分回す
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		if (g_aEffect2D[nCntEffect].bUse == false) continue;

		// 位置の更新
		g_aEffect2D[nCntEffect].pos += g_aEffect2D[nCntEffect].move;

		// 位置を代入
		D3DXVECTOR3 pos = g_aEffect2D[nCntEffect].pos;

		// 半径を代入
		float fRadius = g_aEffect2D[nCntEffect].fRadius;

		// アルファ値の減少スピード
		g_aEffect2D[nCntEffect].DecAlv = 1.0f / g_aEffect2D[nCntEffect].nLife;

		// 寿命を減らす
		g_aEffect2D[nCntEffect].nLife--;

		// アルファ値を減らす
		g_aEffect2D[nCntEffect].col.a -= g_aEffect2D[nCntEffect].DecAlv;

		// 寿命が尽きたら
		if (g_aEffect2D[nCntEffect].nLife <= 0)
		{
			// 画面から消す
			g_aEffect2D[nCntEffect].bUse = false;
		}

		//頂点バッファのロック
		g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEffect;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fRadius,pos.y - fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fRadius,pos.y - fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fRadius,pos.y + fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fRadius,pos.y + fRadius, 0.0f);

		//頂点カラーの設定bh
		pVtx[0].col = g_aEffect2D[nCntEffect].col;
		pVtx[1].col = g_aEffect2D[nCntEffect].col;
		pVtx[2].col = g_aEffect2D[nCntEffect].col;
		pVtx[3].col = g_aEffect2D[nCntEffect].col;

		//頂点バッファのアンロック
		g_pVtxBuffEffect2d->Unlock();
	}
}
//===================================================================================================================
// エフェクト2Dの描画処理
//===================================================================================================================
void DrawEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイスの習得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect2d, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 全エフェクト分ま合わす
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		// 未使用だったら処理を飛ばす
		if (g_aEffect2D[nCntEffect].bUse == false) continue;
		
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEffect2d);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//プリミティブの種類
		
	}

	//aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================================================================================================
// エフェクト2Dの設定処理
//===================================================================================================================
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_2D* pVtx;

	g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		// エフェクトが使用されていない
		if (g_aEffect2D[nCntEffect].bUse == false)
		{ 
			g_aEffect2D[nCntEffect].pos = pos;			// 位置
			g_aEffect2D[nCntEffect].move = move;		// 移動量
			g_aEffect2D[nCntEffect].col = col;			// 色
			g_aEffect2D[nCntEffect].fRadius = fRadius;	// 半径
			g_aEffect2D[nCntEffect].nLife = nLife;		// 寿命
			g_aEffect2D[nCntEffect].bUse = true;		// 使用状態

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fRadius, pos.y - fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fRadius, pos.y - fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fRadius, pos.y + fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fRadius, pos.y + fRadius, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffEffect2d->Unlock();
}
