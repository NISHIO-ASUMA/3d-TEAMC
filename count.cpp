//==============================================================================================================
//
// �J�E���g [count.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "count.h"
#include "mark.h"
#include "game.h"
#include "event.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define ONE_FRAME (60) // 1�t���[��
#define MAX_COUNTER (10) // �J�E���^�[�̍ő吔

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void SetColon(int nCnt,D3DXVECTOR3 pos, float fWidth, float fHeight); // �R�����̐ݒ菈��
void CreateMinute(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight); // ���̍쐬����
void CreateSecond(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight); // �b�̍쐬����
void DrawSetMinute(int nCnt, LPDIRECT3DDEVICE9 pDevice);                      // ���̕`��
void DrawSetSecond(int nCnt, LPDIRECT3DDEVICE9 pDevice);                      // �b�̕`��
void DrawSetColon(int nCnt, LPDIRECT3DDEVICE9 pDevice);                       // �R�����̕`��
void UpdateTextureMinute(int nCnt, int Minute);								  // ���̃e�N�X�`���̍X�V
void UpdateTextureSecond(int nCnt, int Second);								  // �b�̃e�N�X�`���̍X�V
void SetCountDown(int nCnt);												  // �J�E���g�_�E��
void SetCountUp(int nCnt);                                                    // �J�E���g�A�b�v
void SetEventTimer(int nCnt);												  // �C�x���g�̃^�C�}�[�̃J�E���g
void SetTerritoryTimer(int nCnt);											  // �e���g���[�̃^�C�}�[

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCounter = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureColon = NULL;//�e�N�X�`���ւ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCounterColon = NULL; // ���_�o�b�t�@�ւ̃|�C���^
Counter g_Counter[MAX_COUNTER];

//==============================================================================================================
// �J�E���g�̏���������
//==============================================================================================================
void InitCounter(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���̓ǂݍ��݃R����
	D3DXCreateTextureFromFile(pDevice,
		COUNTER_TEXTURE[0],
		&g_pTextureColon);

	// �e�N�X�`���̓ǂݍ��ݕ��A�b
	D3DXCreateTextureFromFile(pDevice,
		COUNTER_TEXTURE[1],
		&g_pTextureCounter);

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COUNTER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCounterColon,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffCounterColon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		g_Counter[nCnt].pos = NULLVECTOR3;
		g_Counter[nCnt].nCountType = COUNTER_COUNTDOWN;
		g_Counter[nCnt].bUse = false;
		g_Counter[nCnt].nMinute = 0;
		g_Counter[nCnt].nSecond = 0;
		g_Counter[nCnt].TimerCnt = 0;
		g_Counter[nCnt].nType = COUNTERTYPE_EVENTTIMER;
		
		//���_���W�̐ݒ�
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

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffCounterColon->Unlock();
}

