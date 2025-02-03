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
#include "sound.h"
#include "mouse.h"
#include "block.h"
#include "HPGauge.h"
#include "spgauge.h"

//============================
//�^�C�g��3d�̏���������
//============================
void InitTitle3d(void)
{
	// �J�[�\���𖳌���
	SetCursorVisibility(false);

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	//�e�̏���������
	InitShadow();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �^�C�g���̏���������
	InitTitle();

	// �Q�[�W�̏���������
	InitGauge();

	// �Q�[��UI�̏���������
	InitGameUI();

	// �u���b�N�̏���������
	InitBlock();
	
	// �v���C���[�̏���������
	InitPlayer();

	// �X�y�V�����Q�[�W�̏���������
	InitSPgauge();

	// �^�C�g���p�̃X�e�[�W��ǂݍ��ޏ���
	LoadTitleState();

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(640.0f, -200.0f, 0.0f), UITYPE_TITLE, 450.0f, 150.0f, 0);
	SetGameUI(D3DXVECTOR3(380.0f, 450.0f, 0.0f), UITYPE_TITLE2, 50.0f, 30.0f, 0);

	// �T�E���h���Đ�
	PlaySound(SOUND_LABEL_TITLE_BGM);
}
//============================
//�^�C�g��3d�̏I������
//============================
void UninitTitle3d(void)
{
	// ���y���~
	StopSound();

	//�J�����̏I������
	UninitCamera();

	// �Q�[�W�̏I������
	UninitGauge();

	// ���C�g�̏I������
	UninitLight();

	// �e�̏I������
	UninitShadow();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// �^�C�g���̏I������
	UninitTitle();

	// �Q�[��UI�̏I������
	UninitGameUI();

	// �u���b�N�̏I������
	UninitBlock();

	// �X�y�V�����Q�[�W�̏I������
	UninitSPgauge();

	// �v���C���[�̏I������
	UninitPlayer();
}
//============================
//�^�C�g��3d�̍X�V����
//============================
void UpdateTitle3d(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �e�̍X�V����
	UpdateShadow();

	// �^�C�g���̍X�V����
	UpdateTitle();

	// �Q�[��UI�̍X�V����
	UpdateGameUI();
}
//============================
//�^�C�g��3d�̕`�揈��
//============================
void DrawTitle3d(void)
{
	// �u���b�N�̕`�揈��
	DrawBlock();

	// �e�̕`�揈��
	DrawShadow();

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �^�C�g���̕`�揈��
	DrawTitle();

	// �Q�[��UI�̕`�揈��
	DrawGameUI();
}