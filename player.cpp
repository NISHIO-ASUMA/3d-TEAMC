//===============================================================================================================
//
// �v���C���[[player.cpp]
// Author:YOSHIDA YUUTO
//
//===============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "player.h"
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "meshfield.h"
#include "block.h"
#include "enemy.h"
#include "wall.h"
#include "mouse.h"
#include "Shadow.h"
#include "Effect.h"
#include "Particle.h"
#include "HPGauge.h"
#include "explosion.h"
#include "gameui.h"
#include "sound.h"
#include "meshsword.h"
#include "spgauge.h"
#include "boss.h"
#include "Bullet.h"
#include "minimap.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)			// �ő啶����
#define PLAYERLIFE (1000)		// �v���C���[�̗̑�
#define MAX_TEXPLAYER (128)		// �e�N�X�`���̍ő吔
#define MAX_JUMP (15.0f)		// �W�����v��
#define MAX_MOVE (1.0f)			// �v���C���[�̈ړ���
#define NUM_MTX (8)				// ���̓����蔻��̃}�g���N�X�̐�
#define LANDINGEXPLOSION (6)	// ���n�����Ƃ��ɏo�鉌

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate); // �v���C���[�̃R���{����
void LoadMotion(int Weponmotion);																 // ���[�V�����̃��[�h����
void StickPad(void);																			 // �p�b�h�̈ړ�����

void LoadPlayer(int nType);                                                                      // �v���C���[�̃��[�h����
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType);							 // �v���C���[�̃��f���̃��[�h����
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);					 // �v���C���[�̃p�[�c�̐ݒ菈��
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);						 // �v���C���[�̃��[�V�����̃��[�h����
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);							 // �v���C���[�̃��[�V�����̃L�[�̓ǂݍ��ݏ���

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
//LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//�v���C���[�̃e�N�X�`���ւ̃|�C���^
Player g_player;//�v���C���[�\����
Player g_LoadPlayer[PLAYERTYPE_MAX]; // �v���C���[�̃��f����ۑ����Ă����ϐ�
MOTION g_LoadMotion[MOTION_MAX];   // ���[�V�����̏���ۑ����Ă����ϐ�
int g_nCounterState,g_AttackState; // ��ԃJ�E���^�[
bool bNohand; // �������������ĂȂ���
bool bUsePad;
//bool bFirstChange;
int nCntMotion,nKey;

