//=====================================================================================================================
//
// �`���[�g���A��3d [tutorial3e.cpp]
// Author: TEAM_C
//
//=====================================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
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
#include "edit.h"
#include "minimap.h"
#include "edit2d.h"
#include "meshsword.h"
#include "Effect.h"
#include "Particle.h"
#include "item.h"

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
bool g_bEditMode2;		// �ҏW���[�h���ǂ���

//==============================================================================================================
//�`���[�g���A��3d�̏���������
//==============================================================================================================
void InitTutorial3d(void)
{
	// �J�[�\���𖳌���
	SetCursorVisibility(false);

	// �J�����̏���������
	InitCamera();
	
	// �e�̏���������
	InitShadow();

	// ���̋O�Ղ̏���������
	InitMeshSword();

	// ���b�V���t�B�[���h�̏���������
	InitMeshField();

	// �ǂ̏���������
	InitWall();

	// �Q�[�W�̏�����
	InitGauge();

	// UI�̏���������
	InitGameUI();

	// �~�j�}�b�v�̏���������
	InitMiniMap();

	// �|���S���̏���������
	InitPolygon();

	// �v���C���[�̏���������
	InitPlayer();

	// �e�̏���������
	InitShadow();

	// �u���b�N�̏���������
	InitBlock();

	// �A�C�e���̏���������
	InitItem();

	// �����̏���������
	InitExplosion();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�`����
	InitParticle();

	// �X�e�[�W�̓ǂݍ���
	LoadEdit();
	LoadEdit2d();

	//// �X�e�[�W��ǂݍ���
	//tutoload();

	// �u���b�N���Z�b�g
	// TODO : �����̔z�u�����C��---------------------
	SetBlock(D3DXVECTOR3(-160.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,D3DX_PI * 0.5f,0.0f), 19, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3( -60.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 20, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3( 40.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 21, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetBlock(D3DXVECTOR3(140.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 47, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// �A�C�e�����Z�b�g
	SetItem(D3DXVECTOR3(40.0f, 0.0f, -90.0f), 29, D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	//-------------------------------------------------
	
	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(640.0f, 40.0f, 0.0f), 4, 600.0f, 40.0f, 0);

	// �O���[�o���ϐ��̏�����
	g_bEditMode2 = false;

	// ���y���Đ�
	PlaySound(SOUND_LABEL_TUTORIAL_BGM);
}
//==============================================================================================================
//�`���[�g���A��3d�̏I������
//==============================================================================================================
void UninitTutorial3d(void)
{
	// ���y���~
	StopSound();

	// �J�����̏I������
	UninitCamera();

	// �e�̏I������
	UninitShadow();

	// �|���S���̏I������
	UninitPolygon();

	// ���̋O�Ղ̏I������
	UninitMeshSword();

	// ���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// �Q�[�W�̏I��
	UninitGauge();

	// UI�̏I��
	UninitGameUI();

	// �ǂ̏I������
	UninitWall();

	// �~�j�}�b�v�̏I������
	UninitMinMap();

	// �v���C���[�̏I������
	UninitPlayer();

	// �u���b�N�̏I������
	UninitBlock();

	// �A�C�e���̏I������
	UninitItem();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	//// �G�f�B�^�[��ʂ̏I������
	//UninitEdit();
}
//==============================================================================================================
//�`���[�g���A��3d�̍X�V����
//==============================================================================================================
void UpdateTutorial3d(void)
{
	// �J�����̍X�V����
	UpdateCamera();

	// �e�̍X�V����
	UpdateShadow();

	// �ǂ̍X�V����
	UpdateWall();

	// UI�̍X�V����
	UpdateGameUI();

	// �u���b�N�̍X�V����
	UpdateBlock();

	// �A�C�e���̍X�V����
	UpdateItem();

	// �����̍X�V����
	UpdateExplosion();

	// ���̋O�Ղ̍X�V����
	UpdateMeshSword();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	if ((KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_START)==true))
	{//Enter�L�[ or Start�{�^���������ꂽ
		//�Q�[����ʂ�
		SetFade(MODE_GAME);

		// ���y�Đ�
		PlaySound(SOUND_LABEL_ENTER_SE);

	}
}
//==============================================================================================================
//�`���[�g���A��3d�̕`�揈��
//==============================================================================================================
void DrawTutorial3d(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// �ǂ̕`��
	DrawWall();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �|���S���̕`�揈��
	DrawPolygon();

	// �e�̕`�揈��
	DrawShadow();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �����̕`�揈��
	DrawExplosion();

	// ���̋O�Ղ̕`�揈��
	DrawMeshSword();

	// �G�t�F�N�g�̕`�揈��
	//DrawEffect();

	// UI�̕`�揈��
	DrawGameUI();

	if (g_bEditMode2)
	{// g_bEditMode2��true
		//DrawEdit();
	}
}
//==============================================================================================================
//�ҏW���[�h�̎擾����
//==============================================================================================================
bool GetEditStatetuto(void)
{
	return g_bEditMode2;
}
