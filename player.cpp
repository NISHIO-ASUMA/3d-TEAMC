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
#include "effectEdit.h"
#include "icon.h"
#include "effect2.h"
#include "meshimpact.h"
#include "meshcylinder.h"
#include "billboard.h"
#include "mark.h"
#include "game.h"
#include "math.h"
#include "easing.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)			// �ő啶����
#define PLAYERLIFE (1000)		// �v���C���[�̗̑�
#define MAX_TEXPLAYER (128)		// �e�N�X�`���̍ő吔
#define MAX_JUMP (12.0f)		// �W�����v��
#define MAX_MOVE (1.0f)			// �v���C���[�̈ړ���
#define NUM_MTX (8)				// ���̓����蔻��̃}�g���N�X�̐�
#define LANDINGEXPLOSION (6)	// ���n�����Ƃ��ɏo�鉌
#define HEAL_VALUE (100)		// �񕜗�
#define AVOID_MOVE (18.0f)      // ����̈ړ���
#define DAMAGEBLOW (20.0f)      // �_���[�W���󂯂����̐�����ї�
#define BLOWCOUNT (5)           // ������уJ�E���g
#define PLAYER_DAMAGE (100)     // �v���C���[�̍U����(�)
#define PLAYER_SPEED (3.5f)     // �v���C���[�̈ړ����x

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate); // �v���C���[�̃R���{����
void LoadMotion(int Weponmotion);																 // ���[�V�����̃��[�h����
void StickPad(void);																			 // �p�b�h�̈ړ�����

void LoadPlayer(int nType);                                                                      // �v���C���[�̃��[�h����
int LoadFilename(FILE* pFile, int nNumModel, char* aString, int nType);							 // �v���C���[�̃��f���̃��[�h����
void LoadCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);					 // �v���C���[�̃p�[�c�̐ݒ菈��
void LoadWeponOffSet(FILE* pFile, char* aString,int nWepontype);								 // �v���C���[�̕���̃I�t�Z�b�g�̐ݒ菈��
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);						 // �v���C���[�̃��[�V�����̃��[�h����
void LoadKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);							 // �v���C���[�̃��[�V�����̃L�[�̓ǂݍ��ݏ���
void LoadWeponMotionSet(FILE* pFile, char* aString, int nNumModel, int nWepontype);				 // �v���C���[�̕���̃I�t�Z�b�g�̃��[�h����
void LoadWeponKeySet(FILE* pFile, char* aString, int nWepontype, int nCntMotion);				 // �v���C���[�̕���̃I�t�Z�b�g�̃L�[�̓ǂݍ��ݏ���

void SetElementEffect(void);																	 // �������Ƃ̃G�t�F�N�g
void SetMotionContller(void);																	 // ���[�V�����̐ݒ菈��
void PlayerMove(void);																			 // �v���C���[�̈ړ�����
void UpdatePlayerCraft(void);                                                                    // �v���C���[�̃N���t�g
void DestroyWepon(void);																		 // �A�C�e���̔j�󏈗�
void DropItem(void);                                                                             // �A�C�e���̃h���b�v����
void HandleSpecialAttack(void);																	 // �X�y�V�����A�^�b�N�̏���
void UpdateItemStock(void);																		 // �v���C���[�̃A�C�e���̃X�g�b�N����
void UpdatePlayerAvoid(void);																	 // �v���C���[�̉������
D3DXVECTOR3 SetMotionMoveAngle(void);                                                            // ���[�V�����̃A���O���̐ݒ�
void SetWeponEffect(void);                                                                       // ���킲�Ƃ̃G�t�F�N�g����
bool IsDamageAction(void);                                                                       // �_���[�W�A�N�V�������ǂ���
void ChangeItemParam(int nHaveIdx, int nType);													 // �A�C�e���̕ύX���̃p�����[�^�[�ݒ�
void MotionTransition(void);																	 // ���[�V�����^�C�v�̑J��
void StateTransition(void);																		 // ��Ԃ̑J��
void ComboTransition(void);																		 // �R���{��Ԃ̑J��
void SetUpPlayerAttack(void);																	 // �v���C���[�̍U���̐ݒ菈��
void SetUpJumpAction(int nKey, int nCounter, int nLastKey, int EndFrame);						 // �W�����v�A�N�V�����̐ݒ�
void SetUpPlayerFirstWepon(void);																 // �v���C���[�̏�������̐ݒ�
void SetFeverTime(void);																		 // �v���C���[�̃t�B�[�o�[�^�C���̐ݒ�
void LoadMinimapMatItem(int nHaveIdx,int nStockIdx);											 // �~�j�}�b�v�̍����̃A�C�e���̍ޗ��̕\��

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
//LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//�v���C���[�̃e�N�X�`���ւ̃|�C���^
Player g_player;//�v���C���[�\����
MOTION g_LoadPlayer[PLAYERTYPE_MAX]; // �v���C���[�̃��f����ۑ����Ă����ϐ�
MOTION g_LoadMotion[MOTION_MAX];   // ���[�V�����̏���ۑ����Ă����ϐ�
int g_nCounterState,g_AttackState; // ��ԃJ�E���^�[
bool bNohand; // �������������ĂȂ���
bool bUsePad;
int nCntMotion,nKey;
int g_EaseCount;

