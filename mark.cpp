//==============================================================================================================
//
// �}�[�N�����@[ mark.cpp ]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================
#include "mark.h"
#include "player.h"
#include "boss.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MARK_TEXTURE (10)						  // �e�N�X�`���̐�
#define FILE_NAME ("data/MODEL/OBJ/yajirusi.x")   // �ǂݍ��ރt�@�C����
#define NULLVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) // �������p�}�N���ϐ�

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
	ENEMY* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();
	Boss* pBoss = Getboss();

	bool bFirst = true;
	int nIdxEnemy = 0;

	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;

	// �G�̍ő吔�����߂�
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// ���������߂�
		float DisposX = pEnemy[nCnt].pos.x - pPlayer->pos.x;
		float DisposY = pEnemy[nCnt].pos.y - pPlayer->pos.y;
		float DisposZ = pEnemy[nCnt].pos.z - pPlayer->pos.z;

		// ���������߂�
		fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

		// �ŏ������ʂ�
		if (bFirst == true)
		{
			fDistanceStock = fDistanceNow;
			bFirst = false;
			nIdxEnemy = nCnt;
		}
		else
		{
			// ���̋������X�g�b�N���ꂽ������菬����������
			if (fDistanceNow < fDistanceStock)
			{
				fDistanceStock = fDistanceNow; // ������ۑ�
				nIdxEnemy = nCnt; // �߂��G�̃C���f�b�N�X��ۑ�
			}
		}		
	}

	bool bFirstBoss = true;
	int nIdxBoss = 0;

	float BossfDistanceNow = 0.0f;
	float BossfDistanceStock = 0.0f;

	// �{�X�̍ő吔�����߂�
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		// ���������߂�
		float DisposX = pBoss[nCnt].pos.x - pPlayer->pos.x;
		float DisposY = pBoss[nCnt].pos.y - pPlayer->pos.y;
		float DisposZ = pBoss[nCnt].pos.z - pPlayer->pos.z;

		// ���������߂�
		BossfDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

		// �ŏ������ʂ�
		if (bFirstBoss == true)
		{
			BossfDistanceStock = BossfDistanceNow;
			bFirstBoss = false;
			nIdxBoss = nCnt;
		}
		else
		{
			// ���̋������X�g�b�N���ꂽ������菬����������
			if (BossfDistanceNow < BossfDistanceStock)
			{
				BossfDistanceStock = BossfDistanceNow; // ������ۑ�
				nIdxBoss = nCnt; // �߂��G�̃C���f�b�N�X��ۑ�
			}
		}
	}

	// �ǂ������̋�����300�ȓ���������
	if (fDistanceStock >= 300.0f && BossfDistanceStock >= 300.0f)
	{
		// �\������
		g_Mark.bUse = true;

		// �G�̂ق����߂�������
		if (BossfDistanceStock > fDistanceStock)
		{
			// �G�܂ł̊p�x�����߂�
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - pPlayer->pos.x, pEnemy[nIdxEnemy].pos.z - pPlayer->pos.z);

			g_Mark.rotDest.y = fAngle + D3DX_PI;
		}
		// �{�X�̂ق����߂�������
		else if (BossfDistanceStock < fDistanceStock)
		{
			// �{�X�܂ł̊p�x�����߂�
			float fAngle = atan2f(pBoss[nIdxBoss].pos.x - pPlayer->pos.x, pBoss[nIdxBoss].pos.z - pPlayer->pos.z);

			g_Mark.rotDest.y = fAngle + D3DX_PI;
		}

		// �ړI�̊p�x�܂ł̋��������߂�
		D3DXVECTOR3 diffRot = g_Mark.rotDest - g_Mark.rot;

		// �p�x�̐��K��
		NormalizeRotation(&diffRot.x, &diffRot.y, &diffRot.z);

		// �ړI�̊p�x�ɋ߂Â���
		g_Mark.rot.y += (g_Mark.rotDest.y - g_Mark.rot.y) * 0.5f;
	}
	else
	{
		g_Mark.bUse = false;
	}
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
	D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y + 100.0f, pPlayer->pos.z);
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
