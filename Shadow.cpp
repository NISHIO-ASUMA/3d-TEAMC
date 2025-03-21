//=====================================================================================================================
//
//影表示関連のヘッダー
//Author;長尾悠成
//
//=====================================================================================================================

//*****************************************************************************************************************
// インクルード一覧
//*****************************************************************************************************************
#include "main.h"
#include "Shadow.h"

//*****************************************************************************************************************
// マクロ定義
//*****************************************************************************************************************
#define SHADOW_AMOUNT (700)  // 表示する最大数

//*****************************************************************************************************************
// グローバル変数
//*****************************************************************************************************************
Shadow g_aShadow[SHADOW_AMOUNT]; // 構造体変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL; // 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		 // テクスチャへのポインタ

//===================================================================================================
// Init処理色々
//===================================================================================================
void InitShadow(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 影のテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureShadow);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_AMOUNT * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 初期化処理一覧
	for (int nCnt = 0; nCnt < SHADOW_AMOUNT; nCnt++)
	{
		g_aShadow[nCnt].bUse = false;
		g_aShadow[nCnt].bHide = false;
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].fSize = 0.0f;
		g_aShadow[nCnt].fALv = 1.0f;
		g_aShadow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nCnt].fALv);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点バッファのロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//左、手前、下がマイナス
	for (int nCnt = 0; nCnt < SHADOW_AMOUNT; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		pVtx[0].col = g_aShadow[nCnt].col;
		pVtx[1].col = g_aShadow[nCnt].col;
		pVtx[2].col = g_aShadow[nCnt].col;
		pVtx[3].col = g_aShadow[nCnt].col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffShadow->Unlock();
}
//===================================================================================================
// Uninit処理色々
//===================================================================================================
void UninitShadow(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
	// テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}
//=====================================================================================================
// 更新処理の抜け殻
//=====================================================================================================
void UpdateShadow(void)
{
	// 必要に応じて追加
}
//===================================================================================================
// 描写処理色々
//===================================================================================================
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 透明度オプション追加
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	// 全て見回して
	for (int nCntShadow = 0; nCntShadow < SHADOW_AMOUNT; nCntShadow++)
	{
		// 生きているなら
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//	ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//非表示じゃないなら描写
			if (g_aShadow[nCntShadow].bHide == false)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
			}
		}
	}

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=================================================================================================================================
// 設置処理色々 ( 左から 場所、向き、大きさ )
//=================================================================================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, float fAlv)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// カウント用変数
	int nCntShadow;

	// 頂点バッファのロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (nCntShadow = 0; nCntShadow < SHADOW_AMOUNT; nCntShadow++)
	{
		// 使われてないなら設置
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].fSize = fSize;
			g_aShadow[nCntShadow].fALv = fAlv;
			g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nCntShadow].fALv);
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;

			// Sizeは半径な事に注意
			pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 2.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 2.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 2.0f, g_aShadow[nCntShadow].pos.z - fSize);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 2.0f, g_aShadow[nCntShadow].pos.z - fSize);

			// 頂点カラーの設定
			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			break;
		}
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffShadow->Unlock();

	// 影の番号を返す
	return nCntShadow;
}
//===================================================================================================
// 実質更新処理色々
//===================================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].fSize = fSize;
	g_aShadow[nIdxShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nIdxShadow].fALv);
	g_aShadow[nIdxShadow].fALv = fAlv;

	// 頂点バッファのロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIdxShadow;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[2].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);

	// 頂点カラーの設定
	pVtx[0].col = g_aShadow[nIdxShadow].col;
	pVtx[1].col = g_aShadow[nIdxShadow].col;
	pVtx[2].col = g_aShadow[nIdxShadow].col;
	pVtx[3].col = g_aShadow[nIdxShadow].col;

	// アンロック
	g_pVtxBuffShadow->Unlock();
}
//===================================================================================================================
// Idxを指定して影消滅させるやつ
//===================================================================================================================
void KillShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
//===================================================================================================================
// Idxを指定して影隠すやつ
//===================================================================================================================
void HideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = true;
}
//===================================================================================================================
// Idxを指定して影再表示させるやつ
//===================================================================================================================
void NoHideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = false;
}