//===============================================================================================================
//�v���C���[�̏���������
//===============================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	MODE mode = GetMode();//���݂̃��[�h���擾

	//�v���C���[�̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);					// ���W
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �p�x
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړI�̊p�x
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	g_player.bJump = false;											// �W�����v�����ۂ�
	g_player.bDisp = true;											// �g�p���
	g_player.nLife = PLAYERLIFE;									// �̗�
	g_player.nMaxLife = PLAYERLIFE;									// �ő�̗�
	g_player.state = PLAYERSTATE_NORMAL;							// ���
	g_player.Motion.bLoopMotion = true;								// ���[�V�����̃��[�v
	g_player.Motion.nKey = 0;										// �L�[��
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;				// ���[�V�����̎��
	g_player.SwordOffpos.x = 0.0f;									// ���̃I�t�Z�b�g�̍��Wx
	g_player.SwordOffpos.y = 85.0f;									// ���̃I�t�Z�b�g�̍��Wy
	g_player.SwordOffpos.z = 0.0f;									// ���̃I�t�Z�b�g�̍��Wz
	g_player.nCounterAction = 0;									// �A�N�V�����J�E���g
	g_nCounterState = 0;											// ��ԃJ�E���^�[
	g_AttackState = 0;												// �U����Ԃ̃J�E���^�[
	bNohand = false;												// ���𓊂����������ĂȂ���
	g_player.speed = PLAYER_SPEED;									// ���̑���
	g_player.nDamage = PLAYER_DAMAGE;								// �U����
	bUsePad = false;												// �p�b�h���g���Ă��邩
	g_player.nStockDamage = PLAYER_DAMAGE;							// �_���[�W�̃X�g�b�N
	g_player.fStockSpeed = PLAYER_SPEED;							// �X�s�[�h�̃X�g�b�N
	g_player.FeverMode = false;										// �t�B�[�o�[���[�h���ǂ���
	g_player.SpMode = false;										// �X�y�V�����𔭓��ł��邩�ǂ���
	g_player.WeponMotion = MOTION_KATANA;							// ���킲�Ƃ̃��[�V����
	g_player.AttackSp = false;										// �X�y�V�����U�������ǂ���
	g_player.bLandingOBB = false;									// OBB�ɏ���Ă邩
	nCntMotion = 0;													// ���[�V�����̃J�E���g
	nKey = 0;														// �L�[�̃J�E���g
	g_player.bCraft = false;										// �N���t�g�����ǂ���
	g_player.nElement = WEPONELEMENT_STANDARD;						// �����̎��
	g_EaseCount = 0;												// �C�[�W���O�̃J�E���g
	g_player.nIdxShadow = SetShadow(g_player.pos, g_player.rot, 20.0f, 1.0f); // �e��ݒ�
	g_player.nIdxMap = SetMiniMap(g_player.pos, MINIMAPTEX_PLAYER); // �~�j�}�b�v�Ƀv���C���[��ݒ�
	g_player.HandState = PLAYERHOLD_NO;								// ��̏��
	g_player.Combostate = COMBO_NO;                                 // �R���{�̏��
	g_player.AttackState = PLAYERATTACKSTATE_NO;                    // �U���̏��
	g_player.nCounterAttack = 0;                                    // �U����Ԃ̃J�E���^�[
	g_player.avoidMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // ����̈ړ���
	g_player.ItemIdx = 0;											// �A�C�e���̃C���f�b�N�X�̏�����
	g_player.BlowCounter = 0;										// ������Ԃ܂ł̃J�E���^�[
	g_player.AttackerIdx = 0;										// �U�����Ă����G�̃C���f�b�N�X
	g_player.bstiffness = false;									// �_���[�W�̍d��
	g_player.StockItemType = ITEMTYPE_NONEXISTENT;                  // �X�g�b�N���Ă���A�C�e���̎��
	g_player.HoldItemType = ITEMTYPE_KATANA;						// �����Ă���A�C�e���̎��
	g_player.bAvoid = false;										// ���G����
	g_player.HitStopCount = 0;										// �q�b�g�X�g�b�v

	// �A�C�e������
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		g_player.Itembreak[nCnt] = false; // �A�C�e������ꂽ��
	}

	// TODO : �����̏����l����

	// �^�C�g���Ń��[�h������Əd���Ȃ�̂�
	if (mode != MODE_TITLE)
	{
		// �؂�ւ�郂�[�V�����̐�����
		for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
		{
			LoadMotion(nCnt);
		}

		// �v���C���[�̎�ޕ�
		for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
		{
			//�K�v�ȏ���ݒ�
			LoadPlayer(nCntPlayer);
			g_LoadPlayer[nCntPlayer].motionType = MOTIONTYPE_NEUTRAL;
			g_LoadPlayer[nCntPlayer].bLoopMotion = true;
		
		}

		//�ŏ��ɕ`�悵�����v���C���[�̏�����
		g_player.Motion = g_LoadPlayer[0];
	}

	// ����ݒ�
	SetMark(g_player.pos, g_player.rot);

	// �_���[�W��UI��ݒ�
	SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_DAMAGE, 740.0f, 460.0f, false, 0);

	// �v���C���[�̏�������̐ݒ�
	SetUpPlayerFirstWepon();
	
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
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].nNumModel; nCntModel++)
		{
			//�e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] != NULL)
				{
					g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]->Release();
					g_LoadPlayer[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			//���b�V���̔j��
			if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh != NULL)
			{
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh->Release();
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat != NULL)
			{
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat->Release();
				 g_LoadPlayer[nCntPlayer].aModel[nCntModel].pBuffMat = NULL;
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
	GAMESTATE gameState = GetGameState();

	// �̗͂̌��ݒl���ő�l�𒴂��Ă���ő�l�ɂ���
	if (g_player.nMaxLife <= g_player.nLife)
	{
		g_player.nLife = g_player.nMaxLife;
	}

	g_player.HitStopCount--;

	if (g_player.HitStopCount > 0)
	{
		g_player.pos.x += (float)(rand() % 3 - 1.5f);
		g_player.pos.z += (float)(rand() % 3 - 1.5f);
	}

	// �q�b�g�X�g�b�v�̃J�E���g��0�ȏ�L������
	if (g_player.HitStopCount >= 0) return;

	// ���[�r�[����Ȃ�������
	if (gameState != GAMESTATE_MOVIE)
	{
		// ���[�V�����̍X�V
		UpdateMotion(&g_player.Motion);

		// �v���C���[�̃N���t�g�̐ݒ�
		UpdatePlayerCraft();

		// ���[�V�����̉��o����
		SetMotionContller();
	}

	// �N���t�g��ԂȂ�
	if (g_player.bCraft == true)
	{
		// �֐��𔲂���
		return;
	}

	// �t�B�[�o�[���[�h�Ȃ�
	if (g_player.FeverMode == true)
	{
		SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25, g_player.pos.z), D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 1, 20, 10, 20.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// �p�b�h���g���Ă��Ȃ����𔻒�
	const bool NotUsePad = GetJoyStick() == false;

	// �A�N�V��������Ȃ����𔻒�
	const bool NotAction = CheckActionMotion(&g_player.Motion) == true;

	// �X�y�V�����U������Ȃ�������
	const bool NotSp = g_player.AttackSp == false;

	// ����ł邩�ǂ����𔻒�
	const bool NotDeth = g_player.nLife > 0;

	// �_���[�W���[�V�������ǂ����𔻒�
	const bool NotDamage = g_player.bstiffness == false;

	// �������邩�ǂ����𔻒�
	const bool is_Move = NotUsePad && NotAction && NotSp && NotDeth && NotDamage && gameState != GAMESTATE_MOVIE;

	// ��������
	if (is_Move == true)
	{
		PlayerMove(); // �v���C���[�̈ړ�����
	}

	const bool is_MovePad = !NotUsePad && NotAction && NotSp && NotDamage && gameState != GAMESTATE_MOVIE;

	if (is_MovePad == true)
	{
		StickPad(); // �p�b�h�̈ړ�����
	}

	// ���̃}�g���N�X����
	D3DXVECTOR3 SwordPos = SetMtxConversion(g_player.SwordMtx);

	switch (g_player.AttackState)
	{
	case PLAYERATTACKSTATE_NO:
		break;
	case PLAYERATTACKSTATE_ATTACK:
		g_player.nCounterAttack--;
		if (g_player.nCounterAttack <= 0)
		{
			g_player.AttackState = PLAYERATTACKSTATE_NO;
		}
		break;
	default:
		break;
	}
	
	// ���[�V�����^�C�v�̑J��
	MotionTransition();

	// �v���C���[�̏�Ԃ̑J��
	StateTransition();

	// �R���{��Ԃ̑J��
	ComboTransition();

	// ���[�r�[����Ȃ�������
	if (gameState != GAMESTATE_MOVIE)
	{
		//�ړ��ʂ�����
		g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
		g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

		//�O��̈ʒu��ۑ�
		g_player.posOld = g_player.pos;

		//�v���C���[�̈ʒu�̍X�V
		g_player.pos += g_player.move;
	}
	else
	{
		//�O��̈ʒu��ۑ�
		g_player.posOld.y = g_player.pos.y;

		//�v���C���[�̈ʒu�̍X�V
		g_player.pos.y += g_player.move.y;
	}

	// ���b�V���t�B�[���h�̓����蔻��
	if (CollisionField(&g_player.pos,&g_player.posOld) == true)
	{
		g_player.bLandingOBB = false;

		// ���[�V�������W�����v��������
		if (g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMP && g_player.Motion.motiontypeBlend != MOTIONTYPE_LANDING)
		{
			SetMotion(&g_player.Motion,MOTIONTYPE_LANDING, true, 10); // ���[�V�����𒅒n�ɂ���
			//SetImpact(g_player.pos, COLOR_GOLD, 32, 10.0f, 0.0f, 3.0f, 60, IMPACTTYPE_NORMAL, 0);
		}
		g_player.bJump = true; // �W�����v���\�ɂ���
	}
	else
	{
		g_player.bJump = false;
	}

	// ���b�V���E�H�[��
	CollisionWall(&g_player.pos,&g_player.posOld,&g_player.move,g_player.speed);

	//�v���C���[�̏d��
	g_player.move.y -= MAX_GLABITY;

	// �e�̌v�Z
	SetPositionShadow(g_player.nIdxShadow, g_player.pos, 30.0f + 30.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	// �~�j�}�b�v�̈ʒu�̐ݒ�
	SetMiniMapPotision(g_player.nIdxMap, &g_player.pos);

	if ((JoypadTrigger(JOYKEY_A) || KeyboardTrigger(DIK_SPACE)) && g_player.Motion.motiontypeBlend != MOTIONTYPE_DEATH && g_player.bstiffness == false)
	{//a�{�^�� or Enter�L�[�������ꂽ

		// ���y�Đ�
		PlaySound(SOUND_LABEL_JUMP_SE);

		if (g_player.bJump == true && g_player.Motion.motionType != MOTIONTYPE_LANDING && g_player.AttackSp == false && gameState != GAMESTATE_MOVIE)
		{
			g_player.bJump = false;						 // �W�����v���ł��Ȃ�����
			SetMotion(&g_player.Motion, MOTIONTYPE_JUMP, true, 10);
			g_player.move.y = 18.0f;					 // �W�����v��		
			g_player.Combostate = COMBO_NO;
		}
	}

	// �v���C���[�̍U���̐ݒ菈��
	SetUpPlayerAttack();

	// �v���C���[�̊p�x�̐��K��
	if (g_player.rotDestPlayer.y - g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDestPlayer.y - g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// �A�C�e���̔j�󏈗�
	DestroyWepon();		

	//// �A�C�e���̃h���b�v����
	//DropItem();

	// �A�C�e���̃X�g�b�N�̍X�V����
	UpdateItemStock();		

	// �v���C���[�̉���̏���
	UpdatePlayerAvoid();

	// �K�E�Z�̏���
	HandleSpecialAttack();

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_H) == true)
	{
		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, D3DCOLOR_RGBA(100, 100, 100, 255), 32, 30.0f, 20.0f, 3.0f, 60, IMPACTTYPE_PLAYER, 0);

		AddSpgauge(100.0f);
	}

#endif // DEBUG

	// �t�H�[�o�[�^�C���̐ݒ�
	SetFeverTime();

	// ���S���[�V������������
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH && g_player.Motion.nKey <= 0)
	{
		g_player.move.x = sinf(g_player.rot.y) * 50.0f;
		g_player.move.y = 10.0f;
		g_player.move.z = cosf(g_player.rot.y) * 50.0f;
	}
	
	// �V�����_�[�̈ʒu�ݒ菈��
	SetPotisionCylinder(g_player.nIdxCylinder, g_player.pos);

	//D3DXVec3TransformCoord
	// getactivewindow
	
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
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.SwordMtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.Motion.aModel[15].rot.y,
		g_player.Motion.aModel[15].rot.x,
		g_player.Motion.aModel[15].rot.z);

	D3DXMatrixMultiply(&g_player.SwordMtx, &g_player.SwordMtx, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.SwordOffpos.x, g_player.SwordOffpos.y, g_player.SwordOffpos.z);
	D3DXMatrixMultiply(&g_player.SwordMtx, &g_player.SwordMtx, &mtxTrans);

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
void HitPlayer(int nDamage,bool SetDamageMotion, int AttackerIdx, int AttackerType)
{
	// �v���C���[�̏�Ԃ��X�y�V������������
	if (g_player.AttackSp == true) return;

	// ���G���Ԃ�������
	if (g_player.bAvoid == true) return;

	// �v���C���[�̗̑͂����炷
	g_player.nLife -= nDamage;

	// �v���C���[�̗̑͂�0�ɂȂ�����
	if (g_player.nLife <= 0)
	{
		// �}�C�i�X�ł�0�ɂ���
		g_player.nLife = 0;

		if (g_player.Motion.motiontypeBlend != MOTIONTYPE_DEATH)
		{
			// ���[�V�����̐ݒ�
			SetMotion(&g_player.Motion, MOTIONTYPE_DEATH, false, 10);

			D3DXVECTOR3 HeadPos(g_player.Motion.aModel[2].mtxWorld._41, g_player.Motion.aModel[2].mtxWorld._42, g_player.Motion.aModel[2].mtxWorld._43);

			// ��
			LoadEffect(1, HeadPos);
		}

		// �v���C���[������
		EnableMap(g_player.nIdxMap);    // �}�b�v�������
	}
	else
	{
		// ��ԃJ�E���^�[
		g_nCounterState = 30;

		// �v���C���[�̏�Ԃ��_���[�W�ɂ���
		g_player.state = PLAYERSTATE_DAMAGE;

		// �J������h�炷
		WaveCamera(15);

		// ������ׂ�Ȃ�
		if (SetDamageMotion == true)
		{
			// ������уJ�E���g
			g_player.BlowCounter++;

			// �J�E���g���ő�ɂȂ�����
			if (g_player.BlowCounter >= BLOWCOUNT)
			{
				g_player.bstiffness = true;

				// �U�����Ă����G�̃C���f�b�N�X����
				g_player.AttackerIdx = AttackerIdx;

				// �A�^�b�J�[�̊p�x�����߂�
				g_player.rotDestPlayer.y = SetAttackerAngle(AttackerIdx, AttackerType);

				g_player.BlowCounter = 0;

				// ���[�V�����̐ݒ�
				SetMotion(&g_player.Motion, MOTIONTYPE_DAMAGE, true, 5);
			}
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

	// �W�����v���Ă邩�𔻒�
	const bool NotJump = g_player.Motion.motiontypeBlend != MOTIONTYPE_JUMP;

	// ���[�V���������[�u���𔻒�
	const bool NotMove = g_player.Motion.motiontypeBlend != MOTIONTYPE_MOVE;

	// �������邩�𔻒�
	const bool is_MotionMove = NotJump == true && NotMove == true;

	// ���[�V������������L�[��3�ȉ�
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_THREE)
	{
		return;
	}

	if (GetJoyStick() == true)
	{
		// L�X�e�B�b�N�̊p�x
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// �f�b�h�]�[��
		float deadzone = 10920;

		// �X�e�B�b�N�̌X�����p�x�����߂�
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		// ��������
		if (magnitude > deadzone)
		{
			// �A���O���𐳋K��
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			// �v���C���[�̈ړ���
			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			// �v���C���[�̈ړ�
			g_player.move.x += moveX * g_player.speed;
			g_player.move.z += moveZ * g_player.speed;

			// �v���C���[�̖ړI�̊p�x�����߂�
			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

			bUsePad = true;
			// �v���C���[��������[�V�����ɂ���
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}
		}
		else
		{
			if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5); // ���[�V�������j���[�g�����ɂ���
			}
			bUsePad = false;
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
	Item* pItem = GetItem();

	bool bHit = false;

	// ���̔��a
	float Radius = 50.0f;

	// ����̈ʒu
	D3DXVECTOR3 SwordTopPos = SetMtxConversion(g_player.SwordMtx);

	// ���̎�����̈ʒu
	D3DXVECTOR3 SwordUnderPos = SetMtxConversion(g_player.Motion.aModel[15].mtxWorld);

	for (int nCntModel = 0; nCntModel < pEnemy->Motion.nNumModel; nCntModel++)
	{
		// �G�̈ʒu
		D3DXVECTOR3 EnemyModel = SetMtxConversion(pEnemy->Motion.aModel[nCntModel].mtxWorld);

		// �����̏I�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvEnd = SwordTopPos - EnemyModel;

		// �����̎n�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvFirst = SwordUnderPos - EnemyModel;

		// �n�_�`�I�_�܂ł̃x�N�g��
		D3DXVECTOR3 sv = SwordTopPos - SwordUnderPos;

		// ����X
		float LengthX = SwordTopPos.x - SwordUnderPos.x;
		// ����Y
		float LengthY = SwordTopPos.y - SwordUnderPos.y;
		// ����Z
		float LengthZ = SwordTopPos.z - SwordUnderPos.z;

		// �����̒��������߂�
		float Length = sqrtf((LengthX * LengthX) + (LengthY * LengthY) + (LengthZ * LengthZ));

		// ���K������
		D3DXVec3Normalize(&sv, &sv);

		// �x�N�g���̏I�_�Ɖ~�̒��S�Ƃ̓���
		float DotEnd = D3DXVec3Dot(&sv, &pvEnd);

		// �x�N�g���̎n�_�Ɖ~�̒��S�Ƃ̓���
		float DotFirst = D3DXVec3Dot(&sv, &pvFirst);

		// ��������
		if (D3DXVec3Length(&pvEnd) < Radius)
		{
			bHit = true;
			break;
		}
		// ��������
		else if (D3DXVec3Length(&pvFirst) < Radius)
		{
			bHit = true;
			break;
		}

		// ���ς̒l��0���傫�������̃x�N�g���̑傫����菬�����Ȃ�
		if (0 < DotEnd && DotEnd < Length)
		{
			// sv�𒷂�Dot�̃x�N�g���ɂ���
			sv *= DotEnd;

			// �x�N�g���̈����Z��pv��[�������狅�̂̒��S�_�܂ł̍ŒZ�x�N�g���ɂ���]
			pvEnd -= sv;

			// ��������
			if (D3DXVec3Length(&pvEnd) < Radius)
			{
				bHit = true;
				break;
			}
		}
		// ���ς̒l��0���傫�������̃x�N�g���̑傫����菬�����Ȃ�
		else if (0 < DotFirst && DotFirst < Length)
		{
			// sv�𒷂�Dot�̃x�N�g���ɂ���
			sv *= DotFirst;

			// �x�N�g���̈����Z��pv��[�������狅�̂̒��S�_�܂ł̍ŒZ�x�N�g���ɂ���]
			pvFirst -= sv;

			// ��������
			if (D3DXVec3Length(&pvEnd) < Radius)
			{
				bHit = true;
				break;
			}
		}
	}

	// ����������Ă��邩���肷��
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// �X�y�V�����U�������Ă��邩�𔻒�
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// �G�̏�Ԃ𔻒肷��
	const bool is_NotDamageState = pEnemy->state != ENEMYSTATE_DAMAGE;

	// �U����Ԃ𔻒肷��
	const bool is_NotAction = isPlayerAttaking() == true;

	// �U���ł��邩�ǂ����𔻒肷��
	const bool is_Damage = is_HaveWepon == true && is_NotSpAttack == true && is_NotDamageState == true && is_NotAction == true;

	// �L�[	
	int nKey = g_player.Motion.nKey;

	// �ő�̃L�[
	int nLastKey = g_player.Motion.aMotionInfo[g_player.Motion.motionType].nNumkey - 1;

	// �Ō�̃t���[��
	int LastFrame = g_player.Motion.aMotionInfo[g_player.Motion.motionType].aKeyInfo[nLastKey].nFrame * 0.5f;

	// ���[�V�����J�E���^�[
	int nCounter = g_player.Motion.nCountMotion;

	// �v���C���[������������Ă��邩�X�y�V�����U������Ȃ�
	if (is_Damage == true && bHit == true && CheckMotionBounds(nKey, nCounter,1, nLastKey,0, LastFrame) == true)
	{
		// �G�ɓ�������
		HitEnemy(nCntEnemy, (g_player.nDamage * 5));

		//// �ϋv�G�~�����炷
		pItem[g_player.ItemIdx].durability--;
		return;
	}

	// ����������Ă��邩���肷��
	const bool is_NotWepon = g_player.Motion.nNumModel == MAX_MODEL - 1;

	// �U���ł��邩�ǂ����𔻒肷��
	const bool is_NotHandDamage = is_NotWepon == true && is_NotSpAttack == true && is_NotAction == true && is_NotDamageState == true;

	// ���f���̈ʒu��ϐ��ɑ��
	D3DXVECTOR3 ModelPos(g_player.Motion.aModel[4].mtxWorld._41, g_player.Motion.aModel[4].mtxWorld._42, g_player.Motion.aModel[4].mtxWorld._43);

	// �f��̎����U�����X�y�V��������Ȃ�
	if(is_NotHandDamage == true && sphererange(&ModelPos, &pEnemy->pos, 30.0f, 65.0f) == true)
	{
		if (g_player.Motion.motionType == MOTIONTYPE_ACTION && g_player.Motion.nKey >= 2)
		{
			HitEnemy(nCntEnemy, g_player.nDamage * 3); // �G�ɓ�������
			return;
		}
	}

	// �X�y�V�����U�������Ă��邩�𔻒�
	const bool is_SpAttack = g_player.AttackSp == true;

	// �U���ł��邩�ǂ����𔻒肷��
	const bool is_SpDamage = is_HaveWepon == true && is_SpAttack == true && is_NotDamageState == true && is_NotAction == true;

	// ���������Ă��邩�X�y�V�����U����
	if (is_SpDamage == true && sphererange(&g_player.pos,&pEnemy->pos,200.0f,50.0f) == true)
	{
		// �͈͓��ɂ�����
		if (g_player.WeponMotion != MOTION_SPPIERCING && g_player.WeponMotion != MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, 3, nLastKey, 0, LastFrame) == true)
		{
			// �G�Ƀ_���[�W��^����
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
		}
		// �͈͓��ɂ�����
		if (sphererange(&g_player.pos, &pEnemy->pos, 200.0f, 50.0f) && g_player.WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, 19, nLastKey, 0, LastFrame) == true)
		{
			// �G�Ƀ_���[�W��^����
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
		}

		// �͈͓��ɂ�����
		if (sphererange(&g_player.pos, &pEnemy->pos, 250.0f, 50.0f) && g_player.WeponMotion == MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, 3, nLastKey, 0, LastFrame) == true)
		{
			// �G�Ƀ_���[�W��^����
			HitEnemy(nCntEnemy, (g_player.nDamage * 50));
			return;
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
			pItem[nIdx].move.y = dest.y * 10.0f;
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

			pItem[nIdx].move.x = dest.x * 10.0f;
			pItem[nIdx].move.y = dest.y * 10.0f;
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
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
	}

	// ��������ɕ��������
	g_player.Motion.nNumModel -= 1;

	// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
	g_player.HandState = PLAYERHOLD_NO;

	// ��Ԃ𓊂����Ă��Ԃɂ���
	pItem[nIdx].state = ITEMSTATE_THROW;

	g_player.speed = 4.0f; // �v���C���[�̈ړ����x�����Z�b�g
	g_player.fStockSpeed = 4.0f; // �v���C���[�̈ړ����x��ۑ�
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
		//g_player.rotDestPlayer.y = SetAttackerAngle(nCnt, ATTACKER_BOSS);
	}
}
//===============================================================================================================
// �v���C���[�ƃA�C�e���̔���
//===============================================================================================================
bool CollisionItem(int nIdx, float Itemrange, float plrange)
{
	GAMESTATE gamestate = GetGameState();

	// ���[�r�[��������
	if (gamestate == GAMESTATE_MOVIE || g_player.bCraft == true || g_player.AttackSp == true)
	{
		// �֐��𔲂���
		return false;
	}

	Item* pItem = GetItem();

	ITEM_INFO* pItemInfo = GetItemInfo();

	Billboard* pBillboard = GetBillBoard();


	MODEL_INFO* ItemTexture = GetItemOrigin();

	bool bCollision = false; // �������Ă��邩�ǂ���

	// �_���[�W��Ԃ��𔻒�
	const bool is_Damage = g_player.bstiffness == false;

	// �͈͓����𔻒�
	const bool Inbounds = sphererange(&g_player.pos, &pItem[nIdx].pos, 100.0f, 20.0f) == true;

	// ��Ԃ��m�[�}��������
	const bool stateNormal = pItem[nIdx].state == ITEMSTATE_NORMAL;

	// ����ł��邩�𔻒�
	const bool NotDeth = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// �E���邩�𔻒�
	const bool CanPickUp = is_Damage && sphererange(&g_player.pos, &pItem[nIdx].pos, 20.0f, 20.0f) == true && stateNormal && NotDeth;

	// �͈͓���������
	if (Inbounds == true)
	{
		bCollision = true;
	}

	// �͈͓��ɓ�����
	if (CanPickUp == true)
	{	
		// �A�C�e���̏�Ԃ����ʂ�������
		if (pItem[nIdx].state == ITEMSTATE_NORMAL)
		{
			int nIdxBillboard = pItem[nIdx].nIdxBillboardCount;
			// �r���{�[�h�Z�b�g
			pBillboard[nIdxBillboard].state = BILLBOARDSTATE_SET;
		}


		if ((KeyboardTrigger(DIK_E) || JoypadTrigger(JOYKEY_LEFT_B)) && g_player.Combostate == COMBO_NO)
		{
			// �C���f�b�N�X�ԍ��̃r���{�[�h���\��
			DeletIdxBillboard(pItem[nIdx].nIdxBillboardCount);

			if (pItem[nIdx].nType == ITEMTYPE_ONIGIRI)
			{
				// ���y�Đ�
				PlaySound(SOUND_LABEL_LIFERECOVERY);

				// ���f���̈ʒu����
				D3DXVECTOR3 pos = SetMtxConversion(g_player.Motion.aModel[1].mtxWorld);

				// �V�����_�[���Z�b�g
				g_player.nIdxCylinder = SetMeshCylinder(D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z),1,60,40.0f,D3DCOLOR_RGBA(59,255,0,255),16,1,2.0f,12.0f);

				LoadEffect(2, pos);

				SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + g_player.Size.y / 1.5f, g_player.pos.z),
					D3DXVECTOR3(1.57f, g_player.rot.y, 1.57f),
					D3DXVECTOR3(0.2f, 3.14f, 0.2f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
					2.0f, 4, 40, 40, 4.0f, 40.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				SetParticle(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + g_player.Size.y / 1.5f, g_player.pos.z),
					D3DXVECTOR3(g_player.rot.x, g_player.rot.y - D3DX_PI, g_player.rot.z),
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
					2.0f, 4, 40, 20, 10.0f, 40.0f,
					true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

				// Hp�𑝂₷
				g_player.nLife += HEAL_VALUE;

				// �A�C�e�����g�p��Ԃɂ���
				pItem[nIdx].bUse = false;

				// �֐��𔲂���
				return false;
			}

			// �~�j�}�b�v�̃A�C�e���̃A�C�R���̃��Z�b�g
			ResetItemMinimap();

			// ���[�V�������j���[�g�����ɖ߂�
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10);

			// ���y�Đ�
			PlaySound(SOUND_LABEL_ITEM_SE);

			pItem[g_player.ItemIdx].Power = pItemInfo[pItem[nIdx].nType].Power;

			Itemchange(nIdx,pItem[nIdx].nType); // �A�C�e�����E��
			
			pItem[nIdx].bUse = false;      // ����

			if (g_player.Itembreak[g_player.ItemIdx] == true)
			{
				// �A�C�e������ꂽ��������Z�b�g
				g_player.Itembreak[g_player.ItemIdx] = false;
			}

			// ����������Ă�����
			if (g_player.Motion.nNumModel == MAX_MODEL)
			{
				// �A�C�e���ύX���̃p�����[�^
				ChangeItemParam(g_player.ItemIdx, pItem[g_player.ItemIdx].nType);
			}

			// ��Ԃ��z�[���h�ɂ���
			pItem[nIdx].state = ITEMSTATE_HOLD;

			// �A�C�e���̃X�e�[�^�X�ύX
			LoadItemChange(pItem[nIdx].nType,pItem[nIdx].Size.y);

			g_player.ItemIdx = nIdx;	   // �C���f�b�N�X��n��

			LoadMinimapMatItem(g_player.ItemIdx, g_player.StockItemIdx);
		}
	}

	// �͈͊O��������
	if (sphererange(&g_player.pos, &pItem[nIdx].pos, Itemrange, plrange) == false)
	{
		int nIdxBillboard = pItem[nIdx].nIdxBillboardCount;

		// �r���{�[�h������
		pBillboard[nIdxBillboard].state = BILLBOARDSTATE_NOSET;
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

	g_player.nCounterAttack = 30;						  // ��ԃJ�E���^�[��ݒ�
	g_AttackState = AttackState;                          // �R���{�̏��
	g_player.AttackState = PLAYERATTACKSTATE_ATTACK;	  // �v���C���[�̏�Ԃ��U���ɂ���	
	g_player.Combostate = Combstate;					  // �R���{�̏�Ԃ�ݒ�
	SetMotion(&g_player.Motion, motiontype, true,10);

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
		if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 50.0f, 200.0f) && pBoss[nCnt].bUse)
		{
			// �͈͂ɂ���Ԃ������b�N�I��
			if (sphererange(&g_player.pos, &pBoss[nCnt].pos, 150.0f, 150.0f))
			{
				// �p�x�����߂�
				float fAngle = atan2f(pBoss[nCnt].pos.x - g_player.pos.x, pBoss[nCnt].pos.z - g_player.pos.z);
				g_player.rotDestPlayer.y = fAngle + D3DX_PI;
				return;
			}

			// �{�X�̏ꏊ������
			if (g_player.WeponMotion == MOTION_DBHAND && g_player.Motion.motionType == MOTIONTYPE_ACTION2 && GetKeyboardPress(DIK_W))
			{
				g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 80.0f;
				g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 80.0f;
				return;
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
				return;
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
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[LoadPlayer].aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[LoadPlayer].aMotionInfo[nCntMotion]; // ���[�V�����̏�����
	}

	if (LoadPlayer == PLAYERTYPE_WEPON) // �v���C���[���m�[�n���h����������������Ȃ�
	{
		g_player.WeponMotion = itemtype; // �A�C�e�����Ƃ̃��[�V�����^�C�v
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// �U��1�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // �U��2�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// �U��3�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// �U��4�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_JUMPACTION] = g_LoadMotion[itemtype].aMotionInfo[4];	// �U��4�̏�����
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
// ���[�V�����̔���ݒ菈��
//===============================================================================================================
bool CheckMotionBounds(int nKey, int nCountFrame, int StartKey, int EndKey, int startFrame, int EndFrame)
{
	// ����p�ϐ�
	bool bFlag = false;

	if (nKey >= StartKey && nKey <= EndKey && nCountFrame >= startFrame && nCountFrame <= EndFrame)
	{
		// ����J�n
		bFlag = true;
	}

	// �����Ԃ�
	return bFlag;
}
//===============================================================================================================
// �A�^�b�J�[�𒲂ׂ�֐�
//===============================================================================================================
float SetAttackerAngle(int AttackerIdx, int AttackerType)
{
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID || g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH)
	{
		return g_player.rot.y;
	}

	// �p�x
	float OutAngle = 0.0f;

	// �G���擾
	ENEMY* pEnemy = GetEnemy();

	// �{�X���擾
	Boss* pBoss = Getboss();

	// �A�^�b�J�[���G
	if (AttackerType == ATTACKER_ENEMY)
	{
		// ���������߂�
		D3DXVECTOR3 DiffPos = g_player.pos - pEnemy[AttackerIdx].pos;

		// �p�x�����߂�
		OutAngle = atan2f(DiffPos.x, DiffPos.z);
	}
	// �A�^�b�J�[���{�X
	else if (AttackerType == ATTACKER_BOSS)
	{
		// ���������߂�
		D3DXVECTOR3 DiffPos = g_player.pos - pBoss[AttackerIdx].posOld;

		// �p�x�����߂�
		OutAngle = atan2f(DiffPos.x, DiffPos.z);
	}

	return OutAngle;
}
//===============================================================================================================
// �����Ɖ~�̓����蔻��
//===============================================================================================================
bool CollisionLine()
{
	return false;
}
//===============================================================================================================
// �v���C���[�̃��[�V�����̃��[�h
//===============================================================================================================
void LoadMotion(int Weponmotion)
{
	int nIdxPos = 0;
	int nIdxRot = 0;

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

	if (Weponmotion != MOTION_SP)
	{
		nIdxPos = 15;
		nIdxRot = 15;
	}

	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			// ������ǂݎ��
			fscanf(pFile, "%s", &aString[0]);

			// �R�����g���΂�
			if (strcmp(&aString[0], "#") == 0 || strcmp(&aString[0], "<<") == 0)
			{
				continue;
			}

			// �p�[�c�Z�b�g��ǂݎ������
			if (strcmp(&aString[0], "PARTSSET") == 0)
			{
				LoadWeponOffSet(pFile, &aString[0], Weponmotion);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadWeponMotionSet(pFile, &aString[0], 0, Weponmotion);
			}

			// END_SCRIPT��ǂݎ������
			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{

				nKey = 0;
				nCntMotion = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
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
	int nScanData = 0;

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
			int nData = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0)
			{//�R�����g��������
				//������ǂݔ�΂�
				continue;
			}

			// NUM_MODEL��ǂݎ������
			if (strcmp(&aStr[0], "NUM_MODEL") == 0)
			{
				nScanData = fscanf(pFile, "%s", &Skip[0]);						// [=]��ǂݔ�΂�
				nScanData = fscanf(pFile, "%d", &nNumModel);					// ���f���̍ő吔����
				g_LoadPlayer[nType].nNumModel = nNumModel;			// ���f���̍ő吔����
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
			if (nData == EOF)
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
	int nScanData = 0; // �Ԃ�l����p

	// �J�E���g�����f������艺��������
	while (nCntLoadModel < nNumModel)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		// MODEL_FILENAME��ǂݎ������
		if (strcmp(aString, "MODEL_FILENAME") == 0)
		{
			nScanData = fscanf(pFile, "%s", &Skip[0]); // [=]��ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", aString);  // �t�@�C������ǂݎ��

			const char* FILE_NAME = {};    // �t�@�C��������p�ϐ�

			FILE_NAME = aString;           // �t�@�C��������

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(FILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].dwNumMat,
				&g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh);

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nType].aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nType].aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadPlayer[nType].aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//���_���̎擾
			nNumVtx = g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.x = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.x - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.x;
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.y = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.y - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.y;
			g_LoadPlayer[nType].aModel[nCntLoadModel].Size.z = g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMax.z - g_LoadPlayer[nType].aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_LoadPlayer[nType].aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

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
	int nScanData = 0; // �Ԃ�l��������ϐ�

	// END_CHARACTERSET��ǂݎ���ĂȂ�������
	while (strcmp(aString, "END_CHARACTERSET") != 0)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		// INDEX��ǂݎ������
		if (strcmp(aString, "INDEX") == 0)
		{
			nScanData = fscanf(pFile, "%s", &Skip[0]); // [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%d", &nIdx);    // �C���f�b�N�X����
		}
		// PARENT��ǂݎ������
		else if (strcmp(aString, "PARENT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �e�̃C���f�b�N�X��ۑ�
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// �v���C���[�̕���̃I�t�Z�b�g�̐ݒ菈��
//===============================================================================================================
void LoadWeponOffSet(FILE* pFile, char* aString, int nWepontype)
{
	int nIdx = 15; // �C���f�b�N�X�i�[�ϐ�
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0; // �Ԃ�l����p

	// END_CHARACTERSET��ǂݎ���ĂȂ�������
	while (strcmp(aString, "END_PARTSSET") != 0)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		// POS��ǂݎ������
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// �v���C���[�̃��[�V�����Z�b�g
//===============================================================================================================
void LoadMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0; // �Ԃ�l����p

	while (1)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// LOOP��ǂݒʂ�����
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩���Ȃ���
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].nNumkey);
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
	int nScanData = 0; // �Ԃ�l����p

	while (1)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// FRAME��ǂݒʂ�����
		if (strcmp(aString, "FRAME") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �t���[����ǂݍ���
			nScanData = fscanf(pFile, "%d", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadPlayer[nType].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
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
//===============================================================================================================
// �v���C���[�̕��킲�Ƃ̃��[�V�����̃��[�h����
//===============================================================================================================
void LoadWeponMotionSet(FILE* pFile, char* aString, int nNumModel, int nWepontype)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0; // �Ԃ�l����p

	while (1)
	{
		// ������ǂݎ��
		nScanData = fscanf(pFile, "%s", aString);

		if (strcmp(aString, "#") == 0 || strcmp(aString, "<<") == 0)
		{// �R�����g��������
			// �R�����g��ǂݔ�΂�
			continue;
		}

		// LOOP��ǂݒʂ�����
		if (strcmp(aString, "LOOP") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩���Ȃ���
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].bLoop);
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].nNumkey);
		}
		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadWeponKeySet(pFile, aString, nWepontype, nCntMotion);
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
// �v���C���[�̕���̂��ƃ��[�V�����̃L�[�̃��[�h����
//===============================================================================================================
void LoadWeponKeySet(FILE* pFile, char* aString, int nWepontype, int nCntMotion)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nCntPos = 0;   // �ʒu�̃J�E���g
	int nCntRot = 0;   // �p�x�̃J�E���g
	int nScanData = 0; // �Ԃ�l����p

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
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �t���[����ǂݍ���
			nScanData = fscanf(pFile, "%d", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadMotion[nWepontype].aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntRot].fRotZ);
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
//===============================================================================================================
// �v���C���[�̃G�t�F�N�g�̐ݒ菈��
//===============================================================================================================
void SetElementEffect(void)
{
	D3DXVECTOR3 SwordPos(
		g_player.SwordMtx._41, // X����
		g_player.SwordMtx._42, // Y����
		g_player.SwordMtx._43  // Z����
	);

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
}
//===============================================================================================================
// �v���C���[�̃��[�V�����̉��o�ݒ�
//===============================================================================================================
void SetMotionContller(void)
{
	// �L�[
	int nKey = g_player.Motion.nKey;

	// ���[�V�����J�E���g
	int nCounter = g_player.Motion.nCountMotion;

	// ����̃X�y�V�����U���̎�
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPDOUBLE && g_player.Motion.motionType == MOTIONTYPE_ACTION && CheckMotionBounds(nKey, nCounter, 2, 2, 5, 5) == true)
	{
		// �R���g���[���[�̐U��(1000�ň�b)
		SetVibration(35000, 35000, 500);

		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, COLOR_ORANGERED, 32, 60.0f, 15.0f, 3.0f, 60, IMPACTTYPE_PLAYER,20);

		// �J�����̗h��
		WaveCamera(25);
	}

	// ����̃X�y�V�����U���̎�
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_ONEHANDBLOW && CheckMotionBounds(nKey, nCounter, 4, 4, 5, 5) == true)
	{
		// �R���g���[���[�̐U��(1000�ň�b)
		SetVibration(45000, 45000,300);

		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 32, 60.0f, 15.0f, 10.0f, 60, IMPACTTYPE_PLAYER, g_player.nDamage * 50);

		// �J�����̗h��
		WaveCamera(25);
	}

	// ���̃X�y�V�����U���̎�
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, 21, 21, 5, 5) == true)
	{
		// �R���g���[���[�̐U��(1000�ň�b)
		SetVibration(45000, 45000, 300);

		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 32, 60.0f, 15.0f, 3.0f, 60, IMPACTTYPE_PLAYER, 20);

		// �J�����̗h��
		WaveCamera(25);
	}

	// ���̃X�y�V�����U���̎�
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SPPIERCING && g_player.Motion.nKey != NULL)
	{
		int SetKey = 4;

		// �����L�[
		if (g_player.Motion.nKey % SetKey== 0 && g_player.Motion.nKey <= 16)
		{
			// �Ռ��g�𔭐��w����
			SetImpact(g_player.pos, D3DCOLOR_RGBA(100,100,100,255), 32, 30.0f, 20.0f, 3.0f, 60, IMPACTTYPE_NORMAL, 0);
		}

		if (CheckMotionBounds(nKey, nCounter, 21, 21, 0, 0) == true)
		{
			// �J�����̗h��
			WaveCamera(25);
		}
	}

	// ���̃X�y�V����
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, 0, 0, 0, 0) == true)
	{
		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, D3DCOLOR_RGBA(0, 161, 255, 255), 32, 200.0f, 180.0f, 1.6f, 90, IMPACTTYPE_SPKATANA, 1);
	}

	// ���̃X�y�V����
	if (g_player.AttackSp == true && g_player.WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, 4, 4, 1, 1) == true)
	{
		// �R���g���[���[�̐U��(1000�ň�b)
		SetVibration(45000, 45000, 300);

		// �Ռ��g�𔭐��w����
		SetImpact(g_player.pos, D3DCOLOR_RGBA(0, 161, 255, 255), 32, 40.0f, 20.0f, 35.0f, 60, IMPACTTYPE_NORMAL, 1);
	}

	// ���[�V�����̎�ނ�����
	if (g_player.Motion.motionType == MOTIONTYPE_MOVE && g_player.Motion.bFirstMotion == false)
	{
		// �L��1�Ԗڂ��J�E���g��5
		if (CheckMotionBounds(nKey, nCounter, 1, 1, 5, 5) == true)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[14].mtxWorld._41, g_player.Motion.aModel[14].mtxWorld._42, g_player.Motion.aModel[14].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
		// �L��3�Ԗڂ��J�E���g��5
		else if (CheckMotionBounds(nKey, nCounter, 3, 3, 5, 5) == true)
		{
			SetExplosion(D3DXVECTOR3(g_player.Motion.aModel[11].mtxWorld._41, g_player.Motion.aModel[11].mtxWorld._42, g_player.Motion.aModel[11].mtxWorld._43),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 20.0f, 20.0f, 0);
		}
	}

	// ���[���h�}�g���b�N�X��ϊ�
	D3DXVECTOR3 SwordPos = SetMtxConversion(g_player.SwordMtx);

	// �n���}�[�̃X�y�V�������[�V�����̐ݒ�
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
			2.0f, 4, 30, 40, 4.0f, 0.0f,
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
			6.0f, 4, 80, 300, 6.0f, 30.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}	//���[�V�����̍X�V

	Boss* pBoss = Getboss();

	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DAMAGE && g_player.Motion.bFirstMotion == true)
	{
		g_player.move.x = sinf(pBoss[g_player.AttackerIdx].rot.y + D3DX_PI) * 15.0f;
		g_player.move.y = 5.0f;
		g_player.move.z = cosf(pBoss[g_player.AttackerIdx].rot.y + D3DX_PI) * 15.0f;
	}

	// �_���[�W���[�V������������
	const bool isDamage = g_player.Motion.motiontypeBlend == MOTIONTYPE_DAMAGE && nKey <= 2;

	// �_���[�W��������d������
	g_player.bstiffness = isDamage ? true : false;
	
	// �_���[�W���[�V������������
	const bool isAvoid = g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && nKey <= 3;

	// ���G���Ԃ������疳�G�ɂ���
	g_player.bAvoid = isAvoid ? true : false;

	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_LANDING && CheckMotionBounds(nKey,nCounter,0,0,1,1) == true)
	{
		// ��
		LoadEffect(0, SetMtxConversion(g_player.Motion.aModel[11].mtxWorld));
	}

	// ��^���탂�[�V�����̎�
	if (g_player.WeponMotion == MOTION_BIGWEPON && g_player.Motion.motionType == MOTIONTYPE_ACTION4 && g_player.Motion.nKey == 0 && GetKeyboardPress(DIK_W))
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 30.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 30.0f;
	}
}
//===============================================================================================================
// �v���C���[�̈ړ�����
//===============================================================================================================
void PlayerMove(void)
{
	Camera* pCamera = GetCamera();

	// �W�����v���Ă邩�𔻒�
	const bool NotJump = g_player.Motion.motiontypeBlend != MOTIONTYPE_JUMP;

	// ���[�V���������[�u���𔻒�
	const bool NotMove = g_player.Motion.motiontypeBlend != MOTIONTYPE_MOVE;

	// �������邩�𔻒�
	const bool is_MotionMove = NotJump == true && NotMove == true;

	// ���[�V������������L�[��3�ȉ�
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_THREE)
	{
		return;
	}

	if (GetKeyboardPress(DIK_A))
	{
		//�v���C���[�̈ړ�(��)
		if (GetKeyboardPress(DIK_W) == true)
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S))
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(��)
		else
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
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
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S))
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(�E)
		else
		{
			if (is_MotionMove == true)
			{
				SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
			}

			g_player.move.z -= sinf(pCamera->rot.y) * g_player.speed;
			g_player.move.x += cosf(pCamera->rot.y) * g_player.speed;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (is_MotionMove == true)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
		}

		g_player.move.x += sinf(pCamera->rot.y) * g_player.speed;
		g_player.move.z += cosf(pCamera->rot.y) * g_player.speed;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (is_MotionMove == true)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_MOVE, true, 5);
		}

		g_player.move.x -= sinf(pCamera->rot.y) * g_player.speed;
		g_player.move.z -= cosf(pCamera->rot.y) * g_player.speed;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 5); // ���[�V�������j���[�g�����ɂ���
		}
	}

}
//===============================================================================================================
// �v���C���[�̃N���t�g����
//===============================================================================================================
void UpdatePlayerCraft(void)
{
	Item* pItem = GetItem();

	// �N���t�g��Ԃ���Ȃ�������
	if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && g_player.bCraft == false && g_player.AttackSp == false)
	{
		// �N���t�g���
		g_player.bCraft = true;

		// �N���t�g��Ԃɂ���
		EnableCraft(true);

		EnableCraftIcon(pItem[g_player.ItemIdx].nType, pItem[g_player.StockItemIdx].nType);
	}
	// �N���t�g��Ԃ�������
	else if ((KeyboardTrigger(DIK_TAB) || JoypadTrigger(JOYKEY_Y)) && g_player.bCraft == true && g_player.AttackSp == false && GetIconAnim() == false)
	{
		// �N���t�g���
		g_player.bCraft = false;

		// �N���t�g��Ԃɂ��Ȃ�
		EnableCraft(false);
	}
}
//===============================================================================================================
// �A�C�e���̔j�󏈗�
//===============================================================================================================
void DestroyWepon(void)
{
	Item* pItem = GetItem();

	// ����������Ă��邩�v���C���[�̎����Ă���A�C�e������ꂽ
	if (g_player.Motion.nNumModel == MAX_MODEL && g_player.Itembreak[g_player.ItemIdx] == true)
	{
		SetGameUI(D3DXVECTOR3(125.0f, 500.0f, 0.0f), UITYPE_DESTORY, 100.0f, 25.0f,true, 240);

		// �u�����h�Ȃ��Ńj���[�g�����ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);

		// �f��̎��̃��[�V����������
		for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		{
			g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // ���f���̏�����
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		{
			g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
		}

		StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;

		// ��Ԃ�ύX
		pItem[g_player.ItemIdx].state = ITEMSTATE_NORMAL;

		// ���[�V�������j���[�g�����ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10); // ���[�V�������j���[�g�����ɂ���
	}
}
//===============================================================================================================
// �A�C�e���̃h���b�v����
//===============================================================================================================
void DropItem(void)
{
	Item* pItem = GetItem();

	// �v���C���[������������Ă��邩�𔻒�
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// �v���C���[������ł��Ȃ����𔻒�
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// �v���C���[���X�y�V�����U�����Ă��Ȃ����𔻒�
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// �v���C���[���U�����ĂȂ����𔻒�
	const bool is_NotAction = g_player.Combostate == COMBO_NO;

	// �v���C���[���W�����v���ĂȂ����𔻒�
	const bool is_NotJumping = g_player.bJump == true;

	// ����u���邩�u���Ȃ����𔻒�
	const bool is_DropItem = is_HaveWepon == true && is_Alive == true && is_NotSpAttack == true && is_NotAction == true && is_NotJumping == true;

	// �����Ă���A�C�e����u������
	if ((KeyboardTrigger(DIK_G) || JoypadTrigger(JOYKEY_B)) && is_DropItem == true)
	{
		// �u�����h�Ȃ��Ńj���[�g�����ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);

		StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;

		pItem[g_player.ItemIdx].bUse = true;
		pItem[g_player.ItemIdx].pos = g_player.pos;

		pItem[g_player.ItemIdx].state = ITEMSTATE_RELEASE;
		pItem[g_player.ItemIdx].nCounterState = 20;
	}
}
//===============================================================================================================
// �X�y�V�����A�^�b�N�̏���
//===============================================================================================================
void HandleSpecialAttack(void)
{
	Item* pItem = GetItem();

	// ����������Ă��邩�𔻒�
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// �������������Ă��Ȃ����𔻒�
	const bool is_NotHaveThrowItem = g_player.HandState != PLAYERHOLD_HOLD;

	// �������Ă��邩�𔻒�
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// �_���[�W��Ԃ�
	const bool is_Damage = g_player.bstiffness == false;

	// �X�y�V�������[�V�����𔭓��ł��邩�𔻒�
	const bool is_SpecialAttack = is_HaveWepon && is_NotHaveThrowItem && is_Alive && is_Damage && g_player.Motion.motiontypeBlend != MOTIONTYPE_AVOID;

	// �X�y�V�������[�h�ɂȂ������̍U��
	if ((KeyboardTrigger(DIK_Q) || JoypadTrigger(JOYKEY_LS) || JoypadTrigger(JOYKEY_RS)) && is_SpecialAttack == true)
	{
		if (g_player.Combostate == COMBO_NO && g_player.AttackSp == false && g_player.SpMode == true)                   // SP�Q�[�W�����܂���
		{
			ResetMeshSword(); //���̋O�Ղ̃��Z�b�g
			DecSpgauge(100.0f);
			g_player.AttackSp = true;          // SP�Z�𔭓����Ă���

			// �u�����h�Ȃ��Ńj���[�g�����ɂ���
			SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

			switch (g_player.WeponMotion)
			{
			case MOTION_KATANA:
				WaveCamera(120); // �J������h�炷
				g_player.SwordOffpos.y = 250.0f;
				MotionChange(MOTION_SP, 0);
				SetGameUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), UITYPE_BLACK, 640.0f, 380.0f,false, 0);
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
				g_player.SwordOffpos.y = 100.0f;
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

	// �Ō�̃L�[
	int LastKey = g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION].nNumkey - 1;

	// �X�y�V�����U����Ԃ��𔻒�
	const bool is_SpAttack = g_player.AttackSp == true;

	// �Ō�̃L�[���𔻒�
	const bool is_LastKey = g_player.Motion.nKey >= LastKey;

	// ���[�V�������I��������ǂ����𔻒�
	const bool is_FinishMotion = g_player.Motion.bFinishMotion == true;

	// �u�����h���I��������ǂ����𔻒�
	const bool is_FinishBlend = g_player.Motion.nCounterBlend >= g_player.Motion.nFrameBlend * 0.5f;

	// ���Ƃɖ߂��邩�𔻒�
	const bool is_restoreMotion = is_SpAttack && is_LastKey && is_FinishMotion && is_FinishBlend;

	// �X�y�V�������[�V����������Ƃɖ߂�
	if (is_restoreMotion == true)
	{
		// ���y�Đ�
		PlaySound(SPUND_LABEL_WEPONBREAK);

		g_player.SwordOffpos.y = 65.0f;		// ����̒�����߂�
		g_player.Itembreak[g_player.ItemIdx] = true;
		//StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50); //�\�͒l��߂�
		g_player.AttackSp = false;
		pItem[g_player.ItemIdx].state = ITEMSTATE_RELEASE;
		pItem[g_player.ItemIdx].nCounterState = 60;
	}
}
//===============================================================================================================
// �v���C���[�̃A�C�e���̃X�g�b�N����
//===============================================================================================================
void UpdateItemStock(void)
{
	Item* pItem = GetItem();

	// �������Ă��邩�𔻒�
	const bool is_Alive = g_player.Motion.motionType != MOTIONTYPE_DEATH;

	// �X�y�V�����U�����Ă邩�𔻒�
	const bool is_NotSpAttack = g_player.AttackSp == false;

	// ����������Ă��邩�𔻒�
	const bool is_HaveWepon = g_player.Motion.nNumModel == MAX_MODEL;

	// �_���[�W��Ԃ�
	const bool is_Damage = g_player.bstiffness == false;

	// �A�C�e�����X�g�b�N�ł��邩�𔻒�
	const bool is_StockItem = is_Alive && is_NotSpAttack && is_HaveWepon && is_Damage;

	// �A�C�e���̃X�g�b�N
	if ((KeyboardTrigger(DIK_F) || JoypadTrigger(JOYKEY_RIGHT_B)) && is_StockItem == true && CheckActionMotion(&g_player.Motion) == true)
	{// F�L�[ or RB�{�^��

		// �~�j�}�b�v�̃A�C�e���̃��Z�b�g
		ResetItemMinimap();

		// �~�j�}�b�v�̃A�C�e���̃��[�h
		LoadMinimapMatItem(g_player.ItemIdx, g_player.StockItemIdx);

		// �u�����h�Ȃ��Ńj���[�g�����ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, false, 10);

		if (pItem[g_player.StockItemIdx].state == ITEMSTATE_STOCK)
		{
			ChangeItemParam(g_player.StockItemIdx, pItem[g_player.StockItemIdx].nType);
		}

		// ���Ƃ��ƃX�g�b�N���Ă����A�C�e�����m�[�}���ɖ߂�
		pItem[g_player.StockItemIdx].state = ITEMSTATE_RELEASE;

		// �����Ă���A�C�e���̏�Ԃ��X�g�b�N�ɂ���
		pItem[g_player.ItemIdx].state = ITEMSTATE_STOCK;

		// �X�g�b�N���ꂽ�A�C�e���̃C���f�b�N�X��n��
		g_player.StockItemIdx = g_player.ItemIdx;

		// �v���C���[�̑��x�����Z�b�g
		StatusChange(3.5f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

		// ���[�V����������ɂ���(��2������1������)
		MotionChange(MOTION_DBHAND, 1);
		//// �f��̎��̃��[�V����������
		//for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
		//{
		//	g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // ���f���̏�����
		//}
		//for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
		//{
		//	g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
		//}

		// ��������ɕ��������
		g_player.Motion.nNumModel -= 1;

		// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
		g_player.HandState = PLAYERHOLD_NO;
	}
}
//===============================================================================================================
// �v���C���[�̉������
//===============================================================================================================
void UpdatePlayerAvoid(void)
{
	Camera* pCamera = GetCamera();
	
	// �C�[�W���O�̃J�E���g
	static int avoidEaseCnt = 0;

	// �C���N�������g
	avoidEaseCnt++;

	// �C�[�W���O��ݒ�
	float time = SetEase(avoidEaseCnt, 20);

	// ������[�V�������𔻒�
	const bool NotAvoid = g_player.Motion.motiontypeBlend != MOTIONTYPE_AVOID;

	//// ���[�V�������I��������ǂ����𔻒�
	//const bool NotFinish = g_player.Motion.bFinishMotion == false;

	// ������[�V�����𔭓��ł��邩�𔻒�
	const bool CanAvoid = NotAvoid == true && g_player.AttackSp == false && g_player.nLife > 0;

	// ���[�V�������������Ȃ�
	if ((OnMouseTriggerDown(RIGHT_MOUSE) == true || JoypadTrigger(JOYKEY_B) == true) && CanAvoid == true)
	{
		// ���y�Đ�
		PlaySound(SOUND_LABEL_AVOIDSE);

		// ���G
		g_player.bAvoid = true;
		g_player.bstiffness = false;
		g_player.avoidMove = SetMotionMoveAngle();

		avoidEaseCnt = 0;

		// ���[�V����������ɂ���
		SetMotion(&g_player.Motion, MOTIONTYPE_AVOID, true, 10);
	}

	// ���[�V�����̎�ނ������������
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID && g_player.Motion.nKey <= KEY_TWO)
	{	
		// �ړ��ʂ����񂾂�߂Â���
		g_player.move.x += (g_player.avoidMove.x - g_player.move.x) * EaseInOutQuad(time);
		g_player.move.z += (g_player.avoidMove.z - g_player.move.z) * EaseInOutQuad(time);
	}
	else
	{
		// 0�ɋ߂Â���
		g_player.move.x += (0.0f - g_player.move.x) * 0.0001f;
		g_player.move.z += (0.0f - g_player.move.z) * 0.0001f;

		// �C�[�W���O�̃J�E���g�����Z�b�g
		avoidEaseCnt = 0;
	}
}
//===============================================================================================================
// ���[�V�����̃A���O���̐ݒ�
//===============================================================================================================
D3DXVECTOR3 SetMotionMoveAngle(void)
{
	static D3DXVECTOR3 OutPutMove = D3DXVECTOR3(0.0f,0.0f,0.0f);
	XINPUT_STATE* pStick;

	pStick = GetJoyStickAngle();

	Camera* pCamera = GetCamera();

	// �p�b�h����͂��ĂȂ�������
	if (GetJoyStick() == false)
	{
		if (GetKeyboardPress(DIK_A))
		{
			//�v���C���[�̈ړ�(��)
			if (GetKeyboardPress(DIK_W) == true)
			{
				OutPutMove.x = sinf(pCamera->rot.y - D3DX_PI * 0.25f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y - D3DX_PI * 0.25f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
			}
			//�v���C���[�̈ړ�(��)
			else if (GetKeyboardPress(DIK_S))
			{
				OutPutMove.x = sinf(pCamera->rot.y - D3DX_PI * 0.75f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y - D3DX_PI * 0.75f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
			}
			//�v���C���[�̈ړ�(��)
			else
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.5f) * -AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.5f) * -AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
			}
		}
		//�v���C���[�̈ړ�(�E)
		else if (GetKeyboardPress(DIK_D))
		{
			//�v���C���[�̈ړ�(��)
			if (GetKeyboardPress(DIK_W))
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.25f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.25f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
			}
			//�v���C���[�̈ړ�(��)
			else if (GetKeyboardPress(DIK_S))
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.75f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.75f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
			}
			//�v���C���[�̈ړ�(�E)
			else
			{
				OutPutMove.x = sinf(pCamera->rot.y + D3DX_PI * 0.5f) * AVOID_MOVE;
				OutPutMove.z = cosf(pCamera->rot.y + D3DX_PI * 0.5f) * AVOID_MOVE;

				g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
			}
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_W) == true)
		{
			OutPutMove.x = sinf(pCamera->rot.y) * AVOID_MOVE;
			OutPutMove.z = cosf(pCamera->rot.y) * AVOID_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			OutPutMove.x = sinf(pCamera->rot.y) * -AVOID_MOVE;
			OutPutMove.z = cosf(pCamera->rot.y) * -AVOID_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y;
		}
		else
		{
			OutPutMove.x = sinf(g_player.rot.y + D3DX_PI) * AVOID_MOVE;
			OutPutMove.z = cosf(g_player.rot.y + D3DX_PI) * AVOID_MOVE;
		}
	}

	// �p�b�h���g���Ă��邩
	if (GetJoyStick() == true)
	{
		// L�X�e�B�b�N�̊p�x
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// �f�b�h�]�[��
		float deadzone = 10920;

		// �X�e�B�b�N�̌X�����p�x�����߂�
		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

		// ��������
		if (magnitude > deadzone)
		{
			// �A���O���𐳋K��
			float normalizeX = (LStickAngleX / magnitude);
			float normalizeY = (LStickAngleY / magnitude);

			// �v���C���[�̈ړ���
			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

			// �v���C���[�̖ړI�̊p�x�����߂�
			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

			// �v���C���[�̈ړ�
			OutPutMove.x = sinf(g_player.rotDestPlayer.y + D3DX_PI) * AVOID_MOVE;
			OutPutMove.z = cosf(g_player.rotDestPlayer.y + D3DX_PI) * AVOID_MOVE;
		}
	}

	// �ړ��ʂ�Ԃ�
	return OutPutMove;
}
//===============================================================================================================
// ���킲�Ƃ̃G�t�F�N�g����
//===============================================================================================================
void SetWeponEffect(void)
{
}
//===============================================================================================================
// �_���[�W�A�N�V�������ǂ���
//===============================================================================================================
bool IsDamageAction(void)
{
	return false;
}
//===============================================================================================================
// �A�C�e���̕ύX�̃p�����[�^�\��
//===============================================================================================================
void ChangeItemParam(int nHaveIdx,int nType)
{
	Item* pItem = GetItem();

	MODEL_INFO* ItemTexture = GetItemOrigin();

	pItem[nHaveIdx].bUse = true;
	pItem[nHaveIdx].ItemTex[nType] = ItemTexture[nType];
	pItem[nHaveIdx].pos.x = g_player.pos.x + (float)(rand() % 50 - 25.0f);
	pItem[nHaveIdx].pos.z = g_player.pos.z + (float)(rand() % 50 - 25.0f);
	pItem[nHaveIdx].state = ITEMSTATE_RELEASE;
	pItem[nHaveIdx].nCounterState = 60;
}
//===============================================================================================================
// ���[�V�����^�C�v�̑J��
//===============================================================================================================
void MotionTransition(void)
{
	// ���[�V�����^�C�v�̑J��
	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_ACTION:
		SetElementEffect();
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	case MOTIONTYPE_ACTION2:
		SetElementEffect();
		break;
	case MOTIONTYPE_ACTION3:
		SetElementEffect();
		break;
	case MOTIONTYPE_ACTION4:
		SetElementEffect();
		break;
	default:
		break;
	}
}
//===============================================================================================================
// ��Ԃ̑J��
//===============================================================================================================
void StateTransition(void)
{
	// �v���C���[�̏�Ԃ̑J��
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
}
//===============================================================================================================
// �R���{��Ԃ̑J��
//===============================================================================================================
void ComboTransition(void)
{
	GAMESTATE gameState = GetGameState();

	// ���[�r�[���ᖳ��������
	if (gameState != GAMESTATE_MOVIE)
	{
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
	}

}
//===============================================================================================================
// �v���C���[�̍U���̐ݒ菈��
//===============================================================================================================
void SetUpPlayerAttack(void)
{
	GAMESTATE gamestate = GetGameState();

	// �Q�[���̏�Ԃ����[�r�[��������
	if (gamestate == GAMESTATE_MOVIE) return;

	// ���[�V�����������������
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_AVOID) return;

	// �X�y�V�����U������������
	if (g_player.AttackSp == true) return;

	// ���[�V���������S��������
	if (g_player.Motion.motiontypeBlend == MOTIONTYPE_DEATH) return;

	// ���[�V�����̃L�[
	int nKey = g_player.Motion.nKey;

	// ���[�V�����̃J�E���^�[
	int nCounter = g_player.Motion.nCountMotion;

	// ���[�V�����̍Ō�̃L�[
	int EndKey = g_player.Motion.aMotionInfo[g_player.Motion.motionType].nNumkey - 1;

	// ���[�V�����̍Ō�̃t���[��
	int EndFrame = g_player.Motion.aMotionInfo[g_player.Motion.motionType].aKeyInfo[EndKey].nFrame;

	// �U���ł��邩����
	const bool isAttack = bNohand == false && g_player.bJump == true;

	// �v���C���[�̏�Ԃ��U������Ȃ����n�ʂɂ���
	if (isAttack == true)
	{
		if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && (g_player.Combostate == COMBO_NO || g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMPACTION))
		{
			PlayerComb(MOTIONTYPE_ACTION, 40, 40, COMBO_ATTACK1); // �R���{1
			ResetMeshSword();
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION &&
			CheckMotionBounds(nKey, nCounter, 0, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 40, 40, COMBO_ATTACK2); // �R���{2
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION2 &&
			CheckMotionBounds(nKey, nCounter, 1, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 40, 40, COMBO_ATTACK3); // �R���{3
		}
		else if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && g_player.Motion.motiontypeBlend == MOTIONTYPE_ACTION3 &&
			CheckMotionBounds(nKey, nCounter, 1, EndKey, 0, EndFrame) == true)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 70, 40, COMBO_ATTACK4); // �R���{4
		}
	}

	// �W�����v�U���ł��邩����
	const bool isJumpAttack = bNohand == false && g_player.bJump == false && g_player.Combostate == COMBO_NO;

	if (isJumpAttack == true && (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)))
	{
		ResetMeshSword();
		PlayerComb(MOTIONTYPE_JUMPACTION, 70, 70, COMBO_ATTACK4);
	}

	// �U�����Ă��Ȃ�����ɓ��I����������Ă���
	const bool canAction = g_player.Combostate == COMBO_NO && bNohand == true && g_player.Motion.nNumModel == MAX_MODEL;

	// �������������Ă���Ƃ��̍U��
	if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X)) && canAction == true)
	{
		PlayerComb(MOTIONTYPE_ACTION, 40, 20, COMBO_ATTACK1); // �R���{1
	}

	// �W�����v�A�N�V�����̐ݒ�
	SetUpJumpAction(nKey, nCounter, EndKey, EndFrame);
}
//===============================================================================================================
// �W�����v�A�N�V�����̐ݒ�
//===============================================================================================================
void SetUpJumpAction(int nKey,int nCounter,int nLastKey,int EndFrame)
{
	// �W�����v���[�V������
	const bool isJumpMotion = g_player.Motion.motiontypeBlend == MOTIONTYPE_JUMPACTION && g_player.Motion.bFirstMotion == false;

	// ���̃W�����v���[�V������
	const bool isKatanaMotion = g_player.WeponMotion == MOTION_KATANA && isJumpMotion;

	if (isKatanaMotion && nKey == 0 && nCounter <= 5)
	{
		g_player.move.y = 5.0f;
	}

	// ���蕐��̃W�����v���[�V������
	const bool isDouble = g_player.WeponMotion == MOTION_DBHAND && isJumpMotion;

	if (isDouble && nKey == 0 && nCounter <= 5)
	{
		g_player.move.y = 5.0f;
	}
	else if (isDouble && nKey >= 1 && nKey <= 4)
	{
		g_player.move.y = -MAX_GLABITY * 0.2f;
	}

	// �����핐��̃W�����v���[�V������
	const bool isSpear = g_player.WeponMotion == MOTION_PIERCING && isJumpMotion;

	if (isSpear && nKey >= 0)
	{
		g_player.move.y = -MAX_GLABITY * 0.5f;
	}
	if (isSpear && nKey == 1)
	{
		g_player.move.x = sinf(g_player.rot.y + D3DX_PI) * 20.0f;
		g_player.move.z = cosf(g_player.rot.y + D3DX_PI) * 20.0f;
	}

	// ��^����̃W�����v���[�V������
	const bool isBigWepon = g_player.WeponMotion == MOTION_BIGWEPON && isJumpMotion;

	if (isBigWepon && nKey == 0 && nCounter == 1)
	{
		g_player.move.y = 10.0f;
	}
	else if (isBigWepon && nKey == 1 && nCounter == 5)
	{
		g_player.move.y = 10.0f;
	}

	// ��^����̃W�����v���[�V������
	const bool isoneHand = g_player.WeponMotion == MOTION_ONE_HAND && isJumpMotion;

	if (isoneHand && nKey <= nLastKey)
	{
		g_player.move.y = -MAX_GLABITY * 0.5f;
	}
}
//===============================================================================================================
// �v���C���[�̏�������̐ݒ�
//===============================================================================================================
void SetUpPlayerFirstWepon(void)
{
	Item* pItem = GetItem();
	ITEM_INFO* pItem_Info = GetItemInfo();

	//// �A�C�e���̎��
	//int nType = pItem[g_player.ItemIdx].nType;

	//// �ő�̑ϋv�͂���
	//pItem[g_player.ItemIdx].Maxdurability = pItem_Info[nType].Maxdurability;

	//// �ϋv�͂���
	//pItem[g_player.ItemIdx].durability = pItem_Info[nType].durability;

	//// �A�C�e���̑��������� 
	//g_player.nElement = pItem_Info[nType].nElement; 

	// ���[�V����������ɂ���(��2������1������)
	MotionChange(MOTION_DBHAND, 1);

	// �f��̎��̃��[�V����������
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[1].aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[1].aMotionInfo[nCntMotion];
	}

	StatusChange(4.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f), 50);

	// ��������ɕ��������
	g_player.Motion.nNumModel -= 1;

	// �v���C���[�̏�Ԃ����������Ă��Ȃ���Ԃɂ���
	g_player.HandState = PLAYERHOLD_NO;

	// ���[�V�������j���[�g�����ɂ���
	SetMotion(&g_player.Motion, MOTIONTYPE_NEUTRAL, true, 10); // ���[�V�������j���[�g�����ɂ���
}
//===============================================================================================================
// �v���C���[�̃t�B�[�o�[�^�C���̐ݒ�
//===============================================================================================================
void SetFeverTime(void)
{
	// �t�H�[�o�[���[�h
	if (g_player.FeverMode == true)
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

	static int FiverCnt = 0; // �񐔐���

	if (g_player.FeverMode == true && FiverCnt == 0 && g_player.AttackSp == false)
	{
		SetGameUI(D3DXVECTOR3(620.0f, 360.0f, 0.0f), UITYPE_SYUTYUSEN, 660.0f, 380.0f, false, 0);
		SetGameUI(D3DXVECTOR3(640.0f, 600.0f, 0.0f), UITYPE_FIVER, 100.0f, 40.0f, false, 0);
		FiverCnt = 1; // �����񐔂𒴂���
	}
	if (g_player.FeverMode == false)
	{
		FiverCnt = 0; // �����񐔂����Z�b�g
	}

}
//===============================================================================================================
// �~�j�}�b�v�̍����̃A�C�e���̍ޗ��̕\��
//==============================================================================================================
void LoadMinimapMatItem(int nHaveIdx, int nStockIdx)
{
	// �A�C�e���̎擾
	Item* pItem = GetItem();

	FILE* pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen("data\\ITEM\\SetminimapItem.txt", "r");

	int playeritem = -1;
	int minimapItem = -2;

	// �t�@�C�����J������
	if (pFile != NULL)
	{
		char aString[MAX_WORD] = {};
		char skip[10] = {};

		while (1)
		{
			// ������ǂݎ��
			int nData = fscanf(pFile, "%s", &aString[0]);

			// PLAYER_ITEM��ǂݎ������
			if (strcmp(&aString[0], "PLAYER_ITEM") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �v���C���[�̃A�C�e����ǂݎ��
				nData = fscanf(pFile, "%d", &playeritem);
			}

			// MINIMAP_ITEM��ǂݎ������
			if (strcmp(&aString[0], "MINIMAP_ITEM") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �\������A�C�e����ǂݎ��
				nData = fscanf(pFile, "%d", &minimapItem);

				// �����Ă���A�C�e������v���Ă�����
				if (playeritem == pItem[nHaveIdx].nType || playeritem == pItem[nStockIdx].nType)
				{
					playeritem = -1;
					break;
				}
			}

			// EOF��ǂݎ������
			if (nData == EOF)
			{
				playeritem = -1;
				minimapItem = -2;

				// �t�@�C�������
				fclose(pFile);

				// while���𔲂���
				return;
			}
		}

	}
	else
	{
		// ���b�Z�[�W�{�b�N�X��\��
		MessageBox(NULL, "�t�@�C�����J���܂���", "LoadMinimapItem", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);

	// �A�C�e������
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// ���g�p��������
		if (pItem[nCntItem].bUse == false) continue;

		// �����Ă���A�C�e���Ɠ�����������
		if (nCntItem == nHaveIdx) continue;

		// �X�g�b�N���Ă���A�C�e���Ɠ�����������
		if (nCntItem == nStockIdx) continue;

		// �\���������A�C�e������Ȃ�������
		if (pItem[nCntItem].nType != minimapItem) continue;

		// �~�j�}�b�v�Ɉʒu��ݒ�
		pItem[nCntItem].nMinimapIdx = SetMiniMap(pItem[nCntItem].pos, MINIMAPTEX_ITEM);

		// �~�j�}�b�v�Ɉʒu��ݒ�
		SetMiniMapPotision(pItem[nCntItem].nMinimapIdx, &pItem[nCntItem].pos);
	}

	// ������
	minimapItem = -2;
}
//===============================================================================================================
// �v���C���[���U����Ԃ�
//===============================================================================================================
bool isPlayerAttaking(void)
{
	// �ŏ��̃u�����h���I�������
	const bool NotFirstBlend = g_player.Motion.bFirstMotion == false;

	// ���[�V�������I����Ė���������
	const bool NotFinishMotion = g_player.Motion.bFinishMotion == false;

	// ���[�V�����^�C�v1���A�N�V������������
	const bool isAttack1 = g_player.Motion.motionType == MOTIONTYPE_ACTION;

	// ���[�V�����^�C�v2���A�N�V������������
	const bool isAttack2 = g_player.Motion.motionType == MOTIONTYPE_ACTION2;

	// ���[�V�����^�C�v3���A�N�V������������
	const bool isAttack3 = g_player.Motion.motionType == MOTIONTYPE_ACTION3;

	// ���[�V�����^�C�v4���A�N�V������������
	const bool isAttack4 = g_player.Motion.motionType == MOTIONTYPE_ACTION4;

	// ���[�V�������W�����v�A�N�V������������
	const bool isJumpAttack = g_player.Motion.motionType == MOTIONTYPE_JUMPACTION;

	// �U����Ԃ��𔻒�
	const bool isAttacking = NotFinishMotion && (isAttack1 || isAttack2 || isAttack3 || isAttack4 || isJumpAttack);

	// �����Ԃ�
	return isAttacking;
}
//===============================================================================================================
// �A�C�e���̕ύX���鎞�̃X�e�[�^�X�ǂݍ���
//===============================================================================================================
void LoadItemChange(int nType, float swordLength)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	FILE* pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen("data\\ITEM\\StatusChange.txt", "r");

	int pType = 0;
	char skip[10] = {};
	int changemotion = 0;
	int playerType = 0;
	float speed = 0.0f;
	int nLoad = 0;

	// �t�@�C����NULL����Ȃ�������
	if (pFile != NULL)
	{
		// �����ǂݎ��ϐ�
		char aString[MAX_WORD];

		while (1)
		{
			// ������ǂݎ��
			int nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				//[=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// ��ނ���
				nData = fscanf(pFile, "%d", &pType);
			}
			
			// �ǂݎ������ނƈ�v���Ă�����
			if (pType == nType && nLoad != 1)
			{
				nLoad = LoadPlayerParamCharge(&aString[0], pFile, swordLength);
			}
			// EOF��ǂݎ������
			if (nData == EOF)
			{
				// while���𔲂���
				break;
			}
		}
	}
}
//===============================================================================================================
// �v���C���[�̃p�����[�^�[�ύX
//===============================================================================================================
int LoadPlayerParamCharge(char* aStr, FILE* pFile, float swordLength)
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	int pType = 0;
	char skip[10] = {};
	int changemotion = 0;
	int playerType = 0;
	float speed = 0.0f;
	int nLoad = 0;

	while (1)
	{
		// ������ǂݎ��
		int nData = fscanf(pFile, "%s", aStr);

		// CHANGEMOTION��ǂݎ������
		if (strcmp(aStr, "CHANGEMOTION") == 0)
		{
			//[=]��ǂݔ�΂�
			nData = fscanf(pFile, "%s", &skip[0]);

			// �ύX���郂�[�V�����̎��
			nData = fscanf(pFile, "%d", &changemotion);
		}

		// PLAYERTYPE��ǂݎ������
		if (strcmp(aStr, "PLAYERTYPE") == 0)
		{
			//[=]��ǂݔ�΂�
			nData = fscanf(pFile, "%s", &skip[0]);

			// �ύX����v���C���[�̎��
			nData = fscanf(pFile, "%d", &playerType);
		}

		// SPEED��ǂݎ������
		if (strcmp(aStr, "SPEED") == 0)
		{
			//[=]��ǂݔ�΂�
			nData = fscanf(pFile, "%s", &skip[0]);

			// �����X�s�[�h
			nData = fscanf(pFile, "%f", &speed);
		}

		// END_PLAYERSET��ǂݎ������
		if (strcmp(aStr, "END_PLAYERSET") == 0)
		{
			// ���[�V������ύX����
			MotionChange(changemotion, playerType); 

			// �X�e�[�^�X��ύX����
			StatusChange(speed, D3DXVECTOR3(0.0f, swordLength, 0.0f), pItem[pPlayer->ItemIdx].Power);

			// ���[�h������Ԃ�
			nLoad = 1;
			break;
		}

	}

	// ���[�h������Ԃ�
	return nLoad;
}


