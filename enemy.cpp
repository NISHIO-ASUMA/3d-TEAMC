//==============================================================================================================
//
// �G [enemy.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
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
#include "item.h"
#include "HPGauge.h"
#include "sound.h"
#include "spgauge.h"
#include "boss.h"
#include "time.h"
#include "game.h"
#include "Bullet.h"
#include "minimap.h"
#include "effectEdit.h"
#include "effect2.h"
#include "Effect.h"
#include "wall.h"
#include "camera.h"
#include "meshimpact.h"
#include "math.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define ENEMY_WORD (128)		 // �G�̍ő吔
#define SWORD_MTX (4)			 // ���̃}�g���b�N�X�̐�
#define TYPEONE_MOVE (2.5f)		 // �G0�̈ړ���
#define TYPETWO_MOVE (1.5f)		 // �G1�̈ړ���
#define TYPETHREE_MOVE (1.0f)	 // �G2�̈ړ���
#define MAX_TEXENEMY (128)		 // �e�N�X�`���̍ő吔
#define MAX_ENEMYMOVE (1.0f)     // �G�̈ړ���
#define SHADOWSIZEOFFSET (20.0f) // �e�̃T�C�Y�̃I�t�Z�b�g
#define SHADOW_A (1.0f)			 // �e�̃A���t�@
#define WAVE_ENEMY (15)			 // �G�̏o����

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void LoadEnemy(int nType);										  // �ǂݍ��ݏ���

int LoadEnemyFilename(FILE* pFile, int nNumModel, char* aString, int nType);							 // �G�̃��f���̃��[�h����
void LoadEnemyCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);					 // �G�̃p�[�c�̐ݒ菈��
void LoadEnemyMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);						 // �G�̃��[�V�����̃��[�h����
void LoadEnemyKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);							 // �G�̃��[�V�����̃L�[�̓ǂݍ��ݏ���


//bool AgentRange(float plrange, float playerrange, int nCntEnemy); // �z�[�~���O�͈̔͂ɂ��邩�ǂ���
void AgentEnemy(int nCntEnemy);									  // �G�̃z�[�~���O����
void CollisionToEnemy(int nCntEnemy);							  // �G�ƓG�̓����蔻��
void UpdateHomingEnemy(int nCntEnemy);                            // �G�̃z�[�~���O����
void UpdateRunAwayEnemy(int nCntEnemy);                           // ������G�̍X�V����
void UpdateAttackState(int nCntEnemy);                            // �G�̍U���̍X�V����
void UpdateDroneEnemy(int nCntEnemy);                             // ���ł�G�̍X�V����
void SetSoundWepon(int nType);									  // �炷���̐ݒ菈��

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
ENEMY g_Enemy[MAX_ENEMY];		  // �\���̕ϐ�
MOTION g_LoadEnemy[ENEMYTYPE_MAX]; // �ǂݍ���
int g_nNumEnemy;				  // �G�̑����J�E���g
bool g_bSound;
int g_nNumKill;
int nCntMotionEnemy, nKeyEnemy;

