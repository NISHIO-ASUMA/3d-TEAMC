//============================
//
// timer [timer.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _TIME_H_
#define _TIME_H_

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
// �v���g�^�C�v�錾
//****************************
void InitTime(void);	 // �^�C�}�[�̏���������
void UninitTime(void);	 // �^�C�}�[�̏I������
void UpdateTime(void);	 // �^�C�}�[�̍X�V����
void DrawTime(void);	 // �^�C�}�[�̕`�揈��
int GetTimeMinute(void); // �^�C�}�[�̎擾����
int GetTimeSecond(void); // �^�C�}�[�̎擾����

#endif
