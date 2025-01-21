//============================
//
// �u���b�N[block.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "block.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

//****************************
//�}�N����`
//****************************
#define MOUSE_SIZE (50.0f)

//****************************
//�O���[�o���ϐ��錾
//****************************
BLOCK g_Block[MAX_BLOCK];
BLOCK g_TexBlock[BLOCKTYPE_MAX];
int nCounterStateBlock;
int g_NumBlock;

//=============================
//�u���b�N�̏���������
//=============================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //���W
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�p�x
		g_Block[nCntBlock].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�g�嗦
		g_Block[nCntBlock].nType = BLOCKTYPE_ONE;				//���
		g_Block[nCntBlock].bUse = false;						//���g�p���
		g_Block[nCntBlock].nLife = 20;							//�̗�
		g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;			//���
		g_Block[nCntBlock].fRadius = 100.0f;					//���a

		// TODO : ���̗̑͂̏������A�ǂ�������������?
		//g_Block[nCntBlock].nLife = 180;						
	}

	//�O���[�o���ϐ��̏�����
	g_NumBlock = 0;		   //�u���b�N�̐�
	nCounterStateBlock = 0;//��ԃJ�E���^�[

	//g_Block.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Block.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODELTYPE[nCntNum],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock,
			NULL,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock);
	}

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//g_TexBlock[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexBlock[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexBlock[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexBlock[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_TexBlock[nCntNum].bUse = true;
		g_TexBlock[nCntNum].nLife = 120;

		D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
					//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//���_���̎擾
		nNumVtx = g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ău���b�N�̍ŏ��l,�ő�l���擾
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

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			g_TexBlock[nCntNum].Size.x = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x;
			g_TexBlock[nCntNum].Size.y = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y;
			g_TexBlock[nCntNum].Size.z = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z;
		}


		//���_�o�b�t�@�̃A�����b�N
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->UnlockVertexBuffer();
	}
}
//=============================
//�u���b�N�̏I������
//=============================
void UninitBlock(void)
{
	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex]->Release();
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock = NULL;
		}

		//�}�e���A��
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
		}
	}
}
//=============================
//�u���b�N�̍X�V����
//=============================
void UpdateBlock(void)
{

}
//=============================
//�u���b�N�̕`�揈��
//=============================
void DrawBlock(void)
{
	MODE mode = GetMode();
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal,mtxParent;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			if (!g_Block[nCntBlock].bUse)
			{
				continue;
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorldBlock);

			//�傫���𔽉f
			D3DXMatrixScaling(&mtxScal, g_Block[nCntBlock].Scal.y, g_Block[nCntBlock].Scal.x, g_Block[nCntBlock].Scal.z);
			D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxScal);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
			D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorldBlock);
			

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Block[nCntBlock].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
			{
				D3DXMATERIAL color;

				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Block[nCntBlock].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Block[nCntBlock].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);

				//�u���b�N(�p�[�c)�̕`��
				g_Block[nCntBlock].BlockTex[nCntNum].g_pMeshBlock->DrawSubset(nCntMat);
			}
		}
	}
}
//=======================
//�u���b�N�̐ݒ菈��
//=======================
void SetBlock(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 Scal)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{
			g_Block[nCntBlock] = g_TexBlock[nType];

			g_Block[nCntBlock].pos = pos;
			g_Block[nCntBlock].Scal = Scal;
			g_Block[nCntBlock].nType = nType;
			g_Block[nCntBlock].bUse = true;

			g_NumBlock++;
			break;
		}
	}
}
//=======================
//�u���b�N�̔��菈��
//=======================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	bool bLanding = false;
	Player* pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			//���E�̂߂荞�ݔ���
			if (pPos->z - pSize->z * 0.5f < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * 0.5f > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
			{
				//x��������E�ɂ߂荞��	
				if (pPosOld->x + pSize->x * 0.5f < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * 0.5f > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.y)
				{
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x - pSize->x * 0.5f - 0.1f;
				}
				//x���E���獶�ɂ߂荞��	
				else if (pPosOld->x - pSize->x * 0.5f > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * 0.5f < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x + pSize->x * 0.5f + 0.1f;
				}
			}

			//�O�ƌ��̔���
			if (pPos->x - pSize->x * 0.5f < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * 0.5f > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
			{
				//z���O������߂荞��
				if (pPosOld->z + pSize->z * 0.5f < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * 0.5f > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z - pSize->z * 0.5f - 0.1f;
				}
				//z���������߂荞��
				else if (pPosOld->z - pSize->z * 0.5f > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * 0.5f < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z + pSize->z * 0.5f + 0.5f;
				}
			}
		}

		if (pPos->x - pSize->x * 0.5f <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * 0.5f >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * 0.5f <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * 0.5f >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
			{
				//�ォ�牺
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
 					bLanding = true;
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}
				//�������
				else if (pPosOld->y + pSize->y * 0.5f <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * 0.5f * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * 0.5f > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * 0.5f * g_Block[nCntBlock].Scal.y)
				{
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}		
	}

	return bLanding;
}
//=======================
//�u���b�N�̎擾����
//=======================
BLOCK* GetBlock()
{
	return &g_Block[0];
}
//=======================
//�u���b�N�̐��擾����
//=======================
int NumBlock(void)
{
	return g_NumBlock;
}