//===============================================================================================================
// �G�̏���������
//===============================================================================================================
void InitEnemy(void)
{
	// �f�o�C�X�̃|�C���^���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	nCntMotionEnemy = 0;
	nKeyEnemy = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_bSound = false;
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���W
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �p�x
		g_Enemy[nCntEnemy].bUse = false;								// ���g�p���
		g_Enemy[nCntEnemy].Motion.bLoopMotion = true;					// ���[�v���邩�ۂ�
		g_Enemy[nCntEnemy].nLife = 20;									// �̗�
		g_Enemy[nCntEnemy].nMaxLife = 20;								// �ő�̗�
		g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					// ���
		g_Enemy[nCntEnemy].Speed = 0.0f;							    // ���̑���
		g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_NO;                // �G���U����Ԃ�
		g_Enemy[nCntEnemy].nCountAction = 0;							// �U���̃C���^�[�o���l
		g_Enemy[nCntEnemy].HitStopCount = 0;						    // �q�b�g���̃q�b�g�X�g�b�v
		g_Enemy[nCntEnemy].nType = ENEMYTYPE_ONE;
		g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;// ���[�V�����̎��
		g_Enemy[nCntEnemy].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �T�C�Y
		g_Enemy[nCntEnemy].nIdxShadow = -1;
	}

	//�O���[�o���ϐ��̏�����
	g_nNumEnemy = 0; // �G�̐�
	g_nNumKill = 0;  // �L����

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		// �G�̓ǂݍ���
		LoadEnemy(nCntEnemyType);
	}
}
//===============================================================================================================
//�G�̏I������
//===============================================================================================================
void UninitEnemy(void)
{
	// ���y���~�߂�
	StopSound();

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].nNumModel; nCntModel++)
		{
			// �e�N�X�`���̔j��
			for (int TexCnt = 0; TexCnt < (int)g_LoadEnemy[nCntEnemyType].aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt] != NULL)
				{
					g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt]->Release();
					g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pTexture[TexCnt] = NULL;
				}
			}

			// ���b�V���̔j��
			if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh != NULL)
			{
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh->Release();
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pMesh = NULL;
			}

			// �}�e���A���̔j��
			if (g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat != NULL)
			{
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat->Release();
				g_LoadEnemy[nCntEnemyType].aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

	// �S�G����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �S���f�������킷
		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
		{
			// �e�N�X�`���̍ő吔����
			for (int TexCnt = 0; TexCnt < (int)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[TexCnt] != NULL)
				{
					g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[TexCnt] = NULL;
				}
			}

			// ���b�V���̔j��
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh != NULL)
			{
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh = NULL;
			}

			// �}�e���A���̔j��
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}
}
//===============================================================================================================
//�G�̍X�V����
//===============================================================================================================
void UpdateEnemy(void)
{
	GAMESTATE gameState = GetGameState();

	g_bSound = false;
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �q�b�g�X�g�b�v�̃J�E���g�����炷
		g_Enemy[nCntEnemy].HitStopCount--;

		if (g_Enemy[nCntEnemy].bUse == false || g_Enemy[nCntEnemy].HitStopCount > 0)
		{// ���g�p��Ԃ�������
			// �Ƃ΂��ăJ�E���g��i�߂�
			continue;
		}
		
		// �ő�̗͂𒴂��Ă��璲��
		if (g_Enemy[nCntEnemy].nMaxLife < g_Enemy[nCntEnemy].nLife)
		{
			g_Enemy[nCntEnemy].nLife = g_Enemy[nCntEnemy].nMaxLife;
		}

		// �G�̏��
		switch (g_Enemy[nCntEnemy].state)
		{
		case ENEMYSTATE_NORMAL:
			break;
		case ENEMYSTATE_AGENT:
			break;
		case ENEMYSTATE_ATTACK:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		case ENEMYSTATE_DAMAGE:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		default:
			break;
		}

		// �ړ��ʂ̌���
		g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.5f;
		g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.5f;

		// �O��̈ʒu��ۑ�
		g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

		// �ʒu�̍X�V
		g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

		if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SIX)
		{
			g_Enemy[nCntEnemy].pos.y = 170.0f;
		}

		// 6�Ԗڂ̓G�ȊO && 7�Ԗڂ̓G�ȊO
		if (g_Enemy[nCntEnemy].nType != ENEMYTYPE_SIX && g_Enemy[nCntEnemy].nType != ENEMYTYPE_SEVEN)
		{
			//���[�V�����̍X�V
			UpdateMotion(&g_Enemy[nCntEnemy].Motion);
		}

		// �Q�[���̏�Ԃ����[�r�[��������
		if (gameState == GAMESTATE_MOVIE)
		{
			continue;
		}

		// �ǂƂ̓����蔻��
		CollisionWall(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, g_Enemy[nCntEnemy].Speed);

		if(CollisionImpact(&g_Enemy[nCntEnemy].pos) == true)
		{
			HitEnemy(nCntEnemy, ImpactDamege(0));
		}

		// �A�C�e��������������
		if (HitThrowItem(&g_Enemy[nCntEnemy].pos,10.0f,40.0f)&& g_Enemy[nCntEnemy].state!=ENEMYSTATE_DAMAGE)
		{
			HitEnemy(nCntEnemy, pPlayer->nDamage * (int)1.5f);
		}

		// ���ƓG�̓����蔻��
		HitSowrd(&g_Enemy[nCntEnemy], nCntEnemy);

		// �e�̌v�Z
		SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Enemy[nCntEnemy].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Enemy[nCntEnemy].pos.y / 30.0f));
		SetMiniMapPotision(g_Enemy[nCntEnemy].nIdxMap, &g_Enemy[nCntEnemy].pos);

		// �z�[�~���O�̍X�V����
		UpdateHomingEnemy(nCntEnemy);

		// ���ł�G�̍X�V����
		UpdateDroneEnemy(nCntEnemy);

		// �G�ƓG�̓����蔻��
		CollisionToEnemy(nCntEnemy);

		// �U���͈͂ɓ�����
		if (sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 20.0f, 20.0f) && g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION)
		{
			g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_ATTACK;
			SetMotion(&g_Enemy[nCntEnemy].Motion,MOTIONTYPE_ACTION, true, 20); // ���[�V�������j���[�g�����ɂ���
			SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].Size.y / 1.5f, g_Enemy[nCntEnemy].pos.z),
				D3DXVECTOR3(1.57f, g_Enemy[nCntEnemy].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 12, 10, 2.0f, 20.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// �U����Ԃ������瓮�����~�߂�
		if (g_Enemy[nCntEnemy].Motion.motionType == MOTIONTYPE_ACTION)
		{
			g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// ���[�V�������Ō�܂ōs������U����Ԃ����Z�b�g
		if (g_Enemy[nCntEnemy].Motion.bFinishMotion == true)
		{
			g_Enemy[nCntEnemy].AttackState = ENEMYATTACK_NO;					// �{�X�̍U����Ԃ��U�����ĂȂ���Ԃɂ���
		}

		// �G�̍U���̍X�V����
		UpdateAttackState(nCntEnemy);

		//�G�̊p�x�̐��K��
		if (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y >= D3DX_PI)
		{
			g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
		}
		else if (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y <= -D3DX_PI)
		{
			g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
		}

		// �ړI�̊p�x�ɋ߂Â���
		g_Enemy[nCntEnemy].rot.y += (g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y) * 0.1f;
	}
}
//===============================================================================================================
//�G�̕`�揈��
//===============================================================================================================
void DrawEnemy(void)
{
	// �v���C���[���擾
	Player* pPlayer = GetPlayer();

	// �f�o�C�X�̃|�C���^���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef; // ���݂̃}�e���A���ۑ��p
	
	D3DXMATERIAL* pMat;  // �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{// ���g�p��������
			// ������ǂݔ�΂�
			continue;
		}

		// ��ނ���
		int nType = g_Enemy[nCntEnemy].nType;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
		D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // �v�Z�p
			D3DXMATRIX mtxParent;				  // �e�̃}�g���b�N�X

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// �p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// �e���f��������ꍇ
				mtxParent = g_Enemy[nCntEnemy].Motion.aModel[g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// �e���f�����Ȃ��ꍇ
				mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
			}

			// �Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
				&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//���������e

			// �p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

			// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// �J���[�ύX�p�̕ϐ�
				D3DXMATERIAL color;

				if (g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Enemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
				{
					// ���
					color = pMat[nCntMat];

					// �J���[��ݒ�
					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.0f;
					color.MatD3D.Diffuse.b = 0.0f;
					color.MatD3D.Diffuse.a = 1.0f;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&color.MatD3D);
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pTexture[nCntMat]);

				// �u���b�N(�p�[�c)�̕`��
				g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}	
	}
}
//=========================================================================================================
//�G�̎擾����
//=========================================================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}
//=========================================================================================================
//�G�̃q�b�g����
//=========================================================================================================
void HitEnemy(int nCnt,int nDamage)
{
	// �A�C�e���擾
	Item* pItem = GetItem();

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	GAMESTATE gamestate = GetGameState();

	// �_���[�W��ݒ�
	SetDamege(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z), // �ʒu
		nDamage,	// �_���[�W																								
		20,			// ����
		false);		

	g_Enemy[nCnt].nLife -= nDamage;

	// �p�[�e�B�N�����Z�b�g
	SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z),
		g_Enemy[nCnt].rot,
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f),
		4.0f, 1, 20, 10, 8.0f, 0.0f,
		false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	int nMin = GetTimeMinute();
	int nSec = GetTimeSecond();

	if (g_Enemy[nCnt].nLife <= 0)
	{// �̗͂�0�ȉ��Ȃ�

		g_nNumKill++;
		int nSpawner = rand() % 4;

		// �G���X�|�[��������
		SpawnEnemy(nSpawner);

		if (pPlayer->nElement == WEPONELEMENT_DARK)
		{
			LoadEffect(1,D3DXVECTOR3(g_Enemy[nCnt].pos.x,g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y,g_Enemy[nCnt].pos.z));
		}
		else
		{
			SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z),
				g_Enemy[nCnt].rot,
				D3DXVECTOR3(3.14f, 3.14f, 3.14f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				4.0f, 8, 15, 20, 5.0f, 0.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// �Q�[���������Ă�����
		if (gamestate != GAMESTATE_END)
		{
			if (pPlayer->FeverMode)
			{
				if (g_Enemy[nCnt].nType == ENEMYTYPE_SEVEN)
				{
					AddScore(32000);		// �X�R�A���擾
					AddSpgauge(5.0f);   // SP�Q�[�W���擾
				}
				else
				{
					AddScore(16000);		// �X�R�A���擾
					AddSpgauge(2.5f);   // SP�Q�[�W���擾
				}
			}
			else if (!pPlayer->FeverMode)
			{
				if (g_Enemy[nCnt].nType == ENEMYTYPE_SEVEN)
				{
					AddFever(10.0f);		// �t�B�[�o�[�|�C���g���擾
					AddScore(16200);		// �X�R�A���擾
					AddSpgauge(4.0f);   // SP�Q�[�W���擾
				}
				else
				{
					AddFever(5.0f);		// �t�B�[�o�[�|�C���g���擾
					AddScore(8100);		// �X�R�A���擾
					AddSpgauge(2.0f);   // SP�Q�[�W���擾
				}
			}
		}

		g_Enemy[nCnt].state = ENEMYSTATE_DEATH; // �G�̏�Ԃ����S��Ԃɂ���
		KillShadow(g_Enemy[nCnt].nIdxShadow);   // �G�̉e������
		EnableMap(g_Enemy[nCnt].nIdxMap);		// �}�b�v�������
		g_Enemy[nCnt].bUse = false;			    // ���g�p����

		g_nNumEnemy--; // �f�N�������g

		if (g_bSound == false) // �������̃t���[������x�������炳��ĂȂ��Ȃ�炷
		{
			// �T�E���h��ݒ肷��
			SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

			g_bSound = true;

		}
		return;
	}
	else
	{
		if (g_bSound == false) // �������̃t���[������x�������炳��ĂȂ��Ȃ�炷
		{
			// �T�E���h��ݒ肷��
			SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

			g_bSound = true;
		}

		g_Enemy[nCnt].state = ENEMYSTATE_DAMAGE; // �G�̏�Ԃ��_���[�W�ɂ���

		g_Enemy[nCnt].g_bDamage = false;		 // �_���[�W��ʂ�Ȃ�����

		g_Enemy[nCnt].nCounterState = 20;		 // �_���[�W��Ԃ���m�[�}���ɖ߂�܂ł̎���
		g_Enemy[nCnt].HitStopCount = 10;         // �q�b�g�X�g�b�v�̎���

		// �X�R�A�����Z
		if (gamestate != GAMESTATE_END)
		{
			AddScore(4300);
		}
	}
}
//=========================================================================================================
//�G�̐ݒ菈��
//=========================================================================================================
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife,float Speed)
{
	// ���݂̃��[�h�̎擾
	MODE mode = GetMode();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == false)
		{
			g_Enemy[nCntEnemy].Motion = g_LoadEnemy[nType]; // ������

			g_Enemy[nCntEnemy].pos = pos;	  // ���W
			g_Enemy[nCntEnemy].nType = nType; // ���
			g_Enemy[nCntEnemy].nLife = nLife; // �̗�
			g_Enemy[nCntEnemy].nMaxLife = nLife; // �ő�̗�
			g_Enemy[nCntEnemy].Speed = Speed; // ���̑���
			g_Enemy[nCntEnemy].nCountAction = 0;
			g_Enemy[nCntEnemy].bUse = true;   // �g�p���

			g_Enemy[nCntEnemy].rotDest.y = (float)(rand()% 628 - 314) * 0.01f;	  // �p�x

			if (nType == ENEMYTYPE_SIX)
			{
				g_Enemy[nCntEnemy].nLife = 1;
			}

			g_Enemy[nCntEnemy].nIdxMap = SetMiniMap(pos,MINIMAPTEX_ENEMY);

			// �e��ݒ�
			g_Enemy[nCntEnemy].nIdxShadow = SetShadow(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 1.0f, g_Enemy[nCntEnemy].pos.z), D3DXVECTOR3(0.0f,0.0f,0.0f), 40.0f, 1.0f);

			g_nNumEnemy++; // �C���N�������g
			break;
		}
	}
}
//=========================================================================================================
//�G�̏o��
//=========================================================================================================
void SpawnEnemy(int nSpawner)
{
	if (g_nNumEnemy < MAX_ENEMY * 0.75f)
	{
		// �X�|�i�[0
		if (nSpawner == 0)
		{
			SetEnemy(D3DXVECTOR3(329.0f, 0.0f, 1283.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3(1065.0f, 0.0f, 188.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
		// �X�|�i�[1
		else if (nSpawner == 1)
		{
			SetEnemy(D3DXVECTOR3(1065.0f, 0.0f, 188.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3(329.0f, 0.0f, 1283.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
		// �X�|�i�[2
		else if (nSpawner == 2)
		{
			SetEnemy(D3DXVECTOR3(-592.0f, 0.0f, -747.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3(-899.0f, 0.0f, 182.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
		else if (nSpawner == 3)
		{
			SetEnemy(D3DXVECTOR3(-899.0f, 0.0f, 182.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3(-592.0f, 0.0f, -747.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3(329.0f, 0.0f, 1283.0f), rand() % ENEMYTYPE_SEVEN, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
	}
	
}
//=========================================================================================================
//�G�̑����擾����
//=========================================================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=========================================================================================================
// �G�̎��E�̔���
//=========================================================================================================
bool CollisionView(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float fRange, float viewAngle)
{
	// ����p�ϐ�
	bool bCollision = false;

	// �v���C���[���擾
	Player* pPlayer = GetPlayer();

	// ���E�͈̔͂̃x�N�g��
	D3DXVECTOR3 ViewVectorL = {};
	D3DXVECTOR3 ViewVectorR = {};

	// �O��
	D3DXVECTOR3 ViewCrossL = {};
	D3DXVECTOR3 ViewCrossR = {};

	// �����̎��E���쐬
	ViewVectorL.x = (pPos->x + sinf(pRot->y + D3DX_PI * viewAngle) * fRange) - pPos->x;
	ViewVectorL.y = 0.0f;
	ViewVectorL.z = (pPos->z + cosf(pRot->y + D3DX_PI * viewAngle) * fRange) - pPos->z;

	// �E���̎��E���쐬
	ViewVectorR.x = (pPos->x + sinf(pRot->y - D3DX_PI * viewAngle) * fRange) - pPos->x;
	ViewVectorR.y = 0.0f;
	ViewVectorR.z = (pPos->z + cosf(pRot->y - D3DX_PI * viewAngle) * fRange) - pPos->z;

	// �v���C���[�܂ł̃x�N�g��
	D3DXVECTOR3 PlayerVec = pPlayer->pos - *pPos;

	// �v���C���[����G�܂ł̃x�N�g���Ƃ̊O��
	D3DXVec3Cross(&ViewCrossL, &ViewVectorL, &PlayerVec);
	D3DXVec3Cross(&ViewCrossR, &ViewVectorR, &PlayerVec);

	// ���K������
	D3DXVec3Normalize(&ViewCrossL, &ViewCrossL);
	D3DXVec3Normalize(&ViewCrossR, &ViewCrossR);


	//D3DXVECTOR3 pos(pPos->x + sinf(pRot->y + D3DX_PI * viewAngle) * fRange,0.0f, pPos->z + cosf(pRot->y + D3DX_PI * viewAngle) * fRange);
	//D3DXVECTOR3 pos2(pPos->x + sinf(pRot->y - D3DX_PI * viewAngle) * fRange, 0.0f, pPos->z + cosf(pRot->y - D3DX_PI * viewAngle) * fRange);

	//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10.0f, 0.02f, 0);
	//SetEffect(pos2, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10, 10.0f, 0.02f, 0);

	// ���E�͈͓̔���������
	if (ViewCrossL.y > 0.0f && ViewCrossR.y < 0.0f)
	{
		// ���E���ɂ���
		bCollision = true;
	}

	// �����Ԃ�
	return bCollision;
}
//===============================================================================================================
// �G�̊p�x�̐ݒ�
//===============================================================================================================
float SetAngle(D3DXVECTOR3* pRot, D3DXVECTOR3* pPos)
{
	Player* pPlayer = GetPlayer();

	float fAngle = atan2f(pPlayer->pos.x - pPos->x, pPlayer->pos.z - pPos->z); // �G����v���C��܂ł̊p�x�����߂�

	// �p�x��Ԃ�
	return fAngle;
}
//===============================================================================================================
//�G�̃��[�h����
//===============================================================================================================
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
	int nCntKey = 0;				// �L�[�J�E���g�p
	int nCntPosKey = 0;				// pos�J�E���g
	int nCntRotkey = 0;				// rot�J�E���g
	int nScanData = 0;              // �߂�l����p

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
	case 5:
		pFile = NULL; //NULL�ɂ���
		g_LoadEnemy[nType].nNumModel = 1;
		g_LoadEnemy[nType].aModel[0].nIdxModelParent = -1;


		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data\\MODEL\\enemy600\\subenemy000_drone.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_LoadEnemy[nType].aModel[0].pBuffMat,
			NULL,
			&g_LoadEnemy[nType].aModel[0].dwNumMat,
			&g_LoadEnemy[nType].aModel[0].pMesh);
		break;
	case 6:
		pFile = NULL; //NULL�ɂ���
		g_LoadEnemy[nType].nNumModel = 1;
		g_LoadEnemy[nType].aModel[0].nIdxModelParent = -1;

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data\\MODEL\\enemy700\\subenemy001_escape.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_LoadEnemy[nType].aModel[0].pBuffMat,
			NULL,
			&g_LoadEnemy[nType].aModel[0].dwNumMat,
			&g_LoadEnemy[nType].aModel[0].pMesh);
		break;
	default:
		pFile = NULL; //NULL�ɂ���
		break;
	}

	char Skip[3] = {};
	int nNumModel = 0;
	int nLoadCnt = 0;

	if (pFile != NULL)
	{//�@NULL ����Ȃ�
		char aStr[ENEMY_WORD];

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
				g_LoadEnemy[nType].nNumModel = nNumModel;	// ���f���̍ő吔����
			}

			// ���f���̓ǂݍ��݂��܂��I����Ă��Ȃ�������
			if (nLoadCnt < nNumModel)
			{
				// ���f���̓ǂݍ��񂾐���Ԃ�
				nLoadCnt = LoadEnemyFilename(pFile, nNumModel, &aStr[0], nType);
			}

			// CHARACTERSET��ǂݎ������
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// �p�[�c�̐ݒ菈��
				LoadEnemyCharacterSet(pFile, &aStr[0], nNumParts, nType);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadEnemyMotionSet(pFile, &aStr[0], nNumModel, nType);
			}

			// END_SCRIPT��ǂݎ������
			if (nData == EOF)
			{
				nCntMotionEnemy = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
			}
		}// while����
	}
	else
	{
		if (nType != ENEMYTYPE_SIX && nType != ENEMYTYPE_SEVEN)
		{
			//���b�Z�[�W�{�b�N�X
			MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(enemy.cpp)", MB_OK);
		}
		return;
    }
	// �t�@�C�������
	fclose(pFile);

}
//===============================================================================================================
// �G�̃t�@�C���l�[���̃��[�h����
//==============================================================================================================
int LoadEnemyFilename(FILE* pFile, int nNumModel, char* aString, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//���_���W�̎擾
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^

	char Skip[3] = {}; // [=]�ǂݔ�΂��悤�ϐ�

	int nCntLoadModel = 0; // ���f���̃��[�h�̃J�E���^�[
	int nScanData = 0;              // �߂�l����p

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
				&g_LoadEnemy[nType].aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadEnemy[nType].aModel[nCntLoadModel].dwNumMat,
				&g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh);

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadEnemy[nType].aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadEnemy[nType].aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadEnemy[nType].aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//���_���̎擾
			nNumVtx = g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.x = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.x - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.x;
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.y = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.y - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.y;
			g_LoadEnemy[nType].aModel[nCntLoadModel].Size.z = g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMax.z - g_LoadEnemy[nType].aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_LoadEnemy[nType].aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
		}
	}
	return nCntLoadModel; // ���f���̃J�E���g��Ԃ�
}
//===============================================================================================================
// �G�̃L�����N�^�[�ݒ菈��
//==============================================================================================================
void LoadEnemyCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
{
	int nIdx = 0; // �C���f�b�N�X�i�[�ϐ�
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0;          // �߂�l����p

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
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aModel[nIdx].offrot.z);
		}
	}
}
//===============================================================================================================
// �G�̃��[�V�����̃��[�h����
//==============================================================================================================
void LoadEnemyMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0;          // �߂�l����p

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
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩���Ȃ���
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotionEnemy].bLoop);
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotionEnemy].nNumkey);
		}
		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadEnemyKeySet(pFile, aString, nType, nCntMotionEnemy);
		}
		// END_MOTIONSET��ǂݒʂ�����
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionEnemy++; // ���[�V�����̃J�E���g�����Z�b�g
			nKeyEnemy = 0;     // �L�[�����Z�b�g
			break;
		}
	}

}
//===============================================================================================================
// �G�̃��[�V�����̃L�[����
//==============================================================================================================
void LoadEnemyKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nCntPos = 0;   // �ʒu�̃J�E���g
	int nCntRot = 0;   // �p�x�̃J�E���g
	int nScanData = 0;          // �߂�l����p

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
			nScanData = fscanf(pFile, "%d", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadEnemy[nType].aMotionInfo[nCntMotion].aKeyInfo[nKeyEnemy].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSET��ǂݒʂ�����
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyEnemy++;		 // �L�[�𑝂₷
			nCntPos = 0; // �ʒu�̃J�E���g�����Z�b�g
			nCntRot = 0; // �p�x�̃J�E���g�����Z�b�g
			break;
		}
	}
}

