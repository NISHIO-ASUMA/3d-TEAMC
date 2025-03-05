//==============================================================================================================
//
// ボスの体力ゲージ [ bosslife.cpp ]
// Author:Asuma Nishio
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "boss.h"
#include "bosslife.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define NUM_BOSSLIFE (20) // 最大数

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
BossLife g_BossLife[NUM_BOSSLIFE]; // 構造体変数
LPDIRECT3DTEXTURE9 g_pTextureBossLife[BOSSTEX_MAX] = {}; // テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossLife = NULL; // 頂点バッファのポインタ

//==============================================================================================================
// ボスの体力バーの初期化処理
//==============================================================================================================
void InitBossLife()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BOSSTEX_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BOSSLIFE_TEX[nCnt],
			&g_pTextureBossLife[nCnt]);
	}

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		g_BossLife[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 座標
		g_BossLife[nCnt].nType = 0;			                // 種類
		g_BossLife[nCnt].fLength = 0.0f;                    // バーの長さ
		g_BossLife[nCnt].bUse = false;						// 使用判定
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BOSSLIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossLife,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
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
	g_pVtxBuffBossLife->Unlock();
}
//==============================================================================================================
// ボスの体力バーの終了処理
//==============================================================================================================
void UninitBossLife()
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < BOSSTEX_MAX; nCnt++)
	{
		if (g_pTextureBossLife[nCnt] != NULL)
		{
			g_pTextureBossLife[nCnt]->Release();
			g_pTextureBossLife[nCnt] = NULL;

		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBossLife != NULL)
	{
		g_pVtxBuffBossLife->Release();
		g_pVtxBuffBossLife = NULL;
	}

}
//==============================================================================================================
// ボスの体力バーの更新処理
//==============================================================================================================
void UpdateBossLife()
{
	
}
//==============================================================================================================
// ボスの体力バーの描画処理
//==============================================================================================================
void DrawBossLife()
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		if (!g_BossLife[nCnt].bUse)
		{// 使われていないとき
			// 下の処理を通らずカウントを進める
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_BossLife[nCnt].mtxWorld);

		// ビューマトリックスを宣言
		D3DXMATRIX mtxView;

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		g_BossLife[nCnt].mtxWorld._11 = mtxView._11;
		g_BossLife[nCnt].mtxWorld._12 = mtxView._21;
		g_BossLife[nCnt].mtxWorld._13 = mtxView._31;
		g_BossLife[nCnt].mtxWorld._21 = mtxView._12;
		g_BossLife[nCnt].mtxWorld._22 = mtxView._22;
		g_BossLife[nCnt].mtxWorld._23 = mtxView._32;
		g_BossLife[nCnt].mtxWorld._31 = mtxView._13;
		g_BossLife[nCnt].mtxWorld._32 = mtxView._23;
		g_BossLife[nCnt].mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_BossLife[nCnt].pos.x, g_BossLife[nCnt].pos.y, g_BossLife[nCnt].pos.z);
		D3DXMatrixMultiply(&g_BossLife[nCnt].mtxWorld, &g_BossLife[nCnt].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_BossLife[nCnt].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBossLife, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (g_BossLife[nCnt].bUse == true)
		{
			// 種類を保存
			int nType = g_BossLife[nCnt].nType;

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossLife[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}
//==============================================================================================================
// ボスの体力バーの設定処理
//==============================================================================================================
int SetBossLife(D3DXVECTOR3 pos, int nType)
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		if (!g_BossLife[nCnt].bUse)
		{// 未使用なら
			g_BossLife[nCnt].pos = pos; // 座標
			g_BossLife[nCnt].nType = nType; // 種類
			g_BossLife[nCnt].bUse = true; // 使用判定

			break;
		}
	}

	return nCnt;
}
//==============================================================================================================
// ボスの体力バーの位置設定
//==============================================================================================================
void SetPositionLifeBar(int nIdx, D3DXVECTOR3 pos)
{
	g_BossLife[nIdx].pos = pos;
}
