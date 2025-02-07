//============================
//
// �u���b�N[block.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include "block.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "Effect.h"

//****************************
// �}�N����`
//****************************
#define MAX_WORD (256)	  // �ő�̕�����
#define HALF_VALUE (0.6f) // ���鐔
#define PLAYERJUMPHEIGHT (50.0f) // �v���C���[�̃W�����v��

//****************************
// �v���g�^�C�v�錾
//****************************
void LoadBlockModel(void); // ���f���ǂݍ��ݏ���
void SetMtx(int nCntBlock); // ���[���h�}�g���b�N�X�̐ݒ�(���Spos)
bool PushPlayer(int nCntBlock); // OBB�̉����o��

//****************************
// �O���[�o���ϐ��錾
//****************************
BLOCK g_Block[MAX_BLOCK];		// �\���̕ϐ�
BLOCK g_TexBlock[BLOCKTYPE_MAX];// �u���b�N�̃e�N�X�`�����
int nCounterStateBlock;			// ��ԊǗ��J�E���^�[
int g_NumBlock;					// �u���b�N�̔z�u������
int g_BlockTypeMax;				// ��ސ�
//bool bLanding;

//=============================
// �u���b�N�̏���������
//=============================
void InitBlock(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�x
		g_Block[nCntBlock].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f); // �g�嗦
		g_Block[nCntBlock].nType = BLOCKTYPE_APARTMENT;			 // ���
		g_Block[nCntBlock].bUse = false;						 // ���g�p���
		g_Block[nCntBlock].nLife = 20;							 // �̗�
		g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;			 // ���
		g_Block[nCntBlock].fRadius = 100.0f;					 // ���a
	}

	LoadBlockModel(); // �u���b�N�̃��[�h
	//bLanding = false;
	//�O���[�o���ϐ��̏�����
	g_NumBlock = 0;		   // �u���b�N�̐�
	nCounterStateBlock = 0;// ��ԃJ�E���^�[

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		D3DXMATERIAL* pMat; // �}�e���A���ւ̃|�C���^

		// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	// ���_���W�̎擾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;  // ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff; // ���_�o�b�t�@�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		// ���_���̎擾
		nNumVtx = g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetFVF());
		
		// ���_�o�b�t�@�̃��b�N
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// ���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		// �T�C�Y�ɑ��
		g_TexBlock[nCntNum].Size.x = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x;
		g_TexBlock[nCntNum].Size.y = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y;
		g_TexBlock[nCntNum].Size.z = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z;

		// ���_�o�b�t�@�̃A�����b�N
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->UnlockVertexBuffer();
	}
}
//=============================
// �u���b�N�̏I������
//=============================
void UninitBlock(void)
{
	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntTex++)
		{
			if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex]->Release();
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock = NULL;
		}

		// �}�e���A���̔j��
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
		{
			// �e�N�X�`���̔j��
			for (int nCntTex = 0; nCntTex < (int)g_Block[nCnt].BlockTex[nCntNum].g_dwNumMatBlock; nCntTex++)
			{
				if (g_Block[nCnt].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
				{
					g_Block[nCnt].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
				}
			}

			// ���b�V���̔j��
			if (g_Block[nCnt].BlockTex[nCntNum].g_pMeshBlock != NULL)
			{
				g_Block[nCnt].BlockTex[nCntNum].g_pMeshBlock = NULL;
			}

			// �}�e���A���̔j��
			if (g_Block[nCnt].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
			{
				g_Block[nCnt].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
			}
		}
	}

}
//=============================
// �u���b�N�̍X�V����
//=============================
void UpdateBlock(void)
{
	Player* pPlayer = GetPlayer(); // �v���C���[�̎擾

	// �S���u���b�N����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// �u���b�N���g���Ă��Ȃ�
		if (!g_Block[nCntBlock].bUse)
		{
			// ������ǂݔ�΂�
			continue;
		}

		CreateObb(nCntBlock);

		// OBB�̔���(������)
		if (collisionObb(nCntBlock))
		{
			if (PushPlayer(nCntBlock))
			{
				// �W�����v��true
				//pPlayer->bJump = true;
				if (pPlayer->Motion.motionType == MOTIONTYPE_JUMP)
				{
					SetMotion(&pPlayer->Motion, MOTIONTYPE_LANDING, MOTIONTYPE_NEUTRAL, true, 10);
				}
			}
			else
			{
				//pPlayer->bLandingOBB = false;
			}
		}

		if (GetKeyboardPress(DIK_L))
		{
			g_Block[nCntBlock].rot.y += 0.01f;
		}
		if (g_Block[nCntBlock].rot.y < -D3DX_PI)
		{
			g_Block[nCntBlock].rot.y += D3DX_PI * 2.0f;
		}
		else if (g_Block[nCntBlock].rot.y > D3DX_PI)
		{
			g_Block[nCntBlock].rot.y += -D3DX_PI * 2.0f;
		}
	}
}
//=============================
// �u���b�N�̕`�揈��
//=============================
void DrawBlock(void)
{
	// �f�o�C�X�̃|�C���^���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���݂̃��[�h���擾
	MODE mode = GetMode();

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans, mtxScal,mtxParent;

	D3DMATERIAL9 matDef; // ���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;  // �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{//���g�p��������
			//�ǂݔ�΂��ăJ�E���g��i�߂�
			continue;
		}

		int nType = g_Block[nCntBlock].nType;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorldBlock);

		// �傫���𔽉f
		D3DXMatrixScaling(&mtxScal, g_Block[nCntBlock].Scal.y, g_Block[nCntBlock].Scal.x, g_Block[nCntBlock].Scal.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxScal);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorldBlock);
		
		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_Block[nCntBlock].BlockTex[nType].g_dwNumMatBlock; nCntMat++)
		{
			// �}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Block[nCntBlock].BlockTex[nType].g_pBuffMatBlock->GetBufferPointer();

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Block[nCntBlock].BlockTex[nType].g_apTextureBlock[nCntMat]);

			// �u���b�N(�p�[�c)�̕`��
			g_Block[nCntBlock].BlockTex[nType].g_pMeshBlock->DrawSubset(nCntMat);
		}
		SetMtx(nCntBlock);
	}
	
}
//=======================
// �u���b�N�̐ݒ菈��
//=======================
void SetBlock(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 Scal)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{//���g�p��Ԃ�������
			g_Block[nCntBlock] = g_TexBlock[nType]; // �e�N�X�`���^�C�v

			g_Block[nCntBlock].pos = pos;	  // ���W
			g_Block[nCntBlock].Scal = Scal;	  // �g�嗦
			g_Block[nCntBlock].nType = nType; // ���
			g_Block[nCntBlock].bUse = true;   // �g�p���

			g_NumBlock++; // �C���N�������g
			break;
		}
	}
}
//=======================
// �u���b�N�̔��菈��
//=======================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// ���n����
	bool bLanding = false;

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse || g_Block[nCntBlock].nType == BLOCKTYPE_MANHOLE)
		{//���g�p��������
			//�ǂݔ�΂��ăJ�E���g��i�߂�
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			// ���E�̂߂荞�ݔ���
			if (pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// x��������E�ɂ߂荞��	
				if (pPosOld->x + pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x - pSize->x * HALF_VALUE - 0.1f;
				}
				// x���E���獶�ɂ߂荞��	
				else if (pPosOld->x - pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x + pSize->x * HALF_VALUE + 0.1f;
				}
			}

			// �O�ƌ��̔���
			if (pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
			{
				// z���O������߂荞��
				if (pPosOld->z + pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z - pSize->z * HALF_VALUE - 0.1f;
				}
				// z���������߂荞��
				else if (pPosOld->z - pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z + pSize->z * HALF_VALUE + HALF_VALUE;
				}
			}
		}

		if (pPos->x - pSize->x * HALF_VALUE <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * HALF_VALUE >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * HALF_VALUE <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// �ォ�牺
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
 					bLanding = true;
					pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y - pSize->y;
					pMove->y = 0.0f;
				}
				// �������
				else if (pPosOld->y + pSize->y * HALF_VALUE <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * HALF_VALUE > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}		
	}

	return bLanding; // ���n�����Ԃ�
}
//=================================
// �u���b�N�ƃA�C�e���̓����蔻��
//=================================
bool CollisionBlockItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// �����������ǂ���
	bool bHit = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{// ���g�p��������
			// �ǂݔ�΂��ăJ�E���g��i�߂�
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			// ���E�̂߂荞�ݔ���
			if (pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// x��������E�ɂ߂荞��	
				if (pPosOld->x + pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x - pSize->x * HALF_VALUE - 0.1f;
				}
				// x���E���獶�ɂ߂荞��	
				else if (pPosOld->x - pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
				{
					bHit = true;
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x + pSize->x * HALF_VALUE + 0.1f;
				}
			}

			// �O�ƌ��̔���
			if (pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
			{
				// z���O������߂荞��
				if (pPosOld->z + pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					bHit = true;
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z - pSize->z * HALF_VALUE - 0.1f;
				}
				// z���������߂荞��
				else if (pPosOld->z - pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					bHit = true;
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z + pSize->z * HALF_VALUE + HALF_VALUE;
				}
			}
		}

		if (pPos->x - pSize->x * HALF_VALUE <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * HALF_VALUE >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * HALF_VALUE <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// �ォ�牺
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y - pSize->y;
					pMove->y = 0.0f;
				}
				// �������
				else if (pPosOld->y + pSize->y * HALF_VALUE <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * HALF_VALUE > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}
	}

	// �����Ԃ�
	return bHit;
}
//=======================
// �u���b�N�̎擾����
//=======================
BLOCK* GetBlock()
{
	return &g_Block[0];
}
//=======================
// �u���b�N�̐��擾����
//=======================
int NumBlock(void)
{
	return g_NumBlock;
}
//=============================
// �X�e�[�W�̃��[�h����
//=============================
void LoadTitleState(void)
{
	FILE* pFile; // �t�@�C���|�C���^��錾

	char Skip[3]; // �C�R�[����ǂݔ�΂��p

	// ���[�J���ϐ��錾--------------------
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);
	int nType = 0;
	int nIdx = 0;

	//-------------------------------------

	// �t�@�C�����J��
	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{// BLOCKSET��ǂݎ����
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{// BLOCKTYPE��ǂݎ����
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{// POS��ǂݎ����
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{// SIZE��ǂ݂Ƃ���
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{// END_BLOCKSET��ǂݎ����
						// �u���b�N�ɃZ�b�g����
						SetBlock(pos, nType, Scal);
						nIdx++;
						break;
					}
				}
			}

		    if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPT��ǂݎ����
				break;
			}
		}
	}
	else
	{//�J���Ȃ������Ƃ�	
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//=============================
// �u���b�N�̃��f���̃��[�h����
//=============================
void LoadBlockModel(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �t�@�C���|�C���^��錾
	FILE* pFile; 

	// ���[�J���ϐ��錾-----
	char skip[5];
	int nType = 0;
	//----------------------

	// �t�@�C�����J��
	pFile = fopen("data\\MODEL_TXT\\BLOCK.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{// MAX_TYPE��ǂݎ����
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_BlockTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAME��ǂݎ����
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				// �t�@�C������ۑ�
				const char* MODEL_FILENAME = {};

				// �ǂݎ�����p�X����
				MODEL_FILENAME = aString;

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexBlock[nType].BlockTex[nType].g_pBuffMatBlock,
					NULL,
					&g_TexBlock[nType].BlockTex[nType].g_dwNumMatBlock,
					&g_TexBlock[nType].BlockTex[nType].g_pMeshBlock);

				nType++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPT��ǂݎ����
				break;
			}
		}
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Block.cpp)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//=====================================
// �`���[�g���A���̃��f���̃��[�h����
//=====================================
void tutoload(void)
{
	// �t�@�C���|�C���^��錾
	FILE* pFile; 

	// ���[�J���ϐ��錾---------------------
	char Skip[3];
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);
	int nType = 0;
	int nIdx = 0;
	//--------------------------------------

	// �t�@�C�����J��
	pFile = fopen("data\\savetuto.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			// ������
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{// BLOCKSET��ǂݎ����
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{// BLOCKTYPE��ǂݎ����
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{// POS��ǂݎ����
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{// SIZE��ǂݎ����
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{// END_BLOCKSET��ǂݎ����
						SetBlock(pos, nType, Scal);
						nIdx++;
						break;
					}
				}
			}

			if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPT��ǂݎ����
				break;
			}
		}
	}
	else
	{//�J���Ȃ������Ƃ�	
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);

}
//=======================
//�u���b�NOBB�̍쐬
//=======================
void CreateObb(int nCnt)
{
	int nType = g_Block[nCnt].nType; // ��ނ���

	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

	// �ʒu����
	D3DXVECTOR3 pos(g_Block[nCnt].Obb.ObbMtx._41, g_Block[nCnt].Obb.ObbMtx._42, g_Block[nCnt].Obb.ObbMtx._43);

	// �ʒu
	g_Block[nCnt].Obb.CenterPos = pos;

	// ��]�s��̐ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCnt].rot.y, g_Block[nCnt].rot.x, g_Block[nCnt].rot.z);

	// ��]�s��
	g_Block[nCnt].Obb.VecRot[0] = D3DXVECTOR3(mtxRot._11, mtxRot._12, mtxRot._13); // ��]�s��X
	g_Block[nCnt].Obb.VecRot[1] = D3DXVECTOR3(mtxRot._21, mtxRot._22, mtxRot._23); // ��]�s��Y
	g_Block[nCnt].Obb.VecRot[2] = D3DXVECTOR3(mtxRot._31, mtxRot._32, mtxRot._33); // ��]�s��Z

	// �����擾
	g_Block[nCnt].Obb.Length[0] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.x * g_Block[nCnt].Scal.x) - (g_Block[nCnt].BlockTex[nType].vtxMin.x * g_Block[nCnt].Scal.x)) * 0.6f; // ����X
	g_Block[nCnt].Obb.Length[1] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.y * g_Block[nCnt].Scal.y) - (g_Block[nCnt].BlockTex[nType].vtxMin.y * g_Block[nCnt].Scal.y)) * 0.6f; // ����Y
	g_Block[nCnt].Obb.Length[2] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.z * g_Block[nCnt].Scal.z) - (g_Block[nCnt].BlockTex[nType].vtxMin.z * g_Block[nCnt].Scal.z)) * 0.6f; // ����Z
}
//=======================
// OBB�̔���
//=======================
bool collisionObb(int nCnt)
{
	Player* pPlayer = GetPlayer();
	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

	float PlayerLength[3];

	// OBB�̉�]
	D3DXVECTOR3 NAe1 = g_Block[nCnt].Obb.VecRot[0], Ae1 = NAe1 * g_Block[nCnt].Obb.Length[0];
	D3DXVECTOR3 NAe2 = g_Block[nCnt].Obb.VecRot[1], Ae2 = NAe2 * g_Block[nCnt].Obb.Length[1];
	D3DXVECTOR3 NAe3 = g_Block[nCnt].Obb.VecRot[2], Ae3 = NAe3 * g_Block[nCnt].Obb.Length[2];

	// ��]�s��
	D3DXVECTOR3 Nbe1(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Nbe2(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Nbe3(0.0f,0.0f,0.0f);

	D3DXVECTOR3 Max(pPlayer->Motion.aModel[14].vtxMax.x, pPlayer->Motion.aModel[14].vtxMax.y, pPlayer->Motion.aModel[14].vtxMax.z);
	D3DXVECTOR3 Min(pPlayer->Motion.aModel[14].vtxMin.x, pPlayer->Motion.aModel[14].vtxMin.y, pPlayer->Motion.aModel[14].vtxMin.z);

	//D3DXVECTOR3 Max(1000.0f,1000.0f,1000.0f);
	//D3DXVECTOR3 Min(10.0f,0.0f,10.0f);

	// Player
	PlayerLength[0] = fabsf(Max.x - Min.x);
	PlayerLength[1] = fabsf(Max.y - Min.y);
	PlayerLength[2] = fabsf(Max.z - Min.z);

	// Player
	D3DXVECTOR3 NBe1 = Nbe1 * PlayerLength[0];
	D3DXVECTOR3 NBe2 = Nbe2 * PlayerLength[1];
	D3DXVECTOR3 NBe3 = Nbe3 * PlayerLength[2];

	//���f�����̑��
	//D3DXVECTOR3 Model(pPlayer->Motion.aModel[2].mtxWorld._41, pPlayer->Motion.aModel[2].mtxWorld._42, pPlayer->Motion.aModel[2].mtxWorld._43);

	// ���S����v���C���[�̈ʒu�����߂�
	D3DXVECTOR3 Interval = pPlayer->pos - g_Block[nCnt].Obb.CenterPos;

	// �����������߂�
	float VecL = fabsf(D3DXVec3Dot(&Interval, &NAe1));
	float rA = D3DXVec3Length(&Ae1);
	float rB = LenSegOnSeparateAxis(&NAe1, &NBe1, &NBe2, &NBe3);

	// �G��Ă��Ȃ�
	if (VecL > rA + rB)
	{
		return false;
	}

	// ������ : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &NBe1, &NBe2, &NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (VecL > rA + rB)
		return false;

	// ������ : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &NBe1, &NBe2, &NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (VecL > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe1);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (VecL > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe2);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (VecL > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (VecL > rA + rB)
		return false;

	// ������ : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	return true; // �������Ă���
}
//=============================
// �����ʂ̌v�Z?
//=============================
float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}
//=======================
//�u���b�N�̃}�g���b�N�X
//=======================
void SetMtx(int nCntBlock)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;
	Player* pPlayer = GetPlayer();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Block[nCntBlock].Obb.ObbMtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx, &g_Block[nCntBlock].Obb.ObbMtx, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, g_Block[nCntBlock].Size.y * 0.5f, 0.0f);
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx, &g_Block[nCntBlock].Obb.ObbMtx, &mtxTrans);


	mtxParent = g_Block[nCntBlock].mtxWorldBlock;

	//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx,
		&g_Block[nCntBlock].Obb.ObbMtx,
		&mtxParent);//���������e

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].Obb.ObbMtx);
}
//=========================
// OBB�̉����o������
//=========================
bool PushPlayer(int nCntBlock)
{
	//float r = 0.0f;
	//D3DXVECTOR3 PlayerVec[3] = {};

	//for (int i = 0; i < 3; i++)
	//{
	//	D3DXVECTOR3 Direct = g_Block[nCntBlock].Obb.VecRot[i];
	//	D3DXVECTOR3 facepos = g_Block[nCntBlock].Obb.CenterPos + Direct * g_Block[nCntBlock].Obb.Length[i];
	//	r += fabs(D3DXVec3Dot(&facepos, &VecMoveF));
	//}

	//D3DXVECTOR3 ObbPos = g_Block[nCntBlock].Obb.CenterPos;
	//D3DXVECTOR3 Interval = ObbPos - pPlayer->pos;

	//float s = D3DXVec3Dot(&Interval, &VecMoveF);

	//// �߂��������Z�o
	//if (s > 0)
	//	pPlayer->pos.x += r - fabs(s);
	//else
	//	pPlayer->pos.x += r + fabs(s);


	//// �Փ˔���
	//if (fabs(s) - r < 0.0f)
	//	return true; // �Փ˂��Ă���

	bool bLanding = false;
	
	Player* pPlayer = GetPlayer();

	int nType = g_Block[nCntBlock].nType;

	D3DXVECTOR3 VecRot[3] = {};

	// �ӂ̉�]�̌X������
	VecRot[0] = g_Block[nCntBlock].Obb.VecRot[0];
	VecRot[1] = g_Block[nCntBlock].Obb.VecRot[1];
	VecRot[2] = g_Block[nCntBlock].Obb.VecRot[2];

	D3DXVec3Normalize(&VecRot[0], &VecRot[0]);               // �i�s�x�N�g���̐��K��
	D3DXVec3Normalize(&VecRot[1], &VecRot[1]);               // �i�s�x�N�g���̐��K��
	D3DXVec3Normalize(&VecRot[2], &VecRot[2]);               // �i�s�x�N�g���̐��K��

	D3DXVECTOR3 VecMoveF = pPlayer->pos - pPlayer->posOld; // �i�s�x�N�g��
	D3DXVec3Normalize(&VecMoveF, &VecMoveF);               // �i�s�x�N�g���̐��K��


	// �ʂ̈ʒuX(�@���v���X)
	D3DXVECTOR3 faceposXp = g_Block[nCntBlock].Obb.CenterPos + (VecRot[0] * g_Block[nCntBlock].Obb.Length[0]);

	// �ʂ̈ʒuX(�@���}�C�i�X)
	D3DXVECTOR3 faceposXm = g_Block[nCntBlock].Obb.CenterPos - (VecRot[0] * g_Block[nCntBlock].Obb.Length[0]);

	D3DXVECTOR3 PlayerVecXp = pPlayer->pos - faceposXp; // �ʂ�Pos�ƃv���C���[�̃x�N�g��+
	D3DXVECTOR3 PlayerVecXm = pPlayer->pos - faceposXm; // �ʂ�Pos�ƃv���C���[�̃x�N�g��-

	D3DXVec3Normalize(&PlayerVecXp, &PlayerVecXp);      // ���K������
	D3DXVec3Normalize(&PlayerVecXm, &PlayerVecXm);      // ���K������

	D3DXVECTOR3 norXm = -VecRot[0];                     // ���̒l�ɂ���

	float DotXp = fabsf(D3DXVec3Dot(&VecRot[0], &PlayerVecXp)); 	// ����X+�����߂�

	float DotXm = fabsf(D3DXVec3Dot(&norXm, &PlayerVecXm));         // ����X-�����߂�



	// �ʂ̈ʒuY(�@���v���X)
	D3DXVECTOR3 faceposYp = g_Block[nCntBlock].Obb.CenterPos + (VecRot[1] * g_Block[nCntBlock].Obb.Length[1]);

	// �ʂ̈ʒuY(�@���}�C�i�X)
	D3DXVECTOR3 faceposYm = g_Block[nCntBlock].Obb.CenterPos - (VecRot[1] * g_Block[nCntBlock].Obb.Length[1]);

	D3DXVECTOR3 PlayerVecYp = pPlayer->pos - faceposYp; // �ʂ̈ʒu�ƃv���C���[�̃x�N�g��
	D3DXVECTOR3 PlayerVecYm = pPlayer->pos - faceposYm; // �ʂ̈ʒu�ƃv���C���[�̃x�N�g��

	D3DXVec3Normalize(&PlayerVecYp, &PlayerVecYp); // ���K������
	D3DXVec3Normalize(&PlayerVecYm, &PlayerVecYm); // ���K������

	D3DXVECTOR3 norYm = -VecRot[1];                // ���̒l�ɂ���

	float DotYp = fabsf(D3DXVec3Dot(&VecRot[1], &PlayerVecYp)); // ����Y+�����߂�
	float DotYm = fabsf(D3DXVec3Dot(&norYm, &PlayerVecYm));     // ����Y-�����߂�




	// �ʂ̈ʒuZ(�@���v���X)
	D3DXVECTOR3 faceposZp = g_Block[nCntBlock].Obb.CenterPos + (VecRot[2] * g_Block[nCntBlock].Obb.Length[2]);

	// �ʂ̈ʒuZ(�@���}�C�i�X)
	D3DXVECTOR3 faceposZm = g_Block[nCntBlock].Obb.CenterPos - (VecRot[2] * g_Block[nCntBlock].Obb.Length[2]);

	D3DXVECTOR3 PlayerVecZp = pPlayer->pos - faceposZp; // �ʂ̈ʒu�ƃv���C���[�̃x�N�g��
	D3DXVECTOR3 PlayerVecZm = pPlayer->pos - faceposZm; // �ʂ̈ʒu�ƃv���C���[�̃x�N�g��

	D3DXVec3Normalize(&PlayerVecZp, &PlayerVecZp); // ���K������
	D3DXVec3Normalize(&PlayerVecZm, &PlayerVecZm); // ���K������

	D3DXVECTOR3 norZm = -VecRot[2];				   // ���̒l�ɂ���

	float DotZp = fabsf(D3DXVec3Dot(&VecRot[2], &PlayerVecZp)); // ����Z+�����߂�
	float DotZm = fabsf(D3DXVec3Dot(&norZm, &PlayerVecZm));     // ����Z-�����߂�

	// �u���b�N�̏�ɏ���Ă��Ȃ�
	// 
	if (DotYp < DotYm && DotYp <= DotXp && DotYp <= DotXm && DotYp <= DotZp && DotYp <= DotZm)
	{
		bLanding = true;
		//pPlayer->bJump = true;

		if (!pPlayer->bJump) 
		{
			pPlayer->bJump = true; // �n�ʂɒ��n�����̂ŃW�����v�t���O��؂�ւ���
			pPlayer->move.y = 0.0f;
		}

		// �@���x�N�g���̌v�Z
		D3DXVECTOR3 Nor = VecRot[1];
		D3DXVec3Normalize(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹�邽�߂̕␳
		float D = -D3DXVec3Dot(&Nor, &faceposYp);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = -(D3DXVec3Dot(&Nor, &pPlayer->pos) + D) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹�ĕ␳
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + facePlayerPos * Nor;

		pPlayer->pos.y = NewPlayerPos.y; // �ʒu��ʂɍ��킹��

		if (pPlayer->bJump && KeyboardTrigger(DIK_SPACE) || JoypadTrigger(JOYKEY_A))
		{
			// �W�����v���̓����i�㏸/���~�j
			pPlayer->move.y = 15.0f; // �W�����v���x�ŏ㏸

			if (pPlayer->pos.y >= PLAYERJUMPHEIGHT)
			{
				pPlayer->bJump = false;  // �ő�W�����v���x�ɒB������A�W�����v�I��
			}
		}
		else
		{
			// �n�ʂɒ��n�����ꍇ�A�d�͂�������
			if (!bLanding) 
			{
				pPlayer->move.y = -15.0f;  // �d�͂ɂ�闎��
				pPlayer->bLandingOBB = true;
			}
		}

	}
	else if (DotYp > DotYm && DotYp >= DotXp && DotYp >= DotXm && DotYp >= DotZp && DotYp >= DotZm)
	{
		// �@���x�N�g���̌v�Z
		D3DXVECTOR3 Nor = VecRot[1];
		D3DXVec3Normalize(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹�邽�߂̕␳
		float D = -D3DXVec3Dot(&Nor, &faceposYm);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = -(D3DXVec3Dot(&Nor, &pPlayer->pos) + D) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹�ĕ␳
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + facePlayerPos * Nor;

		pPlayer->pos.y = NewPlayerPos.y; // �ʒu��ʂɍ��킹��

	}

	// -X�̖ʂ��瓖������
	else if (DotXp < DotXm && DotXp < DotZp && DotXp < DotZm)
	{
		D3DXVECTOR3 Nor = VecRot[0];
		D3DXVec3Normalize(&Nor, &Nor);

		float D = -D3DXVec3Dot(&Nor, &faceposXp);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = (-D - D3DXVec3Dot(&Nor, &pPlayer->pos)) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹��
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + (Nor * facePlayerPos);  // �v���C���[��ʂɍ��킹�ĕ␳

		// �v���C���[�̈ʒu���X�V
		pPlayer->pos = NewPlayerPos;
	}
	// +X�̖ʂ��瓖������
	else if (DotXm < DotXp && DotXm < DotZp && DotXm < DotZm)
	{
		D3DXVECTOR3 Nor = -VecRot[0];
		D3DXVec3Normalize(&Nor, &Nor);

		float D = -D3DXVec3Dot(&Nor, &faceposXm);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = (-D - D3DXVec3Dot(&Nor, &pPlayer->pos)) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹��
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + (Nor * facePlayerPos);  // �v���C���[��ʂɍ��킹�ĕ␳

		// �v���C���[�̈ʒu���X�V
		pPlayer->pos = NewPlayerPos;
	}
	// -Z�̖ʂ��瓖������
	else if (DotZp > DotZm && DotZp > DotXp && DotZp > DotXm)
	{
		D3DXVECTOR3 Nor = -VecRot[2];
		D3DXVec3Normalize(&Nor, &Nor);

		float D = -D3DXVec3Dot(&Nor, &faceposZm);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = (-D - D3DXVec3Dot(&Nor, &pPlayer->pos)) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹��
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + (Nor * facePlayerPos);  // �v���C���[��ʂɍ��킹�ĕ␳

		// �v���C���[�̈ʒu���X�V
		pPlayer->pos = NewPlayerPos;

	}
	// +Z�̖ʂ��瓖������
	else if (DotZp < DotZm && DotZp < DotXp && DotZp < DotXm)
	{
		D3DXVECTOR3 Nor = VecRot[2];
		D3DXVec3Normalize(&Nor, &Nor);

		float D = -D3DXVec3Dot(&Nor, &faceposZp);

		// �v���C���[�̈ʒu (x, y, z) �ɑ΂��Ėʏ�̍��������߂�
		float facePlayerPos = (-D - D3DXVec3Dot(&Nor, &pPlayer->pos)) / D3DXVec3Dot(&Nor, &Nor);

		// �v���C���[�̈ʒu��ʂɍ��킹��
		D3DXVECTOR3 NewPlayerPos = pPlayer->pos + (Nor * facePlayerPos);  // �v���C���[��ʂɍ��킹�ĕ␳

		// �v���C���[�̈ʒu���X�V
		pPlayer->pos = NewPlayerPos;
	}
	
	//if (DotYp > DotYm && DotYp >= DotXp && DotYp >= DotXm && DotYp >= DotZp && DotYp >= DotZm)
	//{
	//	D3DXVECTOR3 Nor = -VecRot[1];

	//	float D = -(Nor.x * faceposYm.x + Nor.y * faceposYm.y + Nor.z * faceposYm.z);

	//	float PlayerPosY = -(Nor.z * pPlayer->pos.z + Nor.x * pPlayer->pos.x + D) / Nor.y;

	//	pPlayer->pos.y = PlayerPosY + pPlayer->Size.y;
	//}

	//SetEffect(faceposYm,D3DXVECTOR3(0.0f,0.0f,0.0f),10,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),10,30.0f);

	return bLanding;
}
