//============================
//
// �����L���O�X�R�A [rankingscore.cpp]
// Author:TEAM_C
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "RankingScore.h"
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"Score.h"
#include "game.h"

//****************************
//�}�N����`
//****************************
#define MAX_POLYGON (40)//�ő�|���S����
#define MAX_DIGIT (8)//����	
#define MAX_HEIGHT (69.0f)
#define MAX_HABA (50.0f)

//****************************
//�O���[�o���ϐ�
//****************************
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;//�e�N�X�`���ւ̃|�C���^//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL;//���_�o�b�t�@�ւ̃|�C���^
RankScore g_RankScore[MAX_RANK];//�����L���O�X�R�A���
END g_End;
int g_nRankUpdate = -1;//�X�V�����NNo,
int g_nTimerRanking;//�����L���O��ʕ\���^�C�}�[
int g_state;
//========================
//�����L���O�X�R�A�̏�����
//========================
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���(�X�R�A)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score000.png",
		&g_pTextureRankScore);
	
	//������
	//g_nRankUpdate = -1;
	g_nTimerRanking = 0;
	g_state = 0;

	//���_�o�b�t�@�̐����E���_���̐ݒ�//�X�R�A
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);


	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	//���ʐ�����(5)
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		g_RankScore[nCntRank].pos = D3DXVECTOR3(500.0f, 150.0f + (MAX_HEIGHT * nCntRank) + (20.0f * nCntRank), 0.0f);
		g_RankScore[nCntRank].nScore = 0;

		//��������(8)
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_HABA * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_HABA * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_HABA * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_HABA * 0.5f;
			pVtx[3].pos.z = 0.0f;

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffRankScore->Unlock();

}
//========================
//�����L���O�X�R�A�̏I��
//========================
void UninitRankingScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}
//========================
//�����L���O�X�R�A�̍X�V
//========================
void UpdateRankingScore(void)
{
	static int nCounter{};

	if (g_nRankUpdate != -1)
	{
		nCounter++;   //���Z

		//�Y���X�R�A��_��
		if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       //���_���̃|�C���^

			//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx += 4;
			}
			g_pVtxBuffRankScore->Unlock();
		}
		else if (nCounter == 20)
		{
			VERTEX_2D* pVtx{};       //���_���̃|�C���^

			//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx += 4;
			}

			g_pVtxBuffRankScore->Unlock();

			nCounter = 0;
		}

	}
	////�Y���̃����L���O�X�R�A��_�Ł�
	//if (g_nRankUpdate != -1)
	//{
	//	//���_�J���[
	//}

	//g_nTimerRanking++;//�^�C�}�[���Z

	if (g_nTimerRanking >= 300 || KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		SetFade(MODE_TITLE);
	}

	//MODE Mode = GetMode();


	//g_End.GameState = GAMEEND_STAGE1;

}
//========================
//�����L���O�X�R�A�̕`��
//========================
void DrawRankingScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̏K��
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_POLYGON; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRankScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);//�v���~�e�B�u�̎��
	}
}
//=====================
//�����L���O�̃��Z�b�g
//=====================
void ResetRanking(void)
{
	//�����L���O�X�R�A���̏����ݒ�
	//�t�@�C�����J��
	FILE* pFile;

	//MODE Mode = GetMode();

	pFile = fopen(RANK000, "r");

	//�t�@�C���J������
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &g_RankScore[nCnt].nScore);
		}
	}
	else
	{//�J���Ȃ������Ƃ�
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(RankingScore.cpp)", MB_OK);
		return;
	}
	//�t�@�C������
	fclose(pFile);

}
///================================
//�����L���O�̏����o��
//=================================
void WriteScore(void)
{
	FILE* pFile;

	MODE Mode = GetMode();

	//�t�@�C�����J��
	pFile = fopen(RANK000, "w");

	//�t�@�C�����J������
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fprintf(pFile, "%d\n", g_RankScore[nCnt].nScore);
		}
	}
	else
	{//�J���Ȃ������Ƃ�
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[", MB_OK);
		return;
	}
	//�t�@�C������
	fclose(pFile);
}

//====================
//�����L���O�̐ݒ�
//====================
void SetRanking(int nScore)
{
	int nCntRank;

	int nData = {};

	int nCntScore;
	//�����L���O�X�R�A�̕��ёւ����w��̃X�R�A�������N�C�������� g_nRankUpdate���X�V

	//5�Ԗڂ̃X�R�A�Ɣ�r
	if (g_RankScore[MAX_RANK - 1].nScore < nScore)
	{
		g_RankScore[MAX_RANK - 1].nScore = nScore;

		g_nRankUpdate = MAX_RANK - 1;
	}

	for (nCntRank = 0; nCntRank < MAX_DIGIT; nCntRank++)
	{
		for (nCntScore = nCntRank + 1; nCntScore < MAX_RANK; nCntScore++)
		{
			if (g_RankScore[nCntRank].nScore <= g_RankScore[nCntScore].nScore)
			{
				//�㏑���O�̃X�R�A��ۑ�
				nData = g_RankScore[nCntRank].nScore;

				//�X�R�A���㏑��
				g_RankScore[nCntRank].nScore = g_RankScore[nCntScore].nScore;

				//�㏑�������X�R�A��ۑ�
				g_RankScore[nCntScore].nScore = nData;
			}
		}
	}
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nScore == g_RankScore[nCntRank].nScore)
		{
			g_nRankUpdate = nCntRank;

			break;
		}
	}
	//�X�R�A�����o��
	WriteScore();
}
//===========================
//�����L���O�̃e�N�X�`��
//===========================
void RankingTexture(void)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};//�e���̐������i�[
	int aData = 10000000;
	int aData2 = 1000000;

	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RANK; nCntScore++)
	{
		for (int nCntScore1 = 0; nCntScore1 < MAX_DIGIT; nCntScore1++)
		{
			if (nCntScore1 == 0)//�O�Ԗ�
			{
				aPosTexU[0] = g_RankScore[nCntScore].nScore / aData;
			}
			else
			{
				//0�ԖڈȊO�̂Ƃ�
				aPosTexU[nCntScore1] = g_RankScore[nCntScore].nScore % aData / aData2;
				aData = aData / 10;
				aData2 = aData2 / 10;
			}
			//�e�N�X�`���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 1.0f);

			pVtx += 4;

		}
		aData = 10000000;
		aData2 = 1000000;
	}
	
	g_pVtxBuffRankScore->Unlock();
}
//===========================
//�I������̎擾
//===========================
END* GetStageEnd(void)
{
	return &g_End;
}