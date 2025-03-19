//===================================================================================================================
//
// �G�t�F�N�g2d [effect2d.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "effect2d.h"
#include <cassert>

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_EFFECT2D (4096) // �G�t�F�N�g�̍ő吔

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect2d = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2d = NULL;//���_�o�b�t�@�ւ̃|�C���^
Effect2D  g_aEffect2D[MAX_EFFECT2D];//�e�̏��

//===================================================================================================================
// �G�t�F�N�g2D�̏���������
//===================================================================================================================
void InitEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�̎擾	

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect2d);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2d,
		NULL);

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

	// �G�t�F�N�g�̍ő吔����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		g_aEffect2D[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aEffect2D[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aEffect2D[nCntEffect].nLife = 0;									// ����
		g_aEffect2D[nCntEffect].fRadius = 0.0f;								// ���a
		g_aEffect2D[nCntEffect].fWidth = 0.0f;								// ����
		g_aEffect2D[nCntEffect].fHeight = 0.0f;								// ����
		g_aEffect2D[nCntEffect].DecAlv = 0.0f;								// �A���t�@�l�̌����X�s�[�h
		g_aEffect2D[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_aEffect2D[nCntEffect].bUse = false;								// �g�p���Ă��Ȃ���Ԃɂ���

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�bh
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4�����₷

		g_pVtxBuffEffect2d->Unlock();
	}
}
//===================================================================================================================
// �G�t�F�N�g2D�̏I������
//===================================================================================================================
void UninitEffect2D(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureEffect2d != NULL)
	{
		g_pTextureEffect2d->Release();
		g_pTextureEffect2d = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect2d != NULL)
	{
		g_pVtxBuffEffect2d->Release();
		g_pVtxBuffEffect2d = NULL;
	}

#ifdef _DEBUG

	// �e�N�X�`���̔j�����ł��Ă��邩�m�F
	assert(g_pTextureEffect2d == NULL && "effect2d");

	// ���_�o�b�t�@�̔j�����ł��Ă��邩�m�F
	assert(g_pVtxBuffEffect2d == NULL && "effect2d");

#endif

}
//===================================================================================================================
// �G�t�F�N�g2D�̍X�V����
//===================================================================================================================
void UpdateEffect2D(void)
{
	// ���_�o�b�t�@�̃|�C���^
	VERTEX_2D* pVtx;

	// �G�t�F�N�g�̍ő吔����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		if (g_aEffect2D[nCntEffect].bUse == false) continue;

		// �ʒu�̍X�V
		g_aEffect2D[nCntEffect].pos += g_aEffect2D[nCntEffect].move;

		// �ʒu����
		D3DXVECTOR3 pos = g_aEffect2D[nCntEffect].pos;

		// ���a����
		float fRadius = g_aEffect2D[nCntEffect].fRadius;

		// �A���t�@�l�̌����X�s�[�h
		g_aEffect2D[nCntEffect].DecAlv = 1.0f / g_aEffect2D[nCntEffect].nLife;

		// ���������炷
		g_aEffect2D[nCntEffect].nLife--;

		// �A���t�@�l�����炷
		g_aEffect2D[nCntEffect].col.a -= g_aEffect2D[nCntEffect].DecAlv;

		// �������s������
		if (g_aEffect2D[nCntEffect].nLife <= 0)
		{
			// ��ʂ������
			g_aEffect2D[nCntEffect].bUse = false;
		}

		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEffect;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fRadius,pos.y - fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fRadius,pos.y - fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fRadius,pos.y + fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fRadius,pos.y + fRadius, 0.0f);

		//���_�J���[�̐ݒ�bh
		pVtx[0].col = g_aEffect2D[nCntEffect].col;
		pVtx[1].col = g_aEffect2D[nCntEffect].col;
		pVtx[2].col = g_aEffect2D[nCntEffect].col;
		pVtx[3].col = g_aEffect2D[nCntEffect].col;

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffEffect2d->Unlock();
	}
}
//===================================================================================================================
// �G�t�F�N�g2D�̕`�揈��
//===================================================================================================================
void DrawEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�̏K��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect2d, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//a�u�����f�B���O
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �S�G�t�F�N�g���܍��킷
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		// ���g�p�������珈�����΂�
		if (g_aEffect2D[nCntEffect].bUse == false) continue;
		
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEffect2d);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//�v���~�e�B�u�̎��
		
	}

	//a�u�����f�B���O�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================================================================================================
// �G�t�F�N�g2D�̐ݒ菈��
//===================================================================================================================
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_2D* pVtx;

	g_pVtxBuffEffect2d->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT2D; nCntEffect++)
	{
		// �G�t�F�N�g���g�p����Ă��Ȃ�
		if (g_aEffect2D[nCntEffect].bUse == false)
		{ 
			g_aEffect2D[nCntEffect].pos = pos;			// �ʒu
			g_aEffect2D[nCntEffect].move = move;		// �ړ���
			g_aEffect2D[nCntEffect].col = col;			// �F
			g_aEffect2D[nCntEffect].fRadius = fRadius;	// ���a
			g_aEffect2D[nCntEffect].nLife = nLife;		// ����
			g_aEffect2D[nCntEffect].bUse = true;		// �g�p���

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fRadius, pos.y - fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fRadius, pos.y - fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fRadius, pos.y + fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fRadius, pos.y + fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffEffect2d->Unlock();
}
