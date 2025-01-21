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
#include<stdio.h>
#include<string.h>
#include "motion.h"
#include "meshfield.h"
#include "block.h"

//****************************
//�}�N����`
//****************************
#define MAX_WORD (128) // �ő啶����
#define PLAYERLIFE (50) // �v���C���[�̗̑�
#define MAX_TEXPLAYER (128) // �e�N�X�`���̍ő吔
#define MAX_JUMP (15.0f) // �W�����v��

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXPLAYER] = {};//�v���C���[�̃e�N�X�`���ւ̃|�C���^
Player g_player;//�v���C���[�\����
Player g_LoadPlayer[PLAYERTYPE_MAX];

//****************************
//�v���g�^�C�v�錾
//****************************

//============================
//�v���C���[�̏���������
//============================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	MODE mode = GetMode();

	//�v���C���[�̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = false;
	g_player.bDisp = true;
	g_player.bMove = false;
	g_player.nLife = PLAYERLIFE;
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.Motion.bLoopMotion = true;
	g_player.Swordrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.Motion.nKey = 0;
	g_player.Motion.motionType = MOTIONTYPE_NEUTRAL;
	g_player.SwordOffpos.x = 0.0f;
	g_player.SwordOffpos.y = 85.0f;
	g_player.SwordOffpos.z = 0.0f;
	g_player.nCounterAction = 0;

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	for (int nCntPlayer = 0; nCntPlayer < PLAYERTYPE_MAX; nCntPlayer++)
	{
		//�K�v�ȏ���ݒ�
		LoadPlayer(0);
		g_LoadPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_LoadPlayer[nCntPlayer].nLife = PLAYERLIFE;
		g_LoadPlayer[nCntPlayer].bDisp = true;
		g_LoadPlayer[nCntPlayer].Motion.motionType = MOTIONTYPE_NEUTRAL;
		g_LoadPlayer[nCntPlayer].Motion.bLoopMotion = true;
		g_LoadPlayer[nCntPlayer].bJumpAttack = false;
		g_LoadPlayer[nCntPlayer].HandState = PLAYERHOLD_NO;

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
			g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

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
	
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.PlayerMove;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.25f) * g_player.PlayerMove;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.PlayerMove;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI * 0.75f) * g_player.PlayerMove;

			g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(��)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z += sinf(pCamera->rot.y) * g_player.PlayerMove;
			g_player.move.x -= cosf(pCamera->rot.y) * g_player.PlayerMove;

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

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.PlayerMove;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_player.PlayerMove;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		//�v���C���[�̈ړ�(��)
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.x += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.PlayerMove;
			g_player.move.z += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_player.PlayerMove;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		//�v���C���[�̈ړ�(�E)
		else
		{
			g_player.Motion.motionType = MOTIONTYPE_MOVE;

			g_player.move.z -= sinf(pCamera->rot.y) * g_player.PlayerMove;
			g_player.move.x += cosf(pCamera->rot.y) * g_player.PlayerMove;

			g_player.rotDestPlayer.y = pCamera->rot.y - D3DX_PI * 0.5f;
		}

	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x += sinf(pCamera->rot.y) * g_player.PlayerMove;
		g_player.move.z += cosf(pCamera->rot.y) * g_player.PlayerMove;

		g_player.rotDestPlayer.y = pCamera->rot.y + D3DX_PI;
	}
	//�v���C���[�̈ړ�(��)
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_player.Motion.motionType = MOTIONTYPE_MOVE;

		g_player.move.x -= sinf(pCamera->rot.y) * g_player.PlayerMove;
		g_player.move.z -= cosf(pCamera->rot.y) * g_player.PlayerMove;

		g_player.rotDestPlayer.y = pCamera->rot.y;
	}
	else
	{
		if (g_player.Motion.motionType == MOTIONTYPE_MOVE ||
			g_player.Motion.motionType == MOTIONTYPE_NEUTRAL)
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
			g_player.Motion.motionType = MOTIONTYPE_JUMP;
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
		break;
	case PLAYERSTATE_ATTACK:
		break;
	case PLAYERSTATE_JUMP:
		break;
	case PLAYERSTATE_LANDING:
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
		g_player.bJump = true;

		if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
		{
			g_player.Motion.nKey = 0;// 0�L�[����n�߂�
			g_player.Motion.bLoopMotion = true;
			g_player.Motion.motionType = MOTIONTYPE_LANDING;
		}
	}
	else if (CollisionField())
	{
		g_player.bJump = true;

		if (g_player.Motion.motionType == MOTIONTYPE_JUMP)
		{
			g_player.Motion.nKey = 0;// 0�L�[����n�߂�
			g_player.Motion.bLoopMotion = true;
			g_player.Motion.motionType = MOTIONTYPE_LANDING;
		}
	}
	else
	{
		g_player.bJump = false;
	}

	//if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.Size))
	//{
	//	g_player.bJump = true;
	//}
	//�v���C���[�̏d��ddd
	g_player.move.y -= 1.0f;

	////�ǂ̏Փ˔���
	//CollisionWall();

	if (JoypadTrigger(JOYKEY_A) == true || KeyboardTrigger(DIK_SPACE)==true)
	{
		if (g_player.bJump == true)
		{
			g_player.bJump = false;
			g_player.Motion.nKey = 0;
			g_player.Motion.motionType = MOTIONTYPE_JUMP;
			g_player.move.y = 15.0f;
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
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans,mtxSize;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

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

void LoadPlayer(int nType)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	FILE* pFile;

	int nNumModel = 0;
	int nCnt = 0;
	char gomi[3];
	int nNumParts = 0;
	int nIdx = 0;
	int Parent = 0;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nNumKey = 0;
	int nCntPartsPos = 0;
	int nCntPartsRot = 0;
	int nCntMotion = 0;
	int nKey= 0;
	char aStr[MAX_WORD];

	switch (nType)
	{
	case 0:
		pFile = fopen(FILENAME001, "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "SCRIPT") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &gomi[0]);
						fscanf(pFile, "%d", &nNumModel);
					}
					else if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &gomi[0]);

						fscanf(pFile, "%s", &aStr[0]);

						const char* MODEL_FILE = {};

						MODEL_FILE = aStr;

						//X�t�@�C���̓ǂݍ���
						D3DXLoadMeshFromX(MODEL_FILE,
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&g_LoadPlayer[nType].Motion.aModel[nCnt].pBuffMat,
							NULL,
							&g_LoadPlayer[nType].Motion.aModel[nCnt].dwNumMat,
							&g_LoadPlayer[nType].Motion.aModel[nCnt].pMesh);

						nCnt++;
					}
					else if (strcmp(aStr, "CHARACTERSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.nNumModel);
							}
							else if (strcmp(aStr, "MOVE") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].PlayerMove);
								break;
							}
						}
					}
					else if (strcmp(aStr, "PARTSSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "INDEX") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &nIdx);

							}
							else if (strcmp(aStr, "PARENT") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aModel[nIdx].nIdxModelParent);
							}
							else if (strcmp(aStr, "POS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.x);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.y);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offpos.z);

							}
							else if (strcmp(aStr, "ROT") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.x);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.y);
								fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aModel[nIdx].offrot.z);
								
							}
							else if (strcmp(aStr, "END_PARTSSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(aStr, "MOTIONSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
								
							}
							else if (strcmp(aStr, "LOOP") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].bLoop);	
							}

							if (strcmp(aStr, "KEYSET") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aStr[0]);

									if (strcmp(aStr, "FRAME") == 0)
									{
										fscanf(pFile, "%s", &gomi[0]);

										fscanf(pFile, "%d", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].nFrame);
									}
									else if (strcmp(aStr, "KEY") == 0)
									{
										while (1)
										{
											fscanf(pFile, "%s", &aStr[0]);

											if (strcmp(aStr, "POS") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsPos].fPosX);
												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsPos].fPosY);
												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsPos].fPosZ);
												
												nCntPartsPos++;
											}
											else if (strcmp(aStr, "ROT") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsRot].fRotX);
												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsRot].fRotY);
												fscanf(pFile, "%f", &g_LoadPlayer[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nKey].aKey[nCntPartsRot].fRotZ);
												
												nCntPartsRot++;

											}
											else if (strcmp(aStr, "END_KEY") == 0)
											{
												break;
											}
										}
									}
									else if (strcmp(aStr, "END_KEYSET") == 0)
									{
										nKey++;
										nCntPartsPos = 0;
										nCntPartsRot = 0;
										break;
									}
								}
							}

							else if (strcmp(aStr, "END_MOTIONSET") == 0)
							{
								nKey = 0;
								nCntMotion++;
								break;
							}
						}
					}

					else if (strcmp(aStr, "END_MODELSET") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(aStr, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		return;
	}
	fclose(pFile);
}

