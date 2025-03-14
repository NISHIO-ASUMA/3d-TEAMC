//==============================================================================================================
//
// �{�X [boss.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "boss.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "player.h"
#include "Shadow.h"
#include "block.h"
#include "Damagepop.h"
#include "Particle.h"
#include "explosion.h"
#include "HPGauge.h"
#include "spgauge.h"
#include "Score.h"
#include "time.h"
#include "sound.h"
#include "game.h"
#include "polygon.h"
#include "wall.h"
#include "meshimpact.h"
#include "minimap.h"
#include "billboard.h"
#include "bosslife.h"
#include "Effect.h"
#include "math.h"
#include "meshcylinder.h"
#include "mark.h"
#include "event.h"
#include "effectEdit.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_WORD (256)			 // �ő�̕�����
#define SHADOWSIZEOFFSET (40.0f) // �e�̑傫���̃I�t�Z�b�g
#define SHADOW_A (1.0f)          // �e�̔Z���̊
#define NUM_MTX (8)				 //�@���̃��[���h�}�g���b�N�X�̐�
#define BOSS_LIFE (10000)        // �{�X�̍ő�HP
#define TRANSPARENT_FRAME (300.0f) // �������̃t���[��
#define MAX_ELEMENT (5) // �����̍ő吔

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void LoadBoss(void);																		// �{�X��ǂݍ���
int LoadBossFilename(FILE* pFile, int nNumModel, char* aString, int nType);					// �{�X�̃��f���̃��[�h����
void LoadBossCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType);			// �{�X�̃p�[�c�̐ݒ菈��
void LoadBossMotionSet(FILE* pFile, char* aString, int nNumModel, int nType);				// �{�X�̃��[�V�����̃��[�h����
void LoadBossKeySet(FILE* pFile, char* aString, int nType, int nCntMotion);					// �{�X�̃��[�V�����̃L�[�̓ǂݍ��ݏ���

void colisionSword(int nCntBoss);															// ���ƃ{�X�̓����蔻��
void CollisionToBoss(int nCntBoss);															// �{�X�ƃ{�X�̓����蔻��
bool SetAbnormalCondition(int nType, int nTime, int nDamage,int nCntBoss);					// ��Ԉُ�̐ݒ�
void UpdateAbnormalCondition(int nCntBoss);													// ��Ԉُ�̍X�V����
void SetRasuAttack(int nCntBoss);															// �{�X�̓ːi�U���̐ݒ�
void SetDoubleRasuAttack(int nCntBoss);														// �{�X�̓��ːi���Ă���U������
void UpdateAgentBoss(int nCntBoss);															// �{�X�̒ǐՂ̍X�V����
void DeathMotionContlloer(int nCntBoss);													// �{�X�̎��S���[�V�����̏���
void EndEventBossState(int nCntBoss);														// �C�x���g���I�������Ƀ{�X����������
void HitBossAbnormalCondition(int nCntBoss);												// �{�X�ɓ����������̃G�t�F�N�g
void HitBossAbnormalConditionParam(int nCntBoss, int nElement, int ChargeValue, int MaxCharge, int stateCnt);  // �{�X�ɓ����������̃G�t�F�N�g�̃p�����[�^
void SpawnItem(D3DXVECTOR3 pos);

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
Boss g_Boss[MAX_BOSS]; // �{�X�̏��
MOTION g_LoadBoss;
int nKeyBoss, nCntMotionBoss;

