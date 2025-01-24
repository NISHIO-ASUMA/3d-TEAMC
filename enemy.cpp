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
#include "Shadow.h"
#include "Particle.h"

//****************************
//�}�N����`
//****************************
#define ENEMY_WORD (128) // �G�̍ő吔
#define SWORD_MTX (4) // ���̃}�g���b�N�X�̐�
#define TYPEONE_MOVE (2.5f) //�G0�̈ړ���
#define TYPETWO_MOVE (1.5f) //�G1�̈ړ���
#define TYPETHREE_MOVE (1.0f) //�G2�̈ړ���
#define MAX_TEXENEMY (128) //�e�N�X�`���̍ő吔
#define MAX_ENEMYMOVE (1.0f) // �G�̈ړ���
#define SHADOWSIZESET (40.0f) // �e�̃T�C�Y�̃I�t�Z�b�g
#define SHADOW_A (1.0f) // �e�̃A���t�@

//****************************
//�v���g�^�C�v�錾
//****************************
void LoadEnemy(int nType);											// �ǂݍ��ݏ���
bool AgentRange(float plrange, float playerrange, int nCntEnemy);   // �z�[�~���O�͈̔͂ɂ��邩�ǂ���
void AgentEnemy(int nCntEnemy);										// �G�̃z�[�~���O����
void CollisionToEnemy(int nCntEnemy);								// �G�ƓG�̓����蔻��

