//==============================================================================================================
//
// �^�C�g��3d [title3d.cpp]
// Author: TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
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
#include "minimap.h"
#include "polygon.h"
#include "edit2d.h"
#include "player.h"
#include "wall.h"

//==============================================================================================================
//�^�C�g��3d�̏���������
//==============================================================================================================
void InitTitle3d(void)
{
	// �J�[�\���𖳌���
	SetCursorVisibility(true);

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

	// �e�̏���������
	InitShadow();

	// �ǂ̏���������
	InitWall();

	// �u���b�N�̏���������
	InitBlock();
	
	// �~�j�}�b�v�̏���������
	InitMiniMap();

	// �v���C���[�̏���������
	InitPlayer();

	// �X�y�V�����Q�[�W�̏���������
	InitSPgauge();

	// �|���S��
	InitPolygon();

	// �^�C�g���̃J�����̂�����
	SetAnimation(0);

	// �^�C�g���p�̃X�e�[�W��ǂݍ��ޏ���
	LoadTitleState();
	LoadEdit2d();

	// �ǂ�ݒu����
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f), 0);

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(1200.0f, 200.0f, 0.0f), UITYPE_KATANA, 450.0f, 50.0f, 0); // �^�C�g���̓�
	SetGameUI(D3DXVECTOR3(640.0f, 200.0f, 0.0f), UITYPE_TITLE, 450.0f, 100.0f, 0); // �^�C�g�����S
	SetGameUI(D3DXVECTOR3(380.0f, 450.0f, 0.0f), UITYPE_TITLE2, 50.0f, 30.0f, 0);  // �I��p�̓�

	// �T�E���h���Đ�
	PlaySound(SOUND_LABEL_TITLE_BGM);
}
//==============================================================================================================
//�^�C�g��3d�̏I������
//==============================================================================================================
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

	// �ǂ̏I������
	UninitWall();

	// �e�̏I������
	UninitShadow();

	// �u���b�N�̏I������
	UninitBlock();

	// �X�y�V�����Q�[�W�̏I������
	UninitSPgauge();

	// �~�j�}�b�v�̏I������
	UninitMinMap();

	// �|���S��
	UninitPolygon();

	// �v���C���[�̏I������
	UninitPlayer();
}
//==============================================================================================================
//�^�C�g��3d�̍X�V����
//==============================================================================================================
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

	// �e�̍X�V����
	UpdateShadow();

	if (KeyboardTrigger(DIK_F2))
	{
		SetFade(MODE_EFFECT);
	}
}
//==============================================================================================================
//�^�C�g��3d�̕`�揈��
//==============================================================================================================
void DrawTitle3d(void)
{
	// �u���b�N�̕`�揈��
	DrawBlock();

	// ���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �ǂ̕`�揈��
	DrawWall();

	// �e�̕`�揈��
	DrawShadow();

	// �|���S��
	DrawPolygon();

	// �^�C�g���̕`�揈��
	DrawTitle();

	// �Q�[��UI�̕`�揈��
	DrawGameUI();
}