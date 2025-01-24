//============================
//
// �v���C���[[player.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "player.h"
#include "input.h"
#include "camera.h"
#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "meshfield.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "wall.h"
#include "mouse.h"
#include "Shadow.h"

//****************************
//�}�N����`
//****************************
#define MAX_WORD (256) // �ő啶����
#define PLAYERLIFE (50) // �v���C���[�̗̑�
#define MAX_TEXPLAYER (128) // �e�N�X�`���̍ő吔
#define MAX_JUMP (15.0f) // �W�����v��
#define MAX_MOVE (1.0f) // ���v���C���[�̈ړ���

//****************************
//�v���g�^�C�v�錾
//****************************
void LoadModel(int nType); // �v���C���[�̃��[�h����
void PlayerComb(MOTIONTYPE motiontype, int AttackState,int nCounterState, COMBOSTATE Combstate);
void LoadMotion(int Weponmotion);
void MotionChange(int itemtype,int LoadPlayer);

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//�v���C���[�̃e�N�X�`���ւ̃|�C���^
Player g_player;//�v���C���[�\����
Player g_LoadPlayer[PLAYERTYPE_MAX]; // �v���C���[�̃��f����ۑ����Ă����ϐ�
MODEL g_LoadWepon[ITEMTYPE_MAX];     // �v���C���[�̕����ۑ����Ă����ϐ�
MOTION g_LoadMotion[MOTION_MAX];   // ���[�V�����̏���ۑ����Ă����ϐ�
int g_nCounterState,g_AttackState;

