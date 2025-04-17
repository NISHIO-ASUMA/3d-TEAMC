//===================================================================================================================
//
// ���b�V���V�����_�[[meshcylinder.cpp]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "meshcylinder.h"
#include "effectEdit.h"
#include "mark.h"
#include "player.h"
#include <cassert>

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_CYLINDER (200) // �V�����_�[�̐�
#define CYLINDER_ALV (0.5f) // ���l�̊
#define MAX_WORD (256)      // �ő�̕�����

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateHealCylinder(int CylinderIdx);		// �񕜂̎��̃V�����_�[�̍X�V����
void UpdateEventCylinder(int CylinderIdx);		// �C�x���g�������̃V�����_�[�̐ݒ�
void UpdateTrritoryCylinder(int CylinderIdx);	// �e���g���[�̃V�����_�[�̐ݒ�
void UpdateItemCylinder(int CylinderIdx);		// �A�C�e���̃V�����_�[�̐ݒ�

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinderOrigin[CYLINDERTYPE_MAX] = {};	    // �e�N�X�`���ւ̃|�C���^
MeshCylinder g_MeshCylinder[MAX_CYLINDER];

//===================================================================================================================
// ���b�V���V�����_�[�̏���������
//===================================================================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //�f�o�C�X�ւ̃|�C���^

	// �S�V�����_�[����
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		g_MeshCylinder[nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_MeshCylinder[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshCylinder[nCnt].nType = CYLINDERTYPE_NORMAL;
		g_MeshCylinder[nCnt].nLife = 0;
		g_MeshCylinder[nCnt].fRadius = 0.0f;
		g_MeshCylinder[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_MeshCylinder[nCnt].nNumPosX = 0;
		g_MeshCylinder[nCnt].nNumPosZ = 0;
		g_MeshCylinder[nCnt].Index = 0;
		g_MeshCylinder[nCnt].Vertex = 0;
		g_MeshCylinder[nCnt].polygon = 0;
		g_MeshCylinder[nCnt].Speed = 0.0f;
		g_MeshCylinder[nCnt].decAlv = 0.0f;
		g_MeshCylinder[nCnt].bUse = false;
	}
}
//===================================================================================================================
// ���b�V���V�����_�[�̏I������
//===================================================================================================================
void UninitMeshCylinder(void)
{
	// �S�V�����_�[����
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// �e�N�X�`���̉��
		if (g_MeshCylinder[nCnt].g_pTextureMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pTextureMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pTextureMeshCylinder = NULL;
		}
		// �C���f�b�N�X�o�b�t�@�̉��
		if (g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder = NULL;
		}
		// ���_�o�b�t�@�̉��
		if (g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder = NULL;
		}
	}

#ifdef _DEBUG
	// ���ׂẴV�����_�[����
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// �e�N�X�`����NULL���m�F
		assert(g_MeshCylinder[nCnt].g_pTextureMeshCylinder == NULL && "cylinder.cpp�������e�N�X�`����j���ł��Ă܂���");

		// �C���f�b�N�X�o�b�t�@��NULL���m�F
		assert(g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder == NULL && "cylinder.cpp�������C���f�b�N�X�o�b�t�@��j���ł��Ă܂���");

		// ���_�o�b�t�@��NULL���m�F
		assert(g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder == NULL && "cylinder.cpp���������_�o�b�t�@��j���ł��Ă܂���");
	}

#endif // DEBUG
}
//===================================================================================================================
// ���b�V���V�����_�[�̍X�V����
//===================================================================================================================
void UpdateMeshCylinder(void)
{	
	// �S�V�����_�[����
	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		// ���g�p��������
		if (g_MeshCylinder[nCntCylinder].bUse == false)
		{
			// ������ǂݔ�΂�
			continue;
		}
		
		// ���V�����_�[�̎��
		switch (g_MeshCylinder[nCntCylinder].nType)
		{
		case CYLINDERTYPE_NORMAL:
			break;
		case CYLINDERTYPE_HEAL:
			// �񕜃V�����_�[
			UpdateHealCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_EVENT:
			// �C�x���g�̃V�����_�[�̍X�V
			UpdateEventCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_TERRITORY:
			// �e���g���[�̃V�����_�[�̍X�V
			UpdateTrritoryCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_ITEM:
			UpdateItemCylinder(nCntCylinder);
			break;
		default:
			break;
		}
	}

}
//===================================================================================================================
// ���b�V���V�����_�[�̕`�揈��
//===================================================================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// �S�V�����_�[����
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// ���g�p��������
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			// ������ǂݔ�΂�
			continue;
		}

		int nType = g_MeshCylinder[nCnt].nType;

		if (nType != CYLINDERTYPE_NORMAL)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
		}

		if (nType == CYLINDERTYPE_TERRITORY)
		{
			//�[�b�g�e�X�g
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_MeshCylinder[nCnt].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder[nCnt].rot.y, g_MeshCylinder[nCnt].rot.x, g_MeshCylinder[nCnt].rot.z);
		D3DXMatrixMultiply(&g_MeshCylinder[nCnt].mtxWorld, &g_MeshCylinder[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder[nCnt].pos.x, g_MeshCylinder[nCnt].pos.y, g_MeshCylinder[nCnt].pos.z);
		D3DXMatrixMultiply(&g_MeshCylinder[nCnt].mtxWorld, &g_MeshCylinder[nCnt].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder[nCnt].mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder);

		//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshCylinder[nCnt].Vertex, 0, g_MeshCylinder[nCnt].polygon);
	}

	//�[�b�g�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================
