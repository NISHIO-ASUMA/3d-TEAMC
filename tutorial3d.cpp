//============================
//
// �`���[�g���A��3d [tutorial3e.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "tutorial3d.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include"damagepop.h"

//****************************
//�O���[�o���ϐ�
//****************************
static int nPressTuto;
//============================
//�`���[�g���A��3d�̏���������
//============================
void InitTutorial3d(void)
{
	InitCamera();

	InitShadow();
}
//============================
//�`���[�g���A��3d�̏I������
//============================
void UninitTutorial3d(void)
{
	UninitCamera();

	UninitShadow();
}
//============================
//�`���[�g���A��3d�̍X�V����
//============================
void UpdateTutorial3d(void)
{
	UpdateCamera();

	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true)
	{
		SetFade(MODE_GAME);
	}
}
//============================
//�`���[�g���A��3d�̕`�揈��
//============================
void DrawTutorial3d(void)
{
	SetCamera();

	DrawShadow();
}
