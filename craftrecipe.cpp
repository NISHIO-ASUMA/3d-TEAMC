//==============================================================================================================
//
// クラフトのレシピ [craftrecipe.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "craftrecipe.h"
#include "mark.h"
#include "item.h"

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRecipe[RECIPETYPE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRecipe = NULL; // 頂点バッファへのポインタ
CraftRecipe g_aRecipe[RECIPETYPE_MAX]; // レシピの情報

//==============================================================================================================
// クラフトレシピの初期化処理
//==============================================================================================================
void InitCraftRecipe(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// テクスチャの読み込みコロン
		D3DXCreateTextureFromFile(pDevice,
			RECIPE_TEXTURE[nCnt],
			&g_pTextureRecipe[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RECIPETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRecipe,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		g_aRecipe[nCnt].pos = NULLVECTOR3; // 位置
		g_aRecipe[nCnt].nType = nCnt;      // 種類
		g_aRecipe[nCnt].fWidth = 0.0f;     // 横幅
		g_aRecipe[nCnt].fHeight = 0.0f;    // 高さ
		g_aRecipe[nCnt].bUse = false;      // 使用状態

		//頂点座標の設定
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

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffRecipe->Unlock();

	SetCraftRecipe(D3DXVECTOR3(400.0f, 400.0f, 0.0f), RECIPETYPE_STONEBAT, 100.0f, 50.0f);
}
//==============================================================================================================
// クラフトレシピの終了処理
//==============================================================================================================
void UninitCraftRecipe(void)
{
	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (g_pTextureRecipe[nCnt] != NULL)
		{
			g_pTextureRecipe[nCnt]->Release();
			g_pTextureRecipe[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRecipe != NULL)
	{
		g_pVtxBuffRecipe->Release();
		g_pVtxBuffRecipe = NULL;
	}

}
//==============================================================================================================
// クラフトレシピの更新処理
//==============================================================================================================
void UpdateCraftRecipe(void)
{
	// アイテムの取得
	Item* pItem = GetItem();

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		D3DXVECTOR3 pos = g_aRecipe[nCnt].pos; // 位置
		float fWidth = g_aRecipe[nCnt].fWidth; // 横幅
		float fHeight = g_aRecipe[nCnt].fHeight; // 高さ

		if ((pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONE || pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_STONE) &&
			(pItem[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_BAT))
		{
			EnableRecipeUI(RECIPETYPE_STONEBAT, true);
		}
		else
		{
			EnableRecipeUI(RECIPETYPE_STONEBAT, false);
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
		
		pVtx += 4;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffRecipe->Unlock();

}
//==============================================================================================================
// クラフトレシピの描画処理
//==============================================================================================================
void DrawCraftRecipe(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRecipe, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// 使用状態だったら
		if (g_aRecipe[nCnt].bUse == true)
		{
			int nType = g_aRecipe[nCnt].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRecipe[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// クラフトレシピの設定処理
//==============================================================================================================
void SetCraftRecipe(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// 使用状態だったら
		if (g_aRecipe[nCnt].bUse == false)
		{
			g_aRecipe[nCnt].pos = pos;		   // 位置
			g_aRecipe[nCnt].nType = nType;     // 種類
			g_aRecipe[nCnt].fWidth = fWidth;   // 横幅
			g_aRecipe[nCnt].fHeight = fHeight; // 高さ

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffRecipe->Unlock();
}
//==============================================================================================================
// クラフトレシピの有効無効処理
//==============================================================================================================
void EnableRecipeUI(int nType, bool bUse)
{
	// 使用状態だったら
	if (g_aRecipe[nType].bUse == true && bUse == false)
	{
		g_aRecipe[nType].bUse = bUse;
	}
	// 使用してなかったら
	else if (g_aRecipe[nType].bUse == false && bUse == true)
	{
		g_aRecipe[nType].bUse = bUse;
	}
}
