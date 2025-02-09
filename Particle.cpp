//================================================================================================================
//
// パーティクル [ Particle.cpp ]
// Author:長尾悠成
//
//================================================================================================================

//****************************************************************************************************************
// インクルードファイル宣言
//****************************************************************************************************************
#include "Particle.h"
#include "main.h"
#include "Effect.h"

//***************************************************************************************************************
// マクロ定義
//***************************************************************************************************************
#define MAX_PARTICLE (128)		// 表示する最大数

//***************************************************************************************************************
// パーティクルの構造体
//***************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	D3DXVECTOR3 move; // 移動量
	D3DXVECTOR3 randrot; // ランダムな角度の方向
	D3DXVECTOR3 rot; // 角度
	D3DXCOLOR col;	 // 色
	D3DXVECTOR3 Emove; // エフェクトの移動(単純化時のみ)
	float fRadius; // 半径
	float fSpeed;  // 移動速度
	float fRandR;  // 出現する位置の誤差の範囲
	int nELife;	   // エフェクトの寿命
	int nLife;	   // 寿命
	int nAmount;   // 1Fで出す量
	bool bUse;	   // 使用しているか否か
	bool bSimple;  // シンプル化するか否か
}particle;

//*********************************************************************************************************************
// グローバル変数宣言
//*********************************************************************************************************************
particle g_aParticle[MAX_PARTICLE];

//=============================================================================================================
// パーティクルの初期化処理
//=============================================================================================================
void InitParticle(void)
{
	//乱数種生成
	//srand(time(NULL));

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 構造体変数の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 座標
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 角度
		g_aParticle[nCntParticle].randrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ランダム角度
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // 移動量
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色
		g_aParticle[nCntParticle].fRadius = 1.0f;	// 半径
		g_aParticle[nCntParticle].fSpeed = 0.0f;	// 移動速度
		g_aParticle[nCntParticle].nELife = 10;		// エフェクトの寿命
		g_aParticle[nCntParticle].nLife = 10;		// 自身の寿命
		g_aParticle[nCntParticle].nAmount = 1;		// 1Fで出すパーティクルの量
		g_aParticle[nCntParticle].bUse = false;		// 未使用判定
		g_aParticle[nCntParticle].bSimple = false;	// シンプルじゃない
	}
}
//==========================================================================================================
// パーティクルの終了処理
//==========================================================================================================
void UninitParticle(void)
{
	// 無し
}
//==========================================================================================================
// パーティクルの更新処理
//==========================================================================================================
void UpdateParticle(void)
{
	// 置き換えや物量のカウント等
	D3DXVECTOR3 pos; // 座標
	D3DXVECTOR3 move;// 移動量
	D3DXCOLOR col;	 // 色
	D3DXVECTOR3 rot; // 角度
	int nCntAppear;

	//float fmove;
	//float fRadius;

	// 全て見回して
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// パーティクルが使われているなら
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
			else
			{// そうでなくシンプルな処理にしてるなら

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
			{// 体力が0以下なら
				// 未使用判定にする
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}
//===========================================================================================================
// パーティクルの描画処理
//===========================================================================================================
void DrawParticle(void)
{
	// 無し
}
//=================================================================================================================
// パーティクルの設定(配置)処理
//=================================================================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove)
{
	// 全て見回して
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// パーティクルが未使用だったら
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos; // 座標
			g_aParticle[nCntParticle].rot = rot; // 角度
			g_aParticle[nCntParticle].randrot = randrot; // 角度の乱数
			g_aParticle[nCntParticle].move = move;	 // 移動量
			g_aParticle[nCntParticle].col = col;	 // 色
			g_aParticle[nCntParticle].fRadius = fRadius; // 半径
			g_aParticle[nCntParticle].nLife = nLife; // 自身の寿命
			g_aParticle[nCntParticle].nELife = nELife; // エフェクトの寿命
			g_aParticle[nCntParticle].nAmount = nAmount; // 1Fで出すパーティクルの量
			g_aParticle[nCntParticle].fSpeed = fSpeed; // 移動速度
			g_aParticle[nCntParticle].fRandR = fRandR; // 場所乱数
			g_aParticle[nCntParticle].bSimple = bSimple; // シンプルかどうか
			g_aParticle[nCntParticle].Emove = Emove; // エフェクトの移動量
			g_aParticle[nCntParticle].bUse = true; // 使用状態

			break;
		}
	}
}
