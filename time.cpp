//==============================================================================================================
//
// timer [timer.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"time.h"
#include "boss.h"
#include "game.h"
#include "sound.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_NUM_TIME (2) // �^�C�}�[�̍ő吔

//**************************************************************************************************************
// �^�C�}�[�\����
//**************************************************************************************************************
typedef struct
{
	int nCountTimeCounter;//0�̃J�E���g
	bool bUse;
}TIME;

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTimeMinute = NULL;			// �e�N�X�`���ւ̃|�C���^��
LPDIRECT3DTEXTURE9 g_pTextureTimeSecond = NULL;			// �e�N�X�`���ւ̃|�C���^�b
LPDIRECT3DTEXTURE9 g_pTextureTimeCircle = NULL;			// �e�N�X�`���ւ̃|�C���^�R����
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeMinute = NULL;	// ���_�o�b�t�@�ւ̃|�C���^��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeSecond = NULL;	// ���_�o�b�t�@�ւ̃|�C���^�b
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeCircle = NULL;	// ���_�o�b�t�@�ւ̃|�C���^�R����

LPDIRECT3DTEXTURE9 g_pTextureContDown = NULL;			// �e�N�X�`���ւ̃|�C���^�R����
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffContDown = NULL;	// ���_�o�b�t�@�ւ̃|�C���^��

D3DXVECTOR3 g_posTime; // �X�R�A�̈ʒu
int g_nCountTime;      // �^�C���̃J�E���g

int g_nTimerMinute;    // ��
int g_nTimerSecond;    // �b

TIME g_aTime[MAX_NUM_TIME]; // �ő吔��

//=======================================================================================================
//�X�R�A�̏���������
//=======================================================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�̎擾

	VERTEX_2D* pVtx; // ���_�o�b�t�@�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���(��)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeMinute);

	//�e�N�X�`���̓ǂݍ���(�b)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeSecond);

	//�e�N�X�`���̓ǂݍ���(�R����)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\timer_circle.png",
		&g_pTextureTimeCircle);

	//�e�N�X�`���̓ǂݍ���(�R����)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureContDown);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeMinute, // ��
		NULL);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeSecond, // �b
		NULL);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeCircle, // �R����
		NULL);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffContDown, // �R����
		NULL);

	// ���̃��b�N
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

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffTimeCircle->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� (�R����)
	pVtx[0].pos = D3DXVECTOR3(1090.0f,20.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(1110.0f,20.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(1090.0f,55.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(1110.0f,55.0f,0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimeCircle->Unlock();

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffContDown->Lock(0, 0, (void**)&pVtx, 0);

	// �b�̕���
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
	g_pVtxBuffContDown->Unlock();

}
//=======================================================================================================
//�X�R�A�̏I������
//=======================================================================================================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimeMinute != NULL)
	{
		g_pTextureTimeMinute->Release();
		g_pTextureTimeMinute = NULL;
	}

	// ���_�o�b�t�@�̔j��
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
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimeSecond != NULL)
	{
		g_pVtxBuffTimeSecond->Release();
		g_pVtxBuffTimeSecond = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureTimeCircle != NULL)
	{
		g_pTextureTimeCircle->Release();
		g_pTextureTimeCircle = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimeCircle != NULL)
	{
		g_pVtxBuffTimeCircle->Release();
		g_pVtxBuffTimeCircle = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureContDown != NULL)
	{
		g_pTextureContDown->Release();
		g_pTextureContDown = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffContDown != NULL)
	{
		g_pVtxBuffContDown->Release();
		g_pVtxBuffContDown = NULL;
	}

}
//=======================================================================================================
//�X�R�A�̍X�V����
//=======================================================================================================
void UpdateTime(void)
{
	VERTEX_2D* pVtx;

	int Min10 = g_nTimerMinute / 10;	  // ���̏\�̌�
	int Min1 = g_nTimerMinute % 10;		  // ���̈�̌�

	int Second10 = g_nTimerSecond / 10;	  // �b�̏\�̌�
	int Second1 = g_nTimerSecond % 10;	  // �b�̏\�̌�

	float offpos = 0.1f;				  // �e�N�X�`�����W�̃I�t�Z�b�g

	GAMESTATE gamestate = GetGameState(); // �Q�[���̏�Ԃ��擾

	// �Q�[���������Ă���Ȃ�
	if (gamestate != GAMESTATE_END)
	{
		g_nCountTime++; // �^�C�}�[�J�E���g�����Z

		// 1�b��������
		if (g_nCountTime >= 60)
		{
			g_nCountTime = 0; // �^�C�}�[�J�E���g��������

			// �b��0���傫��������
			if (g_nTimerSecond > 0)
			{
				g_nTimerSecond--; // �b�������炷
			}
			// �b����0��菬�����Ȃ�����
			else if (g_nTimerSecond <= 0)
			{
				// �b����59�ɂ���
				g_nTimerSecond = 59;

				// ��������炷
				g_nTimerMinute--;
			}

			// �c��10�b�ȉ��Ȃ�
			if (g_nTimerMinute <= 0 && g_nTimerSecond <= 10)
			{
				// ���y�Đ�(��)
				PlaySound(SOUND_LABEL_SELECT_SE);
			}
		}

		// 60�𒴂�����
		if (g_nTimerSecond >= 60)
		{
			g_nTimerMinute++; // ����1���₷

			// �]�肪0����Ȃ�������(60���ゾ������)
			if (g_nTimerSecond % 60 != 0)
			{
				int Time = g_nTimerSecond % 60; // �]������߂�

				g_nTimerSecond = 0; // 0�ɂ���
				g_nTimerSecond += Time; // �]������Z����
			}
			else
			{
				g_nTimerSecond = 0; // 0�ɂ���
			}
		}
	}

	// ���̃o�b�t�@�����b�N
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	// �c��10�b����Ȃ�������

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffTimeSecond->Unlock();

	// �J�E���g�_�E��
	g_pVtxBuffContDown->Lock(0, 0, (void**)&pVtx, 0);

	// 10�b�ȉ��ɂȂ�����
	if (g_nTimerSecond <= 10 && g_nTimerMinute <= 0)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(400.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f, 400.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);

		//���_���W�̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(400.0f + 250.0f, 200.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(600.0f + 250.0f, 200.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(400.0f + 250.0f, 400.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(600.0f + 250.0f, 400.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 50);

		//���_�J���[�̐ݒ�
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 50);

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
	}
	else
	{
		// �b�̕���
		for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			pVtx += 4;
		}
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffContDown->Unlock();

}
//=======================================================================================================
//�X�R�A�̕`�揈��
//=======================================================================================================
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

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeSecond, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
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

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeCircle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimeCircle);

	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//�v���~�e�B�u�̎��


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffContDown, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureContDown);

			//�v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//�v���~�e�B�u�̎��
		}
	}


}
//=====================================================================================================================================================================
// �^�C�}�[�̎擾����
//=====================================================================================================================================================================
int GetTimeMinute(void)
{
	return g_nTimerMinute; // ���̎擾
}
//=====================================================================================================================================================================
// �^�C�}�[�̎擾����
//=====================================================================================================================================================================
int GetTimeSecond(void)
{
	return g_nTimerSecond;
}
//=====================================================================================================================================================================
// �^�C�}�[(��)�̉��Z����
//=====================================================================================================================================================================
void AddTimeMinute(int minute)
{
	g_nTimerMinute += minute;
}
//=====================================================================================================================================================================
// �^�C�}�[(�b)�̉��Z����
//=====================================================================================================================================================================
void AddTimeSecond(int second)
{
	g_nTimerSecond += second;
}