//============================
//�v���C���[�̏���������
//============================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	MODE mode = GetMode();//���݂̃��[�h���擾

	g_player.nIdxShadow = SetShadow(g_player.pos, g_player.rot, 40.0f);

	//�v���C���[�̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);		   //���W
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   //�p�x
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړI�̊p�x
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		   //�ړ���
	g_player.bJump = false;								   //�W�����v�����ۂ�
	g_player.bDisp = true;								   //�g�p���
	g_player.bMove = false;								   //
	g_player.nLife = PLAYERLIFE;						   //�̗�
	g_player.state = PLAYERSTATE_NORMAL;				   //���
	g_player.Motion.bLoopMotion = true;					   //
	g_player.Swordrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	   //���̊p�x
	g_player.Motion.nKey = 0;							   //�L�[��
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;	   //���[�V�����̎��
	g_player.SwordOffpos.x = 0.0f;						   //���̃I�t�Z�b�g�̍��Wx
	g_player.SwordOffpos.y = 85.0f;						   //���̃I�t�Z�b�g�̍��Wy
	g_player.SwordOffpos.z = 0.0f;						   //���̃I�t�Z�b�g�̍��Wz
	g_player.nCounterAction = 0;						   //�A�N�V�����J�E���g
	g_nCounterState = 0;                                   //��ԃJ�E���^�[
	g_AttackState = 0;									   //�U����Ԃ̃J�E���^�[

	//LoadWepon(); // �A�C�e���̃��[�h


	LoadMotion(0);
	LoadMotion(1);
	LoadMotion(2);

	// �؂�ւ�郂�[�V�����̐�����
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
	}

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		LoadModel(nCntPlayer);
		//�K�v�ȏ���ݒ�

		g_LoadPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_LoadPlayer[nCntPlayer].nLife = PLAYERLIFE;
		g_LoadPlayer[nCntPlayer].nMaxLife = PLAYERLIFE;
		g_LoadPlayer[nCntPlayer].bDisp = true;
		g_LoadPlayer[nCntPlayer].Motion.motionType = MOTIONTYPE_NEUTRAL;
		g_LoadPlayer[nCntPlayer].Motion.bLoopMotion = true;
		g_LoadPlayer[nCntPlayer].bJumpAttack = false;
		g_LoadPlayer[nCntPlayer].HandState = PLAYERHOLD_NO;
		g_LoadPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;

		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,						
					pMat[nCntMat].pTextureFilename,
					&g_apTexturePlayer[nCntMat]);
				}
			}
		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
			//���_���̎擾
			nNumVtx = g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_player.vtxMinPlayer.x)
				{
					g_player.vtxMinPlayer.x = vtx.x;
				}
				else if (vtx.y < g_player.vtxMinPlayer.y)
				{
					g_player.vtxMinPlayer.y = vtx.y;
				}
				else if (vtx.z < g_player.vtxMinPlayer.z)
				{
					g_player.vtxMinPlayer.z = vtx.z;
				}

				else if (vtx.x > g_player.vtxMaxPlayer.x)
				{
					g_player.vtxMaxPlayer.x = vtx.x;
				}

				else if (vtx.y > g_player.vtxMaxPlayer.y)
				{
					g_player.vtxMaxPlayer.y = vtx.y;
				}

				else if (vtx.z > g_player.vtxMaxPlayer.z)
				{
					g_player.vtxMaxPlayer.z = vtx.z;
				}

				if (vtx.x < g_player.Motion.aModel[nCntModel].vtxMin.x)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_player.Motion.aModel[nCntModel].vtxMin.y)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_player.Motion.aModel[nCntModel].vtxMin.z)
				{
					g_player.Motion.aModel[nCntModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_player.Motion.aModel[nCntModel].vtxMax.x)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_player.Motion.aModel[nCntModel].vtxMax.y)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_player.Motion.aModel[nCntModel].vtxMax.z)
				{
					g_player.Motion.aModel[nCntModel].vtxMax.z = vtx.z;
				}


				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//�T�C�Y����
			g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_player.Motion.aModel[nCntModel].Size.x = g_player.Motion.aModel[nCntModel].vtxMax.x - g_player.Motion.aModel[nCntModel].vtxMin.x;
			g_player.Motion.aModel[nCntModel].Size.y = g_player.Motion.aModel[nCntModel].vtxMax.y - g_player.Motion.aModel[nCntModel].vtxMin.y;
			g_player.Motion.aModel[nCntModel].Size.z = g_player.Motion.aModel[nCntModel].vtxMax.z - g_player.Motion.aModel[nCntModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_LoadPlayer[nCntPlayer].Motion.aModel[nCntModel].pMesh->UnlockVertexBuffer();
		}
	}

	//�ŏ��ɕ`�悵�����v���C���[�̏�����
	g_player = g_LoadPlayer[0];
}
//============================
//�v���C���[�̏I������
//============================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	for (int nCntMat = 0; nCntMat < MAX_TEXPLAYER; nCntMat++)
	{
		if (g_apTexturePlayer[nCntMat] != NULL)
		{
			g_apTexturePlayer[nCntMat]->Release();
			g_apTexturePlayer[nCntMat] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadPlayer[nCntPlayer].Motion.nNumModel; nCntModel++)
		{
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

	// �A�C�e������
	for (int nCnt = 0; nCnt < ITEMTYPE_MAX; nCnt++)
	{
		// ���b�V���̔j��
		if (g_LoadWepon[nCnt].pMesh != NULL)
		{
			g_LoadWepon[nCnt].pMesh->Release();
			g_LoadWepon[nCnt].pMesh = NULL;
		}
		// �}�e���A���̔j��
		if (g_LoadWepon[nCnt].pBuffMat != NULL)
		{
			g_LoadWepon[nCnt].pBuffMat->Release();
			g_LoadWepon[nCnt].pBuffMat = NULL;
		}
	}

	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel; nCntModel++)
	{
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
//============================
//�v���C���[�̍X�V����
//============================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();

		/*StickPad();*/

	if (GetKeyboardPress(DIK_A) == true)
	{
		//�v���C���[�̈ړ�(��)
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;
	
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(��)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z += sinf(pCamera->rot.y) * MAX_MOVE;
			g_player.move.x -= cosf(pCamera->rot.y) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.5f;
		}
	}
	//�v���C���[�̈ړ�(�E)
	else if (GetKeyboardPress(DIK_D) == true)
	{
		//�v���C���[�̈ړ�(��)
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * MAX_MOVE;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(�E)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z -= sinf(pCamera->rot.y) * MAX_MOVE;
			g_player.move.x += cosf(pCamera->rot.y) * MAX_MOVE;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x += sinf(pCamera->rot.y) * MAX_MOVE;
		g_player.move.z += cosf(pCamera->rot.y) * MAX_MOVE;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x -= sinf(pCamera->rot.y) * MAX_MOVE;
		g_player.move.z -= cosf(pCamera->rot.y) * MAX_MOVE;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE)
		{
			g_player.Motion.motionType = MOTIONTYPE_NEUTRAL; // �L�[�������Ă��Ȃ����Ƀj���[�g�����ɂȂ�
		}
	}
	
	switch (g_player.Motion.motionType)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		if (!g_player.bJump)
		{
			g_player.Motion.motionType = MOTIONTYPE_JUMP; // ���[�V�������W�����v�ɂ���
		}
		break;
	case MOTIONTYPE_ACTION:
		break;
	case MOTIONTYPE_JUMP:
		break;
	case MOTIONTYPE_LANDING:
		break;
	default:
		break;
	}

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
	g_player.move.x += (0.0f - g_player.move.x) * 0.25f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.25f;

	//�O��̈ʒu��ۑ�
	g_player.posOld = g_player.pos;

	//�v���C���[�̈ʒu�̍X�V
	g_player.pos += g_player.move;

	if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.Size))
	{
		g_player.bJump = true; // �W�����v���\�ɂ���
	}
	else if (CollisionField())
	{
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
	SetPositionShadow(g_player.nIdxShadow,g_player.pos,60.0f + 60.0f * g_player.pos.y / 200.0f, 1.0f / (1.0f + g_player.pos.y / 30.0f));

	////�ǂ̏Փ˔���
	//CollisionWall();

	if (JoypadTrigger(JOYKEY_A) == true || KeyboardTrigger(DIK_SPACE)==true)
	{//a�{�^�� or Enter�L�[�������ꂽ
		if (g_player.bJump == true)
		{
			g_player.bJump = false;
			g_player.Motion.nKey = 0;
			g_player.Motion.motionType = MOTIONTYPE_JUMP;
			g_player.move.y = 15.0f;
		}
	}

	// �v���C���[�̏�Ԃ��U������Ȃ����n�ʂɂ���
	if (g_player.bDisp)
	{
		if (OnMouseTriggerDown(LEFT_MOUSE)&&g_player.Combostate == COMBO_NO)
		{
			PlayerComb(MOTIONTYPE_ACTION, 60, 120, COMBO_ATTACK1); // �R���{1
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK1 && g_player.HandState != PLAYERHOLD_HOLD)
		{
			PlayerComb(MOTIONTYPE_ACTION2, 60, 120, COMBO_ATTACK2); // �R���{2
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK2)
		{
			PlayerComb(MOTIONTYPE_ACTION3, 60, 120, COMBO_ATTACK3); // �R���{3
		}
		else if (OnMouseTriggerDown(LEFT_MOUSE) && g_player.Combostate == COMBO_ATTACK3)
		{
			PlayerComb(MOTIONTYPE_ACTION4, 60, 120, COMBO_ATTACK4); // �R���{4
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

	//���[�V�����̍X�V
	UpdateMotion(&g_player.Motion);

	//�v���C���[�̌�����ړI�̌����ɋ߂Â���
	g_player.rot.y += (g_player.rotDestPlayer.y - g_player.rot.y) * 0.1f;
}
//============================
//�v���C���[�̕`�揈��
//============================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxSize;

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

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorldPlayer);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
		{
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
						color.MatD3D.Diffuse.g = 0.0f;
						color.MatD3D.Diffuse.b = 0.0f;
						color.MatD3D.Diffuse.a = 1.0f;

						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&color.MatD3D);
					}

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_apTexturePlayer[nCntMat]);

					//���f��(�p�[�c)�̕`��
					g_player.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
				}
				nCnt++;
			}
			if (nCnt == 15)
			{
				SetMtxPos(); // ���̃��[���h�}�g���b�N�X��ݒ�
			}
		}
	}
}
//============================
//�v���C���[�̎擾����
//============================
Player* GetPlayer(void)
{
	return &g_player;
}
//=============================================
////���[���h�}�g���b�N�X�̃I�t�Z�b�g�ݒ菈��
//=============================================
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
//======================
// �v���C���[�ƓG�̔���
//======================
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;

	if (g_player.nLife <= 0)
	{
		g_player.state = PLAYERSTATE_FALL;
	}
	else
	{
		g_player.nCounterState = 60;
		g_player.state = PLAYERSTATE_DAMAGE;
	}
}
//============================
// �v���C���[�̃X�e�B�b�N����
//============================
void StickPad(void)
{
	//XINPUT_STATE* pStick;

	//pStick = GetJoySticAngle();

	//CAMERA* pCamera = GetCamera();
	//if (g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DAMAGE&& g_player.bJumpAttack ==false && g_player.state != PLAYERSTATE_ATTACK)
	//{
	//	if (GetJoyStick() == true)
	//	{
	//		float LStickAngleY = pStick->Gamepad.sThumbLY;
	//		float LStickAngleX = pStick->Gamepad.sThumbLX;

	//		float deadzone = 10920;
	//		float magnitude = sqrtf(LStickAngleX * LStickAngleX + LStickAngleY * LStickAngleY);

	//		if (magnitude > deadzone)
	//		{
	//			bPad = true;
	//			float normalizeX = (LStickAngleX / magnitude);
	//			float normalizeY = (LStickAngleY / magnitude);

	//			float moveX = normalizeX * cosf(-pCamera->rot.y) - normalizeY * sinf(-pCamera->rot.y);
	//			float moveZ = normalizeX * sinf(-pCamera->rot.y) + normalizeY * cosf(-pCamera->rot.y);

	//			g_player.move.x += moveX * PLAYER_MOVE;
	//			g_player.move.z += moveZ * PLAYER_MOVE;

	//			g_player.rotDestPlayer.y = atan2f(-moveX, -moveZ);

	//			if (g_player.Motion.motionType != MOTIONTYPE_INVISIBLE)
	//			{
	//				g_player.state = PLAYERSTATE_MOVE;
//			}
//		}
//	}
//}
}
//================================
// �v���C���[�̌��ƓG�̓����蔻��
//================================
void HitSowrd(ENEMY* pEnemy)
{

}

