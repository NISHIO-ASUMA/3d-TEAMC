//==============================================================================================================
//
// クラフトUI [craftui.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "HPGauge.h"
#include "player.h"
#include "craftui.h"
#include "icon.h"
#include "craftrecipe.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define UIPOTISION (D3DXVECTOR3(600.0f, 400.0f, 0.0f)) // UIの位置

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCraftUI[CRAFTUITYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureItemIcon[WEPONTYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCraftUI = NULL;      // 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemIcon = NULL;      // 頂点バッファへのポインタ
Craftui g_CraftUI[CRAFTUITYPE_MAX];
Craftui g_MixUI[WEPONTYPE_MAX];

//==============================================================================================================
// UIの初期化処理
//==============================================================================================================
void InitCraftUI(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//InitIcon();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			CRAFTUITYPE_INFO[nCnt],
			&g_pTextureCraftUI[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CRAFTUITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCraftUI,
		NULL);

	// 頂点ロック
	g_pVtxBuffCraftUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		g_CraftUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].fHeight = 0.0f;
		g_CraftUI[nCnt].fWidth = 0.0f;
		g_CraftUI[nCnt].nUseTime = 0;
		g_CraftUI[nCnt].bUse = false;
		g_CraftUI[nCnt].nType = CRAFTUITYPE_MAX;

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
	g_pVtxBuffCraftUI->Unlock();


	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ICON_TEXTURE[nCnt],
			&g_pTextureItemIcon[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * WEPONTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemIcon,
		NULL);

	// 頂点ロック
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		g_MixUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].fHeight = 0.0f;
		g_MixUI[nCnt].fWidth = 0.0f;
		g_MixUI[nCnt].nUseTime = 0;
		g_MixUI[nCnt].bUse = false;
		g_MixUI[nCnt].nIconType = WEPONTYPE_BAT;

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
	g_pVtxBuffItemIcon->Unlock();

	// クラフト画面の背景
	SetCraftUI(D3DXVECTOR3(625.0f, 350.0f, 0.0f), CRAFTUITYPE_BACKGROUND, 500.0f, 250.0f, 0);

	// クラフト画面のクラフトボタン
	SetCraftUI(D3DXVECTOR3(325.0f, 550.0f, 0.0f), CRAFTUITYPE_CRAFT, 100.0f, 50.0f, -1);

	// クラフト画面の閉じるボタン
	SetCraftUI(D3DXVECTOR3(925.0f, 550.0f, 0.0f), CRAFTUITYPE_CLOSE, 100.0f, 50.0f, -1);
}
//==============================================================================================================
// UIの終了処理
//==============================================================================================================
void UninitCraftUI(void)
{
	//UninitIcon();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_pTextureCraftUI[nCnt] != NULL)
		{
			g_pTextureCraftUI[nCnt]->Release();
			g_pTextureCraftUI[nCnt] = NULL;
		}
	}

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_pTextureItemIcon[nCnt] != NULL)
		{
			g_pTextureItemIcon[nCnt]->Release();
			g_pTextureItemIcon[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCraftUI != NULL)
	{
		g_pVtxBuffCraftUI->Release();
		g_pVtxBuffCraftUI = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItemIcon != NULL)
	{
		g_pVtxBuffItemIcon->Release();
		g_pVtxBuffItemIcon = NULL;
	}

}
//==============================================================================================================
// UIの更新処理
//==============================================================================================================
void UpdateCraftUI(void)
{

	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_CraftUI[nCnt].bUse == false)
		{
			continue;
		}
	}

	//// 頂点ロック
	//g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// 合成のUIを設定
		SetMixItemUI(nCnt);

		if (g_MixUI[nCnt].bUse == false)
		{
			continue;
		}

		// アイコンの非表示
		if (pPlayer->HandState != PLAYERHOLD_HOLD)
		{
			switch (g_MixUI[nCnt].nIconType)
			{
			case WEPONTYPE_STONEBAT:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_ICEBLOCKSOWRD:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_TORCHSWORD:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_LIGHTWOOD:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_IRONBAT:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_HEADSTATUTORSO:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_SURFBOARDFISH:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_HEXMANDOLIN:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_BONESPEAR:
				g_MixUI[nCnt].bUse = false;
				break;
			case WEPONTYPE_GOLFHUNMER:
				g_MixUI[nCnt].bUse = false;
				break;
			default:
				break;
			}
		}
		

		//// 頂点座標の設定
		//pVtx[0].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x - g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y - g_MixUI[nCnt].fHeight, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x + g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y - g_MixUI[nCnt].fHeight, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x - g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y + g_MixUI[nCnt].fHeight, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x + g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y + g_MixUI[nCnt].fHeight, 0.0f);

		//pVtx += 4;
	}

	UpdateCraftRecipe();

	////頂点ロック解除
	//g_pVtxBuffItemIcon->Unlock();
}
//==============================================================================================================
// UIの描画処理
//==============================================================================================================
void DrawCraftUI(void)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスへのポインタ

	// デバイスの習得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCraftUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_CraftUI[nCnt].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCraftUI[g_CraftUI[nCnt].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
	}

	// クラフトのレシピの描画処理
	DrawCraftRecipe();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItemIcon, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_MixUI[nCnt].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItemIcon[g_MixUI[nCnt].nIconType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
	}
}
//==============================================================================================================
// UIの設定処理
//==============================================================================================================
void SetCraftUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffCraftUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_CraftUI[nCnt].bUse == false)
		{
			g_CraftUI[nCnt].pos = pos;
			g_CraftUI[nCnt].nType = nType;
			g_CraftUI[nCnt].fWidth = fWidth;
			g_CraftUI[nCnt].fHeight = fHeight;
			g_CraftUI[nCnt].nUseTime = nUseTime;
			g_CraftUI[nCnt].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// 頂点ロック解除
	g_pVtxBuffCraftUI->Unlock();
}
//==============================================================================================================
// 合成UIの設定処理
//==============================================================================================================
void SetMixUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_MixUI[nCnt].bUse == false)
		{
			g_MixUI[nCnt].pos = pos;
			g_MixUI[nCnt].nIconType = nType;
			g_MixUI[nCnt].fWidth = fWidth;
			g_MixUI[nCnt].fHeight = fHeight;
			g_MixUI[nCnt].nUseTime = nUseTime;
			g_MixUI[nCnt].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// 頂点ロック解除
	g_pVtxBuffItemIcon->Unlock();

}
//==============================================================================================================
// 合成したアイテムのUIの設定処理
//==============================================================================================================
void SetMixItemUI(int nCnt)
{
	Item* pItem = GetItem();

	// アイテムの最大数分回す
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 作れるアイテムが石バット
		if (pItem[nCntItem].bMixItem[ITEMTYPE_STONEBAT] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, WEPONTYPE_STONEBAT, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_ICEBLOCKSOWRD] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_ICEBLOCKSOWRD, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_TORCHSWORD] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_TORCHSWORD, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_LIGHTWOOD] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_LIGHTWOOD, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_IRONBAT] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_IRONBAT, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_HEADSTATUTORSO] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_HEADSTATUTORSO, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_HEXMANDOLIN] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_HEXMANDOLIN, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_BONESPEAR] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_BONESPEAR, 80.0f, 80.0f, 0);
		}
		if (pItem[nCntItem].bMixItem[ITEMTYPE_GOLFHUNMER] && g_MixUI[nCnt].bUse == false)
		{
			SetMixUI(UIPOTISION, ITEMTYPE_GOLFHUNMER, 80.0f, 80.0f, 0);
		}

	}
}
//==============================================================================================================
// 取得処理
//==============================================================================================================
Craftui* GetMixUI(void)
{
	return &g_MixUI[0];
}
