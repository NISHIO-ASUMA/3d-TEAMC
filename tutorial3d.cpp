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
#include "damagepop.h"
#include "meshfield.h"
#include "block.h"
#include "wall.h"
#include "sound.h"
#include "explosion.h"
#include "mouse.h"
#include "HPGauge.h"
#include "gameui.h"

//****************************
//�O���[�o���ϐ�
//****************************
static int nPressTuto;

//============================
//�`���[�g���A��3d�̏���������
//============================
void InitTutorial3d(void)
{
	SetCursorVisibility(false);

	//�J�����̏���������
	InitCamera();
	
	//�e�̏���������
	InitShadow();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �ǂ̏���������
	InitWall();

	// �Q�[�W�̏�����
	InitGauge();

	// UI�̏���������
	InitGameUI();

	// �v���C���[�̏���������
	InitPlayer();

	// �u���b�N�̏���������
	InitBlock();

	// �A�C�e���̏���������
	InitItem();

	// �����̏���������
	InitExplosion();

	// �u���b�N���Z�b�g
	SetBlock(D3DXVECTOR3(-160.0f, 20.0f, 0.0f), 19, D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	SetBlock(D3DXVECTOR3( -60.0f, 20.0f, 0.0f), 20, D3DXVECTOR3(1.5f, 1.5f, 1.5f)); 
	SetBlock(D3DXVECTOR3( 40.0f, 20.0f, 0.0f), 21, D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	// �A�C�e�����Z�b�g
	SetItem(D3DXVECTOR3(140.0f, -10.0f, 0.0f), 29, D3DXVECTOR3(1.5f, 1.5f, 1.5f));

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(640.0f, 40.0f, 0.0f), 4, 600.0f, 40.0f, 0);

	// ���y���Đ�
	PlaySound(SOUND_LABEL_TUTORIAL_BGM);
}
//============================
//�`���[�g���A��3d�̏I������
//============================
void UninitTutorial3d(void)
{
	// ���y���~
	StopSound();

	//�J�����̏I������
	UninitCamera();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// �Q�[�W�̏I��
	UninitGauge();

	// UI�̏I��
	UninitGameUI();

	// �ǂ̏I������
	UninitWall();

	// �v���C���[�̏I������
	UninitPlayer();

	// �u���b�N�̏I������
	UninitBlock();

	// �A�C�e���̏I������
	UninitItem();

	// �����̏I������
	UninitExplosion();
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

	// �ǂ̍X�V����
	UpdateWall();

	// UI�̍X�V����
	UpdateGameUI();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �u���b�N�̍X�V����
	UpdateBlock();

	// �A�C�e���̍X�V����
	UpdateItem();

	// �����̍X�V����
	UpdateExplosion();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true)
	{//Enter�L�[ or Start�{�^���������ꂽ
		//�Q�[����ʂ�
		SetFade(MODE_GAME);

		// ���y�Đ�
		PlaySound(SOUND_LABEL_ENTER_SE);

	}
}
//============================
//�`���[�g���A��3d�̕`�揈��
//============================
void DrawTutorial3d(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �ǂ̕`��
	DrawWall();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	//�e�̕`�揈��
	DrawShadow();

	// UI�̕`�揈��
	DrawGameUI();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �����̕`�揈��
	DrawExplosion();

}
