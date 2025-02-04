//============================
//
// timer [timer.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _TIME_H_
#define _TIME_H_

//****************************
// インクルードファイル
//****************************
#include"main.h"

//****************************
// プロトタイプ宣言
//****************************
void InitTime(void);	 // タイマーの初期化処理
void UninitTime(void);	 // タイマーの終了処理
void UpdateTime(void);	 // タイマーの更新処理
void DrawTime(void);	 // タイマーの描画処理
int GetTimeMinute(void); // タイマーの取得処理
int GetTimeSecond(void); // タイマーの取得処理

#endif
