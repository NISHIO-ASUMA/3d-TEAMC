//===================================
//
//HP�Q�[�W�̃v���O����
//Author;�����I��
//
//===================================
#include "Main.h"
#include "player.h"
#include "HPGauge.h"
LPDIRECT3DTEXTURE9 g_pTexture_Gauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Frame = NULL;

LPDIRECT3DTEXTURE9 g_pTexture_Fevgauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Fevframe = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

// �e�Q�[�W�̌v�Z�p�ϐ�
float g_fLength;
float g_fMaxLength;
float g_fPer;

// �t�B�[�o�[�Q�[�W�p�ϐ�
float g_fFeverCharge;
bool g_bFeverON;

//=================
// Init�����F�X
//=================

void InitGauge(void)
{
	g_fFeverCharge = 0;
	g_bFeverON = false;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// HP�̃t���[���ƃQ�[�W�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_frame.png", &g_pTexture_Frame);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_gage.png", &g_pTexture_Gauge);

	// �t�B�[�o�[�̃t���[���ƃQ�[�W�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fever_frame.png", &g_pTexture_Fevframe);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fever_gage.png", &g_pTexture_Fevgauge);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 16, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);
	VERTEX_2D* pVtx;

	// �ʒu�̒����A�s���̗ǂ��ꏊ�T���ĉ�����(�ォ��HP�g�AHP�Q�[�W�A�t�B�[�o�[�g�A�t�B�[�o�[�Q�[�W)
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(5.0f, 9.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(975.0f, 9.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(5.0f, 30.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(975.0f, 30.0f, 0.0f);
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(960.0f, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(960.0f, 26.0f, 0.0f);
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 8;

	pVtx[0].pos = D3DXVECTOR3(5.0f, 69.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(975.0f, 69.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(5.0f, 90.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(975.0f, 90.0f, 0.0f);
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].pos = D3DXVECTOR3(20.0f, 75.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(960.0f, 75.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 86.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(960.0f, 86.0f, 0.0f);
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	g_pVtxBuffGauge->Unlock();
}

//=================
// Uninit�����F�X
//=================
void UninitGauge(void)
{
	if (g_pTexture_Frame != NULL)
	{
		g_pTexture_Frame->Release();
		g_pTexture_Frame = NULL;
	}
	if (g_pTexture_Gauge != NULL)
	{
		g_pTexture_Gauge->Release();
		g_pTexture_Gauge = NULL;
	}
	if (g_pTexture_Fevframe != NULL)
	{
		g_pTexture_Fevframe->Release();
		g_pTexture_Fevframe = NULL;
	}
	if (g_pTexture_Fevgauge != NULL)
	{
		g_pTexture_Fevgauge->Release();
		g_pTexture_Fevgauge = NULL;
	}
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//=================
// �X�V�����F�X
//=================
void UpdateGauge(void)
{
	// �c��HP�ƍő�HP�𑪂蔽�f
	float fLeftHP;
	float fMaxHP;
	Player* pPlayer = GetPlayer();
	fLeftHP = pPlayer->nLife;
	fMaxHP = pPlayer->nMaxLife;

	// �������炻�̊������v�Z�������ɂ���
	g_fPer = fLeftHP / fMaxHP;
	g_fLength = g_fPer * 940;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	// �����`�ʂ�����
	pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 26.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);

	// ��������̓t�B�[�o�[
	pVtx += 8;

	// �������炻�̊������v�Z�������ɂ���A����100�Ń`���[�W����
	g_fPer = g_fFeverCharge / 100.0f;
	g_fLength = g_fPer * 940;

	// �����`�ʂ�����
	pVtx[4].pos = D3DXVECTOR3(20.0f, 75.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 75.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 86.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 86.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);
	g_pVtxBuffGauge->Unlock();

	AddFever(0.1f);

	// �t�B�[�o�[�Q�[�W�̏���
	if (g_fFeverCharge >= 100.0f && g_bFeverON == false)
	{
		g_fFeverCharge = 100.0f;
		g_bFeverON = true;
	}
	else if (g_bFeverON == true)
	{
		if (g_fFeverCharge <= 0.0f)
		{
			g_fFeverCharge = 0.0f;
			g_bFeverON = false;
		}
		g_fFeverCharge -= 0.1f;
	}
}

//=================
// �`�ʏ����F�X
//=================
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
	pDevice->SetTexture(0, g_pTexture_Gauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	pDevice->SetTexture(0, g_pTexture_Fevframe);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
	pDevice->SetTexture(0, g_pTexture_Fevgauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
}

//=================
// �ύX�����F�X...�ł��قړ������������v��Ȃ������A�ꉞ�ۊ�
//=================
void SetGauge(void)
{
	float fLeftHP;
	float fMaxHP;
	Player* pPlayer = GetPlayer();
	fLeftHP = pPlayer->nLife;
	fMaxHP = pPlayer->nMaxLife;

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
	if (g_bFeverON == false)
	{
		g_fFeverCharge += Add;
	}
}