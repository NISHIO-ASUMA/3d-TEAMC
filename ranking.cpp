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
#include "sound.h"

//=============================
//�����L���O�̏���������
//=============================
void InitRanking(void)
{

	InitRankingSet();

	InitRankingScore();

	ResetRanking();

	RankingTexture();

	// ���y���Đ�
	PlaySound(SOUND_LABEL_RANKING_BGM);
}
//=============================
//�����L���O�̏I������
//=============================
void UninitRanking(void)
{
	// ���y���~
	StopSound();

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