//===============================================================================================================
//�v���C���[�̏���������
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	MODE mode = GetMode();//���݂̃��[�h���擾

	//�v���C���[�̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   // ���W
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �p�x
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړI�̊p�x
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   // �ړ���
	g_player.bJump = false;								   // �W�����v�����ۂ�
	g_player.bDisp = true;								   // �g�p���
	g_player.bMove = false;								   // 
	g_player.nLife = PLAYERLIFE;						   // �̗�
	g_player.state = PLAYERSTATE_NORMAL;				   // ���
	g_player.Motion.bLoopMotion = true;					   // 
	g_player.Swordrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	   // ���̊p�x
	g_player.Motion.nKey = 0;							   // �L�[��
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;	   // ���[�V�����̎��
	g_player.SwordOffpos.x = 0.0f;						   // ���̃I�t�Z�b�g�̍��Wx
	g_player.SwordOffpos.y = 85.0f;						   // ���̃I�t�Z�b�g�̍��Wy
	g_player.SwordOffpos.z = 0.0f;						   // ���̃I�t�Z�b�g�̍��Wz
	g_player.nCounterAction = 0;						   // �A�N�V�����J�E���g
	g_nCounterState = 0;                                   // ��ԃJ�E���^�[
	g_AttackState = 0;									   // �U����Ԃ̃J�E���^�[
	bNohand = false;									   // ���𓊂����������ĂȂ���
	g_player.speed = 1.0f;								   // ���̑���
	g_player.nDamage = 100;								   // �U����
	bUsePad = false;
	//bFirstChange = false;
	g_player.nStockDamage = 100;
	g_player.fStockSpeed = 3.5f;
	g_player.FeverMode = false;
	g_player.SpMode = false;
	g_player.WeponMotion = MOTION_KATANA;
	g_player.AttackSp = false;
	g_player.bLandingOBB = false;
	nCntMotion = 0;
	nKey = 0;
	g_player.bCraft = false;
	// TODO : �����̏����l����

	// �^�C�g���Ń��[�h������Əd���Ȃ�̂�
	if (mode != MODE_TITLE)
	{
		LoadMotion(0);
		LoadMotion(1);
		LoadMotion(2);
		LoadMotion(3);
		LoadMotion(4);
		LoadMotion(5);
		LoadMotion(6);
		LoadMotion(7);
		LoadMotion(8);
		LoadMotion(9);

		// �؂�ւ�郂�[�V�����̐�����
		for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
		{
		}

		g_LoadPlayer[0].nIdxShadow = SetShadow(g_player.pos, g_player.rot, 20.0f, 1.0f);

		// �^�C�v����
		g_LoadPlayer[0].PlayerType = PLAYERTYPE_NOHAND;
		g_LoadPlayer[1].PlayerType = PLAYERTYPE_WEPON;

		D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

		for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
		{
			//�K�v�ȏ���ݒ�
			//LoadModel(nCntPlayer);
			LoadPlayer(nCntPlayer);

			g_LoadPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_LoadPlayer[nCntPlayer].nLife = PLAYERLIFE;
			g_LoadPlayer[nCntPlayer].nMaxLife = PLAYERLIFE;
			g_LoadPlayer[nCntPlayer].bDisp = true;
			g_LoadPlayer[nCntPlayer].Motion.motionType = MOTIONTYPE_NEUTRAL;
			g_LoadPlayer[nCntPlayer].Motion.bLoopMotion = true;
			g_LoadPlayer[nCntPlayer].bJumpAttack = false;
			g_LoadPlayer[nCntPlayer].HandState = PLAYERHOLD_NO;
			g_LoadPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;
			g_LoadPlayer[nCntPlayer].Combostate = COMBO_ATTACK1;
			g_LoadPlayer[nCntPlayer].speed = 1.0f;
			g_LoadPlayer[nCntPlayer].nDamage = 100;
			g_LoadPlayer[nCntPlayer].SwordOffpos.x = 0.0f;						   // ���̃I�t�Z�b�g�̍��Wx
			g_LoadPlayer[nCntPlayer].SwordOffpos.y = 85.0f;						   // ���̃I�t�Z�b�g�̍��Wy
			g_LoadPlayer[nCntPlayer].SwordOffpos.z = 0.0f;						   // ���̃I�t�Z�b�g�̍��Wz
			g_LoadPlayer[nCntPlayer].WeponMotion = MOTION_KATANA;				   // ���̃I�t�Z�b�g�̍��Wz
			g_LoadPlayer[nCntPlayer].nStockDamage = 100;
			g_LoadPlayer[nCntPlayer].fStockSpeed = 3.5f;
			g_LoadPlayer[nCntPlayer].FeverMode = false;
			g_LoadPlayer[nCntPlayer].SpMode = false;
			g_LoadPlayer[nCntPlayer].AttackSp = false;
			g_LoadPlayer[nCntPlayer].bLandingOBB = false;
			g_LoadPlayer[nCntPlayer].bCraft = false;

			// �A�C�e������
			for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
			{
				g_LoadPlayer[nCntPlayer].Itembreak[nCnt] = false; // �A�C�e������ꂽ��
			}
		
		}

		g_LoadPlayer[0].nIdxMap = SetMiniMap(g_player.pos, MINIMAPTEX_PLAYER);

		//�ŏ��ɕ`�悵�����v���C���[�̏�����
		g_player = g_LoadPlayer[0];
	}

	/*Itemchange(30);
	MotionChange(MOTION_KATANA, 0);
	StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);*/
}
//===============================================================================================================
//�v���C���[�̏I������
//===============================================================================================================
void UninitPlayer(void)
{
	// ���y���~
	StopSound();

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//�e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
				{
					g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat]->Release();
					g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			//���b�V���̔j��
			if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh != NULL)
			{
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->Release();
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat->Release();
				 g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

	// �e�N�X�`���̔j��
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_player.Motion.aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_player.Motion.aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_player.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_player.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_player.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//===============================================================================================================
//�v���C���[�̍X�V����
//===============================================================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();
	Item* pItem = GetItem();

		/*StickPad();*/

	// �t�B�[�o�[���[�h�Ȃ�
	if (g_player.FeverMode)
	{
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	StickPad(); // �p�b�h�̈ړ�����

	// �p�b�h���g���Ă��Ȃ����U�����[�V��������Ȃ�
	if (!bUsePad &&
		g_player.Motion.motionType != MOTIONTYPE_ACTION&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION2&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION3&&
		g_player.Motion.motionType != MOTIONTYPE_ACTION4)
	{
		if (GetKeyboardPress(DIK_A))
		{
			//�v���C���[�̈ړ�(��)
			if (GetKeyboardPress(DIK_W) == true)
			{
				//g_player.Motion.motionType = MOTIONTYPE_MOVE;

				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
			}
			//�v���C���[�̈ړ�(��)
			else if (GetKeyboardPress(DIK_S))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
			}
			//�v���C���[�̈ړ�(��)
			else
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.z += sinf(pCamera->rot.y) * g_player.speed;
				g_player.move.x -= cosf(pCamera->rot.y) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
			}
		}
		//�v���C���[�̈ړ�(�E)
		else if (GetKeyboardPress(DIK_D))
		{
			//�v���C���[�̈ړ�(��)
			if (GetKeyboardPress(DIK_W))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
			}
			//�v���C���[�̈ړ�(��)
			else if (GetKeyboardPress(DIK_S))
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;
				g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
			}
			//�v���C���[�̈ړ�(�E)
			else
			{
				if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
				{
					g_player.Motion.motionType = MOTIONTYPE_MOVE;
				}

				g_player.move.z -= sinf(pCamera->rot.y) * g_player.speed;
				g_player.move.x += cosf(pCamera->rot.y) * g_player.speed;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
			}

		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_W) == true)
		{
			if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
			{
				g_player.Motion.motionType = MOTIONTYPE_MOVE;
			}

			g_player.move.x += sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
		if (g_player.Motion.motionType != MOTIONTYPE_JUMP)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;
		}

			g_player.move.x -= sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.z -= cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y;
		}
		else
		{
			if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
			{
				SetMotion(&g_player.Motion,MOTIONTYPE_NEUTRAL,MOTIONTYPE_NEUTRAL,true,40); // ���[�V�������j���[�g�����ɂ���
			}
		}
	}

	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_ACTION:
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	case MOTIONTYPE_ACTION2:
		break;
	case MOTIONTYPE_ACTION3:
		break;
	case MOTIONTYPE_ACTION4:
		break;
	default:
		break;
	}

	D3DXVECTOR3 SwordPos(
		g_player.SwordMtx._41, // X����
		g_player.SwordMtx._42, // Y����
		g_player.SwordMtx._43  // Z����
	);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_MOVE:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // ��Ԃ��m�[�}���ɂ���
		}
		break;
	case PLAYERSTATE_ATTACK:
		g_nCounterState--;

		// ���킪�Ȃ��Ƃ�
		if (g_player.Motion.nNumModel != 15)
		{
			if (g_player.nElement == WEPONELEMENT_STANDARD)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 1.0f, 2, 30, 10, 10.0f, 5.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_BLOOD)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2.0f, 2, 30, 10, 10.0f, 5.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_FIRE)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_FREEZE)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 2, 60, 10, 1.0f, 5.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_SPARK)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1.0f, 2, 15, 30, 30.0f, 5.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_AQUA)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 4.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
			else if (g_player.nElement == WEPONELEMENT_DARK)
			{
				SetParticle(SwordPos, D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 4.0f, 2, 45, 10, 6.0f, 5.0f, false, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
		}

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // ��Ԃ��m�[�}���ɂ���
		}
		break;
	case PLAYERSTATE_JUMP:
		g_nCounterState--;
		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // ��Ԃ��m�[�}���ɂ���
		}
		break;
	case PLAYERSTATE_LANDING:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // ��Ԃ��m�[�}���ɂ���
		}
		break;
	case PLAYERSTATE_DAMAGE:
		g_nCounterState--;

		if (g_nCounterState < 0)
		{
			g_player.state = PLAYERSTATE_NORMAL; // ��Ԃ��m�[�}���ɂ���
		}
		break;
	default:
		break;
	}

	switch (g_player.Combostate)
	{
	case COMBO_NO:
		break;
	case COMBO_ATTACK1:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // �R���{��Ԃ�������
		}
		break;
	case COMBO_ATTACK2:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // �R���{��Ԃ�������
		}
		break;
	case COMBO_ATTACK3:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // �R���{��Ԃ�������
		}
		break;
	case COMBO_ATTACK4:
		g_AttackState--;
		if (g_AttackState < 0)
		{
			g_player.Combostate = COMBO_NO; // �R���{��Ԃ�������
		}
		break;
	default:
		break;
	}

	//�ړ��ʂ�����
	g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

	//�O��̈ʒu��ۑ�
	g_player.posOld = g_player.pos;

	//�v���C���[�̈ʒu�̍X�V
	g_player.pos += g_player.move;

	//if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.Size))
	//{
	//	// ���[�V�������W�����v��������
	//	if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
	//	{
	//		// ���n�̂Ƃ��ɏo������
	//		for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
	//		{
	//			// �p�x�����߂�
	//			float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

	//			// �����o��
	//			SetExplosion(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//				60, 15.0f, 15.0f, EXPLOSION_LANDING);
	//		}
	//		SetMotion(&g_player.Motion, MOTIONTYPE_LANDING, MOTIONTYPE_NEUTRAL, true, 10); // ���[�V�����𒅒n�ɂ���
	//	}
	//	g_player.bJump = true; // �W�����v���\�ɂ���
	//}

	if (CollisionField())
	{
		g_player.bLandingOBB = false;
		// ���[�V�������W�����v��������
		if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
		{
			// ���n�̂Ƃ��ɏo������
			for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
			{
				// �p�x�����߂�
				float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

				// �����o��
				SetExplosion(D3DXVECTOR3(g_player.pos.x,0.0f,g_player.pos.z),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
					60,15.0f,15.0f, EXPLOSION_LANDING);
				g_player.pos.y = 0.0f;
			}
			SetMotion(&g_player.Motion, MOTIONTYPE_LANDING, MOTIONTYPE_NEUTRAL, true, 10); // ���[�V�����𒅒n�ɂ���
		}
		g_player.bJump = true; // �W�����v���\�ɂ���
	}
	else
	{
		g_player.bJump = false;
	}

	CollisionWall();

	//�v���C���[�̏d��
	g_player.move.y -= 1.0f;

	// �e�̌v�Z
	SetPositionShadow(g_player.nIdxShadow,g_player.pos,30.0f + 30.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	SetMiniMapPotision(g_player.nIdxMap, &g_player.pos);

	////�ǂ̏Փ˔���
	//CollisionWall();

	if (JoypadTrigger(JOYKEY_A) || KeyboardTrigger(DIK_SPACE))
	{//a�{�^�� or Enter�L�[�������ꂽ

		// ���y�Đ�
		PlaySound(SOUND_LABEL_JUMP_SE);

		if (g_player.bJump == true && g_player.Motion.motionType != MOTIONTYPE_LANDING && g_player.AttackSp == false)
		{
			g_player.bJump = false;						 // �W�����v���ł��Ȃ�����
			g_player.Motion.nKey = 0;					 // �L�[��0����n�߂�
			g_player.Motion.nCountMotion = 0;            // ���[�V�����J�E���g��0����n�߂�
			g_player.Motion.motionType = MOTIONTYPE_JUMP;// ���[�V�����^�C�v���W�����v�ɂ���
			g_player.move.y = 15.0f;					 // �Еz��		
		}
	}

	//SetEffect(D3DXVECTOR3(g_player.Motion.aModel[5].mtxWorld._41, g_player.Motion.aModel[5].mtxWorld._42, g_player.Motion.aModel[5].mtxWorld._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 20.0f);

	// �v���C���[�̏�Ԃ��U������Ȃ����n�ʂɂ���
	if (g_player.bDisp && !bNohand && !g_player.AttackSp)
	{
		if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X))&&g_player.Combostate == COMBO_NO && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION, 40, 40, COMBO_ATTACK1); // �R���{1
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK1 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 40, 40, COMBO_ATTACK2); // �R���{2
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK2 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 40, 40, COMBO_ATTACK3); // �R���{3
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_ATTACK3 && g_AttackState <= 30)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 45, 40, COMBO_ATTACK4); // �R���{4
		}
	}

	// �������������Ă���Ƃ��̍U��
	if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Combostate == COMBO_NO && bNohand && !g_player.AttackSp)
	{
		PlayerComb(MOTIONTYPE_ACTION, 40, 20, COMBO_ATTACK1); // �R���{1
	}

	// ���[�V�����̎�ނ�����
	if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
	{
		// �L��1�Ԗڂ��J�E���g��5
		if (g_player.Motion.nKey == 1 && g_player.Motion.nCountMotion == 5)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[14].mtxWorld._41, g_player.Motion.aModel[14].mtxWorld._42, g_player.Motion.aModel[14].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
		// �L��3�Ԗڂ��J�E���g��5
		else if (g_player.Motion.nKey == 3 && g_player.Motion.nCountMotion == 5)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[11].mtxWorld._41, g_player.Motion.aModel[11].mtxWorld._42, g_player.Motion.aModel[11].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
	}

	//�v���C���[�̊p�x�̐��K��
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// �t�H�[�o�[���[�h
	if (g_player.FeverMode)
	{
		g_player.speed = g_player.fStockSpeed * 1.8f;
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			g_player.Motion.nCountMotion++;
		}
		g_player.nDamage = g_player.nStockDamage * 1.5f;
	}
	else
	{
		// �X�e�[�^�X�����Ƃɖ߂�
		g_player.speed = g_player.fStockSpeed;
		g_player.nDamage = g_player.nStockDamage;
	}

	// ����������Ă��邩�v���C���[�̎����Ă���A�C�e������ꂽ
	if (g_player.Motion.nNumModel == 16 && g_player.Itembreak[g_player.ItemIdx])
	{
		// ���[�V�������j���[�g�����ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, MOTIONTYPE_NEUTRAL, true, 40); // ���[�V�������j���[�g�����ɂ���

		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);

		// �f��̎��̃��[�V����������
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // ���f���̏�����
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;

		// ��Ԃ�ύX
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;

	}

	// �����Ă���A�C�e����u������
	if (g_player.Motion.nNumModel == 16 && (KeyboardTrigger(DIK_G) || JoypadTrigger(JOYKEY_Y)))
	{
		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);

		// �f��̎��̃��[�V����������
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // ���f���̏�����
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;

		SetItem(g_player.pos, pItem[g_player.ItemIdx].nType, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;
	}	

	// ��^���탂�[�V�����̎�
	if (g_player.WeponMotion == MOTION_BIGWEPON && g_player.Motion.motionType == MOTIONTYPE_ACTION4 && g_player.Motion.nKey == 0 && GetKeyboardPress(DIK_W))
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 30.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 30.0f;
	}

	static int FiverCnt = 0; // �񐔐���

	if (g_player.FeverMode && FiverCnt == 0 && !g_player.AttackSp)
	{
		SetGameUI(D3DXVECTOR3(620.0f, 360.0f, 0.0f), UITYPE_SYUTYUSEN, 660.0f, 380.0f, 0);
		SetGameUI(D3DXVECTOR3(640.0f, 650.0f, 0.0f), UITYPE_FIVER, 200.0f, 80.0f, 0);
		FiverCnt = 1; // �����񐔂𒴂���
	}
	if (!g_player.FeverMode)
	{
		FiverCnt = 0; // �����񐔂����Z�b�g
	}

	// �X�y�V�������[�h�ɂȂ������̍U��
	if ((KeyboardTrigger(DIK_Q) || JoypadTrigger(JOYKEY_LS) || JoypadTrigger(JOYKEY_RS)) && g_player.Motion.nNumModel == 16 && g_player.HandState != PLAYERHOLD_HOLD)
	{
		if (g_player.Combostate == COMBO_NO && // �U�����Ă��Ȃ�
			!g_player.AttackSp &&              // SP�Z�𔭓����Ă��Ȃ�
			g_player.SpMode)                   // SP�Q�[�W�����܂���
		{

			g_player.AttackSp = true;          // SP�Z�𔭓����Ă���

			switch (g_player.WeponMotion)
			{
			case MOTION_KATANA:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SP, 0);
				SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_BLACK, 640.0f, 380.0f, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // �R���{1
				break;
			case MOTION_BIGWEPON:
				g_player.SwordOffpos.y = 100.0f;
				MotionChange(MOTION_SPHAMMER, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // �R���{1
				break;
			case MOTION_DBHAND:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SPDOUBLE, 0);
				PlayerComb(MOTIONTYPE_ACTION, 240, 120, COMBO_ATTACK1); // �R���{1
				break;
			case MOTION_ONE_HAND:
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_ONEHANDBLOW, 0);
				PlayerComb(MOTIONTYPE_ACTION, 120, 120, COMBO_ATTACK1); // �R���{1
				break;
			case MOTION_PIERCING:
				g_player.SwordOffpos.y = 200.0f;
				MotionChange(MOTION_SPPIERCING, 0);
				PlayerComb(MOTIONTYPE_ACTION, 180, 120, COMBO_ATTACK1); // �R���{1
				break;
			default:
				break;
			}

			// ���y���Đ�
			PlaySound(SOUND_LABEL_SP_SE);

		}
	}

	// �X�y�V�������[�V�����𔭓�������
	if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.AttackSp)
	{
		// �p�[�e�B�N��
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// �X�y�V�������[�V����������Ƃɖ߂�
	if (g_player.AttackSp && g_player.Motion.nKey >= g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1)
	{
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, MOTIONTYPE_NEUTRAL, true, 40); // ���[�V�������j���[�g�����ɂ���
		g_player.SwordOffpos.y = 65.0f;		// ����̒�����߂�
		MotionChange(MOTION_DBHAND, 1);		// �f��ɖ߂�
		g_player.Motion.nNumModel = 15;		// ���������
		g_player.HandState = PLAYERHOLD_NO; // ���������Ă��Ȃ���Ԃɂ���
		g_player.AttackSp = false;
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;
	}

	if (g_player.WeponMotion == MOTION_SPHAMMER && g_player.AttackSp && g_player.Motion.nKey <= 15)
	{
		g_player.speed = 7.0f; // �X�s�[�h��ݒ�
		g_player.move.x += sinf(g_player.rot.y + D3DX_PI) * g_player.speed; // �ړ��ʂ����Z
		g_player.move.z += cosf(g_player.rot.y + D3DX_PI) * g_player.speed; // �ړ��ʂ����Z
		SetParticle(SwordPos,
			g_player.rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
			2.0f, 4, 30, 60, 4.0f, 0.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (g_player.WeponMotion == MOTION_SPPIERCING &&		// ����sp���[�V������������
		g_player.AttackSp && g_player.Motion.nKey == 21 &&  // ���[�V�����̃L�[��21�Ԗ�
		g_player.Motion.nCountMotion == 1)                  // ���[�V�����̃t���[����1�Ԗ�
	{
		SetParticle(g_player.pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(1.57f, 3.14f, 1.57f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			6.0f, 4, 80, 500, 6.0f, 30.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}	//���[�V�����̍X�V

	// �A�C�e���̃X�g�b�N
	if (g_player.PlayerType == PLAYERTYPE_NOHAND && g_player.HandState == PLAYERHOLD_HOLD && KeyboardTrigger(DIK_F))
	{
		pItem[g_player.ItemIdx].state = ITEMSTATE_STOCK;

		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);

		// �f��̎��̃��[�V����������
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // ���f���̏�����
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
		}

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;
	}

	UpdateMotion(&g_player.Motion);

	//�v���C���[�̌�����ړI�̌����ɋ߂Â���
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//===============================================================================================================
// �v���C���[�̕`�揈��
//===============================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxSize,mtxShadow;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	int nCnt = 0;

	if (g_player.bDisp == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer, &g_player.mtxWorldPlayer, &mtxTrans);

		//D3DXMatrixShadow(&mtxShadow, &Light, &plane);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorldPlayer);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//�v�Z�p
			D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

			//�p�[�c�̃}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_player.Motion.aModel[nCntModel].mtxWorld);

			//�p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.Motion.aModel[nCntModel].rot.y, g_player.Motion.aModel[nCntModel].rot.x, g_player.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//�p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_player.Motion.aModel[nCntModel].pos.x, g_player.Motion.aModel[nCntModel].pos.y, g_player.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld, &g_player.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//�p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_player.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//�e���f��������ꍇ
				mtxParent = g_player.Motion.aModel[g_player.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//�e���f�����Ȃ��ꍇ
				mtxParent = g_player.mtxWorldPlayer;
			}

			//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_player.Motion.aModel[nCntModel].mtxWorld,
				&g_player.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//���������e

			//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_player.Motion.aModel[nCntModel].mtxWorld);


			for (int nCntMat = 0; nCntMat < (int)g_player.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_player.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				D3DXMATERIAL color;

				if (g_player.state != PLAYERSTATE_DAMAGE)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_player.state == PLAYERSTATE_DAMAGE)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&color.MatD3D);
				}

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_player.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_player.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			nCnt++;
			if (nCnt == 15)
			{
				SetMtxPos(); // ���̃��[���h�}�g���b�N�X��ݒ�
			}
		}
	}
	//�}�e���A���̐ݒ�
	pDevice->SetMaterial(&matDef);
}
//===============================================================================================================
//�v���C���[�̎擾����
//===============================================================================================================
Player* GetPlayer(void)
{
	return &g_player;
}
//===============================================================================================================
////���[���h�}�g���b�N�X�̃I�t�Z�b�g�ݒ菈��
//===============================================================================================================
void SetMtxPos(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxParent;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.SwordMtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.Motion.aModel[15].rot.y,
		g_player.Motion.aModel[15].rot.x,
		g_player.Motion.aModel[15].rot.z);

	D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx,&mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.SwordOffpos.x,g_player.SwordOffpos.y,g_player.SwordOffpos.z);
	D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx, &mtxTrans);

	mtxParent = g_player.Motion.aModel[15].mtxWorld;

	//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
	D3DXMatrixMultiply(&g_player.SwordMtx,
		&g_player.SwordMtx,
		&mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.SwordMtx);
}
//===============================================================================================================
// �v���C���[�ƓG�̔���
//===============================================================================================================
void HitPlayer(int nDamage)
{
	if (!g_player.AttackSp)
	{
		g_player.nLife -= nDamage;

		if (g_player.nLife <= 0)
		{
			g_player.state = PLAYERSTATE_FALL;
			g_player.bDisp = false;
			KillShadow(g_player.nIdxShadow); // �e������
		}
		else
		{
			g_nCounterState = 30;
			g_player.state = PLAYERSTATE_DAMAGE;
		}
	}
}
//===============================================================================================================
// �v���C���[�̃X�e�B�b�N����
//===============================================================================================================
void StickPad(void)
{
	XINPUT_STATE* pStick;

	pStick = GetJoyStickAngle();

	Camera* pCamera = GetCamera();

	if (GetJoyStick() == true && g_player.Combostate == COMBO_NO)
	{
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		float deadzone = 10920;
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		if (magnitude > deadzone)
		{
			bUsePad = true;
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			g_player.move.x += moveX * g_player.speed;
			g_player.move.z += moveZ * g_player.speed;

			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);
			
			if (bUsePad)
			{
				g_player.Motion.motionType = MOTIONTYPE_MOVE;
			}
			
		}
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;
		}
	}
}
//===============================================================================================================
// �v���C���[�̃X�e�[�^�X�ύX
//===============================================================================================================
void StatusChange(float speed, D3DXVECTOR3 SwordOffpos, int nDamage)
{
	g_player.speed = speed; // ���̑���
	g_player.fStockSpeed = speed; // ���̑����̌v�Z�p

	// ���̓����蔻��̍L��
	g_player.SwordOffpos.x = SwordOffpos.x; 
	g_player.SwordOffpos.y = SwordOffpos.y;
	g_player.SwordOffpos.z = SwordOffpos.z;

	g_player.nDamage = nDamage; // �v���C���[�̍U����
	g_player.nStockDamage = nDamage; // �v���C���[�̍U���͌v�Z�p

}
//===============================================================================================================
// �v���C���[�̌��ƓG�̓����蔻��
//===============================================================================================================
void HitSowrd(ENEMY* pEnemy,int nCntEnemy)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	D3DXVECTOR3 mtxDis,SwordPos;

	// �v���C���[������������Ă��邩�X�y�V�����U������Ȃ�
	if (g_player.Motion.nNumModel == 16 && !g_player.AttackSp)
	{
		//���̒��������߂�
		mtxDis.x = (g_player.SwordMtx._41 - g_player.Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (g_player.SwordMtx._42 - g_player.Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (g_player.SwordMtx._43 - g_player.Motion.aModel[15].mtxWorld._43);

		// �}�g���N�X�̐���������
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// ���̈ʒu��S�ċ��߂�
			SwordPos.x = g_player.Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = g_player.Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = g_player.Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // �����Z�o�p

			DisPos.x = pEnemy->pos.x - SwordPos.x; // ����X�����߂�
			DisPos.y = pEnemy->pos.y - SwordPos.y; // ����Y�����߂�
			DisPos.z = pEnemy->pos.z - SwordPos.z; // ����Z�����߂�

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // ���������߂�

			float Radius1, Radius2; // ���a

			Radius1 = 15.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // ���a�����߂�

			fRadius = (fRadius * fRadius); // ���a�����߂�

			// �͈͓��ɓ�����
			if (fDistance <= fRadius && pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO)
			{
				// �G�ɓ�������
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 5));

				// �ϋv�G�~�����炷
				pItem[g_player.ItemIdx].durability--;

				// �A�C�e���̑ϋv�͂�0�ɂȂ�����
				if (pItem[g_player.ItemIdx].durability <= 0)
				{
					// 0�ɂȂ����A�C�e��������
					g_player.Itembreak[g_player.ItemIdx] = true;
				}
				break;
			}
		}
	}
	// �f��̎����U�����X�y�V��������Ȃ�
	else if(g_player.Motion.nNumModel == 15 && !g_player.AttackSp)
	{
		// ���f���̈ʒu��ϐ��ɑ��
		D3DXVECTOR3 ModelPos(g_player.Motion.aModel[4].mtxWorld._41, g_player.Motion.aModel[4].mtxWorld._42, g_player.Motion.aModel[4].mtxWorld._43);

		// �~�͈̔�
		if (sphererange(&ModelPos, &pEnemy->pos, 30.0f, 65.0f) && g_player.Combostate != COMBO_NO && pEnemy->state!=ENEMYSTATE_DAMAGE)
		{
			if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.Motion.nKey >= 2)
			{
				HitEnemy(nCntEnemy, g_player.nDamage * 3); // �G�ɓ�������
			}
		}
	}
	// ���������Ă��邩�X�y�V�����U����
	else if (g_player.Motion.nNumModel == 16 && g_player.AttackSp)
	{
		//���̒��������߂�
		mtxDis.x = (g_player.SwordMtx._41 - g_player.Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (g_player.SwordMtx._42 - g_player.Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (g_player.SwordMtx._43 - g_player.Motion.aModel[15].mtxWorld._43);

		// �}�g���N�X�̐���������
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// ���̈ʒu��S�ċ��߂�
			SwordPos.x = g_player.Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = g_player.Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = g_player.Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // �����Z�o�p

			DisPos.x = pEnemy->pos.x - SwordPos.x; // ����X�����߂�
			DisPos.y = pEnemy->pos.y - SwordPos.y; // ����Y�����߂�
			DisPos.z = pEnemy->pos.z - SwordPos.z; // ����Z�����߂�

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // ���������߂�

			float Radius1, Radius2; // ���a

			Radius1 = 200.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // ���a�����߂�

			fRadius = (fRadius * fRadius); // ���a�����߂�

			// �͈͓��ɂ�����
			if (fDistance <= fRadius &&g_player.WeponMotion != MOTION_SPPIERCING && g_player.WeponMotion != MOTION_SPDOUBLE&&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 3)
			{
				// �G�Ƀ_���[�W��^����
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}
			// �͈͓��ɂ�����
			if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPPIERCING &&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 19)
			{
				// �G�Ƀ_���[�W��^����
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}

			// �͈͓��ɂ�����
			if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPDOUBLE &&
				pEnemy->state != ENEMYSTATE_DAMAGE && g_player.Combostate != COMBO_NO && g_player.Motion.nKey >= 6)
			{
				// �G�Ƀ_���[�W��^����
				HitEnemy(nCntEnemy, (pPlayer->nDamage * 50));
				break;
			}
		}

	}
}
//===============================================================================================================
// ���𓊂���
//===============================================================================================================
void ThrowItem(void)
{
	Item* pItem = GetItem();
	ENEMY* pEnemy = GetEnemy();

	int nIdx = g_player.ItemIdx; // ��Ɏ����Ă���A�C�e���̃C���f�b�N�X������
	int nIdxEnemy = 0;

	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;
	float fDistance = 0.0f;

	bool bFirst = true;

	// ���˒n�_��ݒ�
	pItem[nIdx].pos.x = g_player.pos.x;
	pItem[nIdx].pos.y = g_player.Motion.aModel[2].pos.y;
	pItem[nIdx].pos.z = g_player.pos.z;

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (pEnemy[nCnt].bUse)
		{
			// ���������߂�
			float DisposX = pEnemy[nCnt].pos.x - g_player.pos.x;
			float DisposY = pEnemy[nCnt].pos.y - g_player.pos.y;
			float DisposZ = pEnemy[nCnt].pos.z - g_player.pos.z;

			// ���������߂�
			fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

			// �ŏ������ʂ�
			if (bFirst)
			{
				fDistanceStock = fDistanceNow; // �X�g�b�N�ɍŏ��̒l�����Ă���
				bFirst = false; // �ŏ������ʂ������Ȃ��̂�false
				nIdxEnemy = nCnt; // �C���f�b�N�X��ۑ�
			}
			else
			{
				// ���̋������X�g�b�N���ꂽ������菬����������
				if (fDistanceNow < fDistanceStock)
				{
					fDistanceStock = fDistanceNow; // ������ۑ�
					nIdxEnemy = nCnt; // �߂��G�̃C���f�b�N�X��ۑ�
				}
			}

			if (sphererange(&g_player.pos, &pEnemy[nCnt].pos, 50.0f, 300.0f) && pEnemy[nCnt].nType == ENEMYTYPE_SIX)
			{
				nIdxEnemy = nCnt;
				break;
			}
		}
	}

	Boss* pBoss = Getboss();

	// �S�{�X����
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos,50.0f, 300.0f) && pEnemy[nIdxEnemy].nType == ENEMYTYPE_SIX)
		{
			D3DXVECTOR3 dest = pEnemy[nIdxEnemy].pos - g_player.pos; // �߂��G�̕��������߂�
			D3DXVec3Normalize(&dest, &dest); // ���K������

			// ��΂�������ݒ�
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.y = dest.y * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // �g�p��Ԃ�true�ɂ���

				// �v���C���[�̌�������ԋ߂��G�̏ꏊ�ɂ���
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			break;
		}
		// �{�X���͈͓��ɂ�����
		else if (sphererange(&g_player.pos, &pBoss[nCntBoss].pos, 50.0f, 150.0f) && pBoss[nCntBoss].bUse)
		{
			D3DXVECTOR3 dest = pBoss[nCntBoss].pos - g_player.pos; // �߂��{�X�̕��������߂�
			D3DXVec3Normalize(&dest, &dest); // ���K������

			// ��΂�������ݒ�
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // �g�p��Ԃ�true�ɂ���

				// �v���C���[�̌�������ԋ߂��{�X�̏ꏊ�ɂ���
			float fAngle = atan2f(pBoss[nCntBoss].pos.x - g_player.pos.x, pBoss[nCntBoss].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;

			break;
		}
		// �G���͈͓��ɂ�����
		else if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos, 50.0f, 300.0f))
		{
			D3DXVECTOR3 dest = pEnemy[nIdxEnemy].pos - g_player.pos; // �߂��G�̕��������߂�
			D3DXVec3Normalize(&dest, &dest); // ���K������

			// ��΂�������ݒ�
			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.z = dest.z * 10.0f;
			pItem[nIdx].bUse = true; // �g�p��Ԃ�true�ɂ���

				// �v���C���[�̌�������ԋ߂��G�̏ꏊ�ɂ���
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
			g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			break;

		}
		// �͈͓��ɒN�����Ȃ�������
		else
		{
			// ��΂�������ݒ�
			pItem[nIdx].move.x = sinf(g_player.rot.y + D3DX_PI) * 10.0f;
			pItem[nIdx].move.z = cosf(g_player.rot.y + D3DX_PI) * 10.0f;
			pItem[nIdx].bUse = true; // �g�p��Ԃ�true�ɂ���
		}
		
		
	}

	// �f��̎��̃��[�V����������
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].Motion.aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].Motion.aMotionInfo[nCntMotion];
	}

	// ��������ɕ��������
	g_player.Motion.nNumModel -= 1;

	// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
	g_player.HandState = PLAYERHOLD_NO;

	// ��Ԃ𓊂����Ă��Ԃɂ���
	pItem[nIdx].state = ITEMSTATE_THROW;

	g_player.speed = 3.0f; // �v���C���[�̈ړ����x�����Z�b�g
	g_player.fStockSpeed = 3.0f; // �v���C���[�̈ړ����x��ۑ�
}
//===============================================================================================================
// �v���C���[�ƓG�̓����蔻��
//===============================================================================================================
void CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove, float PLradius, float ENradius)
{
	D3DXVECTOR3 DisPos; // �v�Z�p

	// �������v�Z
	DisPos.x = g_player.pos.x - pPos->x;
	DisPos.y = g_player.pos.y - pPos->y;
	DisPos.z = g_player.pos.z - pPos->z;

	// ���������߂�
	float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

	// ���a���v�Z
	float fRadius = PLradius + ENradius;

	// ���a�����߂�
	fRadius = fRadius * fRadius;

	// �͈͓��ɓ�����
	if (fDistance <= fRadius && g_player.state != PLAYERSTATE_DAMAGE)
	{
		// ���y�Đ�
		PlaySound(SOUND_LABEL_DAMAGE_SE);

		// �G��߂�
		pMove->x -= DisPos.x * 0.1f;
		pMove->z -= DisPos.z * 0.1f;
		HitPlayer(50);
	}
}
//===============================================================================================================
// �~�̔���
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // �~�̓����蔻��
{
	bool bRange = false;

	D3DXVECTOR3 DisPos; // �v�Z�p

	// ����XYZ�����߂�
	DisPos.x = pPos1->x - pPos2->x;
	DisPos.y = pPos1->y - pPos2->y;
	DisPos.z = pPos1->z - pPos2->z;

	// ���������߂�
	float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

	// ���a�����߂�
	float fRadius = radius1 + radius2;

	// ���a�����߂�
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}