//================================
// �v���C���[�ƃA�C�e���̔���
//================================
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

		if (KeyboardTrigger(DIK_F))
		{
			Itemchange(pItem[nIdx].nType); // �A�C�e�����E��

			switch (pItem[nIdx].nType)
			{
			case ITEMTYPE_BAT:
				MotionChange(MOTION_DBHAND,0);
				break;
			case ITEMTYPE_GOLF:
				MotionChange(MOTION_DBHAND,0);
				break;
			case ITEMTYPE_HUNMER:
				MotionChange(MOTION_BIGWEPON,0);
				break;
			case ITEMTYPE_WOOD:
				MotionChange(MOTION_DBHAND,1);
				break;
			case ITEMTYPE_STONEBAT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_LIGHT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_LIGHTWOOD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_HARISEN:
				break;
			case ITEMTYPE_ICEBLOCK:
				MotionChange(MOTION_DBHAND, 1);
				break;
			case ITEMTYPE_ICEBLOCKSOWRD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_IRON:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_IRONBAT:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_SURFBOARD:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TORCH:
				MotionChange(MOTION_DBHAND, 0);
				break;
			case ITEMTYPE_TORCHSWORD:
				break;
			case ITEMTYPE_HEADSTATUE:
				break;
			case ITEMTYPE_HEADSTATUTORSO:
				break;
			case ITEMTYPE_MEGAPHONE:
				break;
			case ITEMTYPE_RUBBERCUP:
				break;
			case ITEMTYPE_TELEPHONEPOLE:
				break;
			case ITEMTYPE_TORSO:
				break;
			default:
				break;
			}
		}
	}

	return bCollision;
}

