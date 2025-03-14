//=====================================================================================================================
//
// ���b�V���t�B�[���h[meshfield.cpp]
// Author: TEAM_C
//
//=====================================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "meshfield.h"
#include "input.h"
#include "player.h"
#include <cassert>

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;	    // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; // ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshFiled = NULL;  // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHFILED g_MeshField;

//=================================================================================================================
//���b�V���t�B�[���h�̏���������
//=================================================================================================================
void InitMeshField(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field.png",
		&g_pTextureMeshField);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshFiled,
		NULL);

	g_MeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / ZVTX;
	int nCnt = 0;

	//�c
	for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	{
		//��
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			pVtx[nCnt].pos = D3DXVECTOR3(
				(0.0f + (MAX_FIELDWIDTH / XVTX) * nCntX) - (MAX_FIELDWIDTH * 0.5f),
				0.0f,
				MAX_FIELDHEIGHT - ((MAX_FIELDHEIGHT / ZVTX) * nCntZ) - (MAX_FIELDHEIGHT * 0.5f));

			//�@���x�N�g���̐ݒ�
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX,nCntZ * fTexY);

			nCnt++;//���Z
		}
	}

	g_pVtxBuffMeshField->Unlock();

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < ZVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:�Ō�̍s����Ȃ�������
		if (IndxCount1 < ZVTX - 1) 
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}

#ifdef _DEBUG

		// �z�񂪃I�[�o�[�������ĂȂ������m�F
		assert(IdxCnt <= INDEX && "���b�V���t�B�[���hINDEX�z��I�[�o�[����");

#endif // _DEBUG
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshFiled->Unlock();
}

//=================================================================================================================
//���b�V���t�B�[���h�̏I������
//=================================================================================================================ccc
void UninitMeshField(void)
{
	//�e�N�X�`���̉��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshFiled != NULL)
	{
		g_pIdxBuffMeshFiled->Release();
		g_pIdxBuffMeshFiled = NULL;
	}

#ifdef _DEBUG

	// �e�N�X�`����NULL���m�F
	assert(g_pTextureMeshField == NULL && "meshfield.cpp�������e�N�X�`����j���ł��Ă܂���");

	// �C���f�b�N�X�o�b�t�@��NULL���m�F
	assert(g_pIdxBuffMeshFiled == NULL && "meshfield.cpp�������C���f�b�N�X�o�b�t�@��j���ł��Ă܂���");

	// ���_�o�b�t�@��NULL���m�F
	assert(g_pVtxBuffMeshField == NULL && "meshfield.cpp���������_�o�b�t�@��j���ł��Ă܂���");

#endif // DEBUG

}

//=================================================================================================================
//���b�V���t�B�[���h�̍X�V����
//=================================================================================================================
void UpdateMeshField(void)
{

}
//=================================================================================================================
//���b�V���t�B�[���h�̕`�揈��
//=================================================================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshField.mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorldMeshField);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshFiled);

	//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,VERTEX,0,POLYGON);
}
//==================================================================================================================
//���b�V���t�B�[���h�̓����蔻��
//==================================================================================================================
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3* pPosOld)
{
	bool bLanding = false;

	int nCntPos = 0;
	int nNextWall = 1;

	//if (pPlayer->bDisp == true)
	//{
		D3DXVECTOR3 VecALine;

		D3DXVECTOR3 pos, pos1;

		g_MeshField.vtxPos[0].x = (MAX_FIELDWIDTH / XVTX) * 0 - (MAX_FIELDWIDTH * 0.5f);
		g_MeshField.vtxPos[0].y = 0.0f;
		g_MeshField.vtxPos[0].z = MAX_FIELDHEIGHT - ((MAX_FIELDHEIGHT / ZVTX) * 0) - (MAX_FIELDHEIGHT * 0.5f);

		g_MeshField.vtxPos[1].x = (MAX_FIELDWIDTH / XVTX) * 1 - (MAX_FIELDWIDTH * 0.5f);
		g_MeshField.vtxPos[1].y = 0.0f;
		g_MeshField.vtxPos[1].z = MAX_FIELDHEIGHT - ((MAX_FIELDHEIGHT / ZVTX) * 1) - (MAX_FIELDHEIGHT * 0.5f);

		VecALine = g_MeshField.vtxPos[1] - g_MeshField.vtxPos[0];//0-1...

		D3DXVec3Normalize(&VecALine, &VecALine);

		D3DXVECTOR3 PlayerVec;

		D3DXVECTOR3 VecBLine;//�ǂ���v���C���[

		VecBLine = *pPos - g_MeshField.vtxPos[0];

		D3DXVec3Normalize(&VecBLine, &VecBLine);

		D3DXVECTOR3 Cross = {};

		D3DXVec3Cross(&Cross, &VecALine, &VecBLine);

		if (Cross.x < 0)
		{
			bLanding = true;
			pPos->y = 0.0f;
		}
	//}
	return bLanding;//�����Ԃ�
}

