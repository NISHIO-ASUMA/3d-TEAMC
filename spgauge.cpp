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
// �}�N����`
//**************************************************************************************************************
#define SPGAUGE_LENGTH (390.0f) // �X�y�V�����Q�[�W�̉���

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void SetChargeGage(int nCnt); // �Q�[�W0�̍X�V

//**************************************************************************************************************
// �O���[�o���ϐ�
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
		"data\\TEXTURE\\boss_lifeframe.png",
		&g_pTextureSPgauge[0]);

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SPGAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSPgauge,
		NULL);

	// ���_���b�N
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	g_SPgauge[SPGAUGE_GAUGE_0].col = COLOR_AQUA;
	g_SPgauge[SPGAUGE_GAUGE_1].col = COLOR_YELLOW;
	g_SPgauge[SPGAUGE_GAUGE_2].col = COLOR_RED;

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		g_SPgauge[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ʒu��������
		g_SPgauge[nCnt].SpGauge = 0.0f;						 // �傫��
		g_SPgauge[nCnt].nType = nCnt;						 // ���

		if (nCnt == 1)
		{
			g_SPgauge[nCnt].state = SPGAUGESTATE_CHARGE;
		}
		else
		{
			g_SPgauge[nCnt].state = SPGAUGESTATE_NONE;
		}

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
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

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
		if (g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			SetChargeGage(nCnt);
		}
		
		if (g_SPgauge[nCnt].nType == SPGAUGE_FRAME)
		{
			pVtx += 4 * nCnt;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SPGAUGE_LENGTH, 80.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SPGAUGE_LENGTH, 105.0f, 0.0f);
		}
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

	for (int nCnt = 1; nCnt < SPGAUGE_MAX; nCnt++)
	{
		int nType = g_SPgauge[nCnt].nType;

		if (nType != SPGAUGE_FRAME)
		{
			// �Q�[�W�o�[�̕`��----------------------------------
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
		//---------------------------------------------------
	}

	pDevice->SetTexture(0, g_pTextureSPgauge[0]);

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

	for (int nCnt = 0; nCnt < SPGAUGE_MAX; nCnt++)
	{
		if (g_SPgauge[nCnt].SpGauge <= 100.0f && pPlayer->AttackSp == false && g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			// ���Z����
			g_SPgauge[nCnt].SpGauge += fValue;
		}
	}
}
//=======================================================================================================
// �Q�[�W�̌��Z����
//=======================================================================================================
void DecSpgauge(float fValue)
{
	int NowChargeGage = 0;
	static float Dec = 0.0f;

	for (int nCnt = SPGAUGE_MAX - 1; nCnt >= SPGAUGE_GAUGE_0; nCnt--)
	{
		if (g_SPgauge[nCnt].state == SPGAUGESTATE_CHARGE)
		{
			NowChargeGage = nCnt;
			break;
		}
	}

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	if (NowChargeGage != 1)
	{
		// ���݂̃Q�[�W���猸�点�镪������
		Dec = g_SPgauge[NowChargeGage].SpGauge;
		g_SPgauge[NowChargeGage].SpGauge -= Dec;

		float fRateLength = g_SPgauge[NowChargeGage].SpGauge / 100.0f;
		float fLength = fRateLength * SPGAUGE_LENGTH;

		pVtx += 4 * NowChargeGage;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(10.0f, 125.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fLength, 125.0f, 0.0f);

		float Dec2 = fValue - Dec;

		g_SPgauge[NowChargeGage - 1].SpGauge -= Dec2;

		float fRateLength2 = g_SPgauge[NowChargeGage - 1].SpGauge / 100.0f;
		float fLength2 = fRateLength2 * SPGAUGE_LENGTH;

		pVtx += 4 * (NowChargeGage - 1);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fLength, 105.0f, 0.0f);

	}
	else
	{
		g_SPgauge[NowChargeGage].SpGauge -= fValue;
	}

	// ���_���b�N����
	g_pVtxBuffSPgauge->Unlock();
}
//=======================================================================================================
// �Q�[�W0�̍X�V
//=======================================================================================================
void SetChargeGage(int nCnt)
{
	Player* pPlayer = GetPlayer();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffSPgauge->Lock(0, 0, (void**)&pVtx, 0);

	float fRateLength = g_SPgauge[nCnt].SpGauge / 100.0f;
	float fLength = fRateLength * SPGAUGE_LENGTH;

	pVtx += 4 * nCnt;

	if (g_SPgauge[nCnt].SpGauge >= 100.0f)
	{
		pPlayer->SpMode = true;
	}

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(10.0f, 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fLength, 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(10.0f, 105.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fLength, 105.0f, 0.0f);

	switch (g_SPgauge[nCnt].nType)
	{
	case SPGAUGE_GAUGE_0:

		// �Q�[�W��MAX��������
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_1].state = SPGAUGESTATE_CHARGE;
			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// �Q�[�W���ő傶��Ȃ�������
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;
			g_SPgauge[SPGAUGE_GAUGE_1].state = SPGAUGESTATE_NONE;
		}
		break;
	case SPGAUGE_GAUGE_1:

		// �Q�[�W��MAX��������
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_2].state = SPGAUGESTATE_CHARGE;
			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// �Q�[�W���ő傶��Ȃ�������
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[SPGAUGE_GAUGE_2].state = SPGAUGESTATE_NONE;
		}
		break;
	case SPGAUGE_GAUGE_2:

		// �Q�[�W��MAX��������
		if (g_SPgauge[nCnt].SpGauge >= 100.0f)
		{
			g_SPgauge[nCnt].col.a = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;

			g_SPgauge[nCnt].SpGauge = 100.0f;
		}
		// �Q�[�W���ő傶��Ȃ�������
		else
		{
			//g_SPgauge[nCnt].col.a = 0.5f;
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_SPgauge[nCnt].col;
			pVtx[1].col = g_SPgauge[nCnt].col;
			pVtx[2].col = g_SPgauge[nCnt].col;
			pVtx[3].col = g_SPgauge[nCnt].col;
		}
		break;
	default:
		break;
	}
	// ���_���b�N����
	g_pVtxBuffSPgauge->Unlock();
}
