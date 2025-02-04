//============================
//
// timer [timer.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include"time.h"

//****************************
// �}�N����`
//****************************
#define MAX_NUM_TIME (2) // �^�C�}�[�̍ő吔

//****************************
// �^�C�}�[�\����
//****************************
typedef struct
{
	int nCountTimeCounter;//0�̃J�E���g
	bool bUse;
}TIME;

//****************************
// �O���[�o���ϐ�
//****************************
LPDIRECT3DTEXTURE9 g_pTextureTimeMinute = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimeSecond = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeMinute = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeSecond = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posTime; // �X�R�A�̈ʒu
int g_nCountTime;      // �^�C���̃J�E���g

int g_nTimerMinute;    // ��
int g_nTimerSecond;    // �b

TIME g_aTime[MAX_NUM_TIME]; // �ő吔��

//=====================
//�X�R�A�̏���������
//=====================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeMinute);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeSecond);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeMinute,
		NULL);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeSecond,
		NULL);

	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_NUM_TIME; nCnt1++)
	{
		g_aTime[nCnt1].nCountTimeCounter = 0;
		g_aTime[nCnt1].bUse = true;
	}
	g_nTimerMinute = 1; // ����������
	g_nTimerSecond = 40; // �b��������
	g_nCountTime = 0;   // �^�C���̃J�E���g��������

	// ���̒��_���W
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f + 30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f + 30.0f, 60.0f, 0.0f);
				

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimeMinute->Unlock();

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	// �b�̕���
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f + 30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f + 30.0f, 60.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffTimeSecond->Unlock();

}
//=====================
//�X�R�A�̏I������
//=====================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimeMinute != NULL)
	{
		g_pTextureTimeMinute->Release();
		g_pTextureTimeMinute = NULL;
	}
	if (g_pVtxBuffTimeMinute != NULL)
	{
		g_pVtxBuffTimeMinute->Release();
		g_pVtxBuffTimeMinute = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureTimeSecond != NULL)
	{
		g_pTextureTimeSecond->Release();
		g_pTextureTimeSecond = NULL;
	}
	if (g_pVtxBuffTimeSecond != NULL)
	{
		g_pVtxBuffTimeSecond->Release();
		g_pVtxBuffTimeSecond = NULL;
	}

}
//=====================
//�X�R�A�̍X�V����
//=====================
void UpdateTime(void)
{
	VERTEX_2D* pVtx;

	int Min10 = g_nTimerMinute / 10;
	int Min1 = g_nTimerMinute % 10;

	int Second10 = g_nTimerSecond / 10;
	int Second1 = g_nTimerSecond % 10;

	float offpos = 0.1f;

	g_nCountTime++;

	if (g_nCountTime >= 60)
	{
		g_nCountTime = 0;

		if (g_nTimerSecond > 0)
		{
			g_nTimerSecond--;
		}
		else if(g_nTimerSecond <= 0)
		{
			g_nTimerSecond = 59;
			g_nTimerMinute--;
		}
	}
	
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(offpos * Min10,0.0f);
	pVtx[1].tex = D3DXVECTOR2(offpos * Min10 + offpos,0.0f);
	pVtx[2].tex = D3DXVECTOR2(offpos * Min10,1.0f);
	pVtx[3].tex = D3DXVECTOR2(offpos * Min10 + offpos,1.0f);
	
	//�e�N�X�`���̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(offpos * Min1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(offpos * Min1 + offpos, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(offpos * Min1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(offpos * Min1 + offpos, 1.0f);

	g_pVtxBuffTimeMinute->Unlock();

	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(offpos * Second10, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(offpos * Second10 + offpos, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(offpos * Second10, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(offpos * Second10 + offpos, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(offpos * Second1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(offpos * Second1 + offpos, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(offpos * Second1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(offpos * Second1 + offpos, 1.0f);

	g_pVtxBuffTimeSecond->Unlock();

}
//=====================
//�X�R�A�̕`�揈��
//=====================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̏K��
	pDevice = GetDevice();

	int nCntTime;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeMinute, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTimeMinute);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//�v���~�e�B�u�̎��
		}
	}

	pDevice->SetStreamSource(0, g_pVtxBuffTimeSecond, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTimeSecond);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//�v���~�e�B�u�̎��
		}
	}

}
//=================
// �^�C�}�[�̎擾����
//=================
int GetTimeMinute(void)
{
	return g_nTimerMinute;
}
//=================
// �^�C�}�[�̎擾����
//=================
int GetTimeSecond(void)
{
	return g_nTimerSecond;
}