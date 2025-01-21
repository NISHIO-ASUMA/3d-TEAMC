//===================================
//
//影表示関連のヘッダー
//Author;長尾悠成
//
//===================================

// インクルード一覧
#include "main.h"
#include "Shadow.h"

// 表示する最大数とその桁数
#define SHADOW_AMOUNT (100)
Shadow g_aShadow[SHADOW_AMOUNT];

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;

//=================
// Init処理色々
//=================

void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 影のテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureShadow);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_AMOUNT * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);
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
	g_pVtxBuffShadow->Unlock();
}

//=================
// Uninit処理色々
//=================
void UninitShadow(void)
{
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//=================
// 更新処理の抜け殻
//=================
void UpdateShadow(void)
{

}

//=================
// 描写処理色々
//=================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

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
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
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
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=================
// 設置処理色々
//=================

// 左から場所、向き、大きさ
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize)
{
	int nCntShadow;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	// 全て見回して
	for (nCntShadow = 0; nCntShadow < SHADOW_AMOUNT; nCntShadow++)
	{
		// 使われてないなら設置
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].fSize = fSize;
			g_aShadow[nCntShadow].fALv = 1.0f;
			g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nCntShadow].fALv);
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;

			// Sizeは半径な事に注意
			pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 1.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 1.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 1.0f, g_aShadow[nCntShadow].pos.z - fSize);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 1.0f, g_aShadow[nCntShadow].pos.z - fSize);
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffShadow->Unlock();
	return nCntShadow;
}

//=================
// 実質更新処理色々
//=================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].fSize = fSize;
	g_aShadow[nIdxShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nIdxShadow].fALv);
	g_aShadow[nIdxShadow].fALv = fAlv;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * nIdxShadow;
	pVtx[0].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 1.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 1.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[2].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 1.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 1.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);

	pVtx[0].col = g_aShadow[nIdxShadow].col;
	pVtx[1].col = g_aShadow[nIdxShadow].col;
	pVtx[2].col = g_aShadow[nIdxShadow].col;
	pVtx[3].col = g_aShadow[nIdxShadow].col;

	g_pVtxBuffShadow->Unlock();
}

// Idxを指定して影消滅させるやつ
void KillShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}

// Idxを指定して影隠すやつ
void HideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = true;
}

// Idxを指定して影再表示させるやつ
void NoHideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = false;
}
