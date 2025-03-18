//============================
//
//エフェクト
//Author:YOSHIDA YUUTO
//
//
//============================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//エフェクト構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;//色
	float fRadius;//半径(大きさ)
	float fWidth;
	float fHeight;
	int nLife;//寿命(表示時間)
	bool bUse;//使用しているかどうか
}Effect;

void InitEffect(void);//エフェクトの初期化処理
void UninitEffect(void);//エフェクトの終了処理
void UpdateEffect(void);//エフェクトの更新処理
void DrawEffect(void);//エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);
Effect* GetEffect(void);
#endif
