//====================================================================================================================
//
// エフェクト処理 [ Effect.cpp ]
// Author:長尾悠成
//
//=====================================================================================================================

//***************************
// インクルードファイル宣言
//***************************
#include "Effect.h"

//****************************
// マクロ定義
//****************************
#define MAX_EFFECT (70000)  // エフェクトの最大数

//****************************
// グローバル変数宣言
//****************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL; // 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		 // テクスチャのポインタ
Effect g_Effect[MAX_EFFECT];	// 構造体変数

//============================================================================================================
// エフェクトの初期化処理
//============================================================================================================
void InitEffect(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// エフェクトのテクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.png", 
		&g_pTextureEffect);

	// デバイスの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_EFFECT * 6,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffEffect, NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 左、手前、下がマイナス
	// 初期化と大きさ設定
	// 構造体変数の初期化処理
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_Effect[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標
		g_Effect[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
		g_Effect[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きベクトル
		g_Effect[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // 色
		g_Effect[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Effect[nCnt].bUse = false;	// 未使用判定
		g_Effect[nCnt].nLife = 0;		// 寿命
		g_Effect[nCnt].fSize = 0.0f;	// 大きさ

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点情報を進める
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}
//=========================================================================================================
// エフェクトの終了処理
//=========================================================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//==========================================================================================================
// エフェクトの更新処理
//==========================================================================================================
void UpdateEffect(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// 使われているなら
		if (g_Effect[nCnt].bUse == true)
		{
			// 移動させる
			g_Effect[nCnt].pos.x += g_Effect[nCnt].move.x;
			g_Effect[nCnt].pos.y += g_Effect[nCnt].move.y;
			g_Effect[nCnt].pos.z += g_Effect[nCnt].move.z;
			
			// 寿命を減らし透明度を加算させる
			g_Effect[nCnt].nLife--;
			g_Effect[nCnt].col.a -= 1.0f / g_Effect[nCnt].nMaxLife;

			// カラーの設定
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
				// 未使用状態にする
				g_Effect[nCnt].bUse = false;
			}
		}

		// 頂点情報を進める
		pVtx += 4;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}
//=========================================================================================================
// エフェクトの描画処理
//=========================================================================================================
void DrawEffect(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;
 
	// ライトを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_Effect[nCnt].bUse == true)
		{
			// ビューマトリックス用の変数
			D3DXMATRIX mtxView;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Effect[nCnt].mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// カメラの逆行列を設定
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

			// 向きを反映
			/*D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Effect[nCnt].rot.y, g_Effect[nCnt].rot.x, g_Effect[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxRot);*/

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCnt].pos.x, g_Effect[nCnt].pos.y, g_Effect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCnt].mtxWorld);

			// 頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// テクスチャフォーマットの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==============================================================================================================
// エフェクトの設定(配置)処理 
//==============================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 未使用状態なら
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos; // 位置
			g_Effect[nCntEffect].dir = dir; // 向きベクトル
			g_Effect[nCntEffect].fSpeed = fSpeed; // 移動速度
			g_Effect[nCntEffect].fSize = fSize;   // 大きさ
			g_Effect[nCntEffect].move = D3DXVECTOR3(sinf(g_Effect[nCntEffect].dir.y) * fSpeed, sinf(g_Effect[nCntEffect].dir.x) * fSpeed, cosf(g_Effect[nCntEffect].dir.y) * fSpeed); // 移動量
			g_Effect[nCntEffect].nLife = nLife; // 寿命
			g_Effect[nCntEffect].nMaxLife = nLife;  // 最大寿命
			g_Effect[nCntEffect].col = col; // 色
			g_Effect[nCntEffect].bUse = true; // 使用状態にする

			break;
		}

		// 頂点情報を進める
		pVtx += 4;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}
//======================================================================================================================
// エフェクトの設定(配置)処理 (単純化) 
//======================================================================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 未使用状態なら
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos; // 位置
			g_Effect[nCntEffect].move = move; // 移動量
			g_Effect[nCntEffect].fSpeed = fSpeed; // 移動速度
			g_Effect[nCntEffect].fSize = fSize; // 大きさ
			g_Effect[nCntEffect].nLife = nLife; // 寿命
			g_Effect[nCntEffect].nMaxLife = nLife; // 最大寿命
			g_Effect[nCntEffect].col = col;	// 色
			g_Effect[nCntEffect].bUse = true; // 使用状態にする

			break;
		}

		// 頂点情報を進める
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}