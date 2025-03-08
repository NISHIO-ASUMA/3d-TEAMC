//=====================================================================================================================
//
//HP�Q�[�W�̃v���O����
//Author;�����I��
//
//=====================================================================================================================

//**********************************************************************************************************************
// �C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "player.h"
#include "HPGauge.h"
#include "sound.h"
#include "easing.h"
#include "game.h"

//**********************************************************************************************************************
// �}�N����`
//**********************************************************************************************************************
#define NUM_HPGAUGE (3) // HP�Q�[�W��UI�̐�
#define NUM_FEVERGAUGE (2) // FEVERGAUGE�̐�
#define MAX_HPLENGTH (390.0f) // HP�Q�[�W�̉���
#define FRAME (0)             // �t���[���̃J�E���g

//**********************************************************************************************************************
// �O���[�o���ϐ��錾
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTexture_Gauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_RedGauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Frame = NULL;

LPDIRECT3DTEXTURE9 g_pTexture_Fevgauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Fevframe = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

// �e�Q�[�W�̌v�Z�p�ϐ�
float g_fLength;
float g_fMaxLength;
float g_fPer;
float g_RedLength;
float g_EaseCnt;

// �t�B�[�o�[�Q�[�W�p�ϐ�
float g_fFeverCharge;
bool g_bFeverON;

