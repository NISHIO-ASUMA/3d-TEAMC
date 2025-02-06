//============================
//
// �Q�[����� [game.cpp]
// Author: TEAM_C
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
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

//****************************
//�O���[�o���ϐ�
//****************************
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���
bool g_bEditMode = false; // �G�f�B�b�g���[�h���ǂ���
int g_EnemyWaveTime;

//=======================
//�Q�[����ʂ̏���������
//=======================
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

	// ���̏���������
	InitExplosion();

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

	//�G�̏���������
	InitEnemy();

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

	WaveEnemy(0); // �G���o������
	WaveEnemy(1); // �G���o������

	// TODO : �e�X�g�p�ɃZ�b�g
	// �{�X���Z�b�g
	
	//SetGameUI(D3DXVECTOR3(640.0f,340.0f,0.0f),UITYPE_RED,1280.0f,720.0f,0);

	SetIcon(D3DXVECTOR3(80.0f, 550.0f, 0.0f), 80.0f, 80.0f, 0);

	// �ǂ�ݒu����
	SetWall(D3DXVECTOR3(1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(-1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(10.0f, 1.0f, 1.0f));

	g_gameState = GAMESTATE_NORMAL; // �ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		// ��ʑJ�ڂ̎���

	g_bPause = false; // �|�[�Y����
	g_bEditMode = false;// �G�f�B�b�g���[�h����
	g_EnemyWaveTime = 0; // �G���o�Ă��鎞��

	// ���y���Đ�
	PlaySound(SOUND_LABEL_GAME_BGM);
}
//=======================
//�Q�[����ʂ̏I������
//=======================
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

	//�G�f�B�b�g�̏I������
	UninitEdit();
}
//=======================
//�Q�[����ʂ̍X�V����
//=======================
void UpdateGame(void)
{
	// �G�̏o�����Ԃ����Z����
	g_EnemyWaveTime++;

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �G�̎擾
	int nNumEnemy = GetNumEnemy();

	//// �^�C�}�[�̎擾
	//int nTime = GetTimer();
	
	// �^�C�}�[�̎擾
	int TimeMinute = GetTimeMinute(); // ��
	int TimeSecond = GetTimeSecond(); // �b

	if (g_EnemyWaveTime >= 900 || nNumEnemy <= 0)
	{
		int Spawn_randvalue = rand() % 100; // �o�邩�o�Ȃ���

		if (Spawn_randvalue <= 20)
		{
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

	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START) == true)
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
		InitBlock(); // �o�Ă���I�u�W�F�N�g�̏�����
		InitItem();  // �o�Ă���I�u�W�F�N�g�̏�����
		InitPolygon(); // �|���S���̏�����
		LoadEdit();  // ���[�h
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

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�A�C�e���̍X�V����
			UpdateItem();

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
//=======================
//�Q�[����ʂ̕`�揈��
//=======================
void DrawGame(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	if (!g_bEditMode)
	{
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

	// �e�̕`�揈��
	DrawShadow();

	// �A�C�R���̕`�揈��
	DrawIcon();

	// �Q�[����UI�̕`�揈��
	DrawGameUI();

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
//===========================
//�Q�[����ʂ̏�Ԃ̐ݒ菈��
//===========================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;

	g_nCounterGameState = 0;
}
//=======================
//�Q�[����ʂ̎擾����
//=======================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//=====================
//�|�[�Y�̗L����������
//=====================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//========================
// �G�f�B�b�g���[�h�̎擾
//=====================
bool GetEditState(void)
{
	return g_bEditMode;
}