//=============================================
////���[���h�}�g���b�N�X�̃I�t�Z�b�g�ݒ菈��
//=============================================
void SetMtxPos(void)
{
	//LPDIRECT3DDEVICE9 pDevice;

	//pDevice = GetDevice();
	//CAMERA* pCamera = GetCamera();

	////�v�Z�p�̃}�g���b�N�X
	//D3DXMATRIX mtxRot, mtxTrans,mtxParent;

	////���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&g_player.SwordMtx);

	////�����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.Motion.aModel[PLAYERMODEL_015_SWORD].rot.y,
	//	g_player.Motion.aModel[PLAYERMODEL_015_SWORD].rot.x,
	//	g_player.Motion.aModel[PLAYERMODEL_015_SWORD].rot.z);

	//D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx,&mtxRot);

	////�ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, g_player.SwordOffpos.x,g_player.SwordOffpos.y,g_player.SwordOffpos.z);
	//D3DXMatrixMultiply(&g_player.SwordMtx,&g_player.SwordMtx, &mtxTrans);

	//mtxParent = g_player.Motion.aModel[PLAYERMODEL_015_SWORD].mtxWorld;

	////�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
	//D3DXMatrixMultiply(&g_player.SwordMtx,
	//	&g_player.SwordMtx,
	//	&mtxParent);

	////���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &g_player.SwordMtx);
}
//======================
// �v���C���[�ƓG�̔���
//======================
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;

	if (g_player.nLife <= 0)
	{
		g_player.Motion.motionType = MOTIONTYPE_FALL;
		g_player.state = PLAYERSTATE_FALL;
	}
	else
	{
		g_player.nCounterState = 60;
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.Motion.motionType = MOTIONTYPE_DAMAGE;
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