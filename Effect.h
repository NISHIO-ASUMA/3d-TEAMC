//===================================
//
//エフェクト関連のヘッダー
//Author;長尾悠成
//
//===================================

// 重複確認
#ifndef _EFFECT_H_
#define _EFFECT_H_

// インクルードファイル
#include "main.h"

// 構造体、下４つの上から寿命、移動速度、大きさ、存在してるか否か
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 dir;
	D3DXCOLOR col;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nLife;
	float fSpeed;
	float fSize;
	bool bUse;
}Effect;

// プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int nLife, D3DXCOLOR col, float fSpeed, float fSize);
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fSpeed, float fSize);

#endif