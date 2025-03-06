//==============================================================================================================
//
// �Q�[����� [game.cpp]
// Author: TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "RankingScore.h"
#include "damagepop.h"
#include "time.h"
#include "pause.h"
#include "meshfield.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "item.h"
#include "edit.h"
#include "wall.h"
#include "HPGauge.h"
#include "Effect.h"
#include "Particle.h"
#include "sound.h"
#include "explosion.h"
#include "Score.h"
#include "gameui.h"
#include "mouse.h"
#include "meshsword.h"
#include "spgauge.h"
#include "boss.h"
#include "icon.h"
#include "polygon.h"
#include "edit2d.h"
#include "meshfan.h"
#include "billboard.h"
#include "craftui.h"
#include "Bullet.h"
#include "minimap.h"
#include "effect2.h"
#include "particle2.h"
#include "meshimpact.h"
#include "itemgage.h"
#include "meshcylinder.h"
#include "mark.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define SPAWN_ENEMY (15) // �G�̃X�|�[����

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���
bool g_bEditMode = false; // �G�f�B�b�g���[�h���ǂ���
int g_EnemyWaveTime;
bool g_bCraft = false;

//=========================================================================================================
//�Q�[����ʂ̏���������
//=========================================================================================================
void InitGame(void)
{
	// �J�[�\���𖳌���
	SetCursorVisibility(false);

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	// �Q�[����UI�̏�����
	InitGameUI();

	//�e�̏���������
	InitShadow();

	//�|�[�Y�̏���������
	InitPause();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���C���p�N�g�̏���������
	InitMeshImpact();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	// ���b�V���h�[���̏���������
	InitmeshFan();

	// ���̏���������
	InitExplosion();

	// �~�j�}�b�v�̏���������
	InitMiniMap();

	//�G�̏���������
	InitEnemy();

	// ���̏���������
	InitMark();

	//�v���C���[�̏���������
	InitPlayer();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �G�t�F�N�g�̏���������
	InitEffectX();

	// �p�[�e�B�N���̏���������
	InitParticle();

	// �p�[�e�B�N���̏���������
	InitParticleX();

	// �_���[�W�̏���������
	InitDamege();

	// �X�R�A�̏���������
	InitScore();

	// �N���t�g��ʂ̏���������
	InitCraftUI();

	// �r���{�[�h�̏���������
	InitBillboard();

	//�u���b�N�̏���������
	InitBlock();

	//�A�C�e���̏���������
	InitItem();

	//�ǂ̏���������
	InitWall();
	
	// �A�C�e���Q�[�W�̏�����
	InitItemGage();

	//HP�Q�[�W�̏���������
	InitGauge();

	// �^�C�}�[�̏�����
	InitTime();

	// �O�Ղ̏���������
	InitMeshSword();

	// SP�Q�[�W�̏���������
	InitSPgauge();

	// �{�X�̏���������
	InitBoss();

	// �A�C�R���̏���������
	InitIcon();

	// �|���S���̏���������
	InitPolygon();

	//�e�̏���������
	InitBullet();

#ifdef _DEBUG

	//�G�f�B�b�g�̏���������
	InitEdit();

#endif // DEBUG

	//�G�f�B�b�g�̃��[�h����
	LoadEdit();
	LoadEdit2d();


	for (int nCntEnemy = 0; nCntEnemy < SPAWN_ENEMY; nCntEnemy++)
	{
		SpawnEnemy(2); // �G���o������
		SpawnEnemy(1); // �G���o������
	}

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(70.0f,610.0f,0.0f),UITYPE_ICONFRAME,70.0f, 80.0f,0);
	SetGameUI(D3DXVECTOR3(200.0f, 630.0f, 0.0f), UITYPE_ICONFRAMESTOCK, 60.0f, 60.0f, 0);
	SetGameUI(D3DXVECTOR3(95.0f, 115.0f, 0.0f), UITYPE_KEY, 90.0f, 50.0f, 0);

	// �X�g�b�N�A�C�e���̃A�C�R��
	SetIcon(D3DXVECTOR3(70.0f, 610.0f, 0.0f), 60.0f, 60.0f, 0,ICONTYPE_HOLDITEM);

	// �e�X�g�p : �@�r���{�[�h�̃Z�b�g
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