//===============================================================================================================
// �{�X�̏���������
//===============================================================================================================
void InitBoss(void)
{
	//InitBossLife();

	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �S���̃{�X����
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_Boss[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
		g_Boss[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_Boss[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�x
		g_Boss[nCnt].bUse = false;						   // ���g�p���
		g_Boss[nCnt].nLife = BOSS_LIFE;					   // �̗�
		g_Boss[nCnt].state = BOSSSTATE_NORMAL;			   // ���
		g_Boss[nCnt].Speed = 5.0f;						   // ���̑���
		g_Boss[nCnt].AttackState = BOSSATTACK_NO;		   // �U�����
		g_Boss[nCnt].nHitStopCount = 0;                    // �q�b�g�X�g�b�v�̃J�E���g
		g_Boss[nCnt].BossMat = {};						   // �{�X�̃}�e���A��
		g_Boss[nCnt].bTransparent = false;                 // �{�X�𓧖��ɂ���t���O

		for (int nCnt2 = 0; nCnt2 < MAX_ELEMENT; nCnt2++)
		{
			g_Boss[nCnt].nStateCharge[nCnt2] = 0;
			g_Boss[nCnt].nStateCount[nCnt2] = 0;
		}
	}

	LoadBoss(); // �{�X�̃��[�h
}
//===============================================================================================================
// �{�X�̏I������
//===============================================================================================================
void UninitBoss(void)
{
	//UninitBossLife();

	for (int nCntModel = 0; nCntModel < g_LoadBoss.nNumModel; nCntModel++)
	{
		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < (int)g_LoadBoss.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_LoadBoss.aModel[nCntModel].pMesh != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pMesh->Release();
			g_LoadBoss.aModel[nCntModel].pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (g_LoadBoss.aModel[nCntModel].pBuffMat != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pBuffMat->Release();
			g_LoadBoss.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	// �S�{�X����
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		// ���f������
		for (int nCntModel = 0; nCntModel < g_Boss[nCntBoss].Motion.nNumModel; nCntModel++)
		{
			// �e�N�X�`���̔j��
			for (int nCntTex = 0; nCntTex < (int)g_Boss[nCntBoss].Motion.aModel[nCntModel].dwNumMat; nCntTex++)
			{
				if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
				{
					g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
				}
			}

			// ���b�V���̔j��
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh = NULL;
			}

			// �}�e���A���̔j��
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

}
//===============================================================================================================
// �{�X�̍X�V����
//===============================================================================================================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();
	GAMESTATE gameState = GetGameState();

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_Boss[nCnt].nHitStopCount--;

		// �g�p��Ԃ̂�
		if (g_Boss[nCnt].bUse == false || g_Boss[nCnt].nHitStopCount > 0)
		{
			continue;
		}

		// �{�X��HP�Q�[�W�̍X�V
		UpdateBossLife(&g_Boss[nCnt]);		

		// ��Ԉُ�̍X�V����
		UpdateAbnormalCondition(nCnt);

		switch (g_Boss[nCnt].state)
		{
		case BOSSSTATE_NORMAL:
			break;
		case BOSSSTATE_AGENT:
			break;
		case BOSSSTATE_ATTACK:
			break;
		case BOSSSTATE_DAMAGE:
			g_Boss[nCnt].nCounterState--;

			if (g_Boss[nCnt].nCounterState <= 0)
			{
				g_Boss[nCnt].state = BOSSSTATE_NORMAL; // �G�̏�Ԃ��m�[�}���ɂ���
			}
			break;
		default:
			break;
		}

		// �~�j�}�b�v�̈ʒu�̐ݒ�
		SetMiniMapPotision(g_Boss[nCnt].nIdxMap, &g_Boss[nCnt].pos);

		// HP�Q�[�W�̈ʒu�ݒ菈��
		SetPositionLifeBar(g_Boss[nCnt].nLifeBarIdx, g_Boss[nCnt].nLifeFrame,g_Boss[nCnt].nLifeDelayIdx,g_Boss[nCnt].pos);

		// �ړ��ʂ̌���
		g_Boss[nCnt].move.x += (0.0f - g_Boss[nCnt].move.x) * 0.25f;
		g_Boss[nCnt].move.z += (0.0f - g_Boss[nCnt].move.z) * 0.25f;

		// �O��̈ʒu����
		g_Boss[nCnt].posOld = g_Boss[nCnt].pos;

		// �ʒu�̍X�V
		g_Boss[nCnt].pos += g_Boss[nCnt].move;

		// �e�̈ʒu�̍X�V
		SetPositionShadow(g_Boss[nCnt].nIdxShadow, g_Boss[nCnt].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Boss[nCnt].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Boss[nCnt].pos.y / 30.0f));

		// ���[�V�����̍X�V����
		UpdateMotion(&g_Boss[nCnt].Motion);

		// ���[�V�������I�������
		if (g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].bLoop == false && g_Boss[nCnt].Motion.nKey >= g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].nNumkey - 1)
		{
			g_Boss[nCnt].AttackState = BOSSATTACK_NO;					// �{�X�̍U����Ԃ��U�����ĂȂ���Ԃɂ���
		}

		// �Q�[���̏�Ԃ����[�r�[��������
		if (gameState == GAMESTATE_MOVIE)
		{
			continue;
		}

		if (g_Boss[nCnt].Motion.motiontypeBlend == MOTIONTYPE_DEATH)
		{
			DeathMotionContlloer(nCnt);
			continue;
		}

		// �G�̍U���̐ݒ�
		switch (g_Boss[nCnt].Motion.motiontypeBlend)
		{
		case MOTIONTYPE_ACTION:
			SetRasuAttack(nCnt);
			break;
		case MOTIONTYPE_ACTION2:
			SetDoubleRasuAttack(nCnt);
			break;
		default:
			break;
		}

		// �A�C�e��������������
		if (HitThrowItem(&g_Boss[nCnt].pos, 10.0f, 50.0f) && g_Boss[nCnt].state != BOSSSTATE_DAMAGE)
		{
			HitBoss(nCnt, (float)pPlayer->nDamage * 1.5f);
		}

		// �ǂƂ̓����蔻��
		CollisionWall(&g_Boss[nCnt].pos, &g_Boss[nCnt].posOld, &g_Boss[nCnt].move, g_Boss[nCnt].Speed);

		// �C���p�N�g�̓����蔻��
		if (CollisionImpact(&g_Boss[nCnt].pos) == true)
		{
			HitBoss(nCnt, ImpactDamege(0));
		}
		
		// �͈͓��ɂ���
		if (KeepInCylinder(&pPlayer->pos) == false)
		{
			// �{�X�̒ǐՏ����̍X�V
			UpdateAgentBoss(nCnt);

			// �U�����Ă��Ȃ�
			if (CheckActionMotion(&pPlayer->Motion) == false)
			{
				colisionSword(nCnt);   // ���Ƃ̓����蔻��
			}
		}
		else
		{
			SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_NEUTRAL, true, 10);
			g_Boss[nCnt].AttackState = BOSSATTACK_NO;
		}
		// �U���͈͂ɓ�������
		if (sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 50.0f) && g_Boss[nCnt].AttackState != BOSSATTACK_ATTACK)
		{
			// �s���p�^�[���������_���I����
			g_Boss[nCnt].nAttackPattern = rand() % 2;

			// �U����Ԃ̑J��
			switch (g_Boss[nCnt].nAttackPattern)
			{
			case 0:
				if (g_Boss[nCnt].Motion.motiontypeBlend != MOTIONTYPE_ACTION)
				{
					// ���[�V�������U���ɂ���
					SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_ACTION, true, 10);
				}
				break;
			case 1:
				if (g_Boss[nCnt].Motion.motiontypeBlend != MOTIONTYPE_ACTION2)
				{
					// ���[�V�������U���ɂ���
					SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_ACTION2, true, 10);
				}
				break;
			default:
				break;
			}

			g_Boss[nCnt].AttackState = BOSSATTACK_ATTACK; // �U�����Ă���
		}

		// �{�X�̍U������͈͂ɂ��邩�ǂ����𔻒�
		const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 20.0f) == true;

		// �v���C���[���_���[�W��Ԃ���Ȃ����𔻒�
		const bool is_PlayerNotDamage = pPlayer->state != PLAYERSTATE_DAMAGE;

		// �v���C���[���X�y�V�����A�^�b�N�����ĂȂ����𔻒�
		const bool is_SpAttack = pPlayer->AttackSp == false;

		// �U�����ł��邩�𔻒�
		const bool is_CanDamage = is_sphereBounds == true && is_PlayerNotDamage && is_PlayerNotDamage == true && is_SpAttack == true;

		// �U���͈͂ɓ�����
		if (is_CanDamage == true)
		{
			switch (g_Boss[nCnt].Motion.motiontypeBlend)
			{
			case MOTIONTYPE_ACTION:
				if (g_Boss[nCnt].Motion.nKey >= 3)
				{
					if (g_Boss[nCnt].nStateCount[3] > 0)
					{
						HitPlayer(30,true, nCnt, ATTACKER_BOSS);
					}
					else
					{
						HitPlayer(40,true, nCnt, ATTACKER_BOSS);
					}
				}
				break;
			case MOTIONTYPE_ACTION2:
				if (g_Boss[nCnt].Motion.nKey >= 2)
				{
					if (g_Boss[nCnt].nStateCount[3] > 0)
					{
						HitPlayer(30,true, nCnt, ATTACKER_BOSS);
					}
					else
					{
						HitPlayer(40,true, nCnt, ATTACKER_BOSS);
					}
				}
				break;
			default:
				break;
			}
		}


		if (EnableEvent() == false)
		{
			EndEventBossState(nCnt);
		}
		CollisionToBoss(nCnt); // �{�X�ƃ{�X�̓����蔻��
	}
}
//===============================================================================================================
// �{�X�̕`�揈��
//===============================================================================================================
void DrawBoss(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef; // ���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;	 // �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse == false)
		{
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Boss[nCnt].mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss[nCnt].rot.y, g_Boss[nCnt].rot.x, g_Boss[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Boss[nCnt].pos.x, g_Boss[nCnt].pos.y, g_Boss[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss[nCnt].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_Boss[nCnt].Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // �v�Z�p
			D3DXMATRIX mtxParent;				  // �e�̃}�g���b�N�X

			// �p�[�c�̃}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			// �p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss[nCnt].Motion.aModel[nCntModel].rot.y, g_Boss[nCnt].Motion.aModel[nCntModel].rot.x, g_Boss[nCnt].Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// �p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_Boss[nCnt].Motion.aModel[nCntModel].pos.x, g_Boss[nCnt].Motion.aModel[nCntModel].pos.y, g_Boss[nCnt].Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// �p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// �e���f��������ꍇ
				mtxParent = g_Boss[nCnt].Motion.aModel[g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// �e���f�����Ȃ��ꍇ
				mtxParent = g_Boss[nCnt].mtxWorld;
			}

			// �Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&mtxParent); // ����,����,�e

			// �p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss[nCnt].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Boss[nCnt].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// �J���[�ύX�p�̕ϐ�
				D3DXMATERIAL color;

				if (g_Boss[nCnt].state != BOSSSTATE_DAMAGE)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss[nCnt].state == BOSSSTATE_DAMAGE)
				{
					// �J���[����
					color = pMat[nCntMat];

					// �J���[��ύX
					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&color.MatD3D);
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Boss[nCnt].Motion.aModel[nCntModel].pTexture[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_Boss[nCnt].Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

		}
	}

	//// �}�e���A�������Ƃɖ߂�
	//pDevice->SetMaterial(&matDef);

	//DrawBossLife();
}
//===============================================================================================================
// �{�X�̐ݒ菈��
//===============================================================================================================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse == false)
		{// ���g�p�Ȃ�
			g_Boss[nCnt].Motion = g_LoadBoss;         // ������
			g_Boss[nCnt].pos = pos;					  // �ʒu����
			//g_Boss.Speed = speed;					  // ���̑���
			g_Boss[nCnt].nLife = nLife;				  // �̗͂�}��
			g_Boss[nCnt].state = BOSSSTATE_NORMAL;	  // ���
			g_Boss[nCnt].AttackState = BOSSATTACK_NO; // �U�����
			g_Boss[nCnt].nMaxLife = nLife;            // �ő��HP
			SetMotion(&g_Boss[nCnt].Motion, MOTIONTYPE_NEUTRAL, true, 10);
			g_Boss[nCnt].move = NULLVECTOR3;

			// ��Ԉُ�֌W
			for (int nCnt2 = 0; nCnt2 < 5; nCnt2++)
			{
				g_Boss[nCnt].nStateCharge[nCnt2] = 0;
				g_Boss[nCnt].nStateCount[nCnt2] = 0;
			}

			// �g�p��Ԃɂ���
			g_Boss[nCnt].bUse = true;   

			// �{�X�̃��f���̈ʒu����
			D3DXVECTOR3 BossPos(g_Boss[nCnt].mtxWorld._41, g_Boss[nCnt].mtxWorld._42, g_Boss[nCnt].mtxWorld._43);

			// �|���S�����Z�b�g
			SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20.0f, 100.0f, POLYGON_TYPE_FIVE);

			// �e���Z�b�g
			g_Boss[nCnt].nIdxShadow = SetShadow(BossPos, D3DXVECTOR3(0.0f,0.0f,0.0f), 40.0f,1.0f);

			// �~�j�}�b�v�Ƀ{�X�A�C�R����ݒ�
			g_Boss[nCnt].nIdxMap = SetMiniMap(BossPos, MINIMAPTEX_BOSS);

			// �{�X�̃��C�t�Q�[�W�̐ݒ�
			g_Boss[nCnt].nLifeDelayIdx = SetBossLife(pos, BOSSTEX_DELAY);
			g_Boss[nCnt].nLifeBarIdx = SetBossLife(pos,BOSSTEX_LIFE);
			g_Boss[nCnt].nLifeFrame = SetBossLife(pos, BOSSTEX_FRAME);

			break;
		}
	}
}
//===============================================================================================================
// �{�X�̃q�b�g����
//===============================================================================================================
void HitBoss(int nCntBoss,int nDamage)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �A�C�e���̎擾
	Item* pItem = GetItem();

	if (g_Boss[nCntBoss].nStateCount[4] > 0)
	{
		// HP�����炷
		g_Boss[nCntBoss].nLife -= nDamage * 1.5f;

		// �_���[�W��ݒ�
		SetDamege(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), // �ʒu
			nDamage * 1.5f,	// �_���[�W																								
			20,			// ����
			false);
	}
	else
	{
		// HP�����炷
		g_Boss[nCntBoss].nLife -= nDamage;

		// �_���[�W��ݒ�
		SetDamege(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), // �ʒu
			nDamage,	// �_���[�W																								
			20,			// ����
			false);
	}

	GAMESTATE gamestate = GetGameState();

	// �{�X������
	if (g_Boss[nCntBoss].nLife <= 0 && g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_DEATH)
	{
		// �Q�[�W������
		DeleateLifeBar(g_Boss[nCntBoss].nLifeBarIdx, g_Boss[nCntBoss].nLifeFrame, g_Boss[nCntBoss].nLifeDelayIdx);

		// ���񂾂�p�[�e�B�N�����o��(�G�����h���)
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			7.0f, 40, 60, 20, 7.0f, 20.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// ���[�V������Death����Ȃ�������
		if (g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_DEATH)
		{
			SetMotion(&g_Boss[nCntBoss].Motion, MOTIONTYPE_DEATH, true, 10);
		}

		g_Boss[nCntBoss].nLife = -1;

		if (gamestate != GAMESTATE_END)
		{
			if (pPlayer->FeverMode)
			{
				AddScore(30000);		// �X�R�A���擾
				AddSpgauge(10.5f);   // SP�Q�[�W���擾
			}
			else if (!pPlayer->FeverMode)
			{
				AddFever(10.0f);		// �t�B�[�o�[�|�C���g���擾
				AddScore(15000);		// �X�R�A���擾
				AddSpgauge(2.0f);       // SP�Q�[�W���擾
			}
		}


		// �ʏ핐�킪�����������̃T�E���h
		SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

		// �������퓖���������̃T�E���h
		SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

		return;
	}
	else
	{
		HitBossAbnormalCondition(nCntBoss);

		// �ʏ핐�킪�����������̃T�E���h
		SetSoundWepon(pItem[pPlayer->ItemIdx].nType);

		// �������퓖���������̃T�E���h
		SetCreateWeponSound(pItem[pPlayer->ItemIdx].nType);

		AddFever(10.0f);		// �t�B�[�o�[�|�C���g���擾

		// �p�[�e�B�N�����Z�b�g
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), g_Boss[nCntBoss].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 4.0f, 1, 20, 10, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Boss[nCntBoss].state = ENEMYSTATE_DAMAGE;
		g_Boss[nCntBoss].nCounterState = 20;
		g_Boss[nCntBoss].nHitStopCount = 8;
		AddSpgauge(1.0f);   // SP�Q�[�W���擾
	}
}
//===============================================================================================================
// �{�X�̓ǂݍ��ݏ���
//===============================================================================================================
void LoadBoss(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �t�@�C���|�C���^��錾
	FILE* pFile; 

	// �t�@�C�����J��
	pFile = fopen("data\\MOTION\\boss.txt", "r");

	// ���[�J���ϐ��錾-----------------------------
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;
	int nScanData = 0;
	int nNumModel = 0;
	char Skip[3] = {};
	int nLoadCnt = 0;

	//----------------------------------------------

	if (pFile != NULL)
	{//�@NULL ����Ȃ�
		// ������
		char aStr[MAX_WORD];

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
				g_LoadBoss.nNumModel = nNumModel;	// ���f���̍ő吔����
			}

			// ���f���̓ǂݍ��݂��܂��I����Ă��Ȃ�������
			if (nLoadCnt < nNumModel)
			{
				// ���f���̓ǂݍ��񂾐���Ԃ�
				nLoadCnt = LoadBossFilename(pFile, nNumModel, &aStr[0], 0);
			}

			// CHARACTERSET��ǂݎ������
			if (strcmp(&aStr[0], "CHARACTERSET") == 0)
			{
				// �p�[�c�̐ݒ菈��
				LoadBossCharacterSet(pFile, &aStr[0], nNumParts, 0);
			}

			// MOTIONSET��ǂݎ������
			if (strcmp(&aStr[0], "MOTIONSET") == 0)
			{
				// ���[�V�����̐ݒ菈��
				LoadBossMotionSet(pFile, &aStr[0], nNumModel, 0);
			}

			// END_SCRIPT��ǂݎ������
			if (nData == EOF)
			{
				nCntMotionBoss = 0; // ���[�V�����̃J�E���g�����Z�b�g����
				break;          // While���𔲂���
			}
		}// while����
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Boss.cpp)", MB_OK);
		return;
	}
	// �t�@�C�������
	fclose(pFile);
}
//===========================================================================================================
// �{�X�̃��f���̃��[�h����
//===========================================================================================================
int LoadBossFilename(FILE* pFile, int nNumModel, char* aString, int nType)
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
				&g_LoadBoss.aModel[nCntLoadModel].pBuffMat,
				NULL,
				&g_LoadBoss.aModel[nCntLoadModel].dwNumMat,
				&g_LoadBoss.aModel[nCntLoadModel].pMesh);

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_LoadBoss.aModel[nCntLoadModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadBoss.aModel[nCntLoadModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadBoss.aModel[nCntLoadModel].pTexture[nCntMat]);
				}
			}

			//���_���̎擾
			nNumVtx = g_LoadBoss.aModel[nCntLoadModel].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y�擾
			sizeFVF = D3DXGetFVFVertexSize(g_LoadBoss.aModel[nCntLoadModel].pMesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_LoadBoss.aModel[nCntLoadModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
				if (vtx.x < g_LoadBoss.aModel[nCntLoadModel].vtxMin.x)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadBoss.aModel[nCntLoadModel].vtxMin.y)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadBoss.aModel[nCntLoadModel].vtxMin.z)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMin.z = vtx.z;
				}

				else if (vtx.x > g_LoadBoss.aModel[nCntLoadModel].vtxMax.x)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.x = vtx.x;
				}

				else if (vtx.y > g_LoadBoss.aModel[nCntLoadModel].vtxMax.y)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.y = vtx.y;
				}

				else if (vtx.z > g_LoadBoss.aModel[nCntLoadModel].vtxMax.z)
				{
					g_LoadBoss.aModel[nCntLoadModel].vtxMax.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			////�T�C�Y����
			//g_player.Size.x = g_player.vtxMaxPlayer.x - g_player.vtxMinPlayer.x;
			//g_player.Size.y = g_player.vtxMaxPlayer.y - g_player.vtxMinPlayer.y;
			//g_player.Size.z = g_player.vtxMaxPlayer.z - g_player.vtxMinPlayer.z;

			//�e���f�����Ƃ̃T�C�Y����
			g_LoadBoss.aModel[nCntLoadModel].Size.x = g_LoadBoss.aModel[nCntLoadModel].vtxMax.x - g_LoadBoss.aModel[nCntLoadModel].vtxMin.x;
			g_LoadBoss.aModel[nCntLoadModel].Size.y = g_LoadBoss.aModel[nCntLoadModel].vtxMax.y - g_LoadBoss.aModel[nCntLoadModel].vtxMin.y;
			g_LoadBoss.aModel[nCntLoadModel].Size.z = g_LoadBoss.aModel[nCntLoadModel].vtxMax.z - g_LoadBoss.aModel[nCntLoadModel].vtxMin.z;

			//���_�o�b�t�@�̃A�����b�N
			g_LoadBoss.aModel[nCntLoadModel].pMesh->UnlockVertexBuffer();

			nCntLoadModel++; // ���f���̃J�E���g�𑝂₷
		}
	}
	return nCntLoadModel; // ���f���̃J�E���g��Ԃ�
}
//===========================================================================================================
// �{�X�̃p�[�c�̐ݒ菈��
//===========================================================================================================
void LoadBossCharacterSet(FILE* pFile, char* aString, int nNumparts, int nType)
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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aModel[nIdx].nIdxModelParent);
		}
		// POS��ǂݎ������
		else if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.x);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.y);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.z);
		}
		// ROT��ǂݎ������
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// ���f���̃I�t�Z�b�g����
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.x);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.y);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offrot.z);
		}
	}
}
//===========================================================================================================
// �{�X�̃��[�V�����̃��[�h����
//===========================================================================================================
void LoadBossMotionSet(FILE* pFile, char* aString, int nNumModel, int nType)
{
	char Skip[3] = {}; // [=]�ǂݔ�΂��ϐ�
	int nScanData = 0;          // �߂�l����p

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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotionBoss].bLoop);
		}
		// NUM_KEY��ǂݒʂ�����
		else if (strcmp(aString, "NUM_KEY") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔����
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotionBoss].nNumkey);
		}
		// KEYSET��ǂݒʂ�����
		if (strcmp(aString, "KEYSET") == 0)
		{
			// �L�[�̐ݒ菈��
			LoadBossKeySet(pFile, aString, nType, nCntMotionBoss);
		}
		// END_MOTIONSET��ǂݒʂ�����
		if (strcmp(aString, "END_MOTIONSET") == 0)
		{
			nCntMotionBoss++; // ���[�V�����̃J�E���g�����Z�b�g
			nKeyBoss = 0;     // �L�[�����Z�b�g
			break;
		}
	}
}
//===========================================================================================================
// �{�X�̃��[�V�����̃L�[�̓ǂݍ��ݏ���
//===========================================================================================================
void LoadBossKeySet(FILE* pFile, char* aString, int nType, int nCntMotion)
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
			nScanData = fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].nFrame);
		}

		// POS��ǂݒʂ�����
		if (strcmp(aString, "POS") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �ʒu��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosX);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosY);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntPos].fPosZ);
			nCntPos++;
		}
		// ROT��ǂݒʂ�����
		else if (strcmp(aString, "ROT") == 0)
		{
			// [=]�ǂݔ�΂�
			nScanData = fscanf(pFile, "%s", &Skip[0]);

			// �p�x��ǂݍ���
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotX);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotY);
			nScanData = fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nKeyBoss].aKey[nCntRot].fRotZ);
			nCntRot++;
		}
		// END_KEYSET��ǂݒʂ�����
		else if (strcmp(aString, "END_KEYSET") == 0)
		{
			nKeyBoss++;		 // �L�[�𑝂₷
			nCntPos = 0; // �ʒu�̃J�E���g�����Z�b�g
			nCntRot = 0; // �p�x�̃J�E���g�����Z�b�g
			break;
		}
	}
}
//===========================================================================================================
// ���̓����蔻��
//===========================================================================================================
void colisionSword(int nCntBoss)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	bool bHit = false;

	// ���̔��a
	float Radius = 50.0f;

	// ����̈ʒu
	D3DXVECTOR3 SwordTopPos(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

	// ���̎�����̈ʒu
	D3DXVECTOR3 SwordUnderPos(pPlayer->Motion.aModel[15].mtxWorld._41, pPlayer->Motion.aModel[15].mtxWorld._42, pPlayer->Motion.aModel[15].mtxWorld._43);

	for (int nCntModel = 0; nCntModel < g_Boss[nCntBoss].Motion.nNumModel; nCntModel++)
	{
		// �G�̈ʒu
		D3DXVECTOR3 BossModel(g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[nCntModel].mtxWorld._43);

		// �����̏I�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvEnd = SwordTopPos - BossModel;

		// �����̎n�_���狅�̂̒��S�_�܂ł̃x�N�g��
		D3DXVECTOR3 pvFirst = SwordUnderPos - BossModel;

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

	// ���[�V�����̃L�[
	int nKey = pPlayer->Motion.nKey;

	// ���[�V�����̃J�E���^�[
	int nCounter = pPlayer->Motion.nCountMotion;

	// �Ō�̃L�[
	int LastKey = pPlayer->Motion.aMotionInfo[pPlayer->Motion.motionType].nNumkey - 1;

	// �Ō�̃t���[��
	int EndFrame = pPlayer->Motion.aMotionInfo[pPlayer->Motion.motionType].aKeyInfo[LastKey].nFrame;

	// �v���C���[������������Ă��邩����
	const bool is_HaveWepon = pPlayer->Motion.nNumModel == MAX_MODEL;

	// �v���C���[���X�y�V������Ԃ���Ȃ�������
	const bool is_NotSpAttack = pPlayer->AttackSp == false;

	// �{�X�̏�Ԃ��_���[�W����Ȃ�������
	const bool is_NotDamage = g_Boss[nCntBoss].state != BOSSSTATE_DAMAGE;

	// �v���C���[���U����Ԃ�������
	const bool is_playerAction = isPlayerAttaking() == true;

	// �U���ł���
	const bool CanSwordDamage = is_HaveWepon == true && is_NotSpAttack == true && is_NotDamage == true && is_playerAction == true;

	// �_���[�W�J�b�g
	static int fCutDamage = 1;

	// �N���t�g���킾������
	if (strcmp(&pItem[pPlayer->ItemIdx].Itemtag[0], "CRAFT") == 0)
	{
		// �_���[�W�J�b�g�̊���
		fCutDamage = 1;
	}
	// ���ʂ̕��킾������
	else if (strcmp(&pItem[pPlayer->ItemIdx].Itemtag[0], "NORMAL") == 0)
	{
		// �_���[�W�J�b�g�̊���
		fCutDamage = 5;
	}

	// �X�y�V�����U������Ȃ�
	if (CanSwordDamage == true && bHit == true && CheckMotionBounds(nKey,nCounter,KEY_ONE,LastKey,0,EndFrame) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 5) / fCutDamage);

		pItem[pPlayer->ItemIdx].durability--;

		if (pItem[pPlayer->ItemIdx].durability <= 0)
		{
			pPlayer->Itembreak[pPlayer->ItemIdx] = true;
		}
	}

	// ���f���̈ʒu��ϐ��ɑ��
	D3DXVECTOR3 ModelPos(pPlayer->Motion.aModel[4].mtxWorld._41, pPlayer->Motion.aModel[4].mtxWorld._42, pPlayer->Motion.aModel[4].mtxWorld._43);

	// ����������Ă��Ȃ����𔻒�
	const bool is_NotWepon = pPlayer->Motion.nNumModel == MAX_MODEL - 1;

	// �~�͈͓̔��ɂ��邩�ǂ����𔻒�
	const bool is_sphereBounds = sphererange(&ModelPos, &g_Boss[nCntBoss].pos, 30.0f, 65.0f) == true;

	// �_���[�W��^�����邩�𔻒�
	const bool CanPanchDamage = is_NotWepon == true && is_sphereBounds == true && is_NotDamage == true && is_playerAction == true;

	// �f��ōU���ł���
	if (CanPanchDamage == true && CheckMotionBounds(nKey, nCounter, KEY_TWO, LastKey, 0, EndFrame) == true)
	{
		HitBoss(nCntBoss,(pPlayer->nDamage * 3) / fCutDamage); // �G�ɓ�������
	}

	// �͈͓�
	const bool InBounds = sphererange(&ModelPos, &g_Boss[nCntBoss].pos, 150.0f, 65.0f) == true;

	// �_���[�W��^������
	const bool is_CanSpDamage = pPlayer->Motion.nNumModel == MAX_MODEL && pPlayer->AttackSp == true && InBounds;

	// ���̃X�y�V�����U��
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SP && CheckMotionBounds(nKey, nCounter, KEY_THREE, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // �G�ɓ�������
	}
	// ���莝���̕K�E�Z
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPDOUBLE && CheckMotionBounds(nKey, nCounter, KEY_FOUR, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // �G�ɓ�������
	}
	// �n���}�[�̃X�y�V�����U��
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPHAMMER && CheckMotionBounds(nKey, nCounter, KEY_ONE, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // �G�ɓ�������
	}
	// �Ў�̕K�E�Z
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_ONEHANDBLOW && CheckMotionBounds(nKey, nCounter, KEY_FOUR, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // �G�ɓ�������
	}
	// ���̕K�E�Z
	if (is_CanSpDamage == true && pPlayer->WeponMotion == MOTION_SPPIERCING && CheckMotionBounds(nKey, nCounter, KEY_EIGHTEEN, LastKey, 0, EndFrame / 2) == true)
	{
		HitBoss(nCntBoss, (pPlayer->nDamage * 15) / fCutDamage); // �G�ɓ�������
	}

}
//===========================================================================================================
// �{�X�ƃ{�X�̓����蔻��
//==========================================================================================================
void CollisionToBoss(int nCntBoss)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (g_Boss[nCnt].bUse && nCntBoss != nCnt)
		{
			if (sphererange(&g_Boss[nCntBoss].pos, &g_Boss[nCnt].pos, 50.0f, 50.0f))
			{
				D3DXVECTOR3 vector = g_Boss[nCnt].pos - g_Boss[nCntBoss].pos;

				D3DXVec3Normalize(&vector, &vector);
				g_Boss[nCntBoss].move.x -= vector.x * g_Boss[nCntBoss].Speed;
				g_Boss[nCntBoss].move.z -= vector.z * g_Boss[nCntBoss].Speed;
			}
		}
	}
}
//========================================================================================================
// ��Ԉُ�̐ݒ�
//========================================================================================================
bool SetAbnormalCondition(int nType, int nTime, int nDamage,int nCntBoss)
{			
	bool is_SetParticle = false;

	if (g_Boss[nCntBoss].nStateCount[nType] > 0)
	{
		// ��Ԉُ�o���̏���
		g_Boss[nCntBoss].nStateCount[nType]--;

		is_SetParticle = true;
		if (g_Boss[nCntBoss].nStateCount[nType] % nTime == 0)
		{
			if (nDamage > 0)
			{
				HitBoss(nCntBoss, nDamage);
			}
		}
	}
	return is_SetParticle;
}
//========================================================================================================
// ��Ԉُ�̍X�V����
//========================================================================================================
void UpdateAbnormalCondition(int nCntBoss)
{
	if (SetAbnormalCondition(0, 60, g_Boss[nCntBoss].nLife / 20, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			2.0f, 2, 30, 6, 10.0f, 20.0f, true, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}

	// ��Ԉُ퉊�̏���
	if (SetAbnormalCondition(1, 60, g_Boss[nCntBoss].nLife / 20, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 30, 7, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
			2.0f, 2, 30, 7, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 2.0f, 0.0f));
	}

	// ��Ԉُ�X�̏���
	if (SetAbnormalCondition(2, 60, 0, nCntBoss) == true)
	{
		// �����Ŕ������炢�̃y�[�X�Ń��[�V�����J�E���g�����炵�A�������[�V�������x�����ɂ���
		if (g_Boss[nCntBoss].nStateCount[2] % 2 == 0)
		{
			g_Boss[nCntBoss].Motion.nCountMotion--;
		}
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f, 2, 60, 7, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.6f, 1.0f, 1.0f),
			1.0f, 2, 60, 7, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// ��Ԉُ헋�̏���
	if (SetAbnormalCondition(3, 60, 0, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			1.0f, 2, 120, 6, 1.0f, 25.0f, false, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	}

	// ��Ԉُ퐅�̏���
	if (SetAbnormalCondition(4, 60, 0, nCntBoss) == true)
	{
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + 50.0f, g_Boss[nCntBoss].pos.z),
			D3DXVECTOR3(g_Boss[nCntBoss].rot.x, g_Boss[nCntBoss].rot.y - D3DX_PI, g_Boss[nCntBoss].rot.z),
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 2, 30, 6, 10.0f, 20.0f, true, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}
}
//========================================================================================================
// �{�X�̓ːi�U���̐ݒ�
//========================================================================================================
void SetRasuAttack(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	if (g_Boss[nCntBoss].Motion.nKey == KEY_FIRST || g_Boss[nCntBoss].Motion.nKey == KEY_ONE)// ���߂铮�쒆��
	{
		// �������v���C���[�Ɍ�����
		float fAngle = atan2f(pPlayer->posOld.x - g_Boss[nCntBoss].pos.x, pPlayer->posOld.z - g_Boss[nCntBoss].pos.z);

		// �{�X�̌������
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_TWO)// ���܂�؂������[�V�����ɂȂ���
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// ���̍ŏ��ɃG�t�F�N�g���o��
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				D3DXVECTOR3(1.57f, g_Boss[nCntBoss].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 20, 30, 4.0f, 50.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_THREE && g_Boss[nCntBoss].Motion.bFirstMotion == false)// �ːi���[�V��������			
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// �ŏ��͉���������
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 40.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 40.0f;
		}
		else // ���̌�̓G�t�F�N�g��Z���Ȃ���ړ�����
		{
			g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				g_Boss[nCntBoss].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 4, 60, 30, 6.0f, 60.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}