//===================================================================================================
// �Q�[�W�̏���������
//===================================================================================================
void InitGauge(void)
{
	g_fFeverCharge = 0;
	g_bFeverON = false;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// HP�̃t���[���ƃQ�[�W�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_frame.png", &g_pTexture_Frame);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_red.png", &g_pTexture_RedGauge);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_gage.png", &g_pTexture_Gauge);

	// �t�B�[�o�[�̃t���[���ƃQ�[�W�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fever_frame.png", &g_pTexture_Fevframe);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fever_gage.png", &g_pTexture_Fevgauge);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_HPGAUGE * NUM_FEVERGAUGE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);
	VERTEX_2D* pVtx;

	// �ʒu�̒����A�s���̗ǂ��ꏊ�T���ĉ�����(�ォ��HP�g�AHP�Q�[�W�A�t�B�[�o�[�g�A�t�B�[�o�[�Q�[�W)
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();


	float fLeftHP = (float)pPlayer->nLife;
	float fMaxHP = (float)pPlayer->nMaxLife;

	// �������炻�̊������v�Z�������ɂ���
	float fPer = fLeftHP / fMaxHP;

	float fLength = fPer * MAX_HPLENGTH;

	g_RedLength = fLength;
	g_EaseCnt = 0;

	// HP�Q�[�W����
	for (int nCnt = 0; nCnt < NUM_HPGAUGE; nCnt++)
	{
		// Frame
		if (nCnt == FRAME)
		{
			// FRAME�̒��_���W
			pVtx[0].pos = D3DXVECTOR3(5.0f, 9.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(MAX_HPLENGTH + 5.0f, 9.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(5.0f, 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(MAX_HPLENGTH + 5.0f, 30.0f, 0.0f);
		}
		// gauge
		else if(nCnt != FRAME)
		{
			// GAUGE�̒��_���W
			pVtx[0].pos = D3DXVECTOR3(10.0f, 15.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fLength, 15.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(10.0f, 26.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fLength, 26.0f, 0.0f);
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		pVtx += 4;
	}

	// �t�B�[�o�[�Q�[�W����
	for (int nCnt = 0; nCnt < NUM_FEVERGAUGE; nCnt++)
	{
		// Frame
		if (nCnt == FRAME)
		{
			// FRAME�̒��_���W
			pVtx[0].pos = D3DXVECTOR3(0.0f, 690.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 690.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}
		// gauge
		else if (nCnt != FRAME)
		{
			// GAUGE�̒��_���W
			pVtx[0].pos = D3DXVECTOR3(3.0f, 690.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1270.0f, 690.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(3.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1270.0f, 720.0f, 0.0f);

		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffGauge->Unlock();
}

//===================================================================================================
// Uninit�����F�X
//===================================================================================================
void UninitGauge(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexture_Frame != NULL)
	{
		g_pTexture_Frame->Release();
		g_pTexture_Frame = NULL;
	}
	// �e�N�X�`���̔j��
	if (g_pTexture_Gauge != NULL)
	{
		g_pTexture_Gauge->Release();
		g_pTexture_Gauge = NULL;
	}
	// �e�N�X�`���̔j��
	if (g_pTexture_RedGauge != NULL)
	{
		g_pTexture_RedGauge->Release();
		g_pTexture_RedGauge = NULL;
	}

	// �e�N�X�`���̔j��
	if (g_pTexture_Fevframe != NULL)
	{
		g_pTexture_Fevframe->Release();
		g_pTexture_Fevframe = NULL;
	}
	// �e�N�X�`���̔j��
	if (g_pTexture_Fevgauge != NULL)
	{
		g_pTexture_Fevgauge->Release();
		g_pTexture_Fevgauge = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//===================================================================================================
// �X�V�����F�X
//===================================================================================================
void UpdateGauge(void)
{
	// �c��HP�ƍő�HP�𑪂蔽�f
	float fLeftHP;
	float fMaxHP;

	Player* pPlayer = GetPlayer();

	fLeftHP = (float)pPlayer->nLife;
	fMaxHP = (float)pPlayer->nMaxLife;

	// �������炻�̊������v�Z�������ɂ���
	g_fPer = fLeftHP / fMaxHP;
	g_fLength = g_fPer * MAX_HPLENGTH;

	if (pPlayer->state == PLAYERSTATE_DAMAGE)
	{
		g_EaseCnt = 0;
	}
	else
	{
		g_EaseCnt++;
	}

	// �e�N�X�`�����W�̊���
	static float fDestPer = g_fPer;

	float t = SetEase(g_EaseCnt, 120.0f);

	// �ԃQ�[�W��΃Q�[�W�ɋ߂Â���
	g_RedLength += (g_fLength - g_RedLength) * EaseInCubic(t);

	// �ԃQ�[�W��΃Q�[�W�ɋ߂Â���
	fDestPer += (g_fPer - fDestPer) * EaseInCubic(t);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	VERTEX_2D* pVtx;

	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	// �ԃQ�[�W�̒��_���W�̍X�V
	pVtx[4].pos = D3DXVECTOR3(10.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_RedLength, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(10.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_RedLength, 26.0f, 0.0f);

	// �ԃQ�[�W�e�N�X�`�����W�̍X�V
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(fDestPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(fDestPer, 1.0f);

	// �Q�[�W�̒��_���W�̍X�V
	pVtx[8].pos = D3DXVECTOR3(10.0f, 15.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_fLength, 15.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(10.0f, 26.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_fLength, 26.0f, 0.0f);

	// �Q�[�W�̃e�N�X�`�����W�̍X�V
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(g_fPer, 1.0f);

	pVtx += 12;

	// ��������̓t�B�[�o�[
	
	
	// �������炻�̊������v�Z�������ɂ���A����100�Ń`���[�W����
	g_fPer = g_fFeverCharge / 100.0f;
	g_fLength = g_fPer * 1270;

	// ���_���W�̐ݒ� (�����`�ʂ�����)
	pVtx[4].pos = D3DXVECTOR3(3.0f, 690.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_fLength + 3.0f, 690.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(3.0f, 720.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_fLength + 3.0f, 720.0f, 0.0f);

	// �e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);

	g_pVtxBuffGauge->Unlock();

	GAMESTATE gameState = GetGameState();

	// �t�B�[�o�[�Q�[�W�̏���
	if (g_fFeverCharge >= 100.0f && pPlayer->FeverMode == false)
	{
		g_fFeverCharge = 100.0f;
		PlaySound(SOUND_LABEL_FEVER);
		pPlayer->FeverMode = true;
	}
	else if (pPlayer->FeverMode == true && gameState != GAMESTATE_MOVIE)
	{
		g_fFeverCharge -= 0.2f;
		if (g_fFeverCharge <= 0.0f)
		{
			g_fFeverCharge = 0.0f;
			pPlayer->FeverMode = false;
		}
	}
}

//===================================================================================================
// �`�ʏ����F�X
//===================================================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[����
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexture_Frame);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTexture(0, g_pTexture_RedGauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	pDevice->SetTexture(0, g_pTexture_Gauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

	pDevice->SetTexture(0, g_pTexture_Fevframe);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);

	pDevice->SetTexture(0, g_pTexture_Fevgauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
}

//===================================================================================================
// �ύX�����F�X...�ł��قړ������������v��Ȃ������A�ꉞ�ۊ�
//===================================================================================================
void SetGauge(void)
{
	float fLeftHP;
	float fMaxHP;
	Player* pPlayer = GetPlayer();
	fLeftHP = (float)pPlayer->nLife;
	fMaxHP = (float)pPlayer->nMaxLife;

	g_fPer = fLeftHP / fMaxHP;
	g_fLength = g_fPer * 940;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 26.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);
	g_pVtxBuffGauge->Unlock();
}

void AddFever(float Add)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->FeverMode == false)
	{
		g_fFeverCharge += Add;
	}
}