//===============================================================================================================
// �v���C���[�ƃA�C�e���̔���
//===============================================================================================================
bool CollisionItem(int nIdx, float Itemrange, float plrange)
{
	Item* pItem = GetItem();

	bool bCollision = false; // �������Ă��邩�ǂ���

	float fDistanceX = g_player.pos.x - pItem[nIdx].pos.x; // ����X���v�Z
	float fDistanceY = g_player.pos.y - pItem[nIdx].pos.y; // ����Y���v�Z
	float fDistanceZ = g_player.pos.z - pItem[nIdx].pos.z; // ����Z���v�Z

	//�������Z�o
	float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

	// ���a���v�Z
	float Radius = Itemrange + plrange;

	// ���a���Z�o
	Radius = Radius * Radius;

	// �͈͓��ɓ�����
	if (fDistance <= Radius)
	{
		bCollision = true;

		if ((KeyboardTrigger(DIK_E) || JoypadTrigger(JOYKEY_RIGHT_B) || JoypadTrigger(JOYKEY_LEFT_B) || OnMouseTriggerDown(RIGHT_MOUSE)) && g_player.Combostate == COMBO_NO)
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;
			// ���y�Đ�
			PlaySound(SOUND_LABEL_ITEM_SE);

			Itemchange(pItem[nIdx].nType); // �A�C�e�����E��

			pItem[nIdx].bUse = false;      // ����

			pItem[nIdx].state = ITEMSTATE_HOLD;

			g_player.ItemIdx = nIdx;	   // �C���f�b�N�X��n��
			//g_player.nOldItem = pItem[nIdx].nType;	   // �C���f�b�N�X��n��

			switch (pItem[nIdx].nType)
			{
			case ITEMTYPE_BAT:
				MotionChange(MOTION_DBHAND, 0); // �A�C�e���ɂ��������[�V�����^�C�v������(�f��̏ꍇ�͈���2��1������)
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 70);
				break;
			case ITEMTYPE_GOLF:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 60);
				break;
			case ITEMTYPE_HUNMER:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 80);
				break;
			case ITEMTYPE_STONE:
				MotionChange(MOTION_BIGWEPON, 1);
				StatusChange(3.5f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_WOOD:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 60);
				break;
			case ITEMTYPE_STONEBAT:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
				break;
			case ITEMTYPE_LIGHT:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.7f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_LIGHTWOOD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_HARISEN:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_ICEBLOCK:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.4f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_ICEBLOCKSOWRD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);
				break;
			case ITEMTYPE_IRON:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
				break;
			case ITEMTYPE_IRONBAT:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 90);
				break;
			case ITEMTYPE_SURFBOARD:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TORCH:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_TORCHSWORD:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 100);
				break;
			case ITEMTYPE_BAR:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 90);
				break;
			case ITEMTYPE_HEADSTATUE:
				MotionChange(MOTION_BIGWEPON, 1);
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 65.0f, 0.0f), 50);
				break;
			case ITEMTYPE_HEADSTATUTORSO:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_MEGAPHONE:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 60);
				break;
			case ITEMTYPE_RUBBERCUP:
				MotionChange(MOTION_PIERCING, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TELEPHONEPOLE:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(2.5f, D3DXVECTOR3(0.0f, 150.0f, 0.0f), 120);
				break;
			case ITEMTYPE_TORSO:
				MotionChange(MOTION_DBHAND, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 70);
				break;
			case ITEMTYPE_FLUORESCENTLIGHTMEGAPHONE:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);
				break;
			case ITEMTYPE_BONESPEAR:
				MotionChange(MOTION_PIERCING, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 100);
				break;
			case ITEMTYPE_FISH:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(2.8f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 80);
				break;
			case ITEMTYPE_HEX:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(1.5f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 300);
				break;
			case ITEMTYPE_HEXMANDOLIN:
				MotionChange(MOTION_ONE_HAND, 0);
				StatusChange(2.5f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 100);
				break;
			case ITEMTYPE_SURFBOARDFISH:
				MotionChange(MOTION_BIGWEPON, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 85.0f, 0.0f), 80);
				break;
			case ITEMTYPE_TUTORIAL:
				MotionChange(MOTION_DBHAND, 1);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);
				break;
			case ITEMTYPE_KATANA:
				MotionChange(MOTION_KATANA, 0);
				StatusChange(3.0f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 100);
				break;
			default:
				break;
			}
		}
	}

	return bCollision;
}