//============================
// �v���C���[�̃R���{
//============================
void PlayerComb(MOTIONTYPE motiontype, int AttackState, int nCounterState, COMBOSTATE Combstate)
{
	g_player.Motion.nKey = 0;                 // �L�[��0����n�߂�
	g_player.Motion.nCountMotion = 0;	      // ���[�V�����J�E���g��0����n�߂�
	g_player.Motion.motionType = motiontype;  // ���[�V�����̎�ނ�ύX
	g_nCounterState = nCounterState;		  // ��ԃJ�E���^�[��ݒ�
	g_AttackState = AttackState;			  // �U����ԃJ�E���^�[��ݒ�
	g_player.state = PLAYERSTATE_ATTACK;	  // �v���C���[�̏�Ԃ��U���ɂ���	
	g_player.Combostate = Combstate;		  // �R���{�̏�Ԃ�ݒ�
}
//===============================
// �v���C���[�̃��[�V�����̕ύX
//===============================
void MotionChange(int itemtype,int LoadPlayer)
{
	for (int nCntModel = 0; nCntModel < g_player.Motion.nNumModel - 1; nCntModel++)
	{
		g_player.Motion.aModel[nCntModel] = g_LoadPlayer[LoadPlayer].Motion.aModel[nCntModel]; // ���f���̏�����
	}
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		g_player.Motion.aMotionInfo[nCntMotion] = g_LoadPlayer[LoadPlayer].Motion.aMotionInfo[nCntMotion];
	}

	if (LoadPlayer != PLAYERTYPE_NOHAND) // �v���C���[���m�[�n���h����������������Ȃ�
	{
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION] = g_LoadMotion[itemtype].aMotionInfo[0];		// �U��1�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION2] = g_LoadMotion[itemtype].aMotionInfo[1];    // �U��2�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION3] = g_LoadMotion[itemtype].aMotionInfo[2];	// �U��3�̏�����
		g_player.Motion.aMotionInfo[MOTIONTYPE_ACTION4] = g_LoadMotion[itemtype].aMotionInfo[3];	// �U��4�̏�����
		g_player.Motion.aModel[15].offpos = g_LoadMotion[itemtype].aModel[15].offpos;				// �I�t�Z�b�g�̏�����
		g_player.Motion.aModel[15].offrot = g_LoadMotion[itemtype].aModel[15].offrot;				// �I�t�Z�b�g�̂̏�����
		g_player.HandState = PLAYERHOLD_NO;															// �v���C���[���m�[�n���h�ɂ���
	}
	else
	{
		g_player.HandState = PLAYERHOLD_HOLD;	// �v���C���[���m�[�n���h�ɂ���
	}
}

