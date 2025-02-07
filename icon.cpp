//===============================
//
// 武器のアイコン処理 [icon.cpp]
// Author: Asuma Nishio
//
//===============================

//********************************
// インクルードファイル宣言
//********************************
#include "item.h"
#include "icon.h"

//********************************
// グローバル変数宣言
//********************************
LPDIRECT3DTEXTURE9 g_pTextureIcon[WEPONTYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;      // 頂点バッファへのポインタ
ICON g_Icon[WEPONTYPE_MAX]; // 構造体変数

//===============================
// アイコンの初期化処理
//===============================
void InitIcon()
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

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
		g_Icon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標
		g_Icon[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 移動量
		g_Icon[nCnt].fHeight = 0.0f;					  // 高さ
		g_Icon[nCnt].fWidth = 0.0f;						  // 横幅
		g_Icon[nCnt].bUse = false;						  // 未使用判定
		g_Icon[nCnt].nType = WEPONTYPE_BAT;				  // 種類

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
//===============================
// アイコンの終了処理
//===============================
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
//===============================
// アイコンの更新処理
//===============================
void UpdateIcon()
{
	// 無し
}
//===============================
// アイコンの描画処理
//===============================
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

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_Icon[nCnt].bUse && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
		{// アイコンが使用状態 かつ アイテムの状態がホールドなら

			// 種類
			int nType = pItem[pPlayer->ItemIdx].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
			
		}
	}
}
//===============================
// アイコンの設定処理
//===============================
void SetIcon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
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