//===============================================================================================================
// �v���C���[�̃R���{
//===============================================================================================================
void PlayerComb(MOTIONTYPE motiontype, int AttackState, int nCounterState, COMBOSTATE Combstate)
{
	Camera* pCamera = GetCamera();

	ENEMY* pEnemy = GetEnemy();
	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;
	bool bFirst = true;
	int nIdxEnemy = 0;

	g_player.Motion.nKey = 0;                 // �L�[��0����n�߂�
	g_player.Motion.nCountMotion = 0;	      // ���[�V�����J�E���g��0����n�߂�
	g_player.Motion.motionType = motiontype;  // ���[�V�����̎�ނ�ύX
	g_nCounterState = nCounterState;		  // ��ԃJ�E���^�[��ݒ�
	g_AttackState = AttackState;			  // �U����ԃJ�E���^�[��ݒ�
	g_player.state = PLAYERSTATE_ATTACK;	  // �v���C���[�̏�Ԃ��U���ɂ���	
	g_player.Combostate = Combstate;		  // �R���{�̏�Ԃ�ݒ�
	SetMotion(&g_player.Motion, motiontype, MOTIONTYPE_NEUTRAL, true, 28);

	// �G�̍ő吔�����߂�
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// �g�p��Ԃ̂�
		if (pEnemy[nCnt].bUse && pEnemy[nCnt].nType != ENEMYTYPE_SIX)
		{
			// ���������߂�
			float DisposX = pEnemy[nCnt].pos.x - g_player.pos.x;
			float DisposY = pEnemy[nCnt].pos.y - g_player.pos.y;
			float DisposZ = pEnemy[nCnt].pos.z - g_player.pos.z;

			// ���������߂�
			fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

			// �ŏ������ʂ�
			if (bFirst)
			{
				fDistanceStock = fDistanceNow;
				bFirst = false;
				nIdxEnemy = nCnt;
			}
			else
			{
				// ���̋������X�g�b�N���ꂽ������菬����������
				if (fDistanceNow < fDistanceStock)
				{
					fDistanceStock = fDistanceNow; // ������ۑ�
					nIdxEnemy = nCnt; // �߂��G�̃C���f�b�N�X��ۑ�
				}
			}
		}
	}

	Boss* pBoss = Getboss();

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		// �{�X���v���C���[�̋߂��ɂ�����
		if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 50.0f, 140.0f) && pBoss[nCnt].bUse)
		{
			// �͈͂ɂ���Ԃ������b�N�I��
			if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 150.0f, 150.0f))
			{
				// �p�x�����߂�
				float fAngle = atan2f(pBoss[nCnt].pos.x - g_player.pos.x, pBoss[nCnt].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			}

			// �{�X�̏ꏊ������
			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				break;
			}
		}
		else
		{
			// �͈͂ɂ���Ԃ������b�N�I��
			if (sphererange(&g_player.pos, &pEnemy[nIdxEnemy].pos, 150.0f, 150.0f))
			{
				// �p�x�����߂�
				float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - g_player.pos.x, pEnemy[nIdxEnemy].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
			}

			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				break;
			}
		}
	}
}
//===============================================================================================================
// �v���C���[�̃��[�V�����̕ύX
//===============================================================================================================
void MotionChange(int itemtype,int LoadPlayer)
{
	g_player.Motion.nNumModel = 16; // �ő吔�����Ƃɖ߂�

	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[LoadPlayer].Motion.aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[LoadPlayer].Motion.aMotionInfo[nCntMotion]; // ���[�V�����̏�����
	}

	if (LoadPlayer == PLAYERTYPE_WEPON) // �v���C���[���m�[�n���h����������������Ȃ�
	{
		g_player.WeponMotion = itemtype; // �A�C�e�����Ƃ̃��[�V�����^�C�v
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// �U��1�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // �U��2�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// �U��3�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// �U��4�̏�����
		g_player.Motion.aModel[15].offpos = g_LoadMotion[itemtype].aModel[15].offpos;				// �I�t�Z�b�g�̏�����
		g_player.Motion.aModel[15].offrot = g_LoadMotion[itemtype].aModel[15].offrot;				// �I�t�Z�b�g�̂̏�����
		g_player.HandState = PLAYERHOLD_NO;
		bNohand = false;// �v���C���[���m�[�n���h�ɂ���
		g_player.PlayerType = PLAYERTYPE_WEPON;
	}
	else if(LoadPlayer == PLAYERTYPE_NOHAND)
	{
		g_player.WeponMotion = itemtype; // �A�C�e�����Ƃ̃��[�V�����^�C�v
		g_player.HandState = PLAYERHOLD_HOLD;	// �v���C���[���m�[�n���h�ɂ���
		g_player.PlayerType = PLAYERTYPE_NOHAND;
		bNohand = true;
	}
	
}

