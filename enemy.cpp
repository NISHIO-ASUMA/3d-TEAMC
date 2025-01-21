//============================
//
// �G[enemy.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "enemy.h"
#include "player.h"
#include <stdio.h>
#include "Score.h"
#include "input.h"
#include "damagepop.h"
#include "block.h"
#include "item.h"

//****************************
//�}�N����`
//****************************
#define ENEMY_WORD (128) // �G�̍ő吔
#define SWORD_MTX (4) // ���̃}�g���b�N�X�̐�
#define TYPEONE_MOVE (2.5f) //�G0�̈ړ���
#define TYPETWO_MOVE (1.5f) //�G1�̈ړ���
#define TYPETHREE_MOVE (1.0f) //�G2�̈ړ���
#define MAX_TEXENEMY (128) //�e�N�X�`���̍ő吔

//****************************
//�O���[�o���ϐ��錾
//****************************
ENEMY g_Enemy[MAX_ENEMY];
ENEMY g_LoadEnemy[ENEMYTYPE_MAX];
int g_nNumEnemy;//�G�̑����J�E���g

//****************************
//�v���g�^�C�v�錾
//****************************
void LoadEnemy(int nType);    //�ǂݍ��ݏ���

//=============================
//�u���b�N�̏���������
//=============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���W
		g_Enemy[nCntEnemy].AttackEnemy = D3DXVECTOR3(5.0f, 10.0f, 5.0f);//
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�p�x
		//g_Enemy[nCntEnemy].nType = ENEMYTYPE_ONE;
		g_Enemy[nCntEnemy].bUse = false;								//���g�p���
		g_Enemy[nCntEnemy].Motion.bLoopMotion = true;					//���[�v���邩�ۂ�
		g_Enemy[nCntEnemy].nLife = 10;									//�̗�
		g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					//���
	}

	//�O���[�o���ϐ��̏�����
	g_nNumEnemy = 0;

	//�G�̓ǂݍ���
	for (int nCntEnemyType= 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		//�ǂݍ���
		LoadEnemy(0);

		//g_LoadEnemy[nCntEnemyType].
		//g_LoadEnemy[nCntEnemyType].nLife = 20;
		//g_LoadEnemy[nCntEnemyType].nType = ENEMYTYPE_ONE;
		g_LoadEnemy[nCntEnemyType].Motion.motionType = MOTIONTYPE_NEUTRAL;//���[�V�����̎��
		g_LoadEnemy[nCntEnemyType].Motion.nKey = 0;						  //�L�[��
		g_LoadEnemy[nCntEnemyType].g_bDamage = true;					  //�_���[�W���ۂ�
		g_LoadEnemy[nCntEnemyType].Motion.bLoopMotion = true;			  //���[�v���ۂ�
		g_LoadEnemy[nCntEnemyType].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�T�C�Y
		g_LoadEnemy[nCntEnemyType].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  //���W

		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].dwNumMatEnemy; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[nCntMat]);
				}
			}

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse = pMat->MatD3D.Diffuse;

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].FirstDiffuse = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse;

		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			//���_���̎擾
			nNumVtx = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_LoadEnemy[nCntEnemyType].vtxMin.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadEnemy[nCntEnemyType].vtxMin.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadEnemy[nCntEnemyType].vtxMin.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.z = vtx.z;
				}
				else if (vtx.x > g_LoadEnemy[nCntEnemyType].vtxMax.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.x = vtx.x;
				}
				else if (vtx.y > g_LoadEnemy[nCntEnemyType].vtxMax.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.y = vtx.y;
				}
				else if (vtx.z > g_LoadEnemy[nCntEnemyType].vtxMax.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;

			}
			//���_�o�b�t�@�̃A�����b�N
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->UnlockVertexBuffer();		
		}

		//�T�C�Y�ɑ��
		g_LoadEnemy[nCntEnemyType].Size.x = g_LoadEnemy[nCntEnemyType].vtxMax.x - g_LoadEnemy[nCntEnemyType].vtxMin.x;
		g_LoadEnemy[nCntEnemyType].Size.y = g_LoadEnemy[nCntEnemyType].vtxMax.y - g_LoadEnemy[nCntEnemyType].vtxMin.y;
		g_LoadEnemy[nCntEnemyType].Size.z = g_LoadEnemy[nCntEnemyType].vtxMax.z - g_LoadEnemy[nCntEnemyType].vtxMin.z;

	}
}
//=============================
//�G�̏I������
//=============================
void UninitEnemy(void)
{
		for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
		{
			for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
			{
				//�e�N�X�`���̔j��
				for (int TexCnt = 0; TexCnt < MAX_TEXENEMY; TexCnt++)
				{
					if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] != NULL)
					{
						g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt]->Release();
						g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] = NULL;
					}
				}

				//���b�V���̔j��
				if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy != NULL)
				{
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->Release();
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy = NULL;
				}

				//�}�e���A���̔j��
				if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy != NULL)
				{
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->Release();
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy = NULL;
				}
			}
		}
}
//=============================
//�G�̍X�V����
//=============================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{//���g�p��Ԃ�������
			//�Ƃ΂��ăJ�E���g��i�߂�
			continue;
		}

		if(CollisionEnemy(&g_Enemy[nCntEnemy].pos, // �G�̈ʒu
			30.0f, // �u���b�N�̔��a
			30.0f)) // �G�̔��a 
		{
			HitEnemy(nCntEnemy, 1);
		}

		//���[�V�����̍X�V
		UpdateMotion(&g_Enemy[nCntEnemy].Motion);
	}
}
//=============================
//�G�̕`�揈��
//=============================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	int nNumEnemy = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�S���f���p�[�c�̕`��
			for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
			{
				D3DXMATRIX mtxRotModel, mtxTransform;//�v�Z�p
				D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTransform, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

				//�p�[�c��[�e�̃}�g���b�N�X]��ݒ�
				if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent != -1)
				{
					//�e���f��������ꍇ
					mtxParent = g_Enemy[nCntEnemy].Motion.aModel[g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
				}
				else
				{//�e���f�����Ȃ��ꍇ
					mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
				}

				//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&mtxParent);//���������e

				//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].dwNumMatEnemy; nCntMat++)
				{					
					D3DXMATERIAL color;

					switch (g_Enemy[nCntEnemy].nType)
					{
					case ENEMYTYPE_ONE:
						if (g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
						{
							//�}�e���A���̐ݒ�
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						}
						else if (g_Enemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
						{
							color = pMat[nCntMat];

							color.MatD3D.Diffuse.r = 1.0f;
							color.MatD3D.Diffuse.g = 0.0f;
							color.MatD3D.Diffuse.b = 0.0f;
							color.MatD3D.Diffuse.a = 1.0f;

							pDevice->SetMaterial(&color.MatD3D);
						}
						break;
					default:
						break;
					}

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pTextureEnemy[nCntMat]);

					//�u���b�N(�p�[�c)�̕`��
					g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pMeshEnemy->DrawSubset(nCntMat);
				}
				//�C���N�������g
				nNumEnemy++;
			}
		}
		//������
		nNumEnemy = 0;
	}
}
//=======================
//�G�̎擾����
//=======================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}
//=======================
//�G�̃q�b�g����
//=======================
void HitEnemy(int nCnt,int nDamage)
{
	g_Enemy[nCnt].nLife -= nDamage;

	if (g_Enemy[nCnt].nLife <=0)
	{//�̗͂�0�ȉ��Ȃ�
		g_Enemy[nCnt].state = ENEMYSTATE_DEATH;//�G�̏�Ԃ����S��Ԃɂ���

		g_Enemy[nCnt].bUse = false;			   //���g�p����

		g_nNumEnemy--;						   //�f�N�������g
	}
	else
	{
		g_Enemy[nCnt].state = ENEMYSTATE_DAMAGE;//�G�̏�Ԃ��_���[�W�ɂ���

		g_Enemy[nCnt].g_bDamage = false;//�_���[�W��ʂ�Ȃ�����

		g_Enemy[nCnt].nCounterState = 30;//�_���[�W��Ԃ���m�[�}���ɖ߂�܂ł̎���
	}
}
//=======================
//�G�̐ݒ菈��
//=======================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType,int nLife,D3DXVECTOR3 move)
{
	MODE mode = GetMode();//���݂̃��[�h�̎擾

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{
			g_Enemy[nCntEnemy] = g_LoadEnemy[nType]; // ������

			g_Enemy[nCntEnemy].pos = pos;	 //���W
			g_Enemy[nCntEnemy].move = move;  //�ړ���
			g_Enemy[nCntEnemy].nType = nType;//���
			g_Enemy[nCntEnemy].nLife = nLife;//�̗�
			g_Enemy[nCntEnemy].bUse = true;  //�g�p���

			g_nNumEnemy++;//�C���N�������g
			break;
		}
	}
}
//=======================
//�G�̑����擾����
//=======================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=============================
//�G�̃��[�h����
//=============================
void LoadEnemy(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�|�C���^

	FILE* pFile;//�t�@�C���|�C���^
	char aStr[ENEMY_WORD];//�����^�̔z��

	//���[�J���ϐ�
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
	int nCntNum = 0;
	int EnenKey = 0;

	//��ނ��ƂɃt�@�C����ǂݍ���
	switch (nType)
	{
	case 0:
		pFile = fopen(ENEMYFILE001, "r");
		break;
	case 1:
		pFile = fopen(ENEMYFILE002, "r");
		break;
	case 2:
		pFile = fopen(ENEMYFILE003, "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{//�t�@�C�����J������
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
						fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);

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
							&g_LoadEnemy[nType].EnemyModel[nCntNum].pBuffMatEnemy,
							NULL,
							&g_LoadEnemy[nType].EnemyModel[nCntNum].dwNumMatEnemy,
							&g_LoadEnemy[nType].EnemyModel[nCntNum].pMeshEnemy);

						if (nCntNum < g_LoadEnemy[nType].Motion.nNumModel)
						{
							nCntNum++;
						}

						if (nCntNum >= g_LoadEnemy[nType].Motion.nNumModel)
						{
							nCntNum = 0;
						}

					}
					else if (strcmp(aStr, "CHARACTERSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);

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
								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aModel[nIdx].nIdxModelParent);

							}
							else if (strcmp(aStr, "POS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.x);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.y);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.z);
							}
							else if (strcmp(aStr, "ROT") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.x);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.y);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.z);
							}
							else if (strcmp(aStr, "END_PARTSSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(aStr, "MOTIONSET") == 0)
					{
						char aStr4[ENEMY_WORD];

						while (1)
						{
							fscanf(pFile, "%s", &aStr4[0]);

							if (strcmp(aStr4, "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
							}
							else if (strcmp(aStr4, "LOOP") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].bLoop);
							}

							char aStr5[ENEMY_WORD];

							if (strcmp(aStr4, "KEYSET") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aStr5[0]);

									if (strcmp(aStr5, "FRAME") == 0)
									{
										fscanf(pFile, "%s", &gomi[0]);

										fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].nFrame);
									}
									else if (strcmp(aStr5, "KEY") == 0)
									{
										char aStr6[ENEMY_WORD];

										while (1)
										{
											fscanf(pFile, "%s", &aStr6[0]);

											if (strcmp(aStr6, "POS") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosX);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosY);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosZ);

												nCntPartsPos++;
											}
											else if (strcmp(aStr6, "ROT") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotX);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotY);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotZ);
												nCntPartsRot++;
											}
											else if (strcmp(aStr6, "END_KEY") == 0)
											{
												break;
											}
										}
									}
									else if (strcmp(aStr5, "END_KEYSET") == 0)
									{
										EnenKey++;
										nCntPartsPos = 0;
										nCntPartsRot = 0;
										break;
									}
								}
							}

							else if (strcmp(aStr4, "END_MOTIONSET") == 0)
							{
								EnenKey = 0;
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
