//============================
//
// �Q�[����� [game.h]
// Author: TEAM_C
//
//============================
#ifndef _GAME_H_
#define _GAME_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�Q�[�����
//****************************
typedef enum
{
	GAMESTATE_NONE=0,//�������Ă��Ȃ����
	GAMESTATE_NORMAL,//�ʏ���
	GAMESTATE_END,//�ʏ���
	GAMESTATE_MAX
}GAMESTATE;

//�v���g�^�C�v�錾
void InitGame(void);//�Q�[���̏���������
void UninitGame(void);//�Q�[���̏I������
void UpdateGame(void);//�Q�[���̍X�V����
void DrawGame(void);//�Q�[���̕`�揈��
void SetGameState(GAMESTATE state);//�Q�[����ʂ̏�Ԃ̐ݒ菈��
GAMESTATE GetGameState(void);//�Q�[����ʂ̎擾����
void SetEnablePause(bool bPause);//�|�[�Y�̗L����������
bool GetEditState(void); // �G�f�B�b�g���[�h�̎擾
#endif