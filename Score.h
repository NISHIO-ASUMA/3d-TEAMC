//============================
//
// score [score.h]
// Author:YOSHIDA YUUTO
//
//============================
#ifndef _SCORE_H_
#define _SCORE_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"

//****************************
//プロトタイプ宣言
//****************************
int ScoreCounter(int nScore);
void InitScore(void);//スコアの初期化処理
void UninitScore(void);//スコアの終了処理
void UpdateScore(void);//スコアの更新処理
void DrawScore(void);//スコアの描画処理
void SetScore(int nScore);//スコアの設定処理
void AddScore(int nValue);//スコアの加算処理
int GetScore(void);//スコアの取得処理
void SaveScore(void); // スコア保存処理

#endif
