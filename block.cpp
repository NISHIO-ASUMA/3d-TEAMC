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

//****************************
// �}�N����`
//****************************
#define MAX_WORD (256)	  // �ő�̕�����
#define HALF_VALUE (0.6f) // ���鐔

//****************************
// �v���g�^�C�v�錾
//****************************
void LoadBlockModel(void); // ���f���ǂݍ��ݏ���
void SetMtx(int nCntBlock); // ���[���h�}�g���b�N�X�̐ݒ�(���Spos)
void PushPlayer(int nCntBlock); // OBB�̉����o��

//****************************
// �O���[�o���ϐ��錾
//****************************
BLOCK g_Block[MAX_BLOCK];		// �\���̕ϐ�
BLOCK g_TexBlock[BLOCKTYPE_MAX];// �u���b�N�̃e�N�X�`�����
int nCounterStateBlock;			// ��ԊǗ��J�E���^�[
int g_NumBlock;					// �u���b�N�̔z�u������
int g_BlockTypeMax;				// ��ސ�

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
	DWORD normal;

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
			PushPlayer(nCntBlock);
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
	g_Block[nCnt].Obb.Length[0] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.x * g_Block[nCnt].Scal.x) - (g_Block[nCnt].BlockTex[nType].vtxMin.x * g_Block[nCnt].Scal.x)) * 0.7f; // ����X
	g_Block[nCnt].Obb.Length[1] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.y * g_Block[nCnt].Scal.y) - (g_Block[nCnt].BlockTex[nType].vtxMin.y * g_Block[nCnt].Scal.y)) * 0.7f; // ����Y
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

	D3DXVECTOR3 Max(pPlayer->Motion.aModel[2].vtxMax.x, pPlayer->Motion.aModel[2].vtxMax.y, pPlayer->Motion.aModel[2].vtxMax.z);
	D3DXVECTOR3 Min(pPlayer->Motion.aModel[2].vtxMin.x, pPlayer->Motion.aModel[2].vtxMin.y, pPlayer->Motion.aModel[2].vtxMin.z);

	// Player
	PlayerLength[0] = fabsf(Max.x - Min.x);
	PlayerLength[1] = fabsf(Max.y - Min.y);
	PlayerLength[2] = fabsf(Max.z - Min.z);

	// Player
	D3DXVECTOR3 NBe1 = Nbe1 * PlayerLength[0];
	D3DXVECTOR3 NBe2 = Nbe2 * PlayerLength[1];
	D3DXVECTOR3 NBe3 = Nbe3 * PlayerLength[2];

	//���f�����̑��
	D3DXVECTOR3 Model(pPlayer->Motion.aModel[0].mtxWorld._41, pPlayer->Motion.aModel[0].mtxWorld._42, pPlayer->Motion.aModel[0].mtxWorld._43);

	// ���S����v���C���[�̈ʒu�����߂�
	D3DXVECTOR3 Interval = Model - g_Block[nCnt].Obb.CenterPos;

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
void PushPlayer(int nCntBlock)
{
	Player* pPlayer = GetPlayer();

	int nType = g_Block[nCntBlock].nType;

	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[0], &g_Block[nCntBlock].Obb.VecRot[0]);
	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[1], &g_Block[nCntBlock].Obb.VecRot[1]);
	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[2], &g_Block[nCntBlock].Obb.VecRot[2]);

	D3DXVECTOR3 VecMoveF = pPlayer->pos - pPlayer->posOld; // �i�s�x�N�g��
	float VecA = D3DXVec3Dot(&VecMoveF, &g_Block[nCntBlock].Obb.VecRot[0]);

	D3DXVECTOR3 WallMove = VecMoveF - VecA * g_Block[nCntBlock].Obb.VecRot[0];
	D3DXVec3Normalize(&WallMove, &WallMove);

	
	pPlayer->move.x += WallMove.x * pPlayer->speed;
	pPlayer->move.z += WallMove.z * pPlayer->speed;

}
