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
#include "easing.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define NUM_BOSSLIFE (100) // 最大数
#define MAX_LENGTH (100.0f) // 横幅
#define TOPPOS (105.0f) // いちばん上の位置
#define UNDERPOS (90.0f) // いちばん上の位置

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void UpdateLifegage(Boss* pBoss); // ボスの体力バーの更新
void SetLifeBarLength(int nIdxLifebar, float fRate, float fLength); // ボスの体力バーの長さの設定処理
void SetDelayBarLength(int nIdxDelayBar, float fRate, float fLength,int state); // ボスのディレイバーの長さの設定処理

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
		g_BossLife[nCnt].DelayLength = MAX_LENGTH;                // バーの長さ
		g_BossLife[nCnt].nEasingCnt = 0;                // イージングのカウント
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
void UpdateBossLife(Boss* pBoss)
{
	UpdateLifegage(pBoss);
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
		D3DXMatrixTranslation(&mtxTrans,  g_BossLife[nCnt].pos.x, g_BossLife[nCnt].pos.y, g_BossLife[nCnt].pos.z);
		D3DXMatrixMultiply(&g_BossLife[nCnt].mtxWorld, &g_BossLife[nCnt].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_BossLife[nCnt].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBossLife, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// 種類を保存
		int nType = g_BossLife[nCnt].nType;

		switch (nType)
		{
		case BOSSTEX_DELAY:
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossLife[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			break;
		case BOSSTEX_LIFE:
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossLife[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			break;
		case BOSSTEX_FRAME:
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBossLife[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			break;

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
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	int nCnt = 0;

	for (nCnt = 0; nCnt < NUM_BOSSLIFE; nCnt++)
	{
		if (g_BossLife[nCnt].bUse == false)
		{// 未使用なら
			g_BossLife[nCnt].pos = pos; // 座標
			g_BossLife[nCnt].nType = nType; // 種類
			g_BossLife[nCnt].bUse = true; // 使用判定

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, TOPPOS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + MAX_LENGTH, TOPPOS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, UNDERPOS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + MAX_LENGTH, UNDERPOS, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBossLife->Unlock();

	return nCnt;
}
//==============================================================================================================
// ボスの体力バーの位置設定
//==============================================================================================================
void SetPositionLifeBar(int nIdxBar, int nIdxFrame, int nIdxDelay,D3DXVECTOR3 pos)
{
	g_BossLife[nIdxBar].pos = pos;
	g_BossLife[nIdxFrame].pos = pos;
	g_BossLife[nIdxDelay].pos = pos;
}
//==============================================================================================================
// ボスの体力バーの消去
//==============================================================================================================
void DeleateLifeBar(int nIdxBar, int nIdxFrame, int nIdxDelay)
{
	g_BossLife[nIdxBar].bUse = false;
	g_BossLife[nIdxFrame].bUse = false;
	g_BossLife[nIdxDelay].bUse = false;
}
//==============================================================================================================
// ボスの体力バーの更新
//==============================================================================================================
void UpdateLifegage(Boss* pBoss)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	float fRateLife = (float)pBoss->nLife / (float)pBoss->nMaxLife;

	g_BossLife[pBoss->nLifeBarIdx].fLength = fRateLife * MAX_LENGTH;

	// 体力バーのゲージの設定処理
	SetLifeBarLength(pBoss->nLifeBarIdx, fRateLife, g_BossLife[pBoss->nLifeBarIdx].fLength);

	// ディレイバーの設定処理
	SetDelayBarLength(pBoss->nLifeDelayIdx, fRateLife, g_BossLife[pBoss->nLifeBarIdx].fLength,pBoss->state);
}
//==============================================================================================================
// ボスの体力バーの長さの設定処理
//==============================================================================================================
void SetLifeBarLength(int nIdxLifebar,float fRate,float fLength)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIdxLifebar;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, TOPPOS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + fLength, TOPPOS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, UNDERPOS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + fLength, UNDERPOS, 0.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fRate, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fRate, 1.0f);

	// アンロック
	g_pVtxBuffBossLife->Unlock();

}
//==============================================================================================================
// ボスのディレイバーの長さの設定処理
//==============================================================================================================
void SetDelayBarLength(int nIdxDelayBar,float fRate,float fLength,int state)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBossLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIdxDelayBar;

	if (state == BOSSSTATE_DAMAGE)
	{
		g_BossLife[nIdxDelayBar].nEasingCnt = 0;
	}
	else
	{
		g_BossLife[nIdxDelayBar].nEasingCnt++;
	}

	float Time = SetEase(g_BossLife[nIdxDelayBar].nEasingCnt,180);

	g_BossLife[nIdxDelayBar].DelayLength += (fLength - g_BossLife[nIdxDelayBar].DelayLength) * EaseOutCubic(Time);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, TOPPOS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + g_BossLife[nIdxDelayBar].DelayLength, TOPPOS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f, UNDERPOS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-MAX_LENGTH * 0.5f + g_BossLife[nIdxDelayBar].DelayLength, UNDERPOS, 0.0f);

	static float fDelayRate = fRate;

	fDelayRate += (fRate - fDelayRate) * EaseOutCubic(Time);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fDelayRate, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fDelayRate, 1.0f);

	// アンロック
	g_pVtxBuffBossLife->Unlock();
}
