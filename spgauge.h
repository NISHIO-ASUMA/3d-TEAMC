//=======================================
//
// SPゲージ処理 [spgauge.h]
// Author:YOSHIDA YUTO
//
//=======================================
#ifndef _SPGAUGE_H_
#define _SPGAUGE_H_

//******************************
// インクルードファイル宣言
//******************************
#include "main.h"

//****************************
// スペシャルゲージの種類
//****************************
typedef enum
{
	SPGAUGE_FRAME = 0,
	SPGAUGE_GAUGE,
	SPGAUGE_MAX
}SPGAUGE;

//****************************
// スペシャルゲージの構造体
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // 位置
	int nType; // 種類
	float SpGauge;
}SPgauge;

//****************************
// プロトタイプ宣言
//****************************
void InitSPgauge(void);			// ゲージの初期化
void UninitSPgauge(void);		// ゲージの終了
void UpdateSPgauge(void);		// ゲージの更新
void DrawSPgauge(void);			// ゲージの描画
void AddSpgauge(float fValue);  // ゲージの加算処理
#endif