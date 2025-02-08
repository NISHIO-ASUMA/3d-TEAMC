//====================================================================================================================
//
// �G�t�F�N�g���� [ Effect.cpp ]
// Author:�����I��
//
//=====================================================================================================================

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "Effect.h"

//****************************
// �}�N����`
//****************************
#define MAX_EFFECT (70000)  // �G�t�F�N�g�̍ő吔

//****************************
// �O���[�o���ϐ��錾
//****************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL; // ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		 // �e�N�X�`���̃|�C���^
Effect g_Effect[MAX_EFFECT];	// �\���̕ϐ�

//============================================================================================================
// �G�t�F�N�g�̏���������
//============================================================================================================
void InitEffect(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �G�t�F�N�g�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.png", 
		&g_pTextureEffect);

	// �f�o�C�X�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_EFFECT * 6,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffEffect, NULL);

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���A��O�A�����}�C�i�X
	// �������Ƒ傫���ݒ�
	// �\���̕ϐ��̏���������
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_Effect[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W
		g_Effect[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
		g_Effect[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����x�N�g��
		g_Effect[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �F
		g_Effect[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_Effect[nCnt].bUse = false;	// ���g�p����
		g_Effect[nCnt].nLife = 0;		// ����
		g_Effect[nCnt].fSize = 0.0f;	// �傫��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_����i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}
//=========================================================================================================
// �G�t�F�N�g�̏I������
//=========================================================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//==========================================================================================================
// �G�t�F�N�g�̍X�V����
//==========================================================================================================
void UpdateEffect(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// �S�Č��񂵂�
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// �g���Ă���Ȃ�
		if (g_Effect[nCnt].bUse == true)
		{
			// �ړ�������
			g_Effect[nCnt].pos.x += g_Effect[nCnt].move.x;
			g_Effect[nCnt].pos.y += g_Effect[nCnt].move.y;
			g_Effect[nCnt].pos.z += g_Effect[nCnt].move.z;
			
			// ���������炵�����x�����Z������
			g_Effect[nCnt].nLife--;
			g_Effect[nCnt].col.a -= 1.0f / g_Effect[nCnt].nMaxLife;

			// �J���[�̐ݒ�
			pVtx[0].col = g_Effect[nCnt].col;
			pVtx[1].col = g_Effect[nCnt].col;
			pVtx[2].col = g_Effect[nCnt].col;
			pVtx[3].col = g_Effect[nCnt].col;

			//���̏�ŏꏊ�𒲐�������
			pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, g_Effect[nCnt].fSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Effect[nCnt].fSize, -g_Effect[nCnt].fSize, 0.0f);

			//�����Ŏ���
			if (g_Effect[nCnt].nLife <= 0)
			{
				// ���g�p��Ԃɂ���
				g_Effect[nCnt].bUse = false;
			}
		}

		// ���_����i�߂�
		pVtx += 4;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}
//=========================================================================================================
// �G�t�F�N�g�̕`�揈��
//=========================================================================================================
void DrawEffect(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;
 
	// ���C�g�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_Effect[nCnt].bUse == true)
		{
			// �r���[�}�g���b�N�X�p�̕ϐ�
			D3DXMATRIX mtxView;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Effect[nCnt].mtxWorld);

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �J�����̋t�s���ݒ�
			g_Effect[nCnt].mtxWorld._11 = mtxView._11;
			g_Effect[nCnt].mtxWorld._12 = mtxView._21;
			g_Effect[nCnt].mtxWorld._13 = mtxView._31;
			g_Effect[nCnt].mtxWorld._21 = mtxView._12;
			g_Effect[nCnt].mtxWorld._22 = mtxView._22;
			g_Effect[nCnt].mtxWorld._23 = mtxView._32;
			g_Effect[nCnt].mtxWorld._31 = mtxView._13;
			g_Effect[nCnt].mtxWorld._32 = mtxView._23;
			g_Effect[nCnt].mtxWorld._33 = mtxView._33;

			g_Effect[nCnt].mtxWorld._41 = 0.0f;
			g_Effect[nCnt].mtxWorld._42 = 0.0f;
			g_Effect[nCnt].mtxWorld._43 = 0.0f;

			mtxView._14 = g_Effect[nCnt].mtxWorld._41;
			mtxView._24 = g_Effect[nCnt].mtxWorld._42;
			mtxView._34 = g_Effect[nCnt].mtxWorld._43;

			// �����𔽉f
			/*D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Effect[nCnt].rot.y, g_Effect[nCnt].rot.x, g_Effect[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxRot);*/

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCnt].pos.x, g_Effect[nCnt].pos.y, g_Effect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Effect[nCnt].mtxWorld, &g_Effect[nCnt].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCnt].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// �e�N�X�`���t�H�[�}�b�g�̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==============================================================================================================
// �G�t�F�N�g�̐ݒ�(�z�u)���� 
//==============================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// �S�Č��񂵂�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���g�p��ԂȂ�
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos; // �ʒu
			g_Effect[nCntEffect].dir = dir; // �����x�N�g��
			g_Effect[nCntEffect].fSpeed = fSpeed; // �ړ����x
			g_Effect[nCntEffect].fSize = fSize;   // �傫��
			g_Effect[nCntEffect].move = D3DXVECTOR3(sinf(g_Effect[nCntEffect].dir.y) * fSpeed, sinf(g_Effect[nCntEffect].dir.x) * fSpeed, cosf(g_Effect[nCntEffect].dir.y) * fSpeed); // �ړ���
			g_Effect[nCntEffect].nLife = nLife; // ����
			g_Effect[nCntEffect].nMaxLife = nLife;  // �ő����
			g_Effect[nCntEffect].col = col; // �F
			g_Effect[nCntEffect].bUse = true; // �g�p��Ԃɂ���

			break;
		}

		// ���_����i�߂�
		pVtx += 4;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}
//======================================================================================================================
// �G�t�F�N�g�̐ݒ�(�z�u)���� (�P����) 
//======================================================================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fSpeed, float fSize)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// �S�Č��񂵂�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���g�p��ԂȂ�
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = pos; // �ʒu
			g_Effect[nCntEffect].move = move; // �ړ���
			g_Effect[nCntEffect].fSpeed = fSpeed; // �ړ����x
			g_Effect[nCntEffect].fSize = fSize; // �傫��
			g_Effect[nCntEffect].nLife = nLife; // ����
			g_Effect[nCntEffect].nMaxLife = nLife; // �ő����
			g_Effect[nCntEffect].col = col;	// �F
			g_Effect[nCntEffect].bUse = true; // �g�p��Ԃɂ���

			break;
		}

		// ���_����i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}