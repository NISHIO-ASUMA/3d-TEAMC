//=====================================================================================================================
//
// 2Dミニマップ処理 [ minimap.cpp]
// Author: NISHIO & YOSHIDA
//
//=====================================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "minimap.h"
#include "player.h"
#include "meshfield.h"

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void InitMapField(void);
void UninitMapField(void);
void UpdateMapField(void);
void DrawMapField(void);

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
MINIMAP g_MiniMap[MAX_TREXMINIMAP]; // テクスチャ用構造体情報
LPDIRECT3DTEXTURE9 g_pTextureMiniMap[MINIMAPTEX_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;		   // 頂点バッファへのポインタ

LPDIRECT3DTEXTURE9 g_pTextureMiniMapField = NULL;           // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMapField = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3 MapFiledPos;

//==============================================================================================================
// ミニマップ初期化処理
//==============================================================================================================
void InitMiniMap()
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < MINIMAPTEX_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			CAMERA_TEX[nCnt],
			&g_pTextureMiniMap[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TREXMINIMAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMap,
		NULL);

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		g_MiniMap[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標
		g_MiniMap[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 角度
		g_MiniMap[nCnt].nType = 0; // 種類
		g_MiniMap[nCnt].fHeight = 0.0f; // 高さ
		g_MiniMap[nCnt].fWidth = 0.0f; // 横幅
		g_MiniMap[nCnt].bUse = false; // 未使用判定
		g_MiniMap[nCnt].fAngle = 0.0f; // 角度計算用
	}

	// 頂点バッファのロック
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標関係
	for (int nCntCamera = 0; nCntCamera < MAX_TREXMINIMAP; nCntCamera++)
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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点情報を進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffMiniMap->Unlock();

	InitMapField();

}
//==============================================================================================================
// ミニマップ終了処理
//==============================================================================================================
void UninitMinMap()
{
	UninitMapField();

	// テクスチャの破棄
	for (int nCnt = 0; nCnt < MINIMAPTEX_MAX; nCnt++)
	{
		if (g_pTextureMiniMap[nCnt] != NULL)
		{			  
			g_pTextureMiniMap[nCnt]->Release();
			g_pTextureMiniMap[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMiniMap != NULL)
	{
		g_pVtxBuffMiniMap->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}
//==============================================================================================================
// ミニマップ更新処理
//==============================================================================================================
void UpdateMiniMap()
{

}
//==============================================================================================================
// ミニマップ描画処理
//==============================================================================================================
void DarwMinimap()
{
	DrawMapField();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMiniMap, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 雑魚敵のマーカー探知
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// 種類を保存
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 1)
		{// 使用判定のものだけ

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// ボスのマーカー探知
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// 種類を保存
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 2)
		{// 使用判定のものだけ

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	// プレイヤーのマーカー探知
	for (int nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		// 種類を保存
		int nType = g_MiniMap[nCnt].nType;

		if (g_MiniMap[nCnt].bUse && g_MiniMap[nCnt].nType == 0)
		{// 使用判定のものだけ

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMiniMap[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}
//==============================================================================================================
// ミニマップの位置設定処理
//==============================================================================================================
void SetMiniMapPotision(int nIdx, D3DXVECTOR3* pPos)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 頂点バッファのロック
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	if (g_MiniMap[nIdx].bUse == true)
	{
		// 頂点座標のポインタを進める
		pVtx += 4 * nIdx;

		// プレイヤーの座標をミニマップの座標に変換
		g_MiniMap[nIdx].pos.x = (pPos->x / MAX_FIELDWIDTH) * MINIMAP_WIDTH;
		g_MiniMap[nIdx].pos.y = (-pPos->z / MAX_FIELDHEIGHT) * MINIMAP_HEIGHT;

		// プレイヤーの向きを取得
		float fAngle = pPlayer->rot.y;

		// 角度計算
		float cosA = cosf(fAngle);
		float sinA = sinf(fAngle);

		if (g_MiniMap[nIdx].nType == 0)
		{// 種類がプレイヤーの時
			// 回転を考慮した座標更新
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (-5.0f * cosA - (-5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (-5.0f * sinA + (-5.0f) * cosA),
									  0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (5.0f * cosA - (-5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (5.0f * sinA +(-5.0f) * cosA),
									  0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (-5.0f * cosA - (5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (-5.0f * sinA + (5.0f) * cosA),
									  0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + (5.0f * cosA - (5.0f) * sinA),
									  MapFiledPos.y + g_MiniMap[nIdx].pos.y + (5.0f * sinA + (5.0f) * cosA),
									  0.0f);
		}
		else if (g_MiniMap[nIdx].nType == 1)
		{// 種類が敵の時
			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 5.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 5.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 5.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 5.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 5.0f, 0.0f);

		}
		else if (g_MiniMap[nIdx].nType == 2)
		{// 種類がボスの時
			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 8.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y - 8.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x - 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 8.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + g_MiniMap[nIdx].pos.x + 8.0f, MapFiledPos.y + g_MiniMap[nIdx].pos.y + 8.0f, 0.0f);

		}
	}
	// 頂点バッファのアンロック
	g_pVtxBuffMiniMap->Unlock();
}
//==============================================================================================================
// ミニマップ設定処理
//==============================================================================================================
int SetMiniMap(D3DXVECTOR3 pos, int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	int nCnt;

	// 頂点バッファのロック
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_TREXMINIMAP; nCnt++)
	{
		if (g_MiniMap[nCnt].bUse == false)
		{
			g_MiniMap[nCnt].pos = pos;
			g_MiniMap[nCnt].nType = nType;
			g_MiniMap[nCnt].bUse = true;

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(MapFiledPos.x + pos.x - 5.0f, MapFiledPos.y + pos.y - 5.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MapFiledPos.x + pos.x + 5.0f, MapFiledPos.y + pos.y - 5.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(MapFiledPos.x + pos.x - 5.0f, MapFiledPos.y + pos.y + 5.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MapFiledPos.x + pos.x + 5.0f, MapFiledPos.y + pos.y + 5.0f, 0.0f);

			break;
		}

		// 頂点座標のポインタを進める
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffMiniMap->Unlock();

	return nCnt;
}
//==============================================================================================================
// 指定番号のポリゴンを消す
//==============================================================================================================
void EnableMap(int nIdx)
{
	g_MiniMap[nIdx].bUse = false;
}
//==============================================================================================================
// ミニマップのフィールド初期化処理
//==============================================================================================================
void InitMapField(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\minimap.png",
		&g_pTextureMiniMapField);
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMapField,
		NULL);

	MapFiledPos = D3DXVECTOR3(1100.0f, 180.0f, 0.0f);

	// 頂点バッファのロック
	g_pVtxBuffMiniMapField->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標関係
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(1000.0f, 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1200.0f, 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(1000.0f, 280.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1200.0f, 280.0f, 0.0f);

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

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	g_pVtxBuffMiniMapField->Unlock();

}
//==============================================================================================================
// ミニマップのフィールド終了処理
//==============================================================================================================
void UninitMapField(void)
{
	// テクスチャの破棄
	if (g_pTextureMiniMapField != NULL)
	{
		g_pTextureMiniMapField->Release();
		g_pTextureMiniMapField = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffMiniMapField != NULL)
	{
		g_pVtxBuffMiniMapField->Release();
		g_pVtxBuffMiniMapField = NULL;
	}

}
//==============================================================================================================
// ミニマップのフィールド更新処理
//==============================================================================================================
void UpdateMapField(void)
{
	// なし
}
//==============================================================================================================
// ミニマップのフィールド描画処理
//==============================================================================================================
void DrawMapField(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMiniMapField, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMiniMapField);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
#if 0
// 回転を計算
g_MiniMap[nIdx].rot.y = sqrtf((pPlayer->rot.y * pPlayer->rot.y)) * 0.5f;
g_MiniMap[nIdx].rot.x = sqrtf((pPlayer->rot.z * pPlayer->rot.z)) * 0.5f;

g_MiniMap[nIdx].fAngle = atan2f(pPlayer->rot.y, pPlayer->rot.z);
#endif
