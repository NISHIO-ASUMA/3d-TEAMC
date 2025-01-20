//============================
//
// �Q�[����� [game.cpp]
// Author: TEAM_C
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "RankingScore.h"
#include "damagepop.h"
#include "time.h"
#include"pause.h"
#include "meshfield.h"
#include "player.h"

//****************************
//�}�N����`
//****************************

//****************************
//�O���[�o���ϐ�
//****************************
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���

//=======================
//�Q�[����ʂ̏���������
//=======================
void InitGame(void)
{
	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�e�̏���������
	InitShadow();

	//�|�[�Y�̏���������
	InitPause();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//�v���C���[�̏���������
	InitPlayer();

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	g_bPause = false;//�|�[�Y����

}
//=======================
//�Q�[����ʂ̏I������
//=======================
void UninitGame(void)
{
	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�e�̏I������
	UninitShadow();

	//�|�[�Y�̏I������
	UninitPause();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//�v���C���[�̏I������
	UninitPlayer();
}
//=======================
//�Q�[����ʂ̍X�V����
//=======================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END://�I�����
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ����

			//���(���[�h)�̐ݒ�
			SetFade(MODE_RESULT);

			//�����L���O�̃��Z�b�g
			ResetRanking();

			//�����L���O�̃Z�b�g
			SetRanking(0);
		}
		break;
	}

	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START) == true)
	{//�|�[�Y�L�[(P�L�[)�������ꂽ
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y���̍X�V����
		UpdatePause();
	}
	if (g_bPause == false)
	{//�|�[�Y���łȂ����

		//�J�����̍X�V����
		UpdateCamera();

		//���C�g�̍X�V����
		UpdateLight();

		//�e�̍X�V����
		UpdateShadow();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		if (KeyboardTrigger(DIK_RETURN))
		{
			g_gameState = GAMESTATE_END;
		}
	}
}
//=======================
//�Q�[����ʂ̕`�揈��
//=======================
void DrawGame(void)
{
	//�J�����̐ݒ菈��
	SetCamera();

	//�v���C���[�̉e�̕`�揈��
	DrawShadow();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y���̕`�揈��
		DrawPause();
	}

}
//===========================
//�Q�[����ʂ̏�Ԃ̐ݒ菈��
//===========================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//=======================
//�Q�[����ʂ̎擾����
//=======================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//=====================
//�|�[�Y�̗L����������
//=====================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}