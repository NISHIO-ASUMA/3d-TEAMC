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
bool IsStoneBat(Item* pItem, Player* pPlayer); // ストーンバットの素材を持っているか判定
bool IsSurfboardFish(Item* pItem, Player* pPlayer);     // 魚の素材を持っているか判定
bool IsHexMandorin(Item* pItem, Player* pPlayer);      // 呪物の素材を持っているか判定
bool IsGolfHunmer(Item* pItem, Player* pPlayer);   // ハンマーの素材を持っているか判定
bool IsMixKatana(Item* pItem, Player* pPlayer);   // 刀系統の素材を持っているか判定
bool IsHexSpear(Item* pItem, Player* pPlayer);    // 槍系統の素材を持っているか判定
bool IsHeadTorso(Item* pItem, Player* pPlayer);    // 首付きマネキンの素材を持っているか判定

void UpdateRecipeUIParam(void);				   // レシピUIのパラメータ設定

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
		g_aRecipe[nCnt].pos = D3DXVECTOR3(600.0f,300.0,0.0f); // 位置
		g_aRecipe[nCnt].nType = nCnt;						  // 種類
		g_aRecipe[nCnt].fWidth = 200.0f;					  // 横幅
		g_aRecipe[nCnt].fHeight = 150.0f;					  // 高さ
		g_aRecipe[nCnt].bUse = false;						  // 使用状態
		g_aRecipe[nCnt].state = RECIPEUISTATE_NONE;           // UIの状態

		D3DXVECTOR3 pos = g_aRecipe[nCnt].pos;
		float fWidth = g_aRecipe[nCnt].fWidth;
		float fHeight = g_aRecipe[nCnt].fHeight;

			//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

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
	// レシピのパラメータ設定
	UpdateRecipeUIParam();
	
	// 表示されているUIの種類0
	static int SetUIType1 = 0;

	// 表示されているUIの種類1
	static int SetUIType2 = 0;

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// UIがセットされていたら
		if (g_aRecipe[nCnt].state == RECIPEUISTATE_SET)
		{
			// インデックスを取得
			SetUIType1 = nCnt;

			// 抜ける
			break;
		}
	}

	// レシピの種類分
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// UIがセットされているかつ1つめと別の種類だったら
		if (g_aRecipe[nCnt].state == RECIPEUISTATE_SET && nCnt != SetUIType1)
		{
			// 2つめのUIのインデックスを取得
			SetUIType2 = nCnt;
			break;
		}
	}

	// 1個目と2個目がセットされているかつ、別々の種類
	if (g_aRecipe[SetUIType1].state == RECIPEUISTATE_SET && g_aRecipe[SetUIType2].state == RECIPEUISTATE_SET && SetUIType1 != SetUIType2)
	{
		// 頂点座標を更新
		SetCraftRecipe(g_aRecipe[SetUIType1].pos, SetUIType1, SetUIType2, g_aRecipe[SetUIType1].fWidth, g_aRecipe[SetUIType1].fHeight);
	}
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
			pDevice->SetTexture(0, g_pTextureRecipe[nCnt]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// クラフトレシピの設定処理
//==============================================================================================================
void SetCraftRecipe(D3DXVECTOR3 pos, int nType0,int nType1, float fWidth, float fHeight)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファのポインタを進める
	pVtx += 4 * nType0;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x + fWidth, pos.y + fHeight, 0.0f);

	// 頂点バッファのアンロック
	g_pVtxBuffRecipe->Unlock();

	// 頂点バッファのロック
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファのポインタを進める
	pVtx += 4 * nType1;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x + fWidth, pos.y + fHeight, 0.0f);

	// 頂点バッファのアンロック
	g_pVtxBuffRecipe->Unlock();
}
//==============================================================================================================
// クラフトレシピの有効無効処理
//==============================================================================================================
void EnableRecipeUI(int nType, bool bUse)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 使用状態だったら
	if (g_aRecipe[nType].bUse == true && bUse == false)
	{
		g_aRecipe[nType].state = RECIPEUISTATE_NONE;
		g_aRecipe[nType].bUse = bUse;
	}
	// 使用してなかったら
	else if (g_aRecipe[nType].bUse == false && bUse == true)
	{
		g_aRecipe[nType].state = RECIPEUISTATE_SET;
		g_aRecipe[nType].bUse = bUse;
	}


	if (g_aRecipe[nType].bUse == true)
	{
		// 頂点バッファのロック
		g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点バッファのポインタを進める
		pVtx += 4 * nType;

		// 位置
		D3DXVECTOR3 pos = g_aRecipe[nType].pos;

		// 横幅
		float fWidth = g_aRecipe[nType].fWidth;

		// 高さ
		float fHeight = g_aRecipe[nType].fHeight;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

		// 頂点バッファのアンロック
		g_pVtxBuffRecipe->Unlock();
	}
}
//==============================================================================================================
// ストーンバットの素材を持っているか判定
//==============================================================================================================
bool IsStoneBat(Item* pItem, Player* pPlayer)
{
	// 石を持っているか判定
	const bool HaveStone = pPlayer->HoldItemType == ITEMTYPE_STONE;

	// 石をストックしているか判定
	const bool StockStone = pPlayer->StockItemType == ITEMTYPE_STONE;

	// バットを持っているか判定
	const bool HaveBat = pPlayer->HoldItemType == ITEMTYPE_BAT;

	// バットをストックしているか判定
	const bool StockBat = pPlayer->StockItemType == ITEMTYPE_BAT;

	// 石バットのレシピを表示できるか判定
	const bool CanSetUI = HaveStone || StockStone || HaveBat || StockBat;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;
}
//==============================================================================================================
// 魚の素材を持っているか判定
//==============================================================================================================
bool IsSurfboardFish(Item* pItem, Player* pPlayer)
{
	// 石を持っているか判定
	const bool HaveFish = pPlayer->HoldItemType == ITEMTYPE_FISH;

	// 石をストックしているか判定
	const bool StockFish = pPlayer->StockItemType == ITEMTYPE_FISH;

	// バットを持っているか判定
	const bool HaveSurfBoard = pPlayer->HoldItemType == ITEMTYPE_SURFBOARD;

	// バットをストックしているか判定
	const bool StockSurfBoard = pPlayer->StockItemType == ITEMTYPE_SURFBOARD;

	// サーフボード魚のレシピを表示できるか判定
	const bool CanSetUI = HaveFish || StockFish || HaveSurfBoard || StockSurfBoard;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;

}
//==============================================================================================================
// 呪物の素材を持っているか判定
//==============================================================================================================
bool IsHexMandorin(Item* pItem, Player* pPlayer)
{
	// 呪物を持っているか判定
	const bool HaveHex = pPlayer->HoldItemType == ITEMTYPE_HEX;

	// 呪物をストックしているか判定
	const bool StockHex = pPlayer->StockItemType == ITEMTYPE_HEX;

	// 楽器を持っているか判定
	const bool HaveMandorin = pPlayer->HoldItemType == ITEMTYPE_MANDORIN;

	// 楽器をストックしているか判定
	const bool StockMandorin = pPlayer->StockItemType == ITEMTYPE_MANDORIN;

	// 呪いの楽器のレシピを表示できるか判定
	const bool CanSetUI = HaveHex || StockHex || HaveMandorin || StockMandorin;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;
}
//==============================================================================================================
// ハンマーの素材を持っているか判定
//==============================================================================================================
bool IsGolfHunmer(Item* pItem, Player* pPlayer)
{
	// ゴルフを持っているか判定
	const bool HaveGolf = pPlayer->HoldItemType == ITEMTYPE_GOLF;

	// ゴルフをストックしているか判定
	const bool StockGolf = pPlayer->StockItemType == ITEMTYPE_GOLF;

	// ハンマーを持っているか判定
	const bool HaveHunmer = pPlayer->HoldItemType == ITEMTYPE_HUNMER;

	// ハンマーをストックしているか判定
	const bool StockHunmer = pPlayer->StockItemType == ITEMTYPE_HUNMER;

	// ゴルフハンマーのレシピを表示できるか判定
	const bool CanSetUI = HaveGolf || StockGolf || HaveHunmer || StockHunmer;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;
}
//==============================================================================================================
// 刀系統の素材を持っているか判定
//==============================================================================================================
bool IsMixKatana(Item* pItem, Player* pPlayer)
{
	bool bFlag = false;
	
	// 刀を持っているか判定
	const bool HaveKatana = pPlayer->HoldItemType == ITEMTYPE_KATANA;

	// 刀をストックしているか判定
	const bool StockKatana = pPlayer->StockItemType == ITEMTYPE_KATANA;

	// トーチを持っているか判定
	const bool HaveTorch = pPlayer->HoldItemType == ITEMTYPE_TORCH;

	// トーチをストックしているか判定
	const bool StockTorch = pPlayer->StockItemType == ITEMTYPE_TORCH;

	// 炎の剣のレシピを表示できるか判定
	const bool FireSwordSetUI = HaveKatana || StockKatana || HaveTorch || StockTorch;

	// UIを表示できる
	if (FireSwordSetUI == true)
	{
		// 表示できる
		bFlag = true;
	}

	// 氷を持っているか判定
	const bool HaveIceBlock = pPlayer->HoldItemType == ITEMTYPE_ICEBLOCK;

	// 氷をストックしているか判定
	const bool StockIceBlock = pPlayer->StockItemType == ITEMTYPE_ICEBLOCK;

	// 氷の剣のレシピを表示できるか判定
	const bool IceSwordSetUI = HaveKatana || StockKatana || HaveIceBlock || StockIceBlock;

	// UIを表示できる
	if (IceSwordSetUI == true)
	{
		bFlag = true;
	}

	// 蛍光灯を持っているか判定
	const bool HaveLight = pPlayer->HoldItemType == ITEMTYPE_LIGHT;

	// 蛍光灯をストックしているか判定
	const bool StockLight = pPlayer->StockItemType == ITEMTYPE_LIGHT;

	// 光の剣のレシピを表示できるか判定
	const bool LightSwordSetUI = HaveKatana || StockKatana || HaveLight || StockLight;

	if (LightSwordSetUI == true)
	{
		bFlag = true;
	}

	// 表示できない
	return bFlag;
}
//==============================================================================================================
// 槍系統の素材を持っているか判定
//==============================================================================================================
bool IsHexSpear(Item* pItem, Player* pPlayer)
{
	// 槍を持っているか判定
	const bool HaveSpear = pPlayer->HoldItemType == ITEMTYPE_SPEAR;

	// 槍をストックしているか判定
	const bool StockSpear = pPlayer->StockItemType == ITEMTYPE_SPEAR;

	// 骨を持っているか判定
	const bool HaveBone = pPlayer->HoldItemType == ITEMTYPE_BONE;

	// 骨をストックしているか判定
	const bool StockBone = pPlayer->StockItemType == ITEMTYPE_BONE;

	// 骨の槍のレシピを表示できるか判定
	const bool CanSetUI = HaveSpear || StockSpear || HaveBone || StockBone;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;
}
//==============================================================================================================
// 首付きマネキンの素材を持っているか判定
//==============================================================================================================
bool IsHeadTorso(Item* pItem, Player* pPlayer)
{
	// 頭蓋骨を持っているか判定
	const bool HaveTorso = pPlayer->HoldItemType == ITEMTYPE_TORSO;

	// 頭蓋骨をストックしているか判定
	const bool StockTorso = pPlayer->StockItemType == ITEMTYPE_TORSO;

	// 首無しマネキンを持っているか判定
	const bool HaveHead = pPlayer->HoldItemType == ITEMTYPE_HEADSTATUE;

	// 首無しマネキンをストックしているか判定
	const bool StockHead = pPlayer->StockItemType == ITEMTYPE_HEADSTATUE;

	// 骨の槍のレシピを表示できるか判定
	const bool CanSetUI = HaveTorso || StockTorso || HaveHead || StockHead;

	// UIを表示できる
	if (CanSetUI == true)
	{
		// 表示できる
		return true;
	}

	// 表示できない
	return false;

}
//==============================================================================================================
// レシピUIのパラメータ設定
//==============================================================================================================
void UpdateRecipeUIParam(void)
{
	// アイテムの取得
	Item* pItem = GetItem();

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 素材を持っていたら
	if (IsStoneBat(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_STONEBAT, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_STONEBAT, false);
	}

	// 魚の素材を持っていたら
	if (IsSurfboardFish(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_FISH, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_FISH, false);
	}

	// 呪いの楽器の素材を持っていたら
	if (IsHexMandorin(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_HEX, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_HEX, false);
	}

	// ゴルフハンマーの素材を持っていたら
	if (IsGolfHunmer(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_HUMMER, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_HUMMER, false);
	}

	// 刀系の素材を持っていたら
	if (IsMixKatana(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_KATANA, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_KATANA, false);
	}

	// 槍の素材を持っていたら
	if (IsHexSpear(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_SPEAR, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_SPEAR, false);
	}

	// 首付きマネキンの素材を持っていたら
	if (IsHeadTorso(pItem, pPlayer) == true)
	{
		// 有効にする
		EnableRecipeUI(RECIPETYPE_TORSO, true);
	}
	else
	{
		// 無効にする
		EnableRecipeUI(RECIPETYPE_TORSO, false);
	}

}