//===============================================================================================================
// �v���C���[�̃��[�V�����̃��[�h
//===============================================================================================================
void LoadMotion(int Weponmotion)
{
	FILE* pFile; // �t�@�C���̃|�C���^
	switch (Weponmotion)
	{
	case MOTION_KATANA:
		pFile = fopen("data\\MOTION_CHANGE\\motionSamurai2.txt", "r");
		break;
	case MOTION_BIGWEPON:
		pFile = fopen("data\\MOTION_CHANGE\\hammer.txt", "r");
		break;
	case MOTION_DBHAND:
		pFile = fopen("data\\MOTION_CHANGE\\bat.txt", "r");
		break;
	case MOTION_ONE_HAND:
		pFile = fopen("data\\MOTION_CHANGE\\onehand.txt", "r");
		break;
	case MOTION_PIERCING:
		pFile = fopen("data\\MOTION_CHANGE\\piercing.txt", "r");
		break;
	case MOTION_SP:
		pFile = fopen("data\\MOTION_CHANGE\\sp.txt", "r");
		break;
	case MOTION_SPHAMMER:
		pFile = fopen("data\\MOTION_CHANGE\\sphammer.txt", "r");
		break;
	case MOTION_ONEHANDBLOW:
		pFile = fopen("data\\MOTION_CHANGE\\sponehandblow.txt", "r");
		break;
	case MOTION_SPPIERCING:
		pFile = fopen("data\\MOTION_CHANGE\\sppiercing.txt", "r");
		break;
	case MOTION_SPDOUBLE:
		pFile = fopen("data\\MOTION_CHANGE\\doublehand.txt", "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "PARTSSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.x);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.y);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offpos.z);
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.x);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.y);
						fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aModel[15].offrot.z);
					}
					else if (strcmp(&aString[0], "END_PARTSSET") == 0)
					{
						break;
					}
				}
			}
			
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// MOTIONSET��ǂݎ������
				while (1)
				{
					// ������ǂݔ�΂�
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "LOOP") == 0)
					{// LOOP ��ǂݎ������
						// ������ǂݔ�΂�
						fscanf(pFile, "%s", &Skip[0]);
						// �l����
						fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].bLoop);
					}
					else if (strcmp(aString, "NUM_KEY") == 0)
					{// NUM_KEY��ǂݎ������
						// ������ǂݔ�΂�
						fscanf(pFile, "%s", &Skip[0]);
						// �l����
						fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].nNumkey);

						while (nCntKey < g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].nNumkey)
						{
							// ������ǂݔ�΂�
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "KEYSET") == 0)
							{// KEYSET��ǂݎ������

								while (1)
								{
									// ������ǂݔ�΂�
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "FRAME") == 0)
									{// FRAME��ǂݎ������
										// ������ǂݔ�΂�
										fscanf(pFile, "%s", &Skip[0]);

										fscanf(pFile, "%d", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
										break;
									}
								}

								while (1)
								{
									// ������ǂݔ�΂�
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "KEY") == 0)
									{// KEY��ǂ݂Ƃ�����
										while (1)
										{
											// ������ǂݔ�΂�
											fscanf(pFile, "%s", &aString[0]);

											if (strcmp(&aString[0], "POS") == 0)
											{// POS��ǂݎ������
												// ������ǂݔ�΂�
												fscanf(pFile, "%s", &Skip[0]);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
												nCntPosKey++;		// �C���N�������g										
											}

											else if (strcmp(&aString[0], "ROT") == 0)
											{// ROT��ǂݎ������
												// ������ǂݔ�΂�
												fscanf(pFile, "%s", &Skip[0]);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
												fscanf(pFile, "%f", &g_LoadMotion[Weponmotion].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
												nCntRotkey++;		// �C���N�������g									
											}

											else if (strcmp(&aString[0], "END_KEY") == 0)
											{// END_KEY��ǂݎ������
												break;
											}
										}
									}
									else if (strcmp(&aString[0], "END_KEYSET") == 0)
									{// END_KEYSET��ǂݎ������
										nCntRotkey = 0;
										nCntPosKey = 0;
										nCntKey++;			// �C���N�������g
										break;
									}


								}

							}

						}
					}
					if (strcmp(&aString[0], "END_MOTIONSET") == 0)
					{// END_MOTIONSET��ǂݎ������
						nCntMotion++;		// ���[�V�����̍X�V
						nCntKey = 0;		// 0����n�߂�
						break;
					}
				}
			}

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Player.cpp)", MB_OK);
		return;
    }
	fclose(pFile);
}
//===============================================================================================================
// �v���C���[�̃��[�h����
//===============================================================================================================
void LoadPlayer(int nType)
{
	FILE* pFile;
	char aStr[MAX_WORD] = {};
	char Skip[3] = {};

	int nNumModel = 0;
	int nLoadCnt = 0;
	int nNumParts = 0;

	switch (nType)
	{
	case 0:
		// �t�@�C�����J��
		pFile = fopen("data/MOTION/motionSamurai.txt", "r");
		break;
	case 1:
		pFile = fopen("data/MOTION_CHANGE/motionSamurai_Shot.txt", "r");
		break;
	default:
		pFile = NULL; //NULL�ɂ���
		break;
	}

	if (pFile != NULL)
	{
		while (1)
		{
			// �������ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//�R�����g��������
				//������ǂݔ�΂�
				continue;
			}

			// NUM_MODEL��ǂݎ������
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%s", &Skip[0]);						// [=]��ǂݔ�΂�
				fscanf(pFile, "%d", &nNumModel);					// ���f���̍ő吔����
				g_LoadPlayer[nType].Motion.nNumModel = nNumModel;	// ���f���̍ő吔����
			}

			// ���f���̓ǂݍ��݂��܂��I����Ă��Ȃ�������
			if (nLoadCnt < nNumModel)
			{
				// ���f���̓ǂݍ��񂾐���Ԃ�
				nLoadCnt = LoadFilename(pFile, nNumModel, &aStr[0], nType);
			}

			// CHARACTERSET��ǂݎ������
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// �p�[�c�̐ݒ菈��
				LoadCharacterSet(pFile, &aStr[0], nNumParts, nType);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadMotionSet(pFile, &aStr[0], nNumModel, nType);
			}

			// END_SCRIPT��ǂݎ������
			if (strcmp(&aStr[0], "END_SCRIPT") == 0)
			{
				nCntMotion = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
			}
		}
	}
	else
	{
		// �t�@�C�����J���Ȃ�������
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(LoadPlayer)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);

}
//===============================================================================================================
// �v���C���[�̃��f�����[�h
//===============================================================================================================
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//���_���W�̎擾
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	char Skip[3] = {}; // [=]�ǂݔ�΂��悤�ϐ�

	int nCntLoadModel = 0; // ���f���̃��[�h�̃J�E���^�[

	// �J�E���g�����f������艺��������
	while (nCntLoadModel < nNumModel)
	{
		// ������ǂݎ��
		fscanf(pFile, "%s", aString);

		// MODEL_FILENAME��ǂݎ������
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			fscanf(pFile, "%s", &Skip[0]); // [=]��ǂݔ�΂�
			fscanf(pFile, "%s", aString);  // �t�@�C������ǂݎ��

			const char* FILE_NAME = {};    // �t�@�C��������p�ϐ�

			FILE_NAME = aString;           // �t�@�C��������

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].dwNumMat,
				&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh);

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//���_���̎擾
			nNumVtx = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.x = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.x - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.x;
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.y = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.y - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.y;
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].Size.z = g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMax.z - g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_LoadPlayer[nType].Motion.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
		}
	}
	return nCntLoadModel; // ���f���̃J�E���g��Ԃ�

}
//===============================================================================================================
// �v���C���[�̃L�����N�^�[�Z�b�g
//===============================================================================================================
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
{
	int nIdx = 0; // �C���f�b�N�X�i�[�ϐ�
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�

	// END_CHARACTERSET��ǂݎ���ĂȂ�������
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// ������ǂݎ��
		fscanf(pFile, "%s", aString);

		// INDEX��ǂݎ������
		if (strcmp(aString, "INDEX") == 0)
		{
			fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
			fscanf(pFile, "%d", &nIdx);    // �C���f�b�N�X����
		}
		// PARENT��ǂݎ������
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.x);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.y);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.x);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.y);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// �v���C���[�̃��[�V�����Z�b�g
//===============================================================================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�

	while (1)
	{
		// ������ǂݎ��
		fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// LOOP��ǂݒʂ�����
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩���Ȃ���
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
		}
		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadKeySet(pFile, aString, nType, nCntMotion);
		}
		// END_MOTIONSET��ǂݒʂ�����
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotion++; // ���[�V�����̃J�E���g�����Z�b�g
			nKey = 0;     // �L�[�����Z�b�g
			break;
		}
	}
}
//===============================================================================================================
// �v���C���[�̃��[�V�����̃L�[�Z�b�g
//===============================================================================================================
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nCntPos = 0;   // �ʒu�̃J�E���g
	int nCntRot = 0;   // �p�x�̃J�E���g

	while (1)
	{
		// ������ǂݎ��
		fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// FRAME��ǂݒʂ�����
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �t���[����ǂݍ���
			fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSET��ǂݒʂ�����
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKey++;		 // �L�[�𑝂₷
			nCntPos = 0; // �ʒu�̃J�E���g�����Z�b�g
			nCntRot = 0; // �p�x�̃J�E���g�����Z�b�g
			break;
		}
	}
}
