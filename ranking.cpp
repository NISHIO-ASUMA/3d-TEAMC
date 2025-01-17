//============================
//
// �����L���O [ranking.cpp]
// Author: TEAM_C
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"RankingScore.h"
#include"RankingSet.h"

//=============================
//�����L���O�̏���������
//=============================
void InitRanking(void)
{

	InitRankingSet();

	InitRankingScore();

	ResetRanking();

	RankingTexture();
}
//=============================
//�����L���O�̏I������
//=============================
void UninitRanking(void)
{
	UninitRankingSet();

	UninitRankingScore();
}
//=============================
//�����L���O�̍X�V����
//=============================
void UpdateRanking(void)
{
	UpdateRankingSet();

	UpdateRankingScore();
}
//=============================
//�����L���O�̕`�揈��
//=============================
void DrawRanking(void)
{
	DrawRankingSet();

	DrawRankingScore();
}