//==============================================================================================================
// �J�E���g�̏I������
//==============================================================================================================
void UninitCounter(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureColon != NULL)
	{
		g_pTextureColon->Release();
		g_pTextureColon = NULL;
	}

	// �e�N�X�`���̔j��
	if (g_pTextureCounter != NULL)
	{
		g_pTextureCounter->Release();
		g_pTextureCounter = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffCounterColon != NULL)
	{
		g_pVtxBuffCounterColon->Release();
		g_pVtxBuffCounterColon = NULL;
	}

	// �S���̃J�E���^�[����
	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		// ��������
		for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
		{
			// ���_�o�b�t�@�̔j��
			if (g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit] != NULL)
			{
				g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit]->Release();
				g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit] = NULL;
			}
		}
	}
}
//==============================================================================================================
// �J�E���g�̍X�V����
//==============================================================================================================
void UpdateCounter(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	GAMESTATE gamestate = GetGameState();

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		if (g_Counter[nCnt].bUse == false)
		{
			continue;
		}

		// �Q�[����Ԃ����ʂ̎�
		if (gamestate == GAMESTATE_NORMAL)
		{
			g_Counter[nCnt].TimerCnt++; // �^�C�}�[�J�E���g�����Z
		}

		// �J�E���g�̎��
		switch (g_Counter[nCnt].nCountType)
		{
		case COUNTER_COUNTDOWN:
			// �J�E���g�_�E��
			SetCountDown(nCnt);
			break;
		case COUNTER_COUNTUP:
			// �J�E���g�A�b�v
			SetCountUp(nCnt);
			break;
		default:
			break;
		}

		// �^�C�}�[�̎��
		switch (g_Counter[nCnt].nType)
		{
		case COUNTERTYPE_EVENTTIMER:
			SetEventTimer(nCnt);
			break;
		case COUNTERTYPE_TERRITORY:
			SetTerritoryTimer(nCnt);
			break;
		default:
			break;
		}
		// ���̃e�N�X�`���̍X�V
		UpdateTextureMinute(nCnt, g_Counter[nCnt].nMinute);

		// �b�̃e�N�X�`���̍X�V
		UpdateTextureSecond(nCnt, g_Counter[nCnt].nSecond);
	}
}
//==============================================================================================================
// �J�E���g�̕`�揈��
//==============================================================================================================
void DrawCounter(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		if (g_Counter[nCnt].bUse == false)
		{
			continue;
		}

		// ���̕`��ݒ�
		DrawSetMinute(nCnt, pDevice);

		// �b�̕`��ݒ�
		DrawSetSecond(nCnt, pDevice);

		// �R�����̕`��ݒ�
		DrawSetColon(nCnt, pDevice);
	}
}
//==============================================================================================================
// �J�E���g�̐ݒ菈��
//==============================================================================================================
void SetCounter(D3DXVECTOR3 pos, int nCountType, int nTime, float fWidth, float fHeight,int nType)
{
	// �S���̃J�E���^�[��
	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		// ���g�p��������
		if (g_Counter[nCnt].bUse == false)
		{
			g_Counter[nCnt].pos = pos;					// �ʒu����
			g_Counter[nCnt].nCountType = nCountType;    // ��ނ���
			g_Counter[nCnt].nMinute = nTime / 60;		// ������
			g_Counter[nCnt].nSecond = nTime % 60;		// �b����
			g_Counter[nCnt].nType = nType;				// ���
			g_Counter[nCnt].bUse = true;				// �g�p��Ԃɂ���

			// ���̍쐬
			CreateMinute(nCnt, pos, fWidth, fHeight);

			// �R�����̍쐬
			SetColon(nCnt,D3DXVECTOR3(4.0f * fWidth + pos.x, pos.y, pos.z), fWidth * 0.5f, fHeight * 0.75f);

			// �b�̍쐬
			CreateSecond(nCnt, pos, fWidth, fHeight);

			break;
		}
	}
}
//==============================================================================================================
// �R�����̐ݒ菈��
//==============================================================================================================
void SetColon(int nCnt,D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffCounterColon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
	
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffCounterColon->Unlock();
}
//==============================================================================================================
// ���̍쐬����
//==============================================================================================================
void CreateMinute(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_Counter[nCnt].g_pVtxBuffCounter[0],
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Lock(0, 0, (void**)&pVtx, 0);

	// ��������
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);

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

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Unlock();
}
//==============================================================================================================
// �b�̍쐬����
//==============================================================================================================
void CreateSecond(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_Counter[nCnt].g_pVtxBuffCounter[1],
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Lock(0, 0, (void**)&pVtx, 0);

	// ��������
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(6.0f * fWidth + pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(6.0f * fWidth + pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(6.0f * fWidth + pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(6.0f * fWidth + pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);

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

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Unlock();
}
//==============================================================================================================
 // ���̕`��
 //==============================================================================================================
void DrawSetMinute(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_Counter[nCnt].g_pVtxBuffCounter[0], 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ��������
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCounter);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDigit, 2);
	}
}
//==============================================================================================================
 // �b�̕`��
 //==============================================================================================================
void DrawSetSecond(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_Counter[nCnt].g_pVtxBuffCounter[1], 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ��������
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCounter);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDigit, 2);
	}
}
//==============================================================================================================
 // �R�����̕`��
 //==============================================================================================================
void DrawSetColon(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCounterColon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureColon);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
}
//==============================================================================================================
 // ���̃e�N�X�`���̍X�V
 //==============================================================================================================
