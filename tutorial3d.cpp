//===================================
//
// �`���[�g���A��3d [tutorial3e.cpp]
// Author: TEAM_C
//
//===================================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "tutorial3d.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include"damagepop.h"
#include "meshfield.h"

//****************************
//�O���[�o���ϐ�
//****************************
static int nPressTuto;

//============================
//�`���[�g���A��3d�̏���������
//============================
void InitTutorial3d(void)
{
	//�J�����̏���������
	InitCamera();
	
	//�e�̏���������
	InitShadow();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();
}
//============================
//�`���[�g���A��3d�̏I������
//============================
void UninitTutorial3d(void)
{
	//�J�����̏I������
	UninitCamera();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();
}
//============================
//�`���[�g���A��3d�̍X�V����
//============================
void UpdateTutorial3d(void)
{
	//�J�����̍X�V����
	UpdateCamera();

	//�e�̍X�V����
	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true)
	{//Enter�L�[ or Start�{�^���������ꂽ
		//�Q�[����ʂ�
		SetFade(MODE_GAME);
	}
}
//============================
//�`���[�g���A��3d�̕`�揈��
//============================
void DrawTutorial3d(void)
{
	//�e�̕`�揈��
	DrawShadow();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();
}
