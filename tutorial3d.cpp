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
#include "icon.h"
#include "itemgage.h"
#include "billboard.h"
#include "craftui.h"
#include "TutorialSupport.h"
#include"spgauge.h"

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

	// �N���t�g��ʂ̏���������
	InitCraftUI();

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

	// �A�C�R���̏�����
	InitIcon();

	// �A�C�e���Q�[�W
	InitItemGage();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �X�y�V�����Q�[�W�̏���������
	InitSPgauge();

	// �X�e�[�W�̓ǂݍ���
	LoadEdit();
	LoadEdit2d();

	// �A�C�e�����Z�b�g
	SetItem(D3DXVECTOR3(70.0f, 0.0f, 120.0f), 0); // �o�b�g
	SetItem(D3DXVECTOR3(-10.0f, 0.0f, 120.0f), 3); // ��

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(640.0f, 500.0f, 0.0f), UITYPE_TUTORIAL, 600.0f, 40.0f,false, 0);
	SetGameUI(D3DXVECTOR3(70.0f, 640.0f, 0.0f), UITYPE_ICONFRAME, 70.0f, 80.0f, false, 0);
	SetGameUI(D3DXVECTOR3(200.0f, 660.0f, 0.0f), UITYPE_ICONFRAMESTOCK, 60.0f, 60.0f, false, 0);

	// �ǂ�ݒu����
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f), 0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f), 0);

	// �X�g�b�N�A�C�e���̃A�C�R��
	SetIcon(D3DXVECTOR3(70.0f, 640.0f, 0.0f), 60.0f, 60.0f, 0, ICONTYPE_HOLDITEM);

	// �`���[�g���A���⏕
	InitManager();

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

	// �N���t�g��ʂ̏I������
	UninitCraftUI();

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

	// �A�C�R���̏I��
	UninitIcon();

	// �A�C�e���Q�[�W�̏I��
	UninitItemGage();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �`���[�g���A���⏕�̏I��
	UninitManager();

	// �X�y�V�����Q�[�W�̏I������
	UninitSPgauge();

	//// �G�f�B�^�[��ʂ̏I������
	//UninitEdit();
}
//==============================================================================================================
//�`���[�g���A��3d�̍X�V����
//==============================================================================================================
void UpdateTutorial3d(void)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->bCraft == false)
	{
		// �J�����̍X�V����
		UpdateCamera();

		// �e�̍X�V����
		UpdateShadow();

		// �ǂ̍X�V����
		UpdateWall();

		// UI�̍X�V����
		UpdateGameUI();

		// �����̍X�V����
		UpdateExplosion();

		// ���̋O�Ղ̍X�V����
		UpdateMeshSword();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �A�C�e���Q�[�W�̍X�V
		UpdateItemGage();

		// �A�C�R���̍X�V
		UpdateIcon();

		// �`���[�g���A���⏕�̍X�V
		UpdateManager();

		// �X�y�V�����Q�[�W�̍X�V����
		UpdateSPgauge();
	}
	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �N���t�g��ʂ̍X�V����
	UpdateCraftUI();

	// �u���b�N�̍X�V����
	UpdateBlock();

	// �A�C�e���̍X�V����
	UpdateItem();

	// �r���{�[�h�̍X�V
	UpdateBillboard();

	// HP�Q�[�W�̍X�V
	UpdateGauge();

	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_START)==true))
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

	// �r���{�[�h�̕`��
	DrawBillboard();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �����̕`�揈��
	DrawExplosion();

	// ���̋O�Ղ̕`�揈��
	DrawMeshSword();

	// �G�t�F�N�g�̕`�揈��
	//DrawEffect();

	// �X�y�V�����Q�[�W�̕`�揈��
	DrawSPgauge();

	// UI�̕`�揈��
	DrawGameUI();

	// �A�C�R���̕`�揈��
	DrawIcon();

	// �A�C�e���Q�[�W
	DrawItemGage();

	// �`���[�g���A���⏕�̕`��
	DrawManager();

	// HP�Q�[�W�̕`��
	DrawGauge();

	Player* pPlayer = GetPlayer();

	if (pPlayer->bCraft == true)
	{
		// �N���t�g��ʂ̕`�揈��
		DrawCraftUI();
	}

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
