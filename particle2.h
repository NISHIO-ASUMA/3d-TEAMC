//============================
//
// パーティクル2[particle2.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _PARTICLE2_H_
#define _PARTICLE2_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//パーティクル種類の列挙型
//****************************
typedef enum
{
	PARTICLETYPE_NORMAL = 0,
	PARTICLETYPE_MAX
}PARTICLETYPE;

//****************************
//パーティクルの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
	int nLife;//寿命
	int nType;//種類
	bool bUse;//使用しているかどうか
	bool bLifedecrease; // 寿命をつけるか
	float fSize; // 大きさ
	int nNumParticle;
	int AngleMax;
	int AngleMin;
	float fSpeed;
	D3DXVECTOR3 dir;
	float decfAlv;
	int EffectTex;
}PARTICLE;

//****************************
//パーティクルの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
	int nLife;//寿命
	int nType;//種類
	bool bUse;//使用しているかどうか
	bool bLifedecrease; // 寿命をつけるか
	float fSize; // 大きさ
	int nNumParticle;
	int ColorType;
	int AngleMax;
	int AngleMin;
	float fSpeed;
	D3DXVECTOR3 dir;
	float decfAlv;
	int EffectTex;
}SETPARTICLE;

//****************************
//プロトタイプ宣言
//****************************
void InitParticleX(void);//パーティクルの初期化処理
void UninitParticleX(void);//パーティクルの終了処理
void UpdateParticleX(void);//パーティクルの更新処理
void DrawParticleX(void);//パーティクルの描画処理
void SetParticleX(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nType, float fSize, int nNumParticle, int AngleMax, int AngleMin, float fSpeed, D3DXVECTOR3 dir, float decfAlv,int EffectTex,bool bLifedecrease);
void SetParticleEdit(SETPARTICLE particle);
#endif
