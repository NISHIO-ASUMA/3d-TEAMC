//==============================================================================================================
//
// manual [manual.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "manual.h"
#include "mark.h"
#include "input.h"
#include "game.h"
#include "gameui.h"
#include "mouse.h"

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SetUpManualBoss(int nCnt);	// ボスのマニュアルの設定

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureManual[MANUALTYPE_MAX] = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;	// 頂点バッファへのポインタ
Manual g_Manual[MANUALTYPE_MAX];					// マニュアルの情報
bool g_bmanual = false; // マニュアル画面を出すか

//=======================================================================================================
// マニュアルの初期化処理
//=======================================================================================================
void InitManual(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			MANUAL_TYPE[nCnt],
			&g_pTextureManual[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MANUALTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManual,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

	// マニュアル画面を出すか
	g_bmanual = false;

	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		g_Manual[nCnt].pos = NULLVECTOR3;		// 位置
		g_Manual[nCnt].nType = MANUALTYPE_ONE;	// 種類
		g_Manual[nCnt].fWidth = 0.0f;			// 横幅
		g_Manual[nCnt].fHeight = 0.0f;			// 高さ
		g_Manual[nCnt].bUse = false;			// 使用状態
		g_Manual[nCnt].bFirstSet = true;		// 最初にセットしたらもう通さない

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

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffManual->Unlock();

	// マニュアルの設定
	SetManual(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 550.0f, 250.0f, MANUALTYPE_ONE);
}
//=======================================================================================================
// マニュアルの終了処理
//=======================================================================================================
void UninitManual(void)
{
	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (g_pTextureManual[nCnt] != NULL)
		{
			g_pTextureManual[nCnt]->Release();
			g_pTextureManual[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}
}
//=======================================================================================================
// マニュアルの更新処理
//=======================================================================================================
bool UpdateManual(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		if (g_Manual[nCnt].bUse == false)
		{
			continue;
		}

		switch (g_Manual[nCnt].nType)
		{
		case MANUALTYPE_ONE:
			SetUpManualBoss(nCnt);
			break;
		default:
			break;
		}
		// 位置
		D3DXVECTOR3 pos = g_Manual[nCnt].pos;

		// 横幅
		float fWidth = g_Manual[nCnt].fWidth;

		// 高さ
		float fHeight = g_Manual[nCnt].fHeight;

		// 頂点バッファのロック
		g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

		//アンロック
		g_pVtxBuffManual->Unlock();
	}

	return g_bmanual;
}
//=======================================================================================================
// マニュアルの描画処理
//=======================================================================================================
void DrawManual(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		if (g_Manual[nCnt].bUse == true)
		{
			// 種類
			int nType = g_Manual[nCnt].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureManual[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//=======================================================================================================
// マニュアルの設定処理
//=======================================================================================================
void SetManual(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

	// 種類分回す
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// 未使用だったら
		if (g_Manual[nCnt].bUse == false)
		{
			g_Manual[nCnt].pos = pos;			// 位置
			g_Manual[nCnt].fWidth = fWidth;		// 横幅
			g_Manual[nCnt].fHeight = fHeight;	// 高さ
			g_Manual[nCnt].nType = nType;		// 種類

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffManual->Unlock();
}
//==============================================================================================================
// マニュアルの状態設定
//==============================================================================================================
void EnableManual(bool bManual,int nType)
{
	if (g_Manual[nType].bFirstSet == false) return;

	// 状態を設定
	g_bmanual = bManual;
	g_Manual[nType].bUse = true;

	switch (nType)
	{
	case MANUALTYPE_ONE:
		SetGameUI(D3DXVECTOR3(1000.0f,670.0f,0.0f),UITYPE_BOSSMANUALEXIT,120.0f,60.0f,false,0);
		break;
	default:
		break;
	}
}
//==============================================================================================================
// ボスのマニュアルの設定
//==============================================================================================================
void SetUpManualBoss(int nCnt)
{
	//マニュアル画面を解除できるまでの時間
	static int ExitTime = 0;

	ExitTime++;

	// スペースを押したら && 一度もセットしてなければ
	if (OnMouseTriggerDown(LEFT_MOUSE) == true && ExitTime >= 180)
	{
		g_bmanual = false;
		g_Manual[nCnt].bUse = false;
		g_Manual[nCnt].bFirstSet = false;
	}
}
//==============================================================================================================
// マニュアルの状態を取得
//==============================================================================================================
bool GetManualState(void)
{
	return g_bmanual;
}
