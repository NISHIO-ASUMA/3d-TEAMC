//==============================================================================================================
//
// アイテムのゲージ [itemgage.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#include "itemgage.h"
#include "item.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_LENGTH (150.0f) // 横幅

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureItemGage[ITEMGAGE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemGage = NULL;// 頂点バッファへのポインタ
ItemGage g_ItemGage[ITEMGAGE_MAX]; // アイテムゲージの情報

//==============================================================================================================
// アイテムのゲージの初期化処理
//==============================================================================================================
void InitItemGage(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ITEMGAGE_INFO[nCnt],
			&g_pTextureItemGage[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * ITEMGAGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemGage,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
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

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファのポインタを進める
		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffItemGage->Unlock();

	SetItemGage(D3DXVECTOR3(5.0f,630.0f,0.0f),ITEMGAGE_FRAME,8.0f);
	SetItemGage(D3DXVECTOR3(5.0f,630.0f, 0.0f), ITEMGAGE_GAGE, 8.0f);
}
//==============================================================================================================
// アイテムのゲージの終了処理
//==============================================================================================================
void UninitItemGage(void)
{
	// アイテムの種類分回す
	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// テクスチャの解放
		if (g_pTextureItemGage[nCnt] != NULL)
		{
			g_pTextureItemGage[nCnt]->Release();
			g_pTextureItemGage[nCnt] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_pVtxBuffItemGage != NULL)
	{
		g_pVtxBuffItemGage->Release();
		g_pVtxBuffItemGage = NULL;
	}

}
//==============================================================================================================
// アイテムのゲージの更新処理
//==============================================================================================================
void UpdateItemGage(void)
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// アイテムゲージが使用状態だったら
		if (g_ItemGage[nCnt].bUse == false)
		{
			continue;
		}

		// ゲージの割合を求める
		float RateGage = (float)pItem[pPlayer->ItemIdx].durability / (float)pItem[pPlayer->ItemIdx].Maxdurability;

		// 割合から長さを求める
		g_ItemGage[nCnt].fLength = RateGage * MAX_LENGTH;

		// 位置を代入,高さ
		D3DXVECTOR3 pos = g_ItemGage[nCnt].pos;
		float fHeight = g_ItemGage[nCnt].fHeight;

		switch (g_ItemGage[nCnt].nType)
		{
		case ITEMGAGE_FRAME:

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + MAX_LENGTH, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + MAX_LENGTH, pos.y + fHeight, 0.0f);

			break;
		case ITEMGAGE_GAGE:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + g_ItemGage[nCnt].fLength, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + g_ItemGage[nCnt].fLength, pos.y + fHeight, 0.0f);
			break;
		default:
			break;
		}

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffItemGage->Unlock();
}
//==============================================================================================================
// アイテムのゲージの描画処理
//==============================================================================================================
void DrawItemGage(void)
{
	Item* pItem = GetItem();

	Player* pPlayer = GetPlayer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItemGage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// アイテムが使用状態だったら
		if (g_ItemGage[nCnt].bUse == true && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
		{
			// 種類を代入
			int nType = g_ItemGage[nCnt].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItemGage[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// アイテムのゲージの設定処理
//==============================================================================================================
void SetItemGage(D3DXVECTOR3 pos, int nType,float fHeight)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// アイテムが使用状態だったら
		if (g_ItemGage[nCnt].bUse == false)
		{
			g_ItemGage[nCnt].pos = pos;
			g_ItemGage[nCnt].nType = nType;
			g_ItemGage[nCnt].fHeight = fHeight;
			g_ItemGage[nCnt].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffItemGage->Unlock();
}
