//=================================================================================================================
//
// 武器のアイコン処理 [icon.cpp]
// Author: Asuma Nishio
//
//=================================================================================================================

//******************************************************************************************************************
// インクルードファイル宣言
//******************************************************************************************************************
#include "item.h"
#include "icon.h"
#include"math.h"
#include "easing.h"

//******************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************
void SetWeponDurabilityAnim(int nCnt);		// アイテムアイコンの点滅処理
void CraftScreenAnim(int nCnt);             // クラフト画面のアイコンの設定
void SetVtxBufferIcon(int nCnt,float fWidth,float fHeight, D3DXVECTOR3 pos); // アイコンの頂点バッファの設定
void SetUpDownIcon(int nCnt,int *EaseCnt);
void ShowCraftingAnim(int nCnt);

//******************************************************************************************************************
// グローバル変数宣言
//******************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureIcon[WEPONTYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;      // 頂点バッファへのポインタ
ICON g_Icon[WEPONTYPE_MAX]; // 構造体変数
bool g_bCraftIconAnim = false;

//=================================================================================================================
// アイコンの初期化処理
//=================================================================================================================
void InitIcon()
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	g_bCraftIconAnim = false;

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ICON_TEXTURE[nCnt],
			&g_pTextureIcon[nCnt]);
	}

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * WEPONTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL);

	// 頂点バッファをロック
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		g_Icon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 座標
		g_Icon[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 移動量
		g_Icon[nCnt].fHeight = 0.0f;						  // 高さ
		g_Icon[nCnt].fWidth = 0.0f;							  // 横幅
		g_Icon[nCnt].bUse = false;							  // 未使用判定
		g_Icon[nCnt].nType = WEPONTYPE_BAT;					  // 種類
		g_Icon[nCnt].nIconType = ICONTYPE_HOLDITEM;			  // 種類
		g_Icon[nCnt].EaseCnt = 0;							  // イージング
		g_Icon[nCnt].bUp = true;							  // 上昇できるか
		g_Icon[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 向き
		g_Icon[nCnt].fRadius = 0.0f;						  // 半径

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

		// 頂点情報を進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// アイコンの終了処理
//=================================================================================================================
void UninitIcon()
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_pTextureIcon[nCnt] != NULL)
		{
			g_pTextureIcon[nCnt]->Release();
			g_pTextureIcon[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffIcon != NULL)
	{
		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}
//=================================================================================================================
// アイコンの更新処理
//=================================================================================================================
void UpdateIcon()
{

	Player* pPlayer = GetPlayer();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)// 全てのアイコンを数え
	{
		if (g_Icon[nCnt].bUse == false)
		{
			continue;
		}

		// アイテムアイコンの点滅処理
		SetWeponDurabilityAnim(nCnt);

		//if (g_bCraftIconAnim == false)
		//{
		//	CraftScreenAnim(nCnt);
		//}
		//else
		//{
		//	ShowCraftingAnim(nCnt);
		//}
	}

	// アンロック
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// アイコンの描画処理
//=================================================================================================================
void DrawIcon()
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//if (pPlayer->bCraft == true && strcmp(pItem[pPlayer->ItemIdx].Itemtag, "CRAFT") == 0)
	//{
	//	return;
	//}

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// 持っているアイテムのアイコン
		if (g_Icon[nCnt].bUse == true && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD && g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM)
		{// アイコンが使用状態 かつ アイテムの状態がホールドなら

			// 種類
			int nType = pItem[pPlayer->ItemIdx].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
		// ストックしているアイテムのアイコン
		if (g_Icon[nCnt].bUse == true && pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && g_Icon[nCnt].nIconType != ICONTYPE_HOLDITEM)
		{// アイコンが使用状態 かつ アイテムの状態がホールドなら

			// 種類
			int nType = pItem[pPlayer->StockItemIdx].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
	}
}
//=================================================================================================================
// アイコンの設定処理
//=================================================================================================================
void SetIcon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, int IconType) // 設定処理
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (!g_Icon[nCnt].bUse)
		{
			g_Icon[nCnt].pos = pos;		   // 座標
			g_Icon[nCnt].fWidth = fWidth;  // 横幅
			g_Icon[nCnt].fHeight = fHeight;// 高さ
			g_Icon[nCnt].nType = nType;	   // 種類
			g_Icon[nCnt].nIconType = IconType;	   // 種類
			g_Icon[nCnt].bUse = true;	   // 使用状態

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}

		// 頂点座標を進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// クラフトのアニメーション開始処理
//=================================================================================================================
void EnableCraftIconAnim(bool bCraftAnim)
{
	// 有効、無効にする
	g_bCraftIconAnim = bCraftAnim;
}
//=================================================================================================================
// アイテムアイコンの点滅処理
//=================================================================================================================
void SetWeponDurabilityAnim(int nCnt)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// フレーム用変数
	static int fFrame;
	static bool bUP;

	// アイテムを持っているか判定
	const bool isStateHold = pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD;

	// アイコンの種類が持っているアイテムか判定
	const bool TypeHold = g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM;

	// 点滅できるか判定
	const bool is_frash = (float)pItem[pPlayer->ItemIdx].durability / (float)pItem[pPlayer->ItemIdx].Maxdurability < 0.3f;

	// 点滅の処理に入れるか判定
	const bool CanFrashProcess = isStateHold && TypeHold;

	// 条件が違ったら
	if (CanFrashProcess == false)
	{// 関数を抜ける
		return;
	}

	// クラフト状態だったら
	if (pPlayer->bCraft == true)
	{
		// 頂点バッファのロック
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);

		// アンロック
		g_pVtxBuffIcon->Unlock();

		return;
	}
	// 使用されてるかつアイテム持ち
	if (is_frash == true)// 更に耐久力が減ってたら赤く点滅
	{
		if (bUP)
		{
			fFrame++;
		}
		else
		{
			fFrame--;
		}

		// 頂点バッファのロック
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);

		// アンロック
		g_pVtxBuffIcon->Unlock();

		if (fFrame >= 10)
		{
			bUP = false;
		}
		else if (fFrame <= 0)
		{
			bUP = true;
		}
	}
	else // そうでなかったら色を戻す
	{
		// 頂点バッファのロック
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// アンロック
		g_pVtxBuffIcon->Unlock();

		fFrame = 0;
		bUP = true;
	}
}
//=================================================================================================================
// クラフト画面のアイコンの設定
//=================================================================================================================
void CraftScreenAnim(int nCnt)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	float fWidth = g_Icon[nCnt].fWidth;
	float fHeight = g_Icon[nCnt].fHeight;

	// クラフト画面を開いてるとき
	if (pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD && pPlayer->bCraft == true && g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM)
	{
		SetUpDownIcon(nCnt, &g_Icon[nCnt].EaseCnt);

		// 頂点座標の更新
		SetVtxBufferIcon(nCnt, fWidth * 2.0f, fHeight * 2.0f, D3DXVECTOR3(400.0f, g_Icon[nCnt].pos.y, 0.0f));
	}
	// クラフト画面を閉じているとき
	else if(pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD && pPlayer->bCraft == false && g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM)
	{
		// 頂点座標の更新
		SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(70.0f, 640.0f, 0.0f));
	}

	// クラフト画面を開いてるとき
	if (pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && pPlayer->bCraft == true && g_Icon[nCnt].nIconType == ICONTYPE_STOCKITEM)
	{
		SetUpDownIcon(nCnt,&g_Icon[nCnt].EaseCnt);

		// 頂点座標の更新
		SetVtxBufferIcon(nCnt, fWidth * 3.0f, fHeight * 3.0f, D3DXVECTOR3(840.0f, g_Icon[nCnt].pos.y, 0.0f));
	}
	// クラフト画面を閉じているとき
	else if(pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && pPlayer->bCraft == false && g_Icon[nCnt].nIconType == ICONTYPE_STOCKITEM)
	{
		// 頂点座標の更新
		SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(200.0f, 670.0f, 0.0f));
	}
}
//=================================================================================================================
// アイコンの頂点バッファの設定
//=================================================================================================================
void SetVtxBufferIcon(int nCnt, float fWidth, float fHeight,D3DXVECTOR3 pos)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	// アンロック
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
//=================================================================================================================
void SetUpDownIcon(int nCnt, int* EaseCnt)
{
	Player* pPlayer = GetPlayer();

	const bool bSetPos = g_Icon[nCnt].pos.y >= 390.0f && g_Icon[nCnt].pos.y <= 510.0f;

	if (bSetPos == false)
	{
		g_Icon[nCnt].pos.y = 450.0f;
	}
	if (g_Icon[nCnt].pos.y <= 410.0f)
	{
		(*EaseCnt) = 0;
		g_Icon[nCnt].bUp = false;
	}
	else if (g_Icon[nCnt].pos.y >= 490.0f)
	{
		(*EaseCnt) = 0;
		g_Icon[nCnt].bUp = true;
	}
	if (g_Icon[nCnt].bUp == true)
	{
		(*EaseCnt)++;
		float time = SetEase((*EaseCnt), 360);

		g_Icon[nCnt].pos.y += SetSmoothAprroach(400.0f, g_Icon[nCnt].pos.y, EaseOutSine(time));
	}
	else
	{
		(*EaseCnt)++;
		float time = SetEase((*EaseCnt), 360);

		g_Icon[nCnt].pos.y += SetSmoothAprroach(500.0f, g_Icon[nCnt].pos.y, EaseOutSine(time));
	}
}
//=================================================================================================================
//=================================================================================================================
void ShowCraftingAnim(int nCnt)
{

}