void UpdateTextureMinute(int nCnt,int Minute)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���̏\�̌�
	int MinTensPlace = Minute / 10;

	// ���̈�̌�
	int MinFirstPlace = Minute % 10;

	// ���_�o�b�t�@�̃��b�N
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f * MinTensPlace, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * MinTensPlace + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * MinTensPlace, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * MinTensPlace + 0.1f, 1.0f);

	// �e�N�X�`���ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.1f * MinFirstPlace, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.1f * MinFirstPlace + 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.1f * MinFirstPlace, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.1f * MinFirstPlace + 0.1f, 1.0f);


	// ���_�o�b�t�@�̃A�����b�N
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Unlock();
}
//==============================================================================================================
 // �b�̃e�N�X�`���̍X�V
 //==============================================================================================================
void UpdateTextureSecond(int nCnt, int Second)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �b�̏\�̌�
	int SecondTensPlace = Second / 10;

	// �b�̈�̌�
	int SecondFirstPlace = Second % 10;

	// ���_�o�b�t�@�̃��b�N
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f * SecondTensPlace, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * SecondTensPlace + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * SecondTensPlace, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * SecondTensPlace + 0.1f, 1.0f);

	// �e�N�X�`���ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.1f * SecondFirstPlace, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.1f * SecondFirstPlace + 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.1f * SecondFirstPlace, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.1f * SecondFirstPlace + 0.1f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Unlock();
}
//==============================================================================================================
// �J�E���g�_�E���̐ݒ�
//==============================================================================================================
void SetCountDown(int nCnt)
{
	// 1�b��������
	if (g_Counter[nCnt].TimerCnt >= 60)
	{
		g_Counter[nCnt].TimerCnt = 0; // �^�C�}�[�J�E���g��������

		// �b��0���傫��������
		if (g_Counter[nCnt].nSecond > 0)
		{
			g_Counter[nCnt].nSecond--; // �b�������炷
		}
		// �b����0��菬�����Ȃ�����
		else if (g_Counter[nCnt].nSecond <= 0 && g_Counter[nCnt].nMinute != 0)
		{
			// �b����59�ɂ���
			g_Counter[nCnt].nSecond = 59;

			// ��������炷
			g_Counter[nCnt].nMinute--;
		}
	}

	// 60�𒴂�����
	if (g_Counter[nCnt].nSecond >= 60)
	{
		g_Counter[nCnt].nMinute++; // ����1���₷

		// �]�肪0����Ȃ�������(60���ゾ������)
		if (g_Counter[nCnt].nSecond % 60 != 0)
		{
			int Time = g_Counter[nCnt].nSecond % 60; // �]������߂�

			g_Counter[nCnt].nSecond = 0; // 0�ɂ���
			g_Counter[nCnt].nSecond += Time; // �]������Z����
		}
		else
		{
			g_Counter[nCnt].nSecond = 0; // 0�ɂ���
		}
	}	
}
//==============================================================================================================
// �J�E���g�A�b�v�̐ݒ�
//==============================================================================================================
void SetCountUp(int nCnt)
{
	// 1�b��������
	if (g_Counter[nCnt].TimerCnt >= 60)
	{
		g_Counter[nCnt].TimerCnt = 0; // �^�C�}�[�J�E���g��������

		// �b��0���傫��������
		if (g_Counter[nCnt].nSecond < 59)
		{
			g_Counter[nCnt].nSecond++; // �b�������炷
		}
		// �b����0��菬�����Ȃ�����
		else if (g_Counter[nCnt].nSecond >= 59 && g_Counter[nCnt].nMinute != 599)
		{
			// �b����59�ɂ���
			g_Counter[nCnt].nSecond = 0;

			// ��������炷
			g_Counter[nCnt].nMinute++;
		}
	}
}
//==============================================================================================================
// �C�x���g�̃^�C�}�[�̃J�E���g
//==============================================================================================================
void SetEventTimer(int nCnt)
{
	if (g_Counter[nCnt].nType == COUNTERTYPE_EVENTTIMER)
	{
		if (EnableEvent() == false)
		{
			g_Counter[nCnt].bUse = false;
		}
	}
}
//==============================================================================================================
// �e���g���[�̃^�C�}�[
//==============================================================================================================
void SetTerritoryTimer(int nCnt)
{
	// �J�E���g���I��������
	if (g_Counter[nCnt].nMinute <= 0 && g_Counter[nCnt].nSecond <= 0)
	{
		// ���g�p�ɃX��
		g_Counter[nCnt].bUse = false;
	}
}
