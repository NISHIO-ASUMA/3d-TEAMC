//==============================================================================================================
//
// SP�Q�[�W [spgage.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"result.h"
#include"fade.h"
#include"input.h"
#include "time.h"
#include "camera.h"
#include "resultscore.h"
#include "sound.h"
#include "player.h"
#include "Timer.h"
#include"spgauge.h"
#include "meshsword.h"

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSPgauge[SPGAUGE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSPgauge = NULL;       // ���_�o�b�t�@�ւ̃|�C���^
SPgauge g_SPgauge[SPGAUGE_MAX];							// �\���̕ϐ�

//=======================================================================================================
//���U���g�̏���������
//=======================================================================================================
void InitSPgauge(void)
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SP_frame.png",
		&g_pTextureSPgauge[0]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SP_gage.png",
		&g_pTextureSPgauge[4]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha01.png", &g_pTextureSPgauge[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha02.png", &g_pTextureSPgauge[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\SP_frameCha03.png", &g_pTextureSPgauge[3]);

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSPgauge,
		NULL);

	// ���_���b�N
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	// ��ނ�ݒ�
	g_SPgauge[0].nType = SPGAUGE_FRAME0;
	g_SPgauge[1].nType = SPGAUGE_GAUGE;

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		g_SPgauge[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ʒu��������
		g_SPgauge[nCnt].SpGauge = 0.0f;						 // �傫��

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_���b�N����
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
//���U���g�̏I������
//=======================================================================================================
void UninitSPgauge(void)
{
	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSPgauge[nCnt] != NULL)
		{
			g_pTextureSPgauge[nCnt]->Release();
			g_pTextureSPgauge[nCnt] = NULL;
		}
	}
	// �o�b�t�@�̔j��
	if (g_pVtxBuffSPgauge != NULL)
	{
		g_pVtxBuffSPgauge->Release();
		g_pVtxBuffSPgauge = NULL;
	}
}
//=======================================================================================================
//���U���g�̍X�V����
//=======================================================================================================
void UpdateSPgauge(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	pPlayer->SpMode = false; // �t�B�[�o�[���[�h���I�t�ɂ���

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		if (g_SPgauge[nCnt].nType == SPGAUGE_GAUGE)
		{
			// �ő�̒l
			float fDest = g_SPgauge[nCnt].SpGauge / 300.0f;

			// ����
			float fWidth = fDest * 900.0f;

			// 100.0f���܂����甭���\
			if (g_SPgauge[nCnt].SpGauge >= 100.0f)
			{
				pPlayer->SpMode = true; // �����\
			}

			// 300.0f�ȏ�ɂȂ�����
			if (g_SPgauge[nCnt].SpGauge >= 300.0f)
			{
				// 300.0f�ȏ�ɂȂ�Ȃ��悤�ɂ���
				g_SPgauge[nCnt].SpGauge = 300.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 70.0f, 0.0f);
		}
		else
		{
			// �t���[���̗��܂����Q�[�W�̐��l������
			g_SPgauge[nCnt].nType = g_SPgauge[1].SpGauge / 100.0f;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(900.0f, 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(900.0f, 70.0f, 0.0f);

			if (g_SPgauge[1].SpGauge >= 100)
			{
				// �t���[���p�ϐ�
				static int nFrame = 0;

				// �t���[����2�ɂȂ�����
				if (nFrame == 2)
				{
					nFrame = 0; // �t���[����������
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, rand() % 155 + 100, 255, 255);
				}
				else
				{
					// �C���N�������g
					nFrame++;
				}
			}
		}
		pVtx += 4;
	}
	// ���_���b�N����
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
//���U���g�̕`�揈��
//=======================================================================================================
void DrawSPgauge(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSPgauge, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �Q�[�W�o�[�̕`��----------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[1].nType]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2); // �v���~�e�B�u�̎��
	//---------------------------------------------------

	// �Q�[�W�̘g�̕`��-------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSPgauge[g_SPgauge[0].nType]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // �v���~�e�B�u�̎��
	//--------------------------------------------------
}
//=======================================================================================================
// �Q�[�W�̉��Z����
//=======================================================================================================
void AddSpgauge(float fValue)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	if (g_SPgauge[1].SpGauge < 300.0f && !pPlayer->AttackSp)
	{
		// ���Z����
		g_SPgauge[1].SpGauge += fValue;
	}
}
