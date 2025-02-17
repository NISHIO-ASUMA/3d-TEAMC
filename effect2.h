//============================
//
// エフェクト2[effect.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EFFECT2_H_
#define _EFFECT2_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//エフェクト種類の列挙型
//****************************
typedef enum
{
	EFFECT_NORMAL = 0,
	EFFECT_SMORK,
	EFFECT_MAX
}EFFECTTEX;

static const char* EFFECT_TEXTURE[EFFECT_MAX] =
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\smoke000.png",
};
//****************************
//エフェクト種類の列挙型
//****************************
typedef enum
{
	EFFECTTYPE_PLAYER=0,
	EFFECTTYPE_BULLET,
	EFFECTTYPE_MAX
}EFFECTTYPE;

//****************************
//エフェクトの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	float fRadius;
	int nLife;//寿命
	bool bUse;//使用しているかどうか
	int nType;
	EFFECTTYPE Type;//種類
	int EffectType;
	int g_nCounterAnim, g_nPatternAnim;
	float decfAlv;
}EFFECT;

//****************************
//プロトタイプ宣言
//****************************
void InitEffectX(void);//エフェクトの初期化処理
void UninitEffectX(void);//エフェクトの終了処理
void UpdateEffectX(void);//エフェクトの更新処理
void DrawEffectX(void);//エフェクトの描画処理
void SetEffectX(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float decfAlv,int EffectType);//エフェクトの設定処理
#endif