//================================
// �v���C���[�̃��[�V�����̃��[�h
//================================
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

//============================
// �v���C���[�̃��[�h����
//============================
void LoadModel(int nType)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[�J���ϐ�
	int nModel = 0;					// ���f����
	int nIdx = 0;					// �C���f�b�N�X
	int NumKey = 0;					// �L�[��
	int nCnt = 0;					// ���f���J�E���g�p
	int nNumParts = 0;				// �p�[�c���i�[�p
	int nCntMotion = 0;				// ���[�V�����J�E���g�p
	int nCntKey = 0;				// �L�[�J�E���g�p
	int nCntPosKey = 0;				// pos�J�E���g
	int nCntRotkey = 0;				// rot�J�E���g

	// �t�@�C���|�C���^��錾
	FILE* pFile;

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
	{//�@NULL ����Ȃ�
		char aString[MAX_WORD];

		while (1)
		{
			// �ǂݔ�΂�
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{// SCRIPT��ǂݎ������
				while (1)
				{
					// �ǂݔ�΂�
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// NUM_MODEL��ǂݎ������
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// �l����
							fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.nNumModel);
						}
					}

					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						// MODEL_FILENAME��ǂݎ������
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// ���
							// �������ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							const char* MODEL_FILE = {};	// ���f���t�@�C�����i�[�p�̕ϐ�

							// �ǂݍ��񂾕������ۑ�����
							MODEL_FILE = aString;

							//X�t�@�C���̓ǂݍ���
							D3DXLoadMeshFromX(MODEL_FILE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].pBuffMat,
								NULL,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].dwNumMat,
								&g_LoadPlayer[nType].Motion.aModel[nCnt].pMesh);

							nCnt++; // nCnt���C���N�������g
						}
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{
						while (1)
						{
							// �������ǂݔ�΂�
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// NUM_PARTS��ǂݎ������

								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// �l����
									fscanf(pFile, "%d", &nNumParts);
								}
							}

							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{
								while (1)
								{
									// �������ǂݔ�΂�
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���
											fscanf(pFile, "%d", &nIdx);
										}
									}

									if (strcmp(&aString[0], "PARENT") == 0)
									{// PARENT��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���
											// �y�A�l���g
											fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���W����
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// �p�x����
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].rot.z);
										}
									}

									if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// END_PARTSSET��ǂݎ������
										// while�𔲂���
										break;
									}

								}// while����
							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// MOTIONSET��ǂݎ������


						while (1)
						{
							// ������ǂݔ�΂�
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "LOOP") == 0)
							{// LOOP ��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEY��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey)
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
												fscanf(pFile, "%s", &aString[0]);

												if (strcmp(&aString[0], "=") == 0)
												{// �l����
													fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
													break;
												}
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
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// �C���N�������g
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROT��ǂݎ������
														// ������ǂݔ�΂�
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
															nCntRotkey++;		// �C���N�������g
														}
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

					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else
					{
						continue;
					}
				}// while����

				break;
			}
		}// while����
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Player.cpp)", MB_OK);
		return;
    }
	// �t�@�C�������
	fclose(pFile);
}