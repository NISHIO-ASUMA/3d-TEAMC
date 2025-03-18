//===================================================================================================================
//
// パーティクル2d [particle2d.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "particle2d.h"
#include "effect2d.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_PARTICLE2D (30) // パーティクルの最大数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
Particle2D  g_aParticle2D[MAX_PARTICLE2D];//弾の情報

//===================================================================================================================
// パーティクル2Dの初期化処理
//===================================================================================================================
void InitParticle2D(void)
{
	// パーティクルの最大数分回す
	for (int nCnt = 0; nCnt < MAX_PARTICLE2D; nCnt++)
	{
		g_aParticle2D[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aParticle2D[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aParticle2D[nCnt].col = COLOR_WHITE;						// 色
		g_aParticle2D[nCnt].fWidth = 0.0f;							// 横幅
		g_aParticle2D[nCnt].fHeight = 0.0f;							// 高さ
		g_aParticle2D[nCnt].fRadius = 0.0f;							// 半径
		g_aParticle2D[nCnt].nLife = 0;								// 寿命
		g_aParticle2D[nCnt].nType = 0;								// 種類
		g_aParticle2D[nCnt].nNumParticle = 0;						// パーティクルの粒子の数
		g_aParticle2D[nCnt].speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // パーティクルの移動速度
		g_aParticle2D[nCnt].bUse = false;							// 使用状態
	}
}
//===================================================================================================================
// パーティクル2Dの終了処理
//===================================================================================================================
void UninitParticle2D(void)
{
}
//===================================================================================================================
// パーティクル2Dの更新処理
//===================================================================================================================
void UpdateParticle2D(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE2D; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == false) continue;

		//パーティクル生成
		for (int nCntApper = 0; nCntApper < g_aParticle2D[nCntParticle].nNumParticle; nCntApper++)
		{
			//位置の設定pos
			D3DXVECTOR3 pos = g_aParticle2D[nCntParticle].pos;
			 
			// 移動方向
			D3DXVECTOR3 dir = g_aParticle2D[nCntParticle].dir;

			// 移動速度
			D3DXVECTOR3 speed = g_aParticle2D[nCntParticle].speed;

			//移動量の設定
			g_aParticle2D[nCntParticle].move.x = sinf(dir.x) * speed.x;
			g_aParticle2D[nCntParticle].move.y = cosf(dir.y) * speed.y;
			g_aParticle2D[nCntParticle].move.z = 0.0f;

			// 移動量
			D3DXVECTOR3 move = g_aParticle2D[nCntParticle].move;

			//色の設定col
			D3DXCOLOR col = g_aParticle2D[nCntParticle].col;

			//半径の設定fRadius
			float fRadius = ((float)(rand() % (int)g_aParticle2D[nCntParticle].fRadius));

			int nLife = (rand() % g_aParticle2D[nCntParticle].nLife) + 10;

			//エフェクトの設定SetEffect
			SetEffect2D(pos, move, col, fRadius, nLife);

			g_aParticle2D[nCntParticle].nLife--;

			//寿命の設定
			if (g_aParticle2D[nCntParticle].nLife <= 0)//寿命が尽きた
			{
				g_aParticle2D[nCntParticle].bUse = false;
			}
		}
	}
}
//===================================================================================================================
// パーティクル2Dの描画処理
//===================================================================================================================
void DrawParticle2D(void)
{
}
//===================================================================================================================
// パーティクル2Dの設定処理
//===================================================================================================================
void SetParticle2D(D3DXVECTOR3 pos,int nLife, D3DXCOLOR col,float fRadius, int nType,int nNumParticle, D3DXVECTOR3 dir, D3DXVECTOR3 speed)
{
	// パーティクルの最大数分回す
	for (int nCnt = 0; nCnt < MAX_PARTICLE2D; nCnt++)
	{
		if (g_aParticle2D[nCnt].bUse == false)
		{
			g_aParticle2D[nCnt].pos = pos;						// 位置
			g_aParticle2D[nCnt].col = col;						// 色
			g_aParticle2D[nCnt].fRadius = fRadius;				// 半径
			g_aParticle2D[nCnt].nLife = nLife;					// 寿命
			g_aParticle2D[nCnt].nNumParticle = nNumParticle;	// 粒子の数
			g_aParticle2D[nCnt].nType = nType;					// 種類
			g_aParticle2D[nCnt].speed = speed;					// 移動速度
			g_aParticle2D[nCnt].bUse = true;					// 使用状態

			break;
		}
	}
}
