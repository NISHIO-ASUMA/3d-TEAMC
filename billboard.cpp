//==================================
//
// ビルボード処理 [billboard.cpp]
// Author: Asuma Nishio
//
//==================================

//***************************
// インクルードファイル宣言
//***************************
#include "billboard.h"
#include "block.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "easing.h"
#include "math.h"

//*****************************
// マクロ定義
//*****************************
#define MAX_BOSSHPLENGTH (400.0f) // 最大の横幅

//*****************************
// グローバル変数宣言
//*****************************
Billboard g_Billboard[MAX_BILLBOARD];				// 構造体変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureBillboard[BILLBOARDTYPE_MAX] = {};// テクスチャのポインタ
int EasingCount, EasingCount1;

//=========================
//　ビルボード初期化処理
//=========================
void InitBillboard(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BILLBOARDTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BILLBOARD_TEXTURE[nCnt],
			&g_apTextureBillboard[nCnt]);
	}

	EasingCount = 0;
	EasingCount1 = 0;

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		g_Billboard[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標
		g_Billboard[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
		g_Billboard[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_Billboard[nCnt].nType = 0;			                // 種類
		g_Billboard[nCnt].fHeight = 0.0f;						// 高さ
		g_Billboard[nCnt].fWidth = 0.0f;						// 横幅
		g_Billboard[nCnt].state = BILLBOARDSTATE_NOSET;
		g_Billboard[nCnt].bUse = false;							// 使用判定

	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 各頂点の法線(ベクトル)の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	

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

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();

}
//=========================
//　ビルボード終了処理
//=========================
void UninitBillboard(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < BILLBOARDTYPE_MAX; nCnt++)
	{
		if (g_apTextureBillboard[nCnt] != NULL)
		{
			g_apTextureBillboard[nCnt]->Release();
			g_apTextureBillboard[nCnt] = NULL;

		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{			 
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

}
//=========================
//　ビルボード更新処理
//=========================
void UpdateBillboard(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// falseだったら
			continue;
		}

		switch (g_Billboard[nCnt].nType)
		{
		case BILLBOARDTYPE_FIRST:

			break;
		default:
			break;
		}

		//// 頂点座標の設定
		//pVtx[0].pos = D3DXVECTOR3(-g_Billboard[nCnt].fWidth, g_Billboard[nCnt].fHeight, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_Billboard[nCnt].fWidth, g_Billboard[nCnt].fHeight, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(-g_Billboard[nCnt].fWidth, -g_Billboard[nCnt].fHeight, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_Billboard[nCnt].fWidth, -g_Billboard[nCnt].fHeight, 0.0f);

		//pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();
}
//=========================
//　ビルボード描画処理
//=========================
void DrawBillboard(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	for (int nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// 使われていないとき
			// 下の処理を通らずカウントを進める
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Billboard[nCnt].mtxWorld);

		// ビューマトリックスを宣言
		D3DXMATRIX mtxView;

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		g_Billboard[nCnt].mtxWorld._11 = mtxView._11;
		g_Billboard[nCnt].mtxWorld._12 = mtxView._21;
		g_Billboard[nCnt].mtxWorld._13 = mtxView._31;
		g_Billboard[nCnt].mtxWorld._21 = mtxView._12;
		g_Billboard[nCnt].mtxWorld._22 = mtxView._22;
		g_Billboard[nCnt].mtxWorld._23 = mtxView._32;
		g_Billboard[nCnt].mtxWorld._31 = mtxView._13;
		g_Billboard[nCnt].mtxWorld._32 = mtxView._23;
		g_Billboard[nCnt].mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCnt].pos.x, g_Billboard[nCnt].pos.y, g_Billboard[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Billboard[nCnt].mtxWorld, &g_Billboard[nCnt].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCnt].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (g_Billboard[nCnt].state == BILLBOARDSTATE_SET)
		{
			// 種類
			int nType = g_Billboard[nCnt].nType;

			// 拾うのUiだったら
			if (nType == BILLBOARDTYPE_SECOND && isPlayerAttaking() == false)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureBillboard[g_Billboard[nCnt].nType]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			}
			// 拾うのUi以外だったら
			if(nType != BILLBOARDTYPE_SECOND)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureBillboard[g_Billboard[nCnt].nType]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			}
		}

	}

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//=========================
//　ビルボード設定処理
//=========================
int SetBillboard(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight,int state)
{
	// インデックスカウント用
	int nCnt = 0;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_BILLBOARD; nCnt++)
	{
		if (!g_Billboard[nCnt].bUse)
		{// falseだったら

			g_Billboard[nCnt].pos = pos;	// 座標
			g_Billboard[nCnt].nType = nType;// 種類
			g_Billboard[nCnt].state = state;// 状態
			g_Billboard[nCnt].fWidth = fWidth; // 横幅
			g_Billboard[nCnt].fHeight = fHeight; // 高さ
			g_Billboard[nCnt].bUse = true;	// 使用判定

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth,  fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth,  -fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth,  -fHeight, 0.0f);

			break;
		}

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();

	// インデックス番号を返す
	return nCnt;
}

//=============================================
// 指定したインデックス番号のビルボードを消す
//=============================================
void DeletIdxBillboard(int nIdx)
{
	// インデックス番号に応じたビルボードの非表示
	g_Billboard[nIdx].bUse = false;
}
//=============================================
// ビルボード情報の取得
//=============================================
Billboard* GetBillBoard()
{
	return &g_Billboard[0];
}
//=============================================
// ターゲットの位置設定処理
//=============================================
void SetPositiontarget(int nIdx, D3DXVECTOR3 pos)
{
	g_Billboard[nIdx].pos = pos;
}