//========================================================================================================
// �{�X�̓��ːi���Ă���U������
//========================================================================================================
void SetDoubleRasuAttack(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	if (g_Boss[nCntBoss].Motion.nKey == KEY_FIRST || g_Boss[nCntBoss].Motion.nKey == KEY_FOUR)// ���߂铮�쒆��
	{
		// �������v���C���[�Ɍ�����
		float fAngle = atan2f(pPlayer->posOld.x - g_Boss[nCntBoss].pos.x, pPlayer->posOld.z - g_Boss[nCntBoss].pos.z);

		// �{�X�̌������
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_ONE)// ���܂�؂������[�V�����ɂȂ���
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// ���̍ŏ��ɃG�t�F�N�g���o��
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
				D3DXVECTOR3(1.57f, g_Boss[nCntBoss].rot.y, 1.57f),
				D3DXVECTOR3(0.2f, 3.14f, 0.2f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 3, 20, 55, 4.0f, 50.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_SIX)// �n�C�L�b�N���[�V��������			
	{
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1)// �ŏ��͉���������
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 30.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 30.0f;
		}
		else
		{
			SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 2.0f, g_Boss[nCntBoss].pos.z),
				g_Boss[nCntBoss].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				2.0f, 4, 60, 40, 6.0f, 60.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey == KEY_TWO || g_Boss[nCntBoss].Motion.nKey == KEY_THREE)//�A�b�p�[���ɂ̓G�t�F�N�g���o��
	{
		D3DXVECTOR3 BlowPos(
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._41, // X����
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._42, // Y����
			g_Boss[nCntBoss].Motion.aModel[5].mtxWorld._43  // Z����
		);

		g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
		SetParticle(D3DXVECTOR3(BlowPos),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 4, 20, 12, 2.0f, 5.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (g_Boss[nCntBoss].Motion.nCountMotion == 1 && g_Boss[nCntBoss].Motion.nKey == KEY_TWO)
		{
			g_Boss[nCntBoss].move.x = sinf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 20.0f;
			g_Boss[nCntBoss].move.z = cosf(g_Boss[nCntBoss].rot.y + D3DX_PI) * 20.0f;
		}
	}
	else if (g_Boss[nCntBoss].Motion.nKey >= KEY_SIX)//�R�蒆�ɂ��G�t�F�N�g���o��
	{
		D3DXVECTOR3 BlowPos(
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._41, // X����
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._42, // Y����
			g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._43  // Z����
		);

		g_Boss[nCntBoss].pos += g_Boss[nCntBoss].move;
		SetParticle(D3DXVECTOR3(BlowPos),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			2.0f, 4, 20, 12, 2.0f, 5.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}
//========================================================================================================
// �{�X�̒ǐՂ̍X�V����
//========================================================================================================
void UpdateAgentBoss(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	// �͈͂ɓ�������(�ǂ��ɂ��Ă��ǂ������Ă��邪�ꉞ�~�Ŏ��)
	const bool is_sphereBounds = sphererange(&pPlayer->pos, &g_Boss[nCntBoss].pos, 50.0f, 700.0f) == true;

	// �U�����[�V����0���ǂ����𔻒�
	const bool is_NotAction0 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION;

	// �U�����[�V����1���ǂ����𔻒�
	const bool is_NotAction1 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION2;

	// �U�����[�V����2���ǂ����𔻒�
	const bool is_NotAction2 = g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_ACTION3;

	// �ǐՂł��邩�ǂ����𔻒�
	const bool CanAgent = is_sphereBounds == true && is_NotAction0 == true && is_NotAction1 == true && is_NotAction2 == true;

	if (CanAgent == true)
	{
		// ���f��������
		D3DXVECTOR3 HootR(g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[11].mtxWorld._43);
		D3DXVECTOR3 HootL(g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._41, g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._42, g_Boss[nCntBoss].Motion.aModel[14].mtxWorld._43);

		// ���[�V���������[�u�̎�1�L�[��1�t���[����
		if (g_Boss[nCntBoss].Motion.motiontypeBlend == MOTIONTYPE_MOVE &&
			g_Boss[nCntBoss].Motion.nKey == 1 &&
			g_Boss[nCntBoss].Motion.nCountMotion == 1)
		{
			SetExplosion(HootR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
		}

		// ���[�V���������[�u�̎�3�L�[��1�t���[����
		else if (g_Boss[nCntBoss].Motion.motiontypeBlend == MOTIONTYPE_MOVE &&
			g_Boss[nCntBoss].Motion.nKey == 3 &&
			g_Boss[nCntBoss].Motion.nCountMotion == 1)
		{
			SetExplosion(HootL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
		}

		if (g_Boss[nCntBoss].Motion.motiontypeBlend != MOTIONTYPE_MOVE)
		{
			// ���[�V���������[�u�ɂ���
			SetMotion(&g_Boss[nCntBoss].Motion, MOTIONTYPE_MOVE, true, 10);
		}

		// �{�X�̌������v���C���[�̈ʒu�������悤�ɂ���
		float fAngle = atan2f(pPlayer->pos.x - g_Boss[nCntBoss].pos.x, pPlayer->pos.z - g_Boss[nCntBoss].pos.z);

		// �{�X�̌������
		g_Boss[nCntBoss].rot.y = fAngle + D3DX_PI;

		// �v���C���[�̈ʒu���Z�o
		D3DXVECTOR3 Dest = pPlayer->pos - g_Boss[nCntBoss].pos;

		// ���K��
		D3DXVec3Normalize(&Dest, &Dest);

		// �ړ��ʂɑ��
		if (g_Boss[nCntBoss].nStateCount[2] > 0)
		{
			g_Boss[nCntBoss].move.x = Dest.x * g_Boss[nCntBoss].Speed / 2.0f;
			g_Boss[nCntBoss].move.z = Dest.z * g_Boss[nCntBoss].Speed / 2.0f;
		}
		else
		{
			g_Boss[nCntBoss].move.x = Dest.x * g_Boss[nCntBoss].Speed;
			g_Boss[nCntBoss].move.z = Dest.z * g_Boss[nCntBoss].Speed;
		}
	}
	//else
	//{
	//	if (g_Boss[nCntBoss].Motion.motionType != MOTIONTYPE_ACTION)
	//	{
	//		g_Boss[nCntBoss].Motion.motionType = MOTIONTYPE_NEUTRAL; // �U�����ĂȂ�
	//	}
	//}


}
//========================================================================================================
 // �{�X�̎��S���[�V�����̏���
 //========================================================================================================
void DeathMotionContlloer(int nCntBoss)
{
	Player* pPlayer = GetPlayer();

	// �L�[���Ō�܂ōs�������𔻒�
	const bool LastKey = g_Boss[nCntBoss].Motion.nKey >= g_Boss[nCntBoss].Motion.aMotionInfo[MOTIONTYPE_DEATH].nNumkey - 1;

	// ���S���[�V������������
	if (g_Boss[nCntBoss].Motion.nKey <= 0)
	{
		g_Boss[nCntBoss].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 20.0f;
		g_Boss[nCntBoss].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 20.0f;
	}

	// �Ō�ɃL�[�ɂȂ�����
	if (LastKey == true)
	{
		// ����
		g_Boss[nCntBoss].bUse = false;

		// �}�b�v�������
		EnableMap(g_Boss[nCntBoss].nIdxMap);

		// �e����
		KillShadow(g_Boss[nCntBoss].nIdxShadow);

		AddTimeSecond(20); // 15�b���₷

		// �e���g���[������
		SetEnableBossTerritory(false);

		SpawnItem(g_Boss[nCntBoss].pos);
	}
}
//========================================================================================================
// �C�x���g���I�������Ƀ{�X����������
//========================================================================================================
void EndEventBossState(int nCntBoss)
{
}
//========================================================================================================
// �{�X�ɓ����������̏�Ԉُ�
//========================================================================================================
void HitBossAbnormalCondition(int nCntBoss)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	// �����̃p�����[�^�ݒ�
	switch (pItem[pPlayer->ItemIdx].nType)
	{
	case ITEMTYPE_STONEBAT:
		//�΃o�b�g�Ȃ�o��������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_GOLFHUNMER:
		//�S���t�n���}�[�Ȃ�o��������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_IRONBAT:
		//�����o�b�g�Ȃ�o��������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 0, 25, 100, 300);
		break;
	case ITEMTYPE_TORCHSWORD:
		//�҉Ό��Ȃ牊������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 1, 25, 100, 300);
		break;
	case ITEMTYPE_ICEBLOCKSOWRD:
		// �������Ȃ�X������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 2, 25, 100, 300);
		break;
	case ITEMTYPE_LIGHTWOOD:
		// �������Ȃ痋������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 3, 25, 100, 300);
		break;
	case ITEMTYPE_SURFBOARDFISH:
		// �L�����ւȂ琅������ʂ�^����
		HitBossAbnormalConditionParam(nCntBoss, 4, 25, 100, 300);
		break;
	case ITEMTYPE_BONESPEAR:
		//�����Ȃ�m���ő������ʂ�^����
		if (rand() % 40 == 0)
		{
			HitBoss(nCntBoss, 99999);
		}
		break;
	case ITEMTYPE_HEXMANDOLIN:
		//�_�[�N�n�[�v�Ȃ�m���ő������ʂ�^����
		if (rand() % 32 == 0)
		{
			HitBoss(nCntBoss, 99999);
		}
		break;
	default:
		break;
	}
}
//========================================================================================================
// �{�X�ɓ����������̏�Ԉُ�̃p�����[�^
//========================================================================================================
void HitBossAbnormalConditionParam(int nCntBoss,int nElement,int ChargeValue,int MaxCharge,int stateCnt)
{
	// ��ԃJ�E���g��0�ȉ���������
	if (g_Boss[nCntBoss].nStateCount[nElement] <= 0)
	{
		// ��Ԉُ�l�����Z
		g_Boss[nCntBoss].nStateCharge[nElement] += ChargeValue;

		// �~�ϒl���ő�ɂȂ�����
		if (g_Boss[nCntBoss].nStateCharge[nElement] >= MaxCharge)
		{
			// ��Ԉُ�̎��Ԃ�ݒ�
			g_Boss[nCntBoss].nStateCount[nElement] = stateCnt;

			// �~�ϒl��0�ɖ߂�
			g_Boss[nCntBoss].nStateCharge[nElement] = 0;
		}
	}
}
//=======================================================================================================
// �A�C�e���̃X�|�[��
//=======================================================================================================
void SpawnItem(D3DXVECTOR3 pos)
{
	// �A�C�e����ݒ�
	//SetItem(pos, rand() % ITEMTYPE_MAX);
}
//========================================================================================================
// �{�X�̎擾����
//========================================================================================================
Boss* Getboss(void)
{
	return &g_Boss[0];
}
