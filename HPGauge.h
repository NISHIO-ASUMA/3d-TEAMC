//=====================================================================================================================
//
//HPゲージ関連のヘッダー
//Author;長尾悠成
//
//=====================================================================================================================

// 重複確認
#ifndef _GAUGE_H_
#define _GAUGE_H_

// インクルードファイル
#include "main.h"

// プロトタイプ宣言
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
void SetGauge(void);
void AddFever(float Add);

#endif