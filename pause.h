//============================
//
// �|�[�Y [pause.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _PAUSE_H_
#define _PAUSE_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�|�[�Y���j���[
//****************************
typedef enum
{
	PAUSE_MENU_CONTINUE=0,//�Q�[���ɖ߂�
	PAUSE_MENU_RETRY,//�Q�[������蒼��
	PAUSE_MENU_QUIT,//�^�C�g����ʂɖ߂�
	PAUSE_MENU_MAX
}PAUSE_MENU;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitPause(void);//�|�[�Y�̏���������
void UninitPause(void);//�|�[�Y�̏I������
void UpdatePause(void);//�|�[�Y�̍X�V����
void DrawPause(void);//�|�[�Y�̕`�揈��
void SelectPause(int nNumSelect);//�|�[�Y�̑I������
#endif