//===============================================================================================================
// �G�̃z�[�~���O����
//===============================================================================================================
void AgentEnemy(int nCntEnemy)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	float fRotDest, fRotDiff;

	fRotDest = atan2f(g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x, g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z);//�ڕW�̈ړ�����(�p�x)

	fRotDiff = fRotDest - g_Enemy[nCntEnemy].rot.y;//�ڕW�̈ړ�����(�p�x)

	g_Enemy[nCntEnemy].move.x += sinf(g_Enemy[nCntEnemy].rot.y) * -g_Enemy[nCntEnemy].Speed * MAX_ENEMYMOVE;

	g_Enemy[nCntEnemy].move.z += cosf(g_Enemy[nCntEnemy].rot.y) * -g_Enemy[nCntEnemy].Speed * MAX_ENEMYMOVE;
}
//===============================================================================================================
// �G�ƓG�̓����蔻��
//===============================================================================================================
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

			// ���a��ݒ�
			float Eradius = 15.0f; 

			// �z�[�~���O���Ă��锼�a
			float Radius = Eradius + Eradius;

			// ���a���Z�o
			Radius = Radius * Radius;

			//�͈͓��ɓ�����
			if (fDistance <= Radius)
			{
				D3DXVECTOR3 vector = g_Enemy[nCntEnemy].pos - g_Enemy[nCnt].pos; // �͂��������x�N�g��
				D3DXVec3Normalize(&vector, &vector);							 // ���K��

				// �ړ��ʂ̐ݒ�
				g_Enemy[nCnt].move.x -= vector.x * g_Enemy[nCntEnemy].Speed;
				g_Enemy[nCnt].move.z -= vector.z * g_Enemy[nCntEnemy].Speed;
			}
		}
	}
}
//===============================================================================================================
// �G�̒ǐՏ����̍X�V����
//===============================================================================================================
void UpdateHomingEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// ��΂ɒǐՂ���͈͂ɂ��邩�𔻒�
	const bool is_absolute = sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 50.0f, 500.0f) == true;

	// �ǐՂ���͈͂ɂ��邩����
	const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 50.0f, 1800.0f) == true;

	// ���E���ɂ��邩�𔻒�
	const bool is_ViewBounds = CollisionView(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].rot, 200.0f, 0.75f) == true;

	// �U����Ԃ��𔻒�
	const bool is_NotAction = g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION;

	// �G�̎�ނ�7�Ԗڂ���Ȃ����𔻒�
	const bool is_NotTypeSeven = g_Enemy[nCntEnemy].nType != ENEMYTYPE_SEVEN;

	// �ǐՂł��邩�𔻒�s
	const bool is_CanAgent = ((is_sphereBounds == true && is_ViewBounds == true) || is_absolute == true) && is_NotAction == true && is_NotTypeSeven == true;

	// �ǐՂł���
	if (is_CanAgent == true)
	{
		// �z�[�~���O����
		AgentEnemy(nCntEnemy);

		// ���[�V������������[�V�����ɂ���
		g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_MOVE;

		// �G�̊p�x��ݒ�
		g_Enemy[nCntEnemy].rotDest.y = SetAngle(&g_Enemy[nCntEnemy].rot, &g_Enemy[nCntEnemy].pos) + D3DX_PI;
	}
	else
	{
		// �U�����[�V��������Ȃ�
		if (g_Enemy[nCntEnemy].Motion.motionType != MOTIONTYPE_ACTION)
		{
			// ���[�V�������j���[�g�����ɂ���
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;
		}
	}
}
//===============================================================================================================
// ������G�̍X�V����
//===============================================================================================================
void UpdateRunAwayEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// �G�̎�ނ��X�R�A������Ȃ�
	if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SEVEN)
	{
		// �̗͂��ő�l�Ɠ����A�܂�m�[�_���Ȃ牽�����Ȃ�
		if (g_Enemy[nCntEnemy].nMaxLife == g_Enemy[nCntEnemy].nLife)
		{

		}
		else // �����ł��_���[�W���󂯂Ă�
		{
			// �v���C���[�̋߂��ɋ�����
			if (sphererange(&g_Enemy[nCntEnemy].pos, &pPlayer->pos, 50.0f, 80.0f))
			{
				// �G���v���C���[�Ɍ����킹��
				AgentEnemy(nCntEnemy);

				// �G�̊p�x��ݒ�
				g_Enemy[nCntEnemy].rotDest.y = SetAngle(&g_Enemy[nCntEnemy].rot, &g_Enemy[nCntEnemy].pos);
			}

			// ���̑f
			float fColor[3];
			fColor[0] = ((float)(rand() % 100) / 100.0f);
			fColor[1] = ((float)(rand() % 100) / 100.0f);
			fColor[2] = ((float)(rand() % 100) / 100.0f);

			// ���������ȃI�[�����o��
			SetParticle(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 10, g_Enemy[nCntEnemy].pos.z),
				D3DXVECTOR3(D3DX_PI / 2.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(fColor[0], fColor[1], fColor[2], 1.0f),
				2.0f, 1, 10, 5, 20.0f, 20.0f, true, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		}
	}
}
//===============================================================================================================
// �G�̍U�����[�V�����̍X�V����
//===============================================================================================================
void UpdateAttackState(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	// ���[�V�����̎��
	int Motiontype = g_Enemy[nCntEnemy].Motion.motionType;

	// �L�[
	int nKey = g_Enemy[nCntEnemy].Motion.nKey;

	// �Ō�̃L�[
	int LastKey = g_Enemy[nCntEnemy].Motion.aMotionInfo[Motiontype].nNumkey - 1;

	// ���[�V�����̃J�E���g
	int nCounter = g_Enemy[nCntEnemy].Motion.nCountMotion;

	// �ő�t���[��
	int EndFrame = g_Enemy[nCntEnemy].Motion.aMotionInfo[Motiontype].aKeyInfo[LastKey].nFrame;

	// �v���C���[�̃��f���̏�����
	D3DXVECTOR3 PlayerModel(pPlayer->Motion.aModel[0].mtxWorld._41, pPlayer->Motion.aModel[0].mtxWorld._42, pPlayer->Motion.aModel[0].mtxWorld._43);

	// �G�̃��f���̏�����
	D3DXVECTOR3 EnemyModel(g_Enemy[nCntEnemy].Motion.aModel[3].mtxWorld._41, g_Enemy[nCntEnemy].Motion.aModel[3].mtxWorld._42, g_Enemy[nCntEnemy].Motion.aModel[3].mtxWorld._43);
	
	// �͈͓��ɂ��邩�𔻒�
	const bool is_sphereBounds = sphererange(&PlayerModel, &EnemyModel, 20.0f, 50.0f) == true;

	// �G���U����Ԃ��𔻒�
	const bool is_EnemyNotAction = g_Enemy[nCntEnemy].AttackState == ENEMYATTACK_ATTACK;

	// �v���C���[���_���[�W��Ԃ��ǂ����𔻒�
	const bool is_PlayerNotDamage = pPlayer->state != PLAYERSTATE_DAMAGE;

	// �U�����ł��邩�𔻒�
	const bool CanDamage = is_sphereBounds == true && is_EnemyNotAction == true && is_PlayerNotDamage == true;

	// �U���͈͂ɓ�����
	if (CanDamage == true && CheckMotionBounds(nKey, nCounter,4, LastKey,0, EndFrame) == true)
	{
		// �v���C���[�Ƀ_���[�W��^����
		HitPlayer(50,false, 0, 0);
	}
}
//===============================================================================================================
// ���ł�G�̍X�V����
//===============================================================================================================
void UpdateDroneEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	if (g_Enemy[nCntEnemy].nType == ENEMYTYPE_SIX)
	{
		// �ǔ�����
		AgentEnemy(nCntEnemy);

		// �͈͓���������
		if (sphererange(&pPlayer->pos, &g_Enemy[nCntEnemy].pos, 20.0f, 300.0f))
		{
			g_Enemy[nCntEnemy].nCountAction++;
		}

		float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z); // �G����v���C��܂ł̊p�x�����߂�
		D3DXVECTOR3 dest = pPlayer->pos - g_Enemy[nCntEnemy].pos; // �v���C���[�̃x�N�g�������߂�
		D3DXVec3Normalize(&dest, &dest); // ���K������

		g_Enemy[nCntEnemy].rotDest.y = fAngle + D3DX_PI; // �p�x����

		// �e�𔭎˂��鏈��
		if (g_Enemy[nCntEnemy].nCountAction >= 120)
		{
			// ������ꏊ�A�x�N�g���A�����A�����A�З́A�傫���A���x
			SetBullet(g_Enemy[nCntEnemy].pos, dest, D3DXVECTOR3(0.0f, fAngle, 0.0f), 60, 2, 50.0f, 3.0f, true);

			// �������̓V���b�g�K��������ׂ̏���
			/*float fRand[3];
			for (int nCount = 0; nCount < 10; nCount++)
			{
				fRand[0] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
				fRand[1] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
				fRand[2] = (rand() / (double)RAND_MAX) * 0.4 - 0.2;
				SetBullet(g_Enemy[nCntEnemy].pos, D3DXVECTOR3(dest.x + fRand[0], dest.y + fRand[1], dest.z + fRand[2]), D3DXVECTOR3(0.0f, fAngle, 0.0f), 60, 2, 10.0f, 3.0f, true);
			}*/
			g_Enemy[nCntEnemy].nCountAction = 0;
		}
	}
}
//===============================================================================================================
// ���Ƌ��̓����蔻��
//===============================================================================================================
bool CollisionLine(D3DXVECTOR3* pFirstPos, D3DXVECTOR3* pEndPos,float fRadius)
{
	BLOCK* pBlock = GetBlock();
	Camera* pCamera = GetCamera();

	// ���̔��a
	float radius = fRadius;

	// ���̔��a�����߂�
	float Radius = radius * radius;

	// �u���b�N�̍ő吔����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		// �u���b�N���g���Ă��Ȃ����
		if (pBlock->bUse == false)
		{
			// ������ǂݔ�΂�
			continue;
		}

		// �����̏I�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvEnd = *pFirstPos - pBlock->Obb.CenterPos;

		// �����̏I�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvFirst = *pEndPos - pBlock->Obb.CenterPos;

		// �n�_�`�I�_�܂ł̃x�N�g��
		D3DXVECTOR3 sv = *pFirstPos - *pEndPos;

		// ����X
		float LengthX = pFirstPos->x - pEndPos->x;
		// ����Y
		float LengthY = pFirstPos->y - pEndPos->y;
		// ����Z
		float LengthZ = pFirstPos->z - pEndPos->z;

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
			pCamera->nBlockIdx = nCntBlock;
			return true;
		}
		// ��������
		else if (D3DXVec3Length(&pvFirst) < Radius)
		{
			pCamera->nBlockIdx = nCntBlock;
			return true;
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
				pCamera->nBlockIdx = nCntBlock;
				return true;
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
				pCamera->nBlockIdx = nCntBlock;
				return true;
			}
		}
	}
	return false;
}
//===============================================================================================================
// �L�����̎擾����
//===============================================================================================================
int GetNumKill(void)
{
	return g_nNumKill;
}
//===============================================================================================================
// �炷���̐ݒ�
//===============================================================================================================
void SetSoundWepon(int nType)
{
	switch (nType)
	{
	case ITEMTYPE_BAT: // �o�b�g�����Ă���Ƃ�
		// ���y�Đ�
		PlaySound(SOUND_LABEL_BAT_SE);
		break;

	case ITEMTYPE_STONEBAT: // �΃o�b�g
		// ���y�Đ�
		PlaySound(SOUND_LABEL_BAT_SE);
		break;

	case ITEMTYPE_HUNMER: // �n���}�[�����Ă���Ƃ�
		// ���y�Đ�
		PlaySound(SOUND_LABEL_HAMMER_SE);
		break;

	case ITEMTYPE_TORCH: // �������Ă���Ƃ�
		// ���y�Đ�
		PlaySound(SOUND_LABEL_WEPONFIRESE);
		break;

	case ITEMTYPE_KATANA: // �������Ă���Ƃ�
		// ���y�Đ�
		PlaySound(SOUND_LABEL_ACTION_SE);
		break;

	case ITEMTYPE_FISH: // ���n��
		// ���y�Đ�
		PlaySound(SOUND_LABEL_WATERWEPON);
		break;

	case ITEMTYPE_SPEAR: // ���̕��퉹
		// ���y�Đ�
		PlaySound(SOUND_LABEL_SPEARWEPON);
		break;

	case ITEMTYPE_IRON: // �S�n���̕��퉹
		// ���y�Đ�
		PlaySound(SOUND_LABEL_METALWEPON);
		break;

	case ITEMTYPE_HARISEN: // �n���Z�����퉹
		// ���y�Đ�
		PlaySound(SOUND_LABEL_HARISENWEPON);
		break;

	case ITEMTYPE_TELEPHONEPOLE: // �d��
		// ���y�Đ�
		PlaySound(SOUND_LABEL_TELPHONWEPON);
		break;

	default: // ���펝���Ă��Ȃ��� 
		// ���y�Đ�
		PlaySound(SOUND_LABEL_NOWEPON);
		break;
	}
}