//****************************
//�O���[�o���ϐ��錾
//****************************
ENEMY g_Enemy[MAX_ENEMY];
ENEMY g_LoadEnemy[ENEMYTYPE_MAX];
int g_nNumEnemy;//�G�̑����J�E���g

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
	//LoadEnemy(0);

	//�G�̓ǂݍ���
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		LoadEnemy(nCntEnemyType);

		//�ǂݍ���

		//g_LoadEnemy[nCntEnemyType].
		//g_LoadEnemy[nCntEnemyType].nLife = 20;
		//g_LoadEnemy[nCntEnemyType].nType = ENEMYTYPE_ONE;
		g_LoadEnemy[nCntEnemyType].Motion.motionType = MOTIONTYPE_NEUTRAL;//���[�V�����̎��
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

		//�ړ��ʂ̌���
		g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.5f;
		g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.5f;

		//�O��̈ʒu��ۑ�
		g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

		//�ʒu�̍X�V
		g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

		//�u���b�N�̓����蔻��
		if (CollisionBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].Size))
		{

		}

		// �e�̌v�Z
		SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos, SHADOWSIZESET + SHADOWSIZESET * g_Enemy[nCntEnemy].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Enemy[nCntEnemy].pos.y / 30.0f));


		if (AgentRange(50.0f, 200.0f, nCntEnemy))
		{
			AgentEnemy(nCntEnemy);
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_MOVE;
		}
		else
		{
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;
		}

		CollisionToEnemy(nCntEnemy); // �G�ƓG�̓����蔻��

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
	SetDamege(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z), nDamage, 20, false);
	g_Enemy[nCnt].nLife -= nDamage;

	if (g_Enemy[nCnt].nLife <=0)
	{//�̗͂�0�ȉ��Ȃ�
		SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5, g_Enemy[nCnt].pos.z), g_Enemy[nCnt].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 12.0f, 20, 30, 60, 5.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Enemy[nCnt].state = ENEMYSTATE_DEATH;//�G�̏�Ԃ����S��Ԃɂ���

		g_Enemy[nCnt].bUse = false;			   //���g�p����

		g_nNumEnemy--;						   //�f�N�������g
	}
	else
	{
		SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5, g_Enemy[nCnt].pos.z), g_Enemy[nCnt].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 12.0f, 1, 20, 40, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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

			g_Enemy[nCntEnemy].nIdxShadow = SetShadow(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 1.0f, g_Enemy[nCntEnemy].pos.z), g_Enemy[nCntEnemy].rot, 40.0f);

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
		pFile = fopen("data/MOTION/motionEnemy01.txt", "r");
		break;
	case 1:
		pFile = fopen("data/MOTION/motionEnemy02.txt", "r");
		break;
	case 2:
		pFile = fopen("data/MOTION/motionEnemy03.txt", "r");
		break;
	case 3:
		pFile = fopen("data/MOTION/motionEnemy04.txt", "r");
		break;
	case 4:
		pFile = fopen("data/MOTION/motionEnemy05.txt", "r");
		break;
	default:
		pFile = NULL; //NULL�ɂ���
		break;
	}

	if (pFile != NULL)
	{//�@NULL ����Ȃ�
		char aString[ENEMY_WORD];

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
							fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);
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
								&g_LoadEnemy[nType].EnemyModel[nCnt].pBuffMatEnemy,
								NULL,
								&g_LoadEnemy[nType].EnemyModel[nCnt].dwNumMatEnemy,
								&g_LoadEnemy[nType].EnemyModel[nCnt].pMeshEnemy);

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
											fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���W����
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// �p�x����
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.x);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.y);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.z);
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
									fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEY��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey)
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
													fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
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
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// �C���N�������g
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROT��ǂݎ������
														// ������ǂݔ�΂�
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
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
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(enemy.cpp)", MB_OK);
		return;
    }
	// �t�@�C�������
	fclose(pFile);

}
//================================
// �z�[�~���O�͈̔͂ɂ��邩�ǂ���
//================================
bool AgentRange(float playerrange,float Agentrange,int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	bool bHorming = false; // �z�[�~���O���Ă��邩���Ȃ���

	float fDistanceX = g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x; // ����X�����߂�
	float fDistanceY = g_Enemy[nCntEnemy].pos.y - pPlayer->pos.y; // ����Y�����߂�
	float fDistanceZ = g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z; // ����Z�����߂�

	// ���������߂�
	float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

	// �z�[�~���O���Ă��锼�a
	float Radius = Agentrange + playerrange;

	// ���a���Z�o
	Radius = Radius * Radius;

	//�͈͓��ɓ�����
	if (fDistance <= Radius)
	{
		bHorming = true; // �z�[�~���O���Ă���
	}

	return bHorming;
}
//=============================
// �G�̃z�[�~���O����
//=============================
void AgentEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	D3DXVECTOR3 fDest = pPlayer->pos - g_Enemy[nCntEnemy].pos; // �G����v���C���[�܂ł̃x�N�g��������
	D3DXVec3Normalize(&fDest, &fDest); // ���K��

	float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z); // �G����v���C��܂ł̊p�x�����߂�

	g_Enemy[nCntEnemy].rot.y = fAngle + D3DX_PI; // �p�x����

	// �ړ��ʂ̍X�V
	g_Enemy[nCntEnemy].move.x += fDest.x * MAX_ENEMYMOVE; 
	g_Enemy[nCntEnemy].move.z += fDest.z * MAX_ENEMYMOVE; 
}
//=============================
// �G�ƓG�̓����蔻��
//=============================
void CollisionToEnemy(int nCntEnemy)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse && nCnt != nCntEnemy)
		{
			float fDistanceX = g_Enemy[nCntEnemy].pos.x - g_Enemy[nCnt].pos.x; // ����X�����߂�
			float fDistanceY = g_Enemy[nCntEnemy].pos.y - g_Enemy[nCnt].pos.y; // ����Y�����߂�
			float fDistanceZ = g_Enemy[nCntEnemy].pos.z - g_Enemy[nCnt].pos.z; // ����Z�����߂�

			// ���������߂�
			float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

			float Eradius = 50.0f; // ���a��ݒ�

			// �z�[�~���O���Ă��锼�a
			float Radius = Eradius + Eradius;

			// ���a���Z�o
			Radius = Radius * Radius;

			//�͈͓��ɓ�����
			if (fDistance <= Radius)
			{
				g_Enemy[nCnt].move.x -= sinf(g_Enemy[nCnt].rot.y) * (float)(rand() % 3 - 1) * 0.6f;
				g_Enemy[nCnt].move.z += cosf(g_Enemy[nCnt].rot.z) * (float)(rand() % 3 - 1) * 0.6f;
			}
		}
	}
}

