//=========================================
//
// �����L���O(����) [rankingset.cpp]
// Author: TEAM_C
//
//=========================================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "RankingSet.h"
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"Score.h"

//****************************
//�O���[�o���ϐ�
//****************************
LPDIRECT3DTEXTURE9 g_pTextureRankSet = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankSet = NULL;//���_�o�b�t�@�ւ̃|�C���^

//=============================
//�����L���O(����)�̏���������
//=============================
void InitRankingSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���(����)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\rankingback.png",
		&g_pTextureRankSet);

	//���_�o�b�t�@�̐����E���_���̐ݒ�///����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankSet,
		NULL);

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffRankSet->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffRankSet->Unlock();
}
//=============================
//�����L���O(����)�̏I������
//=============================
void UninitRankingSet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureRankSet != NULL)
	{
		g_pTextureRankSet->Release();
		g_pTextureRankSet = NULL;
	}
	if (g_pVtxBuffRankSet != NULL)
	{
		g_pVtxBuffRankSet->Release();
		g_pVtxBuffRankSet = NULL;
	}
}
//=============================
//�����L���O(����)�̍X�V����
//=============================
void UpdateRankingSet(void)
{
	// ����
}
//=============================
//�����L���O(����)�̕`�揈��
//=============================
void DrawRankingSet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̏K��
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankSet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankSet);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//�v���~�e�B�u�̎��
}
