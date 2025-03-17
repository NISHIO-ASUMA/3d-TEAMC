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
#include "bosslife.h"
#include "event.h"
#include "count.h"
#include "craftrecipe.h"
#include "manual.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define SPAWN_ENEMY (10) // �G�̃X�|�[����

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateEventMovie(void); // �C�x���g�̃��[�r�[

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_NONE;// �Q�[���̏��
int g_nCounterGameState = 0;// ��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���
bool g_bEditMode = false; // �G�f�B�b�g���[�h���ǂ���
int g_MovieCnt = 0;
bool g_bCraft = false;
bool g_bMovie = false;

//=========================================================================================================
// �Q�[����ʂ̏���������
//=========================================================================================================
void InitGame(void)
{
	// �J�[�\���𖳌���
	SetCursorVisibility(false);

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �Q�[����UI�̏�����
	InitGameUI();

	// �e�̏���������
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

	// �{�X�̗̑̓Q�[�W�̏���������
	InitBossLife();

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

	// �N���t�g��ʂ̃��V�s�̏���������
	InitCraftRecipe();

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

	// �C�x���g�̏���������
	InitEvent();

	// �J�E���^�[�̏���������
	InitCounter();

	// �}�j���A���̏���������
	InitManual();

#ifdef _DEBUG

	//�G�f�B�b�g�̏���������
	InitEdit();

#endif // DEBUG

	//�G�f�B�b�g�̃��[�h����
	LoadEdit();
	LoadEdit2d();

	for (int nCntEnemy = 0; nCntEnemy < SPAWN_ENEMY; nCntEnemy++)
	{
		//SpawnEnemy(2); // �G���o������
		//SpawnEnemy(1); // �G���o������
	}

	//UpdateEnemySpawn();

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(70.0f,640.0f,0.0f),UITYPE_ICONFRAME,70.0f, 80.0f,false,0);
	SetGameUI(D3DXVECTOR3(200.0f, 660.0f, 0.0f), UITYPE_ICONFRAMESTOCK, 60.0f, 60.0f, false,0);
	SetGameUI(D3DXVECTOR3(60.0f, 120.0f, 0.0f), UITYPE_SP, 55.0f, 50.0f, false, 0);
	SetGameUI(D3DXVECTOR3(1100.0f, 210.0f, 0.0f), UITYPE_SETENEMYTIME, 40.0f, 25.0f, false, 0);

	// �z�[���h�A�C�e���̃A�C�R��
	SetIcon(D3DXVECTOR3(70.0f, 640.0f, 0.0f), 60.0f, 60.0f, 0,ICONTYPE_HOLDITEM);

	// �X�g�b�N�A�C�e���̃A�C�R��
	SetIcon(D3DXVECTOR3(200.0f, 670.0f, 0.0f), 40.0f, 40.0f, ITEMTYPE_KATANA, ICONTYPE_STOCKITEM);

	// �e�X�g�p : �@�r���{�[�h�̃Z�b�g
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

	// �e�X�g�p : �@�r���{�[�h�̃Z�b�g
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

#ifdef _DEBUG

	//SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 6, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));

	//SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g
	//SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b
#endif

	// �ǂ�ݒu����
	SetWall(D3DXVECTOR3(1500.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(-1550.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(19.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(16.0f, 1.0f, 1.0f),0);
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1850.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(15.0f, 1.0f, 1.0f),0);

	g_gameState = GAMESTATE_NORMAL; // �ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		// ��ʑJ�ڂ̎���

	g_bPause = false;   // �|�[�Y����
	g_bEditMode = false;// �G�f�B�b�g���[�h����
	g_MovieCnt = 0;     // �G���o�Ă��鎞��
	g_bCraft = false;   // �N���t�g��Ԃ̏�����
	g_bMovie = false;   // �J�������[�r�[��Ԃ̏�����

	// ���y���Đ�
	PlaySound(SOUND_LABEL_GAME_BGM);
}
//=========================================================================================================
// �Q�[����ʂ̏I������
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

	// �{�X�̗̑̓Q�[�W�I������
	UninitBossLife();

	// ���b�V���h�[���̏I������
	UninitmeshFan();

	// �O�Ղ̏I������
	UninitMeshSword();

	// �N���t�g��ʂ̏I������
	UninitCraftUI();

	// �N���t�g��ʂ̃��V�s�̏I������
	UninitCraftRecipe();

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

	// �G�̏I������
	UninitEnemy();

	// �u���b�N�̏I������
	UninitBlock();

	// �A�C�e���̏I������
	UninitItem();

	// �ǂ̏I������
	UninitWall();

	// HP�Q�[�W�̏I������
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

	// �G�f�B�b�g�̏I������
	UninitEdit();

#endif // DEBUG

	// �r���{�[�h�̏I������
	UninitBillboard();

	// �e�̏I������
	UninitBullet();

	// �J�E���^�[�̏I������
	UninitCounter();

	// �}�j���A���̏I������
	UninitManual();
}
//=========================================================================================================
//�Q�[����ʂ̍X�V����
//=========================================================================================================
void UpdateGame(void)
{
	// �}�j���A�����[�h��������֐��𔲂���
	if (UpdateManual() == true)
	{
		UpdateGameUI();
		return;
	}

	if (g_bMovie == true && g_bPause == false)
	{// ���[�r�[��Ԃ���Ȃ� ���� �|�[�Y������Ȃ�
		g_gameState = GAMESTATE_MOVIE; // ���[�r�[��Ԃɂ���
		UpdateEventMovie();            // �C�x���g�̃��[�r�[�X�V

		g_MovieCnt--;    // �J�E���g���f�N�������g����

		if (g_MovieCnt <= 0)
		{
			// �T�E���h���~�߂�
			StopSound(SOUND_LABEL_EVENTSE);
			StopSound(SOUND_LABEL_ENEMYPOP_SE);

			g_gameState = GAMESTATE_NORMAL; // �ʏ�̃Q�[����Ԃ�
			g_bMovie = false;               // ���[�r�[�t���O��false�ɂ���
		}
	}

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �G�̎擾
	int nNumEnemy = GetNumEnemy();
	
	// �^�C�}�[�̎擾
	int TimeMinute = GetTimeMinute(); // ��
	int TimeSecond = GetTimeSecond(); // �b


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
	case GAMESTATE_NORMAL: // �ʏ���
		break;

	case GAMESTATE_END: // �I�����
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;	 // �J�E���g��������
			g_gameState = GAMESTATE_NONE;// �������Ă��Ȃ����

			// ���(���[�h)�̐ݒ�
			SetFade(MODE_RESULT);

			// �����L���O�̃��Z�b�g
			ResetRanking();

			// �����L���O�̃Z�b�g
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

	// �J�������̎擾
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

				// �G�̍X�V����
				UpdateEnemy();

				// HP�Q�[�W�̍X�V����
				UpdateGauge();

				// �^�C�}�[�̍X�V����
				UpdateTime();

				// �ǂ̍X�V����
				UpdateWall();

				// �O�Ղ̍X�V����
				UpdateMeshSword();

				// SP�Q�[�W�̍X�V����
				UpdateSPgauge();

				// �{�X�̍X�V����
				UpdateBoss();

				// �|���S���̍X�V����
				UpdatePolygon();

				// ���b�V���h�[���̍X�V����
				UpdatemeshFan();

				// �e�̍X�V����
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

				// �C�x���g�̍X�V����
				UpdateEvent();

				// �J�E���^�[�̍X�V����
				UpdateCounter();
			}

			// �A�C�R���̍X�V����
			UpdateIcon();

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
		// �Q�[�����I����Ԃɂ���
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

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �G�̕`�揈��
	DrawEnemy();

	// ���̕`�揈��
	DrawMark();

	// �{�X�̕`�揈��
	DrawBoss();

#ifdef _DEBUG

	if (g_bEditMode == true)
	{
		//�G�f�B�b�g�̕`�揈��
		DrawEdit();
	}

#endif // DEBUG

	if (!g_bEditMode)
	{
		// �|���S���̕`�揈��
		DrawPolygon();

		//�A�C�e���̕`�揈��
		DrawItem();

		// �u���b�N�̕`�揈��
		DrawBlock();
	}

	//���b�V���C���p�N�g�̕`�揈��
	DrawMeshImpact();

	// ���̕`�揈��
	DrawExplosion();

	// �e�̕`�揈��
	DrawShadow();

	// �ǂ̕`�揈��
	DrawWall();

	// �{�X�̗̑̓Q�[�W�`�揈��
	DrawBossLife();

	// ���b�V���V�����_�[�̕`�揈��
	DrawMeshCylinder();

	// �_���[�W�̕`�揈��
	DrawDamege();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �G�t�F�N�g�̕`�揈��
	DrawEffectX();

	// �p�[�e�B�N���̕`�揈��
	DrawParticleX();

	// �r���{�[�h�̕`�揈��
	DrawBillboard();

	// �O�Ղ̕`�揈��
	DrawMeshSword();

	// �Q�[����UI�̕`�揈��
	DrawGameUI();

	// HP�Q�[�W�̕`�ʏ���
	DrawGauge();

	// SP�Q�[�W�̕`�揈��
	DrawSPgauge();

	// �e�̕`�揈��
	DrawBullet();

	// �A�C�e���Q�[�W�̕`�揈��
	DrawItemGage();

	// �^�C�}�[�̕`�揈��
	DrawTime();

	// �J�E���^�[�̕`�揈��
	DrawCounter();

	// �~�j�}�b�v�̕`�揈��
	DarwMinimap();

	if (g_bCraft == true)
	{
		// �N���t�g��ʂ̕`�揈��
		DrawCraftUI();
	}

	// �A�C�R���̕`�揈��
	DrawIcon();

	// �}�j���A���̕`�揈��
	DrawManual();

	if (g_bPause == true)
	{// �|�[�Y��
		// �|�[�Y���̕`�揈��
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
//==========================================================================================================
// ���[�r�[�̗L����������
//=======================================================================================================
void EnableMovie(bool bMovie)
{
	g_bMovie = bMovie;
}
//==========================================================================================================
 // ���[�r�[�̐ݒ菈��
 //=======================================================================================================
void SetMovie(int nTime)
{
	g_MovieCnt = nTime;
}
//==========================================================================================================
// �C�x���g�̃��[�r�[
//=======================================================================================================
void UpdateEventMovie(void)
{
	// �C�x���g�J�����̍X�V
	UpdateEventCamera();
}