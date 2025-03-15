//==============================================================================================================
//
// �}�[�N�����@[ mark.cpp ]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================
#include "mark.h"
#include "player.h"
#include "boss.h"
#include "math.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MARK_TEXTURE (10)						  // �e�N�X�`���̐�
#define FILE_NAME ("data/MODEL/OBJ/yajirusi.x")   // �ǂݍ��ރt�@�C����

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMark[MARK_TEXTURE] = {};	//�v���C���[�̃e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshMark;							//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatMark;						//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatMark;								//�}�e���A���̐�
Mark g_Mark;

//=========================================================================================================
// ���̏���������
//=========================================================================================================
void InitMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	g_Mark.pos = NULLVECTOR3;
	g_Mark.rot = NULLVECTOR3;
	g_Mark.move = NULLVECTOR3;
	g_Mark.offpos = NULLVECTOR3;
	g_Mark.offrot = NULLVECTOR3;
	g_Mark.bUse = false;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatMark,
		NULL,
		&g_dwNumMatMark,
		&g_pMeshMark);

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatMark->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatMark; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureMark[nCntMat]);
		}
	}

}
//=========================================================================================================
// ���̏I������
//=========================================================================================================
void UninitMark(void)
{
	for (int nCnt = 0; nCnt < (int)g_dwNumMatMark; nCnt++)
	{
		if (g_pTextureMark[nCnt] != NULL)
		{
			g_pTextureMark[nCnt]->Release();
			g_pTextureMark[nCnt] = NULL;
		}
	}
		//���b�V���̔j��
	if (g_pMeshMark != NULL)
	{
		g_pMeshMark->Release();
		g_pMeshMark = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatMark != NULL)
	{
		g_pBuffMatMark->Release();
		g_pBuffMatMark = NULL;
	}
}
//=========================================================================================================
// ���̍X�V����
//=========================================================================================================
void UpdateMark(void)
{
	// �g�p��Ԃ���Ȃ���
	if (g_Mark.bUse == false)
	{
		// �֐��𔲂���
		return;
	}

	// ���̊p�x�̐��K��
	if (g_Mark.rotDest.y - g_Mark.rot.y >= D3DX_PI)
	{
		g_Mark.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_Mark.rotDest.y - g_Mark.rot.y <= -D3DX_PI)
	{
		g_Mark.rot.y -= D3DX_PI * 2.0f;
	}

	g_Mark.rot.y += SetSmoothAprroach(g_Mark.rotDest.y,g_Mark.rot.y,0.1f);
}
//=========================================================================================================
// ���̕`�揈��
//=========================================================================================================
void DrawMark(void)
{
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Mark.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Mark.rot.y, g_Mark.rot.x, g_Mark.rot.z);
	D3DXMatrixMultiply(&g_Mark.mtxWorld, &g_Mark.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z);
	D3DXMatrixMultiply(&g_Mark.mtxWorld, &g_Mark.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Mark.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatMark->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatMark; nCntMat++)
	{
		if (g_Mark.bUse == true)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMark[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshMark->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

}
//=========================================================================================================
// ���̐ݒ菈��
//=========================================================================================================
void SetMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_Mark.bUse == false)
	{
		g_Mark.pos = pos;
		g_Mark.rot = rot;
		g_Mark.bUse = true;
	}
}
//=========================================================================================================
// ���̎擾
//=========================================================================================================
Mark* GetMark(void)
{
	return &g_Mark;
}
