//======================
//
//Author:長尾悠成
//パーティクル表示のプログラム
//
//======================

#include "Particle.h"
#include "main.h"
#include "Effect.h"

// 表示する最大数
#define MAX_PARTICLE (128)

// 構造体、上から場所、移動、角度ランダムの範囲、角度、色、エフェクト移動(単純化時のみ)
// 大きさ、移動速度、出現位置の誤差範囲、エフェクトの寿命、自身の寿命、1Fで出す物量、存在するか否か、シンプル化するか否か(シンプルだとMOVEで決定、そうで無いなら角度と速度で決定)
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 randrot;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXVECTOR3 Emove;
	float fRadius;
	float fSpeed;
	float fRandR;
	int nELife;
	int nLife;
	int nAmount;
	bool bUse;
	bool bSimple;
}particle;
particle g_aParticle[MAX_PARTICLE];

//=================
// Init処理色々
//=================
void InitParticle(void)
{
	//乱数種生成
	//srand(time(NULL));
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	pDevice = GetDevice();
	
	// 初期化
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].randrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 1.0f;
		g_aParticle[nCntParticle].fSpeed = 0.0f;
		g_aParticle[nCntParticle].nELife = 10;
		g_aParticle[nCntParticle].nLife = 10;
		g_aParticle[nCntParticle].nAmount = 1;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].bSimple = false;
	}
}

//=================
// Uninit処理は空っぽ
//=================
void UninitParticle(void)
{
}

//=================
// 更新処理の色々
//=================
void UpdateParticle(void)
{
	// 置き換えや物量のカウント等
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	D3DXVECTOR3 rot;
	int nCntParticle;
	int nCntAppear;
	float fmove;
	float fRadius;

	// 全て見回して
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// 働いていて
		if (g_aParticle[nCntParticle].bUse == true)
		{
			// 複雑な処理で実行してるならこっち
			if (g_aParticle[nCntParticle].bSimple == false)
			{
				// このパーティクルをmoveで移動させる
				g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

				// -1.00 から 1.00 の範囲でランダムな浮動小数点数を生成
				float random_value;

				// 既定の範囲、角度に出現させる処理
				for (nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nAmount; nCntAppear++)
				{
					// 初期位置からfRandRの大きさにより場所にランダム性を与える
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos = g_aParticle[nCntParticle].pos;
					pos.x += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.y += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.z += g_aParticle[nCntParticle].fRandR * random_value;

					// 初期位置からrandrotの大きさにより角度にランダム性を与える
					rot = g_aParticle[nCntParticle].rot;
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.x += g_aParticle[nCntParticle].randrot.x * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.y += g_aParticle[nCntParticle].randrot.y * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.z += g_aParticle[nCntParticle].randrot.z * random_value;

					// 初期位置からfRandRの大きさによりサ場所にランダム性を与え、やっと生成する
					col = g_aParticle[nCntParticle].col;
					SetEffect(pos, rot, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}
			// そうでなくシンプルな処理にしてるなら
			else
			{
				// このパーティクルをmoveで移動させる
				g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

				// 乱数セット変数
				float random_value;

				// 既定の範囲に出現させる処理
				for (nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nAmount; nCntAppear++)
				{
					// 初期位置からfRandRの大きさにより場所にランダム性を与える
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos = g_aParticle[nCntParticle].pos;
					pos.x += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.y += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.z += g_aParticle[nCntParticle].fRandR * random_value;

					// 初期位置からfRandRの大きさにより場所にランダム性を与え、やっと生成する
					col = g_aParticle[nCntParticle].col;
					SetEffect2(pos, g_aParticle[nCntParticle].Emove, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}

			// 寿命が無くなると死ぬ
			g_aParticle[nCntParticle].nLife -= 1;
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=================
// 描写処理も空っぽ
//=================
void DrawParticle(void)
{
}

//=================
// 配置処理色々
//=================

// 左から位置、角度、角度乱数の大きさ、自身の移動。色、大きさ、寿命、エフェクトの寿命、1Fの物量、エフェクト移動速度、場所乱数の大きさ、シンプル化するか否か、(シンプル化するならエフェクトの移動)
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove)
{
	int nCntParticle;
	//全て見回して
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//働いてない奴に呼びかける
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].randrot = randrot;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].Emove = Emove;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].fRandR = fRandR;
			g_aParticle[nCntParticle].nELife = nELife;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nAmount = nAmount;
			g_aParticle[nCntParticle].fSpeed = fSpeed;
			g_aParticle[nCntParticle].bSimple = bSimple;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}
