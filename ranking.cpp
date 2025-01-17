//============================
//
// ランキング [ranking.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"RankingScore.h"
#include"RankingSet.h"

//=============================
//ランキングの初期化処理
//=============================
void InitRanking(void)
{

	InitRankingSet();

	InitRankingScore();

	ResetRanking();

	RankingTexture();
}
//=============================
//ランキングの終了処理
//=============================
void UninitRanking(void)
{
	UninitRankingSet();

	UninitRankingScore();
}
//=============================
//ランキングの更新処理
//=============================
void UpdateRanking(void)
{
	UpdateRankingSet();

	UpdateRankingScore();
}
//=============================
//ランキングの描画処理
//=============================
void DrawRanking(void)
{
	DrawRankingSet();

	DrawRankingScore();
}
