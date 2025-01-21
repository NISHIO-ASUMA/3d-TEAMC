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
#include "title.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "meshfield.h"
#include "gameui.h"

//****************************
//�O���[�o���錾
//****************************

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

	//�^�C�g���̏���������
	InitTitle();

	//�Q�[��UI�̏���������
	InitGameUI();

	//UI���Z�b�g
	SetGameUI(D3DXVECTOR3(640.0f, -200.0f, 0.0f), UITYPE_TITLE, 300.0f, 100.0f, 0);
	SetGameUI(D3DXVECTOR3(380.0f, 450.0f, 0.0f), UITYPE_TITLE2, 50.0f, 30.0f, 0);
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

	//�^�C�g���̏I������
	UninitTitle();

	//�Q�[��UI�̏I������
	UninitGameUI();
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

	//�^�C�g���̍X�V����
	UpdateTitle();

	//�Q�[��UI�̍X�V����
	UpdateGameUI();
}
//============================
//�^�C�g��3d�̕`�揈��
//============================
void DrawTitle3d(void)
{
	//�e�̕`�揈��
	DrawShadow();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�^�C�g���̕`�揈��
	DrawTitle();

	//�Q�[��UI�̕`�揈��
	DrawGameUI();
}