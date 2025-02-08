//=====================================================================================================================
//
// パーティクル [ Particle.h ]
// Author;長尾悠成
//
//=====================================================================================================================

//************************************
// 2重インクルード防止
//************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//************************************
// インクルードファイル宣言
//************************************
#include "main.h"

//***********************************
// プロトタイプ宣言
//***********************************
void InitParticle(void);   // 初期化処理
void UninitParticle(void); // 終了処理
void UpdateParticle(void); // 更新処理
void DrawParticle(void);   // 描画処理
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove); // 設定処理
#endif