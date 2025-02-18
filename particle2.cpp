//=====================================================================================================
//
// パーティクル2[particle2.cpp]
// Author:YOSHIDA YUUTO
//
//=====================================================================================================

//*****************************************************************************************************
//インクルードファイル
//*****************************************************************************************************
#include "bullet.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "effect2.h"
#include "particle2.h"

//*****************************************************************************************************
//マクロ定義
//*****************************************************************************************************
#define BULLET_WIDTH (25.0f) //横幅
#define BULLET_HEIGHT (25.0f) //高さ
#define MAX_PARTICLE (256) //パーティクルの最大数
#define MAX_MOVE (5.0f) //移動量

//*****************************************************************************************************
//グローバル変数宣言
//*****************************************************************************************************
PARTICLE g_aParticle[MAX_PARTICLE];

//=====================================================================================================
//パーティクルの初期化処理
//=====================================================================================================
void InitParticleX(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		g_aParticle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aParticle[nCnt].nLife = 0;
		g_aParticle[nCnt].nNumParticle = 1;
		g_aParticle[nCnt].nType = 0;
		g_aParticle[nCnt].AngleMax = 628;
		g_aParticle[nCnt].AngleMin = -314;
		g_aParticle[nCnt].fSize = 0.0f;
		g_aParticle[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].bUse = false;
		g_aParticle[nCnt].bLifedecrease = true;
		g_aParticle[nCnt].EffectTex = EFFECT_NORMAL;
	}
}
//=====================================================================================================
//パーティクルの終了処理
//=====================================================================================================
void UninitParticleX(void)
{

}
//=====================================================================================================
//パーティクルの更新処理
//=====================================================================================================
void UpdateParticleX(void)
{
	int nCntParticle, nCntApper;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	D3DXVECTOR3 fScal;
	float fSize;

	Player* pPlayer = GetPlayer();

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{//使用している
			continue;
		}

		//パーティクル生成
		for (nCntApper = 0; nCntApper < g_aParticle[nCntParticle].nNumParticle/*発生させたい粒子の数*/; nCntApper++)
		{
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			int MAX = g_aParticle[nCntParticle].AngleMax;
			int MIN = g_aParticle[nCntParticle].AngleMin;

			float fAngleX = (float)(rand() % MAX - MIN) * 0.01f;
			float fAngleY = (float)(rand() % MAX - MIN) * 0.01f;
			float fAngleZ = (float)(rand() % MAX - MIN) * 0.01f;

			float fMoveX = ((float)(rand() % 1) + 1.0f);
			float fMoveY = ((float)(rand() % 1) + 1.0f);
			float fMoveZ = ((float)(rand() % 1) + 1.0f);

			//位置の設定pos
			pos.x = g_aParticle[nCntParticle].pos.x /*+ (float)(rand() % 50 - 50.0f)*/;
			pos.y = g_aParticle[nCntParticle].pos.y /*+ (float)(rand() % 50 - 50.0f)*/;
			pos.z = g_aParticle[nCntParticle].pos.z /*+ (float)(rand() % 50 - 50.0f)*/;

			//pos.x += (float)(rand() % 100 - 100.0f);
			//pos.y += (float)(rand() % 100 - 100.0f);
			//pos.z += (float)(rand() % 100 - 100.0f);

			//移動量の設定
			move.x = cosf(fAngleX) * g_aParticle[nCntParticle].dir.x * fMoveX;
			move.y = sinf(fAngleY) * g_aParticle[nCntParticle].dir.y * fMoveY;
			move.z = cosf(fAngleZ) * g_aParticle[nCntParticle].dir.z * fMoveZ;

			//色の設定col
			col = g_aParticle[nCntParticle].col;

			fSize = g_aParticle[nCntParticle].fSize;

			int nLife = (rand() % g_aParticle[nCntParticle].nLife) + 60;

			/*int nLife = (rand() % 120) + 20;*/
			
			//エフェクトの設定SetEffect
			SetEffectX(pos, move, col, nLife, fSize,g_aParticle[nCntParticle].decfAlv,g_aParticle[nCntParticle].EffectTex);
		}

		if (g_aParticle[nCntParticle].bLifedecrease)
		{
			g_aParticle[nCntParticle].nLife--;
		}

		//寿命の設定
		if (g_aParticle[nCntParticle].nLife <= 0)//寿命が尽きた
		{
			g_aParticle[nCntParticle].bUse = false;
		}
		
	}

}
//=====================================================================================================
//パーティクルの描画処理
//=====================================================================================================
void DrawParticleX(void)
{

}
//=====================================================================================================
//パーティクルの描画処理
//=====================================================================================================
void SetParticleX(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nType, float fSize, int nNumParticle, int AngleMax, int AngleMin, float fSpeed, D3DXVECTOR3 dir, float decfAlv, int EffectTex, bool bLifedecrease)
{
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == false)
		{
			g_aParticle[nCnt].pos = pos; // 位置
			g_aParticle[nCnt].move = move; // 移動量
			g_aParticle[nCnt].fSize = fSize; // 大きさ
			g_aParticle[nCnt].nType = nType; // 種類
			g_aParticle[nCnt].col = col; // 色
			g_aParticle[nCnt].nNumParticle = nNumParticle; // パーティクルの粒子の数
			g_aParticle[nCnt].nLife = nLife; // 寿命
			g_aParticle[nCnt].AngleMax = AngleMax; // アングルの最大
			g_aParticle[nCnt].AngleMin = AngleMin; // アングルの最小
			g_aParticle[nCnt].fSpeed = fSpeed; // 移動速度
			g_aParticle[nCnt].dir = dir; // 移動方向
			g_aParticle[nCnt].decfAlv = decfAlv; // α値の減少値
			g_aParticle[nCnt].EffectTex = EffectTex; // エフェクトの種類
			g_aParticle[nCnt].bLifedecrease = bLifedecrease; // 寿命をつけるか
			g_aParticle[nCnt].bUse = true;

			break;
		}
	}
}
//=====================================================================================================
// パーティクルの設定処理
//=====================================================================================================
void SetParticleEdit(SETPARTICLE particle)
{
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{
			g_aParticle[nCnt].pos = particle.pos; // 位置
			g_aParticle[nCnt].move = particle.move; // 移動量
			g_aParticle[nCnt].fSize = particle.fSize; // 大きさ
			g_aParticle[nCnt].nType = particle.nType; // 種類
			g_aParticle[nCnt].col = particle.col; // 色
			g_aParticle[nCnt].nNumParticle = particle.nNumParticle; // パーティクルの粒子の数
			g_aParticle[nCnt].nLife = particle.nLife; // 寿命
			g_aParticle[nCnt].AngleMax = particle.AngleMax; // アングルの最大
			g_aParticle[nCnt].AngleMin = particle.AngleMin; // アングルの最小
			g_aParticle[nCnt].fSpeed = particle.fSpeed; // 移動速度
			g_aParticle[nCnt].dir = particle.dir; // 移動方向
			g_aParticle[nCnt].decfAlv = particle.decfAlv; // α値の減少値
			g_aParticle[nCnt].EffectTex = particle.EffectTex; // エフェクトの種類

			g_aParticle[nCnt].bLifedecrease = particle.bLifedecrease; // 寿命をつけるか

			break;
		}
	}
}
