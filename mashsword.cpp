//=================================
//
// ���b�V���\�[�h[meshsword.h]
// Author:YOSHIDA YUUTO
//
//=================================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"meshsword.h"
#include "input.h"
#include "player.h"

//****************************
//�v���g�^�C�v�錾
//****************************

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_pTextureMeshSword = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshSword = NULL; //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshSword = NULL;//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHSOAD g_MeshSword;
D3DXVECTOR3 g_posOlds[X * 2];

//===============================
//���b�V���t�B�[���h�̏���������
//===============================
void InitMeshSword(void)
{
	int nCnt = 0;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();//�f�o�C�X���擾

		//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Swordorbit.png",
		&g_pTextureMeshSword);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ORBIT_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshSword,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ORBIT_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshSword,
		NULL);

	// ������
	g_MeshSword.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshSword.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshSword.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	g_MeshSword.Scal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_3D* pVtx = NULL;

	Player* pPlayer = GetPlayer();  // �v���C���[�����擾

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshSword->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

	pVtx[1].pos = D3DXVECTOR3(pPlayer->Motion.aModel[15].mtxWorld._41, pPlayer->Motion.aModel[15].mtxWorld._42, pPlayer->Motion.aModel[15].mtxWorld._43);

	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].pos = g_posOlds[nCount * 2];
		pVtx[nCount * 2 + 3].pos = g_posOlds[nCount * 2 + 1];
	}

	////���_���W�̐ݒ�
	//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCount * 2 + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	//���_���W�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCount * 2 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//for (int nCnt = 0; nCnt < ORBIT_VERTEX; nCnt++)
	//{
		////���_���W�̐ݒ�
		//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}
	//���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshSword->Unlock();

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	g_pIdxBuffMeshSword->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < ORBIT_INDEX; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}
	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshSword->Unlock();

}

//===============================
//���b�V���t�B�[���h�̏I������
//===============================ccc
void UninitMeshSword(void)
{
	//�e�N�X�`���̉��
	if (g_pTextureMeshSword != NULL)
	{
		g_pTextureMeshSword->Release();
		g_pTextureMeshSword = NULL;
	}

	//���_�o�b�t�@�̉��
	if (g_pVtxBuffMeshSword != NULL)
	{
		g_pVtxBuffMeshSword->Release();
		g_pVtxBuffMeshSword = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshSword != NULL)
	{
		g_pIdxBuffMeshSword->Release();
		g_pIdxBuffMeshSword = NULL;
	}
}

//===============================
//���b�V���t�B�[���h�̍X�V����
//===============================
void UpdateMeshSword(void)
{

	Player* pPlayer = GetPlayer();  // �v���C���[�����擾
	VERTEX_3D* pVtx;

	int vtxCnt = 0;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffMeshSword->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->state == PLAYERSTATE_ATTACK)
	{
		// 0�̈ʒu����
		pVtx[0].pos.x = pPlayer->SwordMtx._41;
		pVtx[0].pos.y = pPlayer->SwordMtx._42;
		pVtx[0].pos.z = pPlayer->SwordMtx._43;

		// 1�̈ʒu����
		pVtx[1].pos.x = pPlayer->Motion.aModel[15].mtxWorld._41;
		pVtx[1].pos.y = pPlayer->Motion.aModel[15].mtxWorld._42;
		pVtx[1].pos.z = pPlayer->Motion.aModel[15].mtxWorld._43;

		g_MeshSword.oldvtx[0] = pVtx[0].pos;
		g_MeshSword.oldvtx[1] = pVtx[1].pos;

		g_posOlds[4] = g_posOlds[2];
		g_posOlds[5] = g_posOlds[3];

		g_posOlds[2] = g_posOlds[0];
		g_posOlds[3] = g_posOlds[1];

		g_posOlds[0] = pVtx[0].pos;
		g_posOlds[1] = pVtx[1].pos;
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshSword->Unlock();
}

//===============================
//���b�V���t�B�[���h�̕`�揈��
//===============================
void DrawMeshSword(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player* pPlayer = GetPlayer();  // �v���C���[�����擾

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	if (pPlayer->state == PLAYERSTATE_ATTACK)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_MeshSword.mtxWorldMeshSword);

		pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshSword.rot.y, g_MeshSword.rot.x, g_MeshSword.rot.z);
		D3DXMatrixMultiply(&g_MeshSword.mtxWorldMeshSword, &g_MeshSword.mtxWorldMeshSword, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshSword.pos.x, g_MeshSword.pos.y, g_MeshSword.pos.z);
		D3DXMatrixMultiply(&g_MeshSword.mtxWorldMeshSword, &g_MeshSword.mtxWorldMeshSword, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshSword.mtxWorldMeshSword);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshSword, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshSword);

		//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ORBIT_VERTEX, 0, ORBIT_POLYGON);
	}
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===============================
//���b�V��sword��
//===============================

void vtx(D3DXMATRIX vtx0, D3DXMATRIX vtx1)
{
	Player* pPlayer = GetPlayer();  // �v���C���[�����擾

	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffMeshSword->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->state == PLAYERSTATE_ATTACK)
	{
		// 2�̈ʒu����
		pVtx[2].pos.x = vtx0._41;
		pVtx[2].pos.y = vtx0._42;
		pVtx[2].pos.z = vtx0._43;

		// 3�̈ʒu����
		pVtx[3].pos.x = vtx1._41;
		pVtx[3].pos.y = vtx1._42;
		pVtx[3].pos.z = vtx1._43;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshSword->Unlock();
}
