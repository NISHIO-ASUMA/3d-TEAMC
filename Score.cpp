//============================
//
// �X�R�A���� [score.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"Score.h"
#include "ranking.h"

//****************************
//�}�N����`
//****************************
#define MAX_NUM_SCORE (8)

//****************************
//�X�R�A�̍\����
//****************************
typedef struct
{
	int nCountScoreCounter;//0�̃J�E���g
	bool bUse;
}SCORE;

//****************************
//�O���[�o���ϐ�
//****************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore=NULL;//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore;//�X�R�A�̈ʒu
int g_nScore;//�X�R�A�̒l
SCORE g_aScore[MAX_NUM_SCORE];

//=====================
//�X�R�A�J�E���^�[
//=====================
int ScoreCounter(int nScore)
{
	int nNum = nScore;//�X�R�A�̂O�̕������J�E���g
	int nKeepNum = 0;//�X�R�A�̌���

	while (nNum != 0)//����؂�Ȃ��Ȃ�܂ŌJ��Ԃ�(�O�܂�)
	{
		nNum /= 10;//nNum��10�Ŋ����Ă���
		nKeepNum++;//���������Z

	}
	if (nScore == 0)
	{
		//1���ڂ�0�������Ă���Ƃ�
		nKeepNum = 1;
	}
	return nKeepNum;//�X�R�A�̌�����Ԃ�
}
//=====================
//�X�R�A�̏���������
//=====================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\adddamage.png",
		&g_pTextureScore);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_NUM_SCORE; nCnt1++)
	{
		g_aScore[nCnt1].nCountScoreCounter = 0;
		g_aScore[nCnt1].bUse = false;
	}
		g_nScore = 0;//�l������������

	for (nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f+(nCntScore*50),0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(50.0f+(nCntScore*50),0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f+(nCntScore*50) ,50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(50.0f+(nCntScore*50),50.0f, 0.0f);
	
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
	g_pVtxBuffScore->Unlock();
}
//=====================
//�X�R�A�̏I������
//=====================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//=====================
//�X�R�A�̍X�V����
//=====================
void UpdateScore(void)
{
	int nData;//�����ۑ��p�̃��[�J���ϐ�
	int nData1 = 0;//���[�J���ϐ�

	//���߂��X�R�A�̌�����ۑ�
	nData = ScoreCounter(g_nScore);

	for (nData1 = 0; nData1 < MAX_NUM_SCORE; nData1++)
	{
		if (nData1 >= MAX_NUM_SCORE - nData)
		{
			//MAX_NUM_SCORE-nData�łO���\������Ă���Ƃ�(nData1��茅���̒l���������Ƃ�)
			g_aScore[nData1].bUse = true;//��̏�����L��������
		}
		else
		{
			//����ȊO�̏�Ԃ̂Ƃ�
			g_aScore[nData1].bUse = false;
		}
	}	
}
//=====================
//�X�R�A�̕`�揈��
//=====================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntScore;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{		
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntScore*4 ,2);//�v���~�e�B�u�̎��
	}
}
//=================
//�X�R�A�̐ݒ菈��
//=================
void SetScore(int nScore)
{
	int aPosTexU[MAX_NUM_SCORE] = {};//�e���̐������i�[
	int aData = 10000000;
	int aData2 = 1000000;

	g_nScore = nScore;

	VERTEX_2D* pVtx;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
		if (nCntScore == 0)//0�Ԗ�
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			//0�ԖڈȊO�̂Ƃ�
			aPosTexU[nCntScore] = g_nScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}

	//�A�����b�N
	g_pVtxBuffScore->Unlock();

}
//=================
//�X�R�A�̉��Z����
//=================
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_NUM_SCORE] = {};//�e���̐������i�[
	int aData = 10000000;
	int aData2 = 1000000;

	g_nScore += nValue;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	for (int nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
		if (nCntScore == 0)//0�Ԗ�
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			//0�ԖڈȊO�̂Ƃ�
			aPosTexU[nCntScore] = g_nScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//�e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}

	//�A�����b�N
	g_pVtxBuffScore->Unlock();
}
//=================
//�X�R�A�̎擾����
//=================
int GetScore(void)
{
	return g_nScore;
}