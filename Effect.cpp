//======================
//
//Author:長尾悠成
//エフェクト描画
//
//======================
#include "main.h"
#include "Effect.h"

// 表示する最大数
#define MAX_EFFECT (70000)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
Effect g_Effect[MAX_EFFECT];

//=================
// Init処理色々
//=================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// エフェクトのテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.png", &g_pTextureEffect);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_EFFECT * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	//左、手前、下がマイナス
	// 初期化と大きさ設定
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_Effect[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Effect[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Effect[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Effect[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Effect[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Effect[nCnt].bUse = false;
		g_Effect[nCnt].nLife = 0;
		g_Effect[nCnt].fSize = 0.0f;

		/*pVtx[0].pos = D3DXVECTOR3(g_Effect[nCnt].pos.x - g_Effect[nCnt].fSize, g_Effect[nCnt].pos.y + g_Effect[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].pos.x + g_Effect[nCnt].fSize, g_Effect[nCnt].pos.y + g_Effect[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Effect[nCnt].pos.x - g_Effect[nCnt].fSize, g_Effect[nCnt].pos.y - g_Effect[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].pos.x + g_Effect[nCnt].fSize, g_Effect[nCnt].pos.y - g_Effect[nCnt].fSize, 0.0f);*/

		pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//=================
// Uninit処理色々
//=================

void UninitEffect(void)
{
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//=================
// 更新処理色々
//=================

void UpdateEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	// 全て見回して
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// もし存在してるなら
		if (g_Effect[nCnt].bUse == true)
		{
			// 移動させる
			g_Effect[nCnt].pos.x += g_Effect[nCnt].move.x;
			g_Effect[nCnt].pos.y += g_Effect[nCnt].move.y;
			g_Effect[nCnt].pos.z += g_Effect[nCnt].move.z;
			
			//寿命を減らし透明度を加算させる
			g_Effect[nCnt].nLife--;
			g_Effect[nCnt].col.a -= 0.04f;
			pVtx[0].col = g_Effect[nCnt].col;
			pVtx[1].col = g_Effect[nCnt].col;
			pVtx[2].col = g_Effect[nCnt].col;
			pVtx[3].col = g_Effect[nCnt].col;

			//その上で場所を調整させる
			pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);

			//寿命で死ぬ
			if (g_Effect[nCnt].nLife <= 0)
			{
				g_Effect[nCnt].bUse = false;
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//=================
// 描写処理色々
//=================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_Effect[nCnt].bUse == true)
		{
			// ビルボードの色々
			D3DXMATRIX mtxView;
			D3DXMatrixIdentity(&g_Effect[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_Effect[nCnt].mtxWorld._11 = mtxView._11;
			g_Effect[nCnt].mtxWorld._12 = mtxView._21;
			g_Effect[nCnt].mtxWorld._13 = mtxView._31;
			g_Effect[nCnt].mtxWorld._21 = mtxView._12;
			g_Effect[nCnt].mtxWorld._22 = mtxView._22;
			g_Effect[nCnt].mtxWorld._23 = mtxView._32;
			g_Effect[nCnt].mtxWorld._31 = mtxView._13;
			g_Effect[nCnt].mtxWorld._32 = mtxView._23;
			g_Effect[nCnt].mtxWorld._33 = mtxView._33;

			g_Effect[nCnt].mtxWorld._41 = 0.0f;
			g_Effect[nCnt].mtxWorld._42 = 0.0f;
			g_Effect[nCnt].mtxWorld._43 = 0.0f;
			mtxView._14 = g_Effect[nCnt].mtxWorld._41;
			mtxView._24 = g_Effect[nCnt].mtxWorld._42;
			mtxView._34 = g_Effect[nCnt].mtxWorld._43;

			/*D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Effect[nCnt].rot.y, g_Effect[nCnt].rot.x, g_Effect[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxRot);*/

			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCnt].pos.x, g_Effect[nCnt].pos.y, g_Effect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxTrans);
			pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCnt].mtxWorld);
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));
			pDevice->SetTexture(0, g_pTextureEffect);
			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=================
// 配置処理色々
//=================

// 左から位置、移動方向、寿命、色、移動速度、大きさ
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	// 全て見回して
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 働いてない奴に呼びかける
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos;
			g_Effect[nCntEffect].dir = dir;
			g_Effect[nCntEffect].fSpeed = fSpeed;
			g_Effect[nCntEffect].fSize = fSize;
			g_Effect[nCntEffect].move = D3DXVECTOR3(sinf(g_Effect[nCntEffect].dir.y) * fSpeed, sinf(g_Effect[nCntEffect].dir.x) * fSpeed, cosf(g_Effect[nCntEffect].dir.y) * fSpeed);
			g_Effect[nCntEffect].nLife = nLife;
			g_Effect[nCntEffect].bUse = true;
			g_Effect[nCntEffect].col = col;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}

//=================
// 配置処理色々(単純化)
//=================

// 左から位置、移動、寿命、色、移動速度、大きさ
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	// 全て見回して
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 働いてない奴に呼びかける
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos;
			g_Effect[nCntEffect].move = move;
			g_Effect[nCntEffect].fSpeed = fSpeed;
			g_Effect[nCntEffect].fSize = fSize;
			g_Effect[nCntEffect].nLife = nLife;
			g_Effect[nCntEffect].bUse = true;
			g_Effect[nCntEffect].col = col;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}
