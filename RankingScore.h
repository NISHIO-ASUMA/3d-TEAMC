//============================
//
// �����L���O�X�R�A [rankingscore.h]
// Author: TEAM_C
//
//============================

#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�}�N����`
//****************************
#define RANK000 ("data\\rank.txt")
#define RANK001 ("data\\rank2.txt")

//****************************
//�Q�[���̎��
//****************************
typedef enum
{
	GAMEEND_STAGE1 = 0,
	GAMEEND_STAGE2,
	GAMEEND_MAX
}GAMEEND;

//****************************
//�����L���O�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nScore;//�X�R�A
	int State;
	GAMEEND GameState;
}RankScore;

typedef struct
{
	GAMEEND GameState;
}END;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitRankingScore(void);//�����L���O�X�R�A���ʂ̏���������
void UninitRankingScore(void);//�����L���O�X�R�A���ʂ̏I������
void UpdateRankingScore(void);//�����L���O�X�R�A���ʂ̍X�V����
void DrawRankingScore(void);//�����L���O�X�R�A���ʂ̕`�揈��
void WriteScore(void);//�X�R�A�����o��	
void ResetRanking(void);//�����L���O�X�R�A�̃��Z�b�g
void SetRanking(int nScore);//�����L���O�X�R�A�̐ݒ�
void RankingTexture(void);//�����L���O�e�N�X�`��

#endif