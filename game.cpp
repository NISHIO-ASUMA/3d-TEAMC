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
#include "Timer.h"
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

	// ���b�V���h�[���̏���������
	InitmeshFan();

	// ���̏���������
	InitExplosion();

	// �~�j�}�b�v�̏���������
	InitMiniMap();

	//�G�̏���������
	InitEnemy();

	//�v���C���[�̏���������
	InitPlayer();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �p�[�e�B�N���̏���������
	InitParticle();

	// �_���[�W�̏���������
	InitDamege();

	// �X�R�A�̏���������
	InitScore();

	// �r���{�[�h�̏���������
	InitBillboard();

	//�u���b�N�̏���������
	InitBlock();

	//�A�C�e���̏���������
	InitItem();

	//�ǂ̏���������
	InitWall();

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

	//�G�f�B�b�g�̏���������
	InitEdit();

	//�G�f�B�b�g�̃��[�h����
	LoadEdit();
	LoadEdit2d();

	//�e�̏���������
	InitBullet();

	//WaveEnemy(0); // �G���o������
	//WaveEnemy(1); // �G���o������

	// UI���Z�b�g
	SetGameUI(D3DXVECTOR3(80.0f,550.0f,0.0f),UITYPE_ICONFRAME,80.0f,80.0f,0);

	SetIcon(D3DXVECTOR3(80.0f, 550.0f, 0.0f), 80.0f, 80.0f, 0);

	// �e�X�g�p : �@�r���{�[�h�̃Z�b�g
	//SetBillboard(D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.f, 0.0f), 0, 200.0f, 100.0f);

	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 200.0f), 5, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));

	SetBoss(D3DXVECTOR3(761.0f, 0.0f, 675.0f), 3.0f, 10000); // �{�X���Z�b�g

	//// �ǂ�ݒu����
	//SetWall(D3DXVECTOR3(1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(-1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	//SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));

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

	//�e�̏I������
	UninitShadow();

	//�|�[�Y�̏I������
	UninitPause();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	// ���b�V���h�[���̏I������
	UninitmeshFan();

	// �O�Ղ̏I������
	UninitMeshSword();

	// ���̏I������
	UninitExplosion();

	//�v���C���[�̏I������
	UninitPlayer();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

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

	// �~�j�}�b�v�̏I������
	UninitMinMap();

	//�G�f�B�b�g�̏I������
	UninitEdit();

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

	if (g_EnemyWaveTime >= 900)
	{
		int Spawn_randvalue = rand() % 100; // �o�邩�o�Ȃ���

		if (Spawn_randvalue <= 10)
		{
			int nSpawner = rand() % 2; // �ǂ�����o����

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
		}
	}
	// �G���o�Ă���܂ł̎���
	if (g_EnemyWaveTime >= 900 || nNumEnemy <= 0)
	{// �J�E���g��900 or ��ɏo�Ă���G��0�̈ȉ��̎�
		int nSpawner = rand() % 2;

		// �G���o������
		WaveEnemy(nSpawner);

		// �^�C��������������
		g_EnemyWaveTime = 0;
	}

	if (TimeMinute <= 0 && TimeSecond <= 0)
	{// �G���S�� or �^�C�}�[��0�b�ȉ�
		g_gameState = GAMESTATE_END;
	}

	if (!pPlayer->bDisp)
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

	if (g_bPause == false)
	{// �|�[�Y���łȂ����
		// �J�[�\���𖳌���
		SetCursorVisibility(false);

		if (!g_bEditMode)
		{// �ҏW���[�h����Ȃ�������

			if (!g_bCraft)
			{
				//�J�����̍X�V����
				UpdateCamera();

				//���C�g�̍X�V����
				UpdateLight();

				//�e�̍X�V����
				UpdateShadow();

				// ���̍X�V����
				UpdateExplosion();

				if (pPlayer->bDisp)
				{// bDisp��true
					//�v���C���[�̍X�V����
					UpdatePlayer();
				}

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

				//�e�̍X�V����
				UpdateBullet();

				// �~�j�}�b�v�̍X�V����
				UpdateMiniMap();

			}
			// �r���{�[�h�̍X�V����
			UpdateBillboard();

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�A�C�e���̍X�V����
			UpdateItem();
		}
		else if (g_bEditMode)
		{
			//�G�f�B�b�g�̍X�V����
			UpdateEdit();

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

	// �{�X�̕`�揈��
	DrawBoss();

	//�ǂ̕`�揈��
	DrawWall();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	//HP�Q�[�W�̕`�ʏ���
    DrawGauge();

	// �^�C�}�[�̕`�揈��
	DrawTime();

	// ���̕`�揈��
	DrawExplosion();

	// �O�Ղ̕`�揈��
	DrawMeshSword();

	// �_���[�W�̕`�揈��
	DrawDamege();

	// �X�R�A�̕`�揈��
	DrawScore();

	// SP�Q�[�W�̕`�揈��
	DrawSPgauge();

	// �A�C�R���̕`�揈��
	DrawIcon();

	// �e�̕`�揈��
	DrawShadow();

	// �Q�[����UI�̕`�揈��
	DrawGameUI();

	//�e�̕`�揈��
	DrawBullet();

	// �~�j�}�b�v�̕`�揈��
	DarwMinimap();

	if (g_bEditMode)
	{
		//�G�f�B�b�g�̕`�揈��
		DrawEdit();
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
