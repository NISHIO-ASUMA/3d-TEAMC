//=====================================================================================================================
//
//�e�\���֘A�̃w�b�_�[
//Author;�����I��
//
//=====================================================================================================================

//*****************************************************************************************************************
// �C���N���[�h�ꗗ
//*****************************************************************************************************************
#include "main.h"
#include "Shadow.h"

//*****************************************************************************************************************
// �}�N����`
//*****************************************************************************************************************
#define SHADOW_AMOUNT (7000)  // �\������ő吔

//*****************************************************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************************************************
Shadow g_aShadow[SHADOW_AMOUNT]; // �\���̕ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL; // ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		 // �e�N�X�`���ւ̃|�C���^

//===================================================================================================
// Init�����F�X
//===================================================================================================
void InitShadow(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureShadow);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_AMOUNT * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// �����������ꗗ
	for (int nCnt = 0; nCnt < SHADOW_AMOUNT; nCnt++)
	{
		g_aShadow[nCnt].bUse = false;
		g_aShadow[nCnt].bHide = false;
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].fSize = 0.0f;
		g_aShadow[nCnt].fALv = 1.0f;
		g_aShadow[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nCnt].fALv);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//���A��O�A�����}�C�i�X
	for (int nCnt = 0; nCnt < SHADOW_AMOUNT; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		pVtx[0].col = g_aShadow[nCnt].col;
		pVtx[1].col = g_aShadow[nCnt].col;
		pVtx[2].col = g_aShadow[nCnt].col;
		pVtx[3].col = g_aShadow[nCnt].col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffShadow->Unlock();
}
//===================================================================================================
// Uninit�����F�X
//===================================================================================================
void UninitShadow(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
	// �e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}
//=====================================================================================================
// �X�V�����̔����k
//=====================================================================================================
void UpdateShadow(void)
{
	// �K�v�ɉ����Ēǉ�
}
//===================================================================================================
// �`�ʏ����F�X
//===================================================================================================
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// �����x�I�v�V�����ǉ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	// �S�Č��񂵂�
	for (int nCntShadow = 0; nCntShadow < SHADOW_AMOUNT; nCntShadow++)
	{
		// �����Ă���Ȃ�
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//	���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X��ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//��\������Ȃ��Ȃ�`��
			if (g_aShadow[nCntShadow].bHide == false)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
			}
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=================================================================================================================================
// �ݒu�����F�X ( ������ �ꏊ�A�����A�傫�� )
//=================================================================================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, float fAlv)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// �J�E���g�p�ϐ�
	int nCntShadow;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	// �S�Č��񂵂�
	for (nCntShadow = 0; nCntShadow < SHADOW_AMOUNT; nCntShadow++)
	{
		// �g���ĂȂ��Ȃ�ݒu
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].fSize = fSize;
			g_aShadow[nCntShadow].fALv = fAlv;
			g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nCntShadow].fALv);
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;

			// Size�͔��a�Ȏ��ɒ���
			pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 2.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 2.0f, g_aShadow[nCntShadow].pos.z + fSize);
			pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - fSize, 2.0f, g_aShadow[nCntShadow].pos.z - fSize);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + fSize, 2.0f, g_aShadow[nCntShadow].pos.z - fSize);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			break;
		}
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffShadow->Unlock();

	// �e�̔ԍ���Ԃ�
	return nCntShadow;
}
//===================================================================================================
// �����X�V�����F�X
//===================================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fSize, float fAlv)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].fSize = fSize;
	g_aShadow[nIdxShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aShadow[nIdxShadow].fALv);
	g_aShadow[nIdxShadow].fALv = fAlv;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nIdxShadow;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z + g_aShadow[nIdxShadow].fSize);
	pVtx[2].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x - g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].pos.x + g_aShadow[nIdxShadow].fSize, 2.0f, g_aShadow[nIdxShadow].pos.z - g_aShadow[nIdxShadow].fSize);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_aShadow[nIdxShadow].col;
	pVtx[1].col = g_aShadow[nIdxShadow].col;
	pVtx[2].col = g_aShadow[nIdxShadow].col;
	pVtx[3].col = g_aShadow[nIdxShadow].col;

	// �A�����b�N
	g_pVtxBuffShadow->Unlock();
}
//===================================================================================================================
// Idx���w�肵�ĉe���ł�������
//===================================================================================================================
void KillShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
//===================================================================================================================
// Idx���w�肵�ĉe�B�����
//===================================================================================================================
void HideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = true;
}
//===================================================================================================================
// Idx���w�肵�ĉe�ĕ\����������
//===================================================================================================================
void NoHideShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bHide = false;
}