#ifdef _DEBUG

	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 6, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));

	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g
	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g

#endif // _DEBUG

	// �ǂ�ݒu����
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f),0);

	//// �ǂ�ݒu����
	//SetWall(D3DXVECTOR3(1600.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(-1650.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 4.0f, 1.0f), 1);
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1950.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 4.0f, 1.0f), 1);

	g_gameState = GAMESTATE_NORMAL; // �ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		// ��ʑJ�ڂ̎���

	g_bPause = false; // �|�[�Y����
	g_bEditMode = false;// �G�f�B�b�g���[�h����
	g_EnemyWaveTime = 0; // �G���o�Ă��鎞��
	g_bCraft = false;

	// ���y���Đ�
	PlaySound(SOUND_LABEL_GAME_BGM);
}
//=========================================================================================================
//�Q�[����ʂ̏I������
//=========================================================================================================
void UninitGame(void)
{
	// ���y���~
	StopSound();

	// �Q�[���̃X�R�A��ۑ�
	SaveScore();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	// �Q�[����UI�̏I������
	UninitGameUI();

	// �A�C�e���Q�[�W�̏I������
	UninitItemGage();

	//�e�̏I������
	UninitShadow();

	//�|�[�Y�̏I������
	UninitPause();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���C���p�N�g�̏I������
	UninitMeshImpact();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	// ���b�V���h�[���̏I������
	UninitmeshFan();

	// �O�Ղ̏I������
	UninitMeshSword();

	// �N���t�g��ʂ̏I������
	UninitCraftUI();

	// ���̏I������
	UninitExplosion();

	//�v���C���[�̏I������
	UninitPlayer();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �G�t�F�N�g�̏I������
	UninitEffectX();

	// �p�[�e�B�N���̏I������
	UninitParticleX();

	// �_���[�W�̏I������
	UninitDamege();
	
	// �^�C�}�[�̏I��
	UninitTime();

	// �X�R�A�̏I������
	UninitScore();

	//�G�̏I������
	UninitEnemy();

	//�u���b�N�̏I������
	UninitBlock();

	//�A�C�e���̏I������
	UninitItem();

	//�ǂ̏I������
	UninitWall();

	//HP�Q�[�W�̏I������
	UninitGauge();

	// SP�Q�[�W�̏I������
	UninitSPgauge();

	// �{�X�̏I������
	UninitBoss();

	// �A�C�R���̏I������
	UninitIcon();

	// �|���S���̏I������
	UninitPolygon();

	// ���̏I������
	UninitMark();

	// �~�j�}�b�v�̏I������
	UninitMinMap();

#ifdef _DEBUG

	//�G�f�B�b�g�̏I������
	UninitEdit();

#endif // DEBUG

	// �r���{�[�h�̏I������
	UninitBillboard();

	// �e�̏I������
	UninitBullet();
}
//=========================================================================================================
//�Q�[����ʂ̍X�V����
//=========================================================================================================
void UpdateGame(void)
{
	if (g_bCraft == false && g_bPause == false && g_bEditMode == false)
	{
		// �G�̏o�����Ԃ����Z����
		g_EnemyWaveTime++;
	}

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �G�̎擾
	int nNumEnemy = GetNumEnemy();

	//// �^�C�}�[�̎擾
	//int nTime = GetTimer();
	
	// �^�C�}�[�̎擾
	int TimeMinute = GetTimeMinute(); // ��
	int TimeSecond = GetTimeSecond(); // �b

	int nNumKill = GetNumKill(); // �L����

	//// �S�̈ʁA�\�̈ʁA��̈�
	//int HandredPlace = nNumKill / 100 % 10;
	//int tenPlace = nNumKill / 10 % 10;
	//int onePlace = nNumKill % 10;

	//static bool bSet = false;

	//int nSetSpawn = (HandredPlace * 100) + (tenPlace * 10) + onePlace;

	if (g_EnemyWaveTime >= 1800)
	{
		int Spawn_randvalue = rand() % 100; // �o�邩�o�Ȃ���
		int nSpawner = rand() % 3; // �ǂ�����o����

		switch (nSpawner)
		{
		case 0:
			SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g
			break;
		case 1:
			SetBoss(D3DXVECTOR3(-526.0f, 0.0f, -455.0f), 3.0f, 10000); // �{�X���Z�b�g
			break;
		case 2:
			SetBoss(D3DXVECTOR3(-506.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g
			break;
		default:
			break;
		}

		g_EnemyWaveTime = 0;
	}
	//else if ((nSetSpawn % 40 != 0))
	//{
	//	bSet = false;
	//}

	//// �G���o�Ă���܂ł̎���
	//if (g_EnemyWaveTime >= 900)
	//{// �J�E���g��900 or ��ɏo�Ă���G��0�̈ȉ��̎�

	//	int nSpawner = rand() % 4;

	//	for (int nCntEnemy = 0; nCntEnemy < SPAWN_ENEMY; nCntEnemy++)
	//	{
	//		// �G���o������
	//		Enemy(nSpawner);
	//	}

	//	// �^�C��������������
	//}

	if (TimeMinute <= 0 && TimeSecond <= 0)
	{// �G���S�� or �^�C�}�[��0�b�ȉ�
		g_gameState = GAMESTATE_END;
	}

	if (pPlayer->nLife <= 0)
	{// �v���C���[�����g�p����
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL: //�ʏ���
		break;

	case GAMESTATE_END: //�I�����
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;	 // �J�E���g��������
			g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ����

			//���(���[�h)�̐ݒ�
			SetFade(MODE_RESULT);

			//�����L���O�̃��Z�b�g
			ResetRanking();

			//�����L���O�̃Z�b�g
			SetRanking(GetScore());
		}

		break;
	}

	int nNumObj = GetNumobj(); // �I�u�W�F�N�g�̐����擾

	// �G�f�B�b�g���[�h��������
	if (KeyboardTrigger(DIK_F2) && g_bEditMode)
	{
		g_bEditMode = false;
		InitBlock();   // �o�Ă���I�u�W�F�N�g�̏�����
		InitItem();    // �o�Ă���I�u�W�F�N�g�̏�����
		InitPolygon(); // �|���S���̏�����
		LoadEdit();    // ���[�h
		LoadEdit2d();
	}
	// �G�f�B�b�g���[�h����Ȃ�������
	else if (KeyboardTrigger(DIK_F2) && !g_bEditMode)
	{
		g_bEditMode = true;
	}

	Camera* pCamera = GetCamera();

	if (g_bPause == false)
	{// �|�[�Y���łȂ����
		// �J�[�\���𖳌���
		SetCursorVisibility(false);

		if (!g_bEditMode && pCamera->bEditMode == false)
		{// �ҏW���[�h����Ȃ�������

			if (g_bCraft == false)
			{
				//�J�����̍X�V����
				UpdateCamera();

				//���C�g�̍X�V����
				UpdateLight();

				//�e�̍X�V����
				UpdateShadow();

				// �G�t�F�N�g�̍X�V����
				UpdateEffectX();

				// �p�[�e�B�N���̍X�V����
				UpdateParticleX();

				// ���̍X�V����
				UpdateExplosion();
				
				// �_���[�W�̍X�V����
				UpdateDamege();

				// �G�t�F�N�g�̍X�V����
				UpdateEffect();

				// �p�[�e�B�N���̍X�V����
				UpdateParticle();

				// �Q�[����UI�̍X�V����
				UpdateGameUI();

				// �X�R�A�̍X�V����
				UpdateScore();

				//�G�̍X�V����
				UpdateEnemy();

				//HP�Q�[�W�̍X�V����
				UpdateGauge();

				// �^�C�}�[�̍X�V����
				UpdateTime();

				//�ǂ̍X�V����
				UpdateWall();

				// �O�Ղ̍X�V����
				UpdateMeshSword();

				// SP�Q�[�W�̍X�V����
				UpdateSPgauge();

				// �{�X�̍X�V����
				UpdateBoss();

				// �A�C�R���̍X�V����
				UpdateIcon();

				// �|���S���̍X�V����
				UpdatePolygon();

				// ���b�V���h�[���̍X�V����
				UpdatemeshFan();

				//�X�V����
				UpdateBullet();

				// �~�j�}�b�v�̍X�V����
				UpdateMiniMap();

				// ���b�V���C���p�N�g�̍X�V����
				UpdateMeshImpact();

				// �A�C�e���Q�[�W�̍X�V����
				UpdateItemGage();

				//���b�V���V�����_�[�̍X�V����
				UpdateMeshCylinder();

				// ���̍X�V����
				UpdateMark();

				// �u���b�N�̍X�V����
				UpdateBlock();
			}

			//�v���C���[�̍X�V����
			UpdatePlayer();

			// �N���t�g��ʂ̍X�V����
			UpdateCraftUI();

			// �r���{�[�h�̍X�V����
			UpdateBillboard();

			//�A�C�e���̍X�V����
			UpdateItem();

		}
		else if (g_bEditMode == true || pCamera->bEditMode == true)
		{
#ifdef _DEBUG

			//�G�f�B�b�g�̏I������
			UpdateEdit();

#endif // DEBUG

			// �J�����̍X�V����
			UpdateCamera();
		}
	}
#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F10))
	{// F10��������
		g_gameState = GAMESTATE_END;
	}

#endif // _DEBUG

	if ((KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START) == true) && g_bCraft == false)
	{//�|�[�Y�L�[(P�L�[)�������ꂽ
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y���̍X�V����
		UpdatePause();

		// �J�[�\�����o��
		SetCursorVisibility(true);
	}
}
//=========================================================================================================
//�Q�[����ʂ̕`�揈��
//=========================================================================================================
void DrawGame(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	// ���b�V���h�[���̕`�揈��
	DrawmeshFan();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	// ���̕`�揈��
	DrawMark();

#ifdef _DEBUG

	if (g_bEditMode == true)
	{
		//�G�f�B�b�g�̕`�揈��
		DrawEdit();
	}

#endif // DEBUG

	if (!g_bEditMode)
	{
		// �r���{�[�h�̕`�揈��
		DrawBillboard();

		//�u���b�N�̕`�揈��
		DrawBlock();

		// �|���S���̕`�揈��
		DrawPolygon();

		//�A�C�e���̕`�揈��
		DrawItem();
	}

	//���b�V���C���p�N�g�̕`�揈��
	DrawMeshImpact();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// ���̕`�揈��
	DrawExplosion();

	// �G�t�F�N�g�̕`�揈��
	DrawEffectX();

	// �p�[�e�B�N���̕`�揈��
	DrawParticleX();

	// �_���[�W�̕`�揈��
	DrawDamege();

	// �e�̕`�揈��
	DrawShadow();

	// �A�C�R���̕`�揈��
	DrawIcon();

	//�ǂ̕`�揈��
	DrawWall();

	// �{�X�̕`�揈��
	DrawBoss();

	// �O�Ղ̕`�揈��
	DrawMeshSword();

	//���b�V���V�����_�[�̕`�揈��
	DrawMeshCylinder();

	//HP�Q�[�W�̕`�ʏ���
	DrawGauge();

	// SP�Q�[�W�̕`�揈��
	DrawSPgauge();

	// �Q�[����UI�̕`�揈��
	DrawGameUI();

	// �A�C�e���Q�[�W�̕`�揈��
	DrawItemGage();

	// �^�C�}�[�̕`�揈��
	DrawTime();

	//�e�̕`�揈��
	DrawBullet();

	// �~�j�}�b�v�̕`�揈��
	DarwMinimap();

	if (g_bCraft == true)
	{
		// �N���t�g��ʂ̕`�揈��
		DrawCraftUI();
	}

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y���̕`�揈��
		DrawPause();
	}
}
//=============================================================================================================
//�Q�[����ʂ̏�Ԃ̐ݒ菈��
//=============================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//=========================================================================================================
//�Q�[����ʂ̎擾����
//=========================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//=======================================================================================================
//�|�[�Y�̗L����������
//=======================================================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//==========================================================================================================
// �G�f�B�b�g���[�h�̎擾
//=======================================================================================================
bool GetEditState(void)
{
	return g_bEditMode;
}
//==========================================================================================================
// �N���t�g��Ԃ̐ݒ菈��
//=======================================================================================================
bool EnableCraft(bool bCraft)
{
	g_bCraft = bCraft;
	return g_bCraft;
}
