//===================================
//
//パーティクル関連のヘッダー
//Author;長尾悠成
//
//===================================

// 重複確認
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

// インクルードファイル
#include "Main.h"

// プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove);
#endif