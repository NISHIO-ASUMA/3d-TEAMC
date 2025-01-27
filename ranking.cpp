//============================
//
// ランキング [ranking.cpp]
// Author: TEAM_C
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
#include "sound.h"

//=============================
//ランキングの初期化処理
//=============================
void InitRanking(void)
{

	InitRankingSet();

	InitRankingScore();

	ResetRanking();

	RankingTexture();

	// 音楽を再生
	PlaySound(SOUND_LABEL_RANKING_BGM);
}
//=============================
//ランキングの終了処理
//=============================
void UninitRanking(void)
{
	// 音楽を停止
	StopSound();

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
