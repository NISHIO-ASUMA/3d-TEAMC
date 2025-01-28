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

//****************************
//�O���[�o���ϐ�
//****************************
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���
bool g_bEditMode = false; // �G�f�B�b�g���[�h���ǂ���

//=======================
//�Q�[����ʂ̏���������
//=======================
void InitGame(void)
{
	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

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
	InitTimer();

	//�G�f�B�b�g�̏���������
	InitEdit();

	//�G�f�B�b�g�̃��[�h����
	LoadEdit();

	SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 0.0f), ENEMYTYPE_ONE, 1000,1.0f);
	SetEnemy(D3DXVECTOR3(20.0f, 0.0f, 60.0f), ENEMYTYPE_TWO, 1000, 1.0f);
	SetEnemy(D3DXVECTOR3(100.0f, 0.0f, 280.0f), ENEMYTYPE_THREE, 1000, 1.0f);
	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 180.0f), ENEMYTYPE_FOUR, 1000,1.0f);
	SetEnemy(D3DXVECTOR3(300.0f, 0.0f, 280.0f), ENEMYTYPE_FIVE, 1000,1.0f);

	SetWall(D3DXVECTOR3(1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f,D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(5.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(-1000.0f, WALL_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 1.0f, D3DXVECTOR3(5.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, D3DXVECTOR3(5.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 1.0f, D3DXVECTOR3(5.0f, 1.0f, 1.0f));

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	g_bPause = false;//�|�[�Y����
	g_bEditMode = false;//�G�f�B�b�g���[�h����

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

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�e�̏I������
	UninitShadow();

	//�|�[�Y�̏I������
	UninitPause();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

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
	UninitTimer();

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

	//�G�f�B�b�g�̏I������
	UninitEdit();
}
//=======================
//�Q�[����ʂ̍X�V����
//=======================
void UpdateGame(void)
{
	// �G�̎擾
	int nNumEnemy = GetNumEnemy();

	// �^�C�}�[�̎擾
	int nTime = GetTimer();

	// TODO : �G���S�ł�����Q�[���I��

	if (KeyboardTrigger(DIK_O))
	{
		SetEnemy(D3DXVECTOR3((float)(rand() % 500 - 100), 0.0f, (float)(rand() % 500 - 100)),rand()%ENEMYTYPE_MAX,rand()%3 + 1,(float)(rand()%2 + 0.7f));
	}

	if (nNumEnemy <= 0 || nTime <= 0)
	{// �G���S�� or �^�C�}�[��0�b�ȉ�
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END://�I�����
		g_nCounterGameState++;

		if (g_nCounterGameState >= 60)
		{
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ����

			//���(���[�h)�̐ݒ�
			SetFade(MODE_RESULT);

			//�����L���O�̃��Z�b�g
			ResetRanking();

			//�����L���O�̃Z�b�g
			SetRanking(0);
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
	}
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y���̍X�V����
	}

	int nNumObj = GetNumobj(); // �I�u�W�F�N�g�̐����擾

	//�G�f�B�b�g���[�h��������
	if (KeyboardTrigger(DIK_F2) && g_bEditMode)
	{
		g_bEditMode = false;
		InitBlock(); // �o�Ă���I�u�W�F�N�g�̏�����
		InitItem();  // �o�Ă���I�u�W�F�N�g�̏�����
		LoadEdit();  // ���[�h
	}
	//�G�f�B�b�g���[�h����Ȃ�������
	else if (KeyboardTrigger(DIK_F2) && !g_bEditMode)
	{
		g_bEditMode = true;
	}

	if (g_bPause == false)
	{//�|�[�Y���łȂ����

		if (!g_bEditMode)
		{
			//�J�����̍X�V����
			UpdateCamera();

			//���C�g�̍X�V����
			UpdateLight();

			//�e�̍X�V����
			UpdateShadow();

			// ���̍X�V����
			UpdateExplosion();

			//�v���C���[�̍X�V����
			UpdatePlayer();

			// �_���[�W�̍X�V����
			UpdateDamege();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �p�[�e�B�N���̍X�V����
			UpdateParticle();

			//�G�̍X�V����
			UpdateEnemy();

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�A�C�e���̍X�V����
			UpdateItem();

			//HP�Q�[�W�̍X�V����
			UpdateGauge();

			// �^�C�}�[�̍X�V����
			UpdateTimer();

			//�ǂ̍X�V����
			UpdateWall();
		}
		else if (g_bEditMode)
		{
			//�G�f�B�b�g�̍X�V����
			UpdateEdit();

			UpdateCamera();
		}	
	}

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F10))
	{
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

	//�u���b�N�̕`�揈��
	DrawBlock();

	//�A�C�e���̕`�揈��
	DrawItem();

	//�ǂ̕`�揈��
	DrawWall();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	//HP�Q�[�W�̕`�ʏ���
    DrawGauge();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	// ���̕`�揈��
	DrawExplosion();

	// �_���[�W�̕`�揈��
	DrawDamege();

    //�v���C���[�̉e�̕`�揈��
    DrawShadow();


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
