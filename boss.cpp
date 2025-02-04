//============================
//
// �{�X [boss.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
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

//****************************
// �}�N����`
//****************************
#define MAX_WORD (256)			 // �ő�̕�����
#define SHADOWSIZEOFFSET (40.0f) // �e�̑傫���̃I�t�Z�b�g
#define SHADOW_A (1.0f)          // �e�̔Z���̊
#define NUM_MTX (8)

//****************************
// �v���g�^�C�v�錾
//****************************
void LoadBoss(void);	    // �{�X��ǂݍ���
void colisionSword(void);   // 

//****************************
// �O���[�o���ϐ��錾
//****************************
Boss g_Boss; // �{�X�̏��

//=============================
// �{�X�̏���������
//=============================
void InitBoss(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
	g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�x
	g_Boss.bUse = false;						 // ���g�p���
	g_Boss.nLife = 20;							 // �̗�
	g_Boss.state = BOSSSTATE_NORMAL;			 // ���
	g_Boss.Speed = 5.0f;						 // ���̑���
	g_Boss.AttackState = BOSSATTACK_NO;			 // �U�����

	LoadBoss(); // �{�X�̃��[�h

	D3DXMATERIAL* pMat; // �}�e���A���ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}

	// ���_���W�̎擾
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff; //���_�o�b�t�@�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		// ���_���̎擾
		nNumVtx = g_Boss.Motion.aModel[nCntModel].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_Boss.Motion.aModel[nCntModel].pMesh->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		g_Boss.Motion.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W���r���ă{�X�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_Boss.Motion.aModel[nCntModel].vtxMin.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_Boss.Motion.aModel[nCntModel].vtxMin.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_Boss.Motion.aModel[nCntModel].vtxMin.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_Boss.Motion.aModel[nCntModel].vtxMax.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_Boss.Motion.aModel[nCntModel].vtxMax.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_Boss.Motion.aModel[nCntModel].vtxMax.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			// �T�C�Y�ɑ��
			g_Boss.Motion.aModel[nCntModel].Size.x = g_Boss.Motion.aModel[nCntModel].vtxMax.x - g_Boss.Motion.aModel[nCntModel].vtxMin.x;
			g_Boss.Motion.aModel[nCntModel].Size.y = g_Boss.Motion.aModel[nCntModel].vtxMax.y - g_Boss.Motion.aModel[nCntModel].vtxMin.y;
			g_Boss.Motion.aModel[nCntModel].Size.z = g_Boss.Motion.aModel[nCntModel].vtxMax.z - g_Boss.Motion.aModel[nCntModel].vtxMin.z;
		}

		// ���_�o�b�t�@�̃A�����b�N
		g_Boss.Motion.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}