// ���b�V���V�����_�[�̐ݒ菈��
//===================================================================================================================
int SetMeshCylinder(D3DXVECTOR3 pos, int nType, int nLife, float fRadius, D3DXCOLOR col, int nNumPosX, int nNumPosZ,float speed,float fHeight) // ���b�V���V�����_�[�̐ݒ�
{
	int nCnt = 0;

	// �S�V�����_�[����
	for (nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// ���g�p��������
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			g_MeshCylinder[nCnt].Vertex = (nNumPosX + 1) * (nNumPosZ + 1);
			g_MeshCylinder[nCnt].polygon = ((nNumPosX * nNumPosZ * 2)) + (4 * nNumPosZ - 1);
			g_MeshCylinder[nCnt].Index = g_MeshCylinder[nCnt].polygon + 2;
			g_MeshCylinder[nCnt].pos = pos;
			g_MeshCylinder[nCnt].nType = nType;
			g_MeshCylinder[nCnt].nLife = nLife;
			g_MeshCylinder[nCnt].fRadius = fRadius;
			g_MeshCylinder[nCnt].col = col;
			g_MeshCylinder[nCnt].col.a = CYLINDER_ALV;
			g_MeshCylinder[nCnt].nNumPosX = nNumPosX;
			g_MeshCylinder[nCnt].nNumPosZ = nNumPosZ;
			g_MeshCylinder[nCnt].decAlv = CYLINDER_ALV / nLife;
			g_MeshCylinder[nCnt].Speed = speed;
			g_MeshCylinder[nCnt].fHeight = fHeight;
			g_MeshCylinder[nCnt].bUse = true;

			// ���
			int Vetex = g_MeshCylinder[nCnt].Vertex;
			int Index = g_MeshCylinder[nCnt].Index;

			// �V�����_�[�̍쐬����
			CreateMeshCylinder(nCnt, Vetex, Index, nNumPosX, nNumPosZ, fRadius,fHeight);
			break;
		}
	}
	// �C���f�b�N�X��Ԃ�
	return nCnt;
}
//===================================================================================================================
// ���b�V���V�����_�[�̍쐬����
//===================================================================================================================
void CreateMeshCylinder(int nCntCylinder, int Vertex, int Index, int nNumPosX, int nNumPosZ,float fRadius,float fHeight) // ���b�V���V�����_�[�̍쐬����
{
	int nCnt = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //�f�o�C�X�ւ̃|�C���^

	//if (g_MeshCylinder[nCntCylinder].nType == CYLINDERTYPE_HEAL)
	//{
	//	int nType = g_MeshCylinder[nCntCylinder].nType;
	//	g_MeshCylinder[nCntCylinder].g_pTextureMeshCylinder = g_pTextureMeshCylinderOrigin[nType];
	//}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * Index,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / nNumPosX;
	float fTexY = 1.0f / nNumPosZ;

	D3DXVECTOR3 nor;
	D3DXVECTOR3 pos = g_MeshCylinder[nCntCylinder].pos;

	//�c
	for (int nCntV = 0; nCntV <= nNumPosZ; nCntV++)
	{
		//��
		for (int nCntH = 0; nCntH <= nNumPosX; nCntH++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nNumPosX * nCntH;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				pos.x + sinf(fAngel) * fRadius,
				(nCntV * (fHeight / nNumPosZ)),
				pos.z + cosf(fAngel) * fRadius);

			//�@���x�N�g���̐ݒ�
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			nor = pVtx[nCnt].pos - pos;

			D3DXVec3Normalize(&pVtx[nCnt].nor, &nor);

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_MeshCylinder[nCntCylinder].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, nCntV * fTexY);

			nCnt++;//���Z
		}
	}

	g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder->Unlock();

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	int IndxCount3 = nNumPosX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < nNumPosZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nNumPosX; IndxCount2++, IndxCount3++, Num++)
		{
			pIdx[IdxCnt] = IndxCount3;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		if (IndxCount1 < nNumPosZ - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxCount3;
			IdxCnt += 2;
		}
#ifdef _DEBUG

		// �z�񂪃I�[�o�[�������ĂȂ������m�F
		assert(IdxCnt <= Index && "�V�����_�[INDEX�z��I�[�o�[����");

#endif // _DEBUG
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// ���b�V���V�����_�[�̏�������
//===================================================================================================================
void DeleteCylinder(int nIdx)
{
	g_MeshCylinder[nIdx].bUse = false;
}
//===================================================================================================================
// ���b�V���V�����_�[�̓����蔻��
//===================================================================================================================
bool KeepInCylinder(D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx;
	bool bHit = false;

	// �S���̃V�����_�[����
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// �g�p��Ԃ���Ȃ����C�x���g�̃V�����_�[����Ȃ�������
		if (g_MeshCylinder[nCnt].bUse == false || g_MeshCylinder[nCnt].nType != CYLINDERTYPE_EVENT)
		{
			// �ǂݔ�΂�
			continue;
		}

		// �S���̒��_����
		for (int nCntvtx = 0; nCntvtx < g_MeshCylinder[nCnt].nNumPosX; nCntvtx++)
		{
			//���_�o�b�t�@�����b�N
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

			// ���_����ڕW�܂ł̃x�N�g��
			D3DXVECTOR3 posDirection = *pPos - (g_MeshCylinder[nCnt].pos + pVtx[nCntvtx].pos);

			D3DXVECTOR3 edgeVector = (pVtx[(nCntvtx + 1) % g_MeshCylinder[nCnt].nNumPosX].pos + g_MeshCylinder[nCnt].pos) - (g_MeshCylinder[nCnt].pos + pVtx[nCntvtx].pos);

			D3DXVECTOR3 Cross = NULLVECTOR3;
			D3DXVec3Cross(&Cross, &edgeVector, &posDirection);
			D3DXVec3Normalize(&Cross, &Cross);

			if (Cross.y <= 0.0f)
			{
				bHit = true;
			}

			//���_�o�b�t�@���A�����b�N
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Unlock();
		}
	}
	return bHit;
}
//===================================================================================================================
// ���b�V���V�����_�[�̈ʒu�ݒ菈��
//===================================================================================================================
void SetPotisionCylinder(int nIdx,D3DXVECTOR3 pos)
{
	if (g_MeshCylinder[nIdx].nType == CYLINDERTYPE_HEAL)
	{
		g_MeshCylinder[nIdx].pos.x = pos.x;
		g_MeshCylinder[nIdx].pos.z = pos.z;
	}
}
//===================================================================================================================
// �񕜂̎��̃V�����_�[�̍X�V����
//===================================================================================================================
void UpdateHealCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	// �ʒu�̍X�V
	g_MeshCylinder[CylinderIdx].pos.y += g_MeshCylinder[CylinderIdx].Speed;

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̃J�E���g
	int nCnt = 0;

	// ���_X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// ���_Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// ���a
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// ����
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// �ʒu����
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//�c
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//��
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// �p�x�����߂�
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//���Z
		}
	}

	// �����x���グ��
	g_MeshCylinder[CylinderIdx].col.a -= g_MeshCylinder[CylinderIdx].decAlv;

	// ���������炷
	g_MeshCylinder[CylinderIdx].nLife--;

	// ������0�ɂȂ�����
	if (g_MeshCylinder[CylinderIdx].nLife <= 0)
	{
		// ���g�p�ɂ���
		g_MeshCylinder[CylinderIdx].bUse = false;
	}

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// �C�x���g�������̃V�����_�[�̐ݒ�
//===================================================================================================================
void UpdateEventCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̃J�E���g
	int nCnt = 0;

	// ���_X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// ���_Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// ���a
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// ����
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// �ʒu����
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//�c
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//��
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// �p�x�����߂�
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//���Z
		}
	}

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// �e���g���[�̃V�����_�[�̐ݒ�
//===================================================================================================================
void UpdateTrritoryCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̃J�E���g
	int nCnt = 0;

	// ���_X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// ���_Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// ���a
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// ����
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// �ʒu����
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//�c
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//��
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// �p�x�����߂�
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			g_MeshCylinder[CylinderIdx].col.a = 0.3f;
			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//���Z
		}
	}

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// �A�C�e���̃V�����_�[�̐ݒ�
//===================================================================================================================
void UpdateItemCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̃J�E���g
	int nCnt = 0;

	// ���_X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// ���_Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// ���a
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// ����
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// �ʒu����
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//�c
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//��
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// �p�x�����߂�
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//���_���W�̐ݒ�
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			g_MeshCylinder[CylinderIdx].col.a = 0.4f;

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//���Z
		}
	}

	//���_�o�b�t�@�����b�N
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// �V�����_�[�̏���
//===================================================================================================================
void ResetItemCylinder(void)
{
	// �A�C�e���̎擾
	Item* pItem = GetItem();

	// �A�C�e������
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		int nIdx = pItem[nCntItem].nCylinderIdx;

		if (g_MeshCylinder[nIdx].nType != CYLINDERTYPE_ITEM) continue;

		g_MeshCylinder[nIdx].bUse = false;
	}
}
//===================================================================================================================
// �V�����_�[�̃C���f�b�N�X
//===================================================================================================================
void SetItemCylinder(int nHaveIdx)
{
	// �A�C�e���̎擾
	Item* pItem = GetItem();

	FILE* pFile; // �t�@�C���̃|�C���^

	// �t�@�C�����J��
	pFile = fopen("data\\ITEM\\SetminimapItem.txt", "r");

	int playeritem = -1;
	int minimapItem = -2;

	// �t�@�C�����J������
	if (pFile != NULL)
	{
		char aString[MAX_WORD] = {};
		char skip[10] = {};

		while (1)
		{
			// ������ǂݎ��
			int nData = fscanf(pFile, "%s", &aString[0]);

			// PLAYER_ITEM��ǂݎ������
			if (strcmp(&aString[0], "PLAYER_ITEM") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �v���C���[�̃A�C�e����ǂݎ��
				nData = fscanf(pFile, "%d", &playeritem);
			}

			// MINIMAP_ITEM��ǂݎ������
			if (strcmp(&aString[0], "MINIMAP_ITEM") == 0)
			{
				// [=]���΂�
				nData = fscanf(pFile, "%s", &skip[0]);

				// �\������A�C�e����ǂݎ��
				nData = fscanf(pFile, "%d", &minimapItem);

				// �����Ă���A�C�e������v���Ă�����
				if (playeritem == pItem[nHaveIdx].nType)
				{
					playeritem = -1;
					break;
				}
			}

			// EOF��ǂݎ������
			if (nData == EOF)
			{
				playeritem = -1;
				minimapItem = -2;

				// �t�@�C�������
				fclose(pFile);

				// while���𔲂���
				return;
			}
		}

	}
	else
	{
		// ���b�Z�[�W�{�b�N�X��\��
		MessageBox(NULL, "�t�@�C�����J���܂���", "LoadMinimapItem", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);

	// �A�C�e������
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// ���g�p��������
		if (pItem[nCntItem].bUse == false) continue;

		// �����Ă���A�C�e���Ɠ�����������
		if (nCntItem == nHaveIdx) continue;

		// �\���������A�C�e������Ȃ�������
		if (pItem[nCntItem].nType != minimapItem) continue;

		// �ʒu
		D3DXVECTOR3 pos(pItem[nCntItem].pos.x, 0.0f, pItem[nCntItem].pos.z);

		// �A�C�e���̏ꏊ�Ɉʒu��ݒ�
		pItem[nCntItem].nCylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_ITEM, 0, 10.0f, COLOR_PINK, 8, 1, 0.0f, 50.0f);
	}

	// ������
	minimapItem = -2;
}
//===================================================================================================================
// �V�����_�[�̓����蔻��
//===================================================================================================================
bool CollisionCylinder(int nIdx, D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx;

	// �S���̒��_����
	for (int nCntvtx = 0; nCntvtx < g_MeshCylinder[nIdx].nNumPosX; nCntvtx++)
	{
		if (g_MeshCylinder[nIdx].nType != CYLINDERTYPE_TERRITORY) continue;

		//���_�o�b�t�@�����b�N
		g_MeshCylinder[nIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

		// ���_����ڕW�܂ł̃x�N�g��
		D3DXVECTOR3 posDirection = *pPos - (g_MeshCylinder[nIdx].pos + pVtx[nCntvtx].pos);

		D3DXVECTOR3 edgeVector = (pVtx[(nCntvtx + 1) % g_MeshCylinder[nIdx].nNumPosX].pos + g_MeshCylinder[nIdx].pos) - (g_MeshCylinder[nIdx].pos + pVtx[nCntvtx].pos);

		D3DXVECTOR3 Cross = NULLVECTOR3;
		D3DXVec3Cross(&Cross, &edgeVector, &posDirection);
		D3DXVec3Normalize(&Cross, &Cross);

		if (Cross.y <= 0.0f)
		{
			return true;
		}

		//���_�o�b�t�@���A�����b�N
		g_MeshCylinder[nIdx].g_pVtxBuffMeshCylinder->Unlock();
	}
	return false;
}
