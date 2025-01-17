//============================
//
// �^�C�g��3d [title3d.cpp]
// Author: TEAM_C
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "title3d.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "meshfield.h"

//****************************
//�O���[�o���錾
//****************************
bool bFlash;

//============================
//�^�C�g��3d�̏���������
//============================
void InitTitle3d(void)
{
	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�e�̏���������
	InitShadow();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();
}
//============================
//�^�C�g��3d�̏I������
//============================
void UninitTitle3d(void)
{
	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

}
//============================
//�^�C�g��3d�̍X�V����
//============================
void UpdateTitle3d(void)
{
	UpdateCamera();

	UpdateLight();

	//�e�̍X�V����
	UpdateShadow();

	if (KeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TUTORIAL);
	}
}
//============================
//�^�C�g��3d�̕`�揈��
//============================
void DrawTitle3d(void)
{
	SetCamera();

	//�e�̕`�揈��
	DrawShadow();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();
}