//=============================
// �{�X�̏I������
//=============================
void UninitBoss(void)
{
	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_Boss.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pMesh->Release();
			g_Boss.Motion.aModel[nCntModel].pMesh = NULL;
		}

		// �}�e���A���̔j��
		if (g_Boss.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pBuffMat->Release();
			g_Boss.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//=============================
// �{�X�̍X�V����
//=============================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();

	// �g�p��Ԃ̂�
	if (g_Boss.bUse)
	{
		switch (g_Boss.state)
		{
		case BOSSSTATE_NORMAL:
			break;
		case BOSSSTATE_AGENT:
			break;
		case BOSSSTATE_ATTACK:
			break;
		case BOSSSTATE_DAMAGE:
			g_Boss.nCounterState--;

			if (g_Boss.nCounterState <= 0)
			{
				g_Boss.state = BOSSSTATE_NORMAL; // �G�̏�Ԃ��m�[�}���ɂ���
			}
			break;
		default:
			break;
		}
		// �ړ��ʂ̌���
		g_Boss.move.x += (0.0f - g_Boss.move.x) * 0.25f;
		g_Boss.move.z += (0.0f - g_Boss.move.z) * 0.25f;

		// �O��̈ʒu����
		g_Boss.posOld = g_Boss.pos;

		// �ʒu�̍X�V
		g_Boss.pos += g_Boss.move;

		// �u���b�N�̔���
		CollisionBlock(&g_Boss.pos, &g_Boss.posOld, &g_Boss.move, &g_Boss.Size);

		// �e�̈ʒu�̍X�V
		SetPositionShadow(g_Boss.nIdxShadow, g_Boss.pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Boss.pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Boss.pos.y / 30.0f));

		// �͈͂ɓ�������(�ǂ��ɂ��Ă��ǂ������Ă��邪�ꉞ�~�Ŏ��)
		if (sphererange(&pPlayer->pos,&g_Boss.pos,50.0f,20000.0f) && g_Boss.Motion.motionType != MOTIONTYPE_ACTION)
		{
			// ���f��������
			D3DXVECTOR3 HootR(g_Boss.Motion.aModel[11].mtxWorld._41, g_Boss.Motion.aModel[11].mtxWorld._42, g_Boss.Motion.aModel[11].mtxWorld._43);
			D3DXVECTOR3 HootL(g_Boss.Motion.aModel[14].mtxWorld._41, g_Boss.Motion.aModel[14].mtxWorld._42, g_Boss.Motion.aModel[14].mtxWorld._43);

			// ���[�V���������[�u�̎�1�L�[��1�t���[����
			if (g_Boss.Motion.motionType == MOTIONTYPE_MOVE &&
				g_Boss.Motion.nKey == 1 &&
				g_Boss.Motion.nCountMotion == 1)
			{
				SetExplosion(HootR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
			}
			// ���[�V���������[�u�̎�3�L�[��1�t���[����
			else if (g_Boss.Motion.motionType == MOTIONTYPE_MOVE &&
				g_Boss.Motion.nKey == 3 &&
				g_Boss.Motion.nCountMotion == 1)
			{
				SetExplosion(HootL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
			}

			g_Boss.Motion.motionType = MOTIONTYPE_MOVE; // ���[�V�����̎�ނ��ړ��ɂ���

			// �{�X�̌������v���C���[�̈ʒu�������悤�ɂ���
			float fAngle = atan2f(pPlayer->pos.x - g_Boss.pos.x, pPlayer->pos.z - g_Boss.pos.z);

			// �{�X�̌������
			g_Boss.rot.y = fAngle + D3DX_PI;

			// �v���C���[�̈ʒu���Z�o
			D3DXVECTOR3 Dest = pPlayer->pos - g_Boss.pos;

			// ���K��
			D3DXVec3Normalize(&Dest, &Dest);

			// �ړ��ʂɑ��
			g_Boss.move.x = Dest.x * g_Boss.Speed;
			g_Boss.move.z = Dest.z * g_Boss.Speed;

		}
		else
		{
			if (g_Boss.Motion.motionType != MOTIONTYPE_ACTION)
			{
				g_Boss.Motion.motionType = MOTIONTYPE_NEUTRAL; // �U�����ĂȂ�
			}
		}

		// �U���͈͂ɓ�������
		if (sphererange(&pPlayer->pos, &g_Boss.pos, 50.0f, 30.0f)&& g_Boss.AttackState != BOSSATTACK_ATTACK)
		{
			// ���[�V�������U���ɂ���
			SetMotion(&g_Boss.Motion, // ���[�V�����\���̂̃A�h���X
				MOTIONTYPE_ACTION,    // ���[�V�����^�C�v
				MOTIONTYPE_NEUTRAL,   // �u�����h���[�V�����^�C�v
				true,                 // �u�����h���邩���Ȃ���
				10);				  // �u�����h�̃t���[��

			g_Boss.AttackState = BOSSATTACK_ATTACK; // �U�����Ă���
		}

		// �U���͈͂ɓ�����
		if (sphererange(&pPlayer->pos, &g_Boss.pos, 50.0f, 20.0f) &&
			pPlayer->state != PLAYERSTATE_DAMAGE &&
			g_Boss.Motion.nKey >= 4 && !pPlayer->AttackSp)
		{
			HitPlayer(50);
		}
		
		colisionSword();
		
		// ���[�v���Ȃ����[�V�������Ō�܂ōs������
		if (!g_Boss.Motion.aMotionInfo[g_Boss.Motion.motionType].bLoop && g_Boss.Motion.nKey >= g_Boss.Motion.aMotionInfo[g_Boss.Motion.motionType].nNumkey - 1)
		{
			g_Boss.AttackState = BOSSATTACK_NO;					// �{�X�̍U����Ԃ��U�����ĂȂ���Ԃɂ���
		}

		
		// ���[�V�����̍X�V����
		UpdateMotion(&g_Boss.Motion);

		if (g_Boss.Motion.motionType == MOTIONTYPE_ACTION)
		{
			if (g_Boss.Motion.nKey == 0 || g_Boss.Motion.nKey == 1)
			{
				float fAngle = atan2f(pPlayer->pos.x - g_Boss.pos.x, pPlayer->pos.z - g_Boss.pos.z);

				// �{�X�̌������
				g_Boss.rot.y = fAngle + D3DX_PI;
			}
			else if (g_Boss.Motion.nKey == 2)
			{
				if (g_Boss.Motion.nCountMotion == 1)
				{
					SetParticle(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + g_Boss.Size.y / 1.5f, g_Boss.pos.z),
						D3DXVECTOR3(1.57f, g_Boss.rot.y, 1.57f),
						D3DXVECTOR3(0.2f, 3.14f, 0.2f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
						2.0f, 3, 20, 150, 4.0f, 50.0f,
						false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
			}
			else if (g_Boss.Motion.nKey == 3)
			{
				if (g_Boss.Motion.nCountMotion == 1)
				{
					g_Boss.move.x = sinf(g_Boss.rot.y + D3DX_PI) * 70;
					g_Boss.move.z = cosf(g_Boss.rot.y + D3DX_PI) * 70;
				}
				else
				{
					g_Boss.pos += g_Boss.move;
					SetParticle(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + g_Boss.Size.y / 1.5f, g_Boss.pos.z),
						g_Boss.rot,
						D3DXVECTOR3(1.0f, 1.0f, 1.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
						D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 
						2.0f, 4, 60, 40, 6.0f, 60.0f, 
						false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
			}
		}
	}
}
//=============================
// �{�X�̕`�揈��
//=============================
void DrawBoss(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef; // ���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;	 // �}�e���A���f�[�^�ւ̃|�C���^

	if (g_Boss.bUse == true)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Boss.mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss.rot.y, g_Boss.rot.x, g_Boss.rot.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // �v�Z�p
			D3DXMATRIX mtxParent;				  // �e�̃}�g���b�N�X

			// �p�[�c�̃}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			// �p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss.Motion.aModel[nCntModel].rot.y, g_Boss.Motion.aModel[nCntModel].rot.x, g_Boss.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// �p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_Boss.Motion.aModel[nCntModel].pos.x, g_Boss.Motion.aModel[nCntModel].pos.y, g_Boss.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// �p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_Boss.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// �e���f��������ꍇ
				mtxParent = g_Boss.Motion.aModel[g_Boss.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// �e���f�����Ȃ��ꍇ
				mtxParent = g_Boss.mtxWorld;
			}

			// �Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent); // ����,����,�e

			// �p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// �J���[�ύX�p�̕ϐ�
				D3DXMATERIAL color;

				if (g_Boss.state != BOSSSTATE_DAMAGE)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss.state == BOSSSTATE_DAMAGE)
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
				pDevice->SetTexture(0, g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);

				// ���f��(�p�[�c)�̕`��
				g_Boss.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&matDef);
		}
	}
}
//=============================
// �{�X�̐ݒ菈��
//=============================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	if (!g_Boss.bUse)
	{// ���g�p�Ȃ�
		g_Boss.pos = pos;	  // �ʒu����
		//g_Boss.Speed = speed; // ���̑���
		g_Boss.nLife = nLife; // �̗͂�}��
		g_Boss.bUse = true;   // �g�p��Ԃɂ���

		g_Boss.nIdxShadow = SetShadow(g_Boss.pos,g_Boss.rot,40.0f);
	}
}
//=============================
// �{�X�̃q�b�g����
//=============================
void HitBoss(int nDamage)
{
	Player* pPlayer = GetPlayer();

	g_Boss.nLife -= nDamage;

	// �_���[�W��ݒ�
	SetDamege(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + g_Boss.Size.y / 1.5f, g_Boss.pos.z), // �ʒu
		nDamage,	// �_���[�W																								
		20,			// ����
		false);

	if (g_Boss.nLife <= 0)
	{
		// ���񂾂�p�[�e�B�N�����o��
		SetParticle(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + g_Boss.Size.y / 1.5f, g_Boss.pos.z),
			g_Boss.rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			4.0f, 8, 15, 20, 5.0f, 0.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// ����
		g_Boss.bUse = false;

		// �e�������
		KillShadow(g_Boss.nIdxShadow);

		if (pPlayer->FeverMode)
		{
			AddScore(30000);		// �X�R�A���擾
			AddSpgauge(2.5f);   // SP�Q�[�W���擾
		}
		else if (!pPlayer->FeverMode)
		{
			AddFever(10.0f);		// �t�B�[�o�[�|�C���g���擾
			AddScore(15000);		// �X�R�A���擾
			AddSpgauge(2.0f);   // SP�Q�[�W���擾
		}
		AddTimeSecond(15); // 15�b���₷
	}
	else
	{
		// �p�[�e�B�N�����Z�b�g
		SetParticle(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + g_Boss.Size.y / 1.5f, g_Boss.pos.z), g_Boss.rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 4.0f, 1, 20, 30, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Boss.state = ENEMYSTATE_DAMAGE;
		g_Boss.nCounterState = 20;
		AddSpgauge(1.0f);   // SP�Q�[�W���擾
	}
}
//=============================
// �{�X�̓ǂݍ��ݏ���
//=============================
void LoadBoss(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �t�@�C���|�C���^��錾
	FILE* pFile; 

	// �t�@�C�����J��
	pFile = fopen("data\\MOTION\\boss.txt", "r");

	// ���[�J���ϐ��錾-----------------------------
	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;
	//----------------------------------------------

	if (pFile != NULL)
	{//�@NULL ����Ȃ�
		// ������
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
							fscanf(pFile, "%d", &g_Boss.Motion.nNumModel);
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
								&g_Boss.Motion.aModel[nCnt].pBuffMat,
								NULL,
								&g_Boss.Motion.aModel[nCnt].dwNumMat,
								&g_Boss.Motion.aModel[nCnt].pMesh);

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
											fscanf(pFile, "%d", &g_Boss.Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���W����
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// �p�x����
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.z);
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
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEY��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey)
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
													fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
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
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// �C���N�������g
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROT��ǂݎ������
														// ������ǂݔ�΂�
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
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
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Boss.cpp)", MB_OK);
		return;
	}
	// �t�@�C�������
	fclose(pFile);
}
//=========================
// ���̓����蔻��
//=========================
void colisionSword(void)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	D3DXVECTOR3 mtxDis, SwordPos;

	if (pPlayer->Motion.nNumModel == 16 && !pPlayer->AttackSp)
	{
		//���̒��������߂�
		mtxDis.x = (pPlayer->SwordMtx._41 - pPlayer->Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (pPlayer->SwordMtx._42 - pPlayer->Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (pPlayer->SwordMtx._43 - pPlayer->Motion.aModel[15].mtxWorld._43);

		// �}�g���N�X�̐���������
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// ���̈ʒu��S�ċ��߂�
			SwordPos.x = pPlayer->Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = pPlayer->Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = pPlayer->Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // �����Z�o�p

			DisPos.x =g_Boss.pos.x - SwordPos.x; // ����X�����߂�
			DisPos.y =g_Boss.pos.y - SwordPos.y; // ����Y�����߂�
			DisPos.z =g_Boss.pos.z - SwordPos.z; // ����Z�����߂�

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // ���������߂�

			float Radius1, Radius2; // ���a

			Radius1 = 15.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // ���a�����߂�

			fRadius = (fRadius * fRadius); // ���a�����߂�

			if (fDistance <= fRadius && g_Boss.state != BOSSSTATE_DAMAGE && pPlayer->Combostate != COMBO_NO)
			{
				HitBoss(pPlayer->nDamage * 5);

				pItem[pPlayer->ItemIdx].durability--;

				if (pItem[pPlayer->ItemIdx].durability <= 0)
				{
					pPlayer->Itembreak[pPlayer->ItemIdx] = true;
				}
				break;
			}
		}
	}
	else if (pPlayer->Motion.nNumModel == 15 && !pPlayer->AttackSp)
	{
		// ���f���̈ʒu��ϐ��ɑ��
		D3DXVECTOR3 ModelPos(pPlayer->Motion.aModel[4].mtxWorld._41, pPlayer->Motion.aModel[4].mtxWorld._42, pPlayer->Motion.aModel[4].mtxWorld._43);

		// �~�͈̔�
		if (sphererange(&ModelPos, &g_Boss.pos, 30.0f, 65.0f) && pPlayer->Combostate != COMBO_NO && g_Boss.state != ENEMYSTATE_DAMAGE)
		{
			if (pPlayer->Motion.motionType == MOTIONTYPE_ACTION && pPlayer->Motion.nKey >= 2)
			{
				HitBoss(pPlayer->nDamage * 3); // �G�ɓ�������
			}
		}
	}
	else if (pPlayer->Motion.nNumModel == 16 && pPlayer->AttackSp)
	{
		//���̒��������߂�
		mtxDis.x = (pPlayer->SwordMtx._41 - pPlayer->Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (pPlayer->SwordMtx._42 - pPlayer->Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (pPlayer->SwordMtx._43 - pPlayer->Motion.aModel[15].mtxWorld._43);

		// �}�g���N�X�̐���������
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// ���̈ʒu��S�ċ��߂�
			SwordPos.x = pPlayer->Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = pPlayer->Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = pPlayer->Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // �����Z�o�p

			DisPos.x = g_Boss.pos.x - SwordPos.x; // ����X�����߂�
			DisPos.y = g_Boss.pos.y - SwordPos.y; // ����Y�����߂�
			DisPos.z = g_Boss.pos.z - SwordPos.z; // ����Z�����߂�

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // ���������߂�

			float Radius1, Radius2; // ���a

			Radius1 = 200.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // ���a�����߂�

			fRadius = (fRadius * fRadius); // ���a�����߂�

			if (fDistance <= fRadius && g_Boss.state != ENEMYSTATE_DAMAGE && pPlayer->Combostate != COMBO_NO && pPlayer->Motion.nKey >= 3)
			{
				HitBoss(pPlayer->nDamage * 50);
				break;
			}
		}
	}

}
//======================
// �{�X�̎擾����
//======================
Boss* Getboss(void)
{
	return &g_Boss;
}