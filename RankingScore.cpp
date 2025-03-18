//=========================================================================================================================
//
// �����L���O�X�R�A���� [ rankingscore.cpp ]
// Author:TEAM_C
//
//=========================================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "RankingScore.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>
#include "Score.h"
#include "game.h"
#include "mouse.h"
#include "math.h"
#include "easing.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_POLYGON (40) // �ő�|���S����
#define MAX_DIGIT (8)    // ����	
#define MAX_HEIGHT (105.0f) // ����
#define MAX_WIDTH (80.0f)	// ����
#define NUM_DIGITS_8 (10000000) // ����ő包��(8��)
#define NUM_DIGITS_7 (1000000) // ���錅��(7��)

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateRankScoreAnim(void);		// �����L���O�X�R�A�̃A�j���[�V��������
void SkipRankScoreAnim(void);		// �����L���O�X�R�A�̃A�j���[�V�����̃X�L�b�v

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL; // ���_�o�b�t�@�ւ̃|�C���^
RankScore g_RankScore[MAX_RANK];					// �����L���O�X�R�A���
int g_nRankUpdate = -1; // �X�V�����NNo,
int g_nTimerRanking;	// �����L���O��ʕ\���^�C�}�[
int Avalue;
int Rank;
bool bFlash;
int g_nRankingCount;

//==========================================================================================================
// �����L���O�X�R�A�̏�����
//==========================================================================================================
void InitRankingScore(void)
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	VERTEX_2D* pVtx;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score001.png",
		&g_pTextureRankScore);
	
	// �O���[�o���ϐ��̏�����
	g_nTimerRanking = 0;
	Avalue = 0;
	Rank = MAX_RANK - 1;
	bFlash = false;
	g_nRankingCount = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// ���ʐ�����(5)
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// �\���̕ϐ��̏�����
		g_RankScore[nCntRank].pos = D3DXVECTOR3(1400.0f + (nCntRank * 150.0f), 165.0f + (MAX_HEIGHT * nCntRank) + (20.0f * nCntRank), 0.0f);
		g_RankScore[nCntRank].nScore = 0;
		g_RankScore[nCntRank].nEasingCnt = 0;

		// ��������(8)
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[3].pos.z = 0.0f;

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

			// �e�N�X�`���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			// ���_����i�߂�
			pVtx += 4;
		}
	}

	// �A�����b�N
	g_pVtxBuffRankScore->Unlock();

}
//==========================================================================================================
// �����L���O�X�R�A�̏I��
//==========================================================================================================
void UninitRankingScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}
//==========================================================================================================
// �����L���O�X�R�A�̍X�V
//==========================================================================================================
void UpdateRankingScore(void)
{
	// �J�E���g�����Z
	g_nRankingCount++;

	if ((g_nRankingCount >= 600 || KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_A) || OnMouseTriggerDown(LEFT_MOUSE)) && bFlash == true)
	{// 10�b�o�� or Enter�L�[ or A�{�^�� or ���N���b�N
		// �^�C�g����ʂɑJ��
		SetFade(MODE_TITLE);
	}

	// �����L���O�X�R�A�̃A�j���[�V����
	UpdateRankScoreAnim();

	// �����L���O�X�R�A�̃A�j���[�V�����X�L�b�v����
	SkipRankScoreAnim();

	// �X�L�b�v
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(LEFT_MOUSE) == true) && bFlash == false)
	{
		VERTEX_2D* pVtx{};       // ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);


		// ���ʕ���
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// ��������
			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				// �F�����Ƃɖ߂��Ă���
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				pVtx += 4;
			}
		}

		bFlash = true; // �t���b�V���ł���

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffRankScore->Unlock();
	}
	// ���[�J���ϐ�
	static int nCounter{};

	// �t���b�V���ł���
	if (g_nRankUpdate != -1 && bFlash == true)
	{
		nCounter++;   // ���Z

		// �Y���X�R�A��_��
		if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       // ���_���̃|�C���^

			// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				// �������̃J���[��ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);

				// ���_����i�߂�
				pVtx += 4;
			}
			// �A�����b�N
			g_pVtxBuffRankScore->Unlock();
		}
		else if (nCounter == 20)
		{
			VERTEX_2D* pVtx{};       // ���_���̃|�C���^

			// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				// ���_����i�߂�
				pVtx += 4;
			}
			// �A�����b�N
			g_pVtxBuffRankScore->Unlock();

			// �J�E���^�[��������
			nCounter = 0;
		}
	}
}
//==========================================================================================================
// �����L���O�X�R�A�̕`��
//==========================================================================================================
void DrawRankingScore(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_POLYGON; nCntScore++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRankScore);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//=======================================================================================================
// �����L���O�̃��Z�b�g
//=======================================================================================================
void ResetRanking(void)
{
	// �����L���O�X�R�A���̏����ݒ�
	// �t�@�C�����J��
	FILE* pFile;

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
///==================================================================================================================
// �����L���O�̏����o��
//===================================================================================================================
void WriteScore(void)
{
	// �t�@�C���|�C���^��錾
	FILE* pFile;

	// �t�@�C�����J��
	pFile = fopen(RANK000, "w");

	// �t�@�C�����J������
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			// ���ʐ��������o��
			fprintf(pFile, "%d\n", g_RankScore[nCnt].nScore);
		}
	}
	else
	{// �J���Ȃ������Ƃ�
		// ���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[", MB_OK);
		return;
	}

	//�t�@�C������
	fclose(pFile);
}
//======================================================================================================
// �����L���O�̐ݒ�
//======================================================================================================
void SetRanking(int nScore)
{// �����L���O�X�R�A�̕��ёւ����w��̃X�R�A�������N�C��������g_nRankUpdate���X�V
	// �X�R�A�ۑ��p�̕ϐ�
	int nData = {};

	// ���ʂ�������
	g_nRankUpdate = -1;

	//5�Ԗڂ̃X�R�A�Ɣ�r
	if (g_RankScore[MAX_RANK - 1].nScore < nScore)
	{
		g_RankScore[MAX_RANK - 1].nScore = nScore;

		g_nRankUpdate = MAX_RANK - 1;
	}
	
	// ��������
	for (int nCntRank = 0; nCntRank < MAX_DIGIT; nCntRank++)
	{
		// ���ʕ�
		for (int nCntScore = nCntRank + 1; nCntScore < MAX_RANK; nCntScore++)
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
	for (int nCntRankSet = 0; nCntRankSet < MAX_RANK; nCntRankSet++)
	{
		if (nScore == g_RankScore[nCntRankSet].nScore)
		{// ���ʂ̃X�R�A���X�V
			g_nRankUpdate = nCntRankSet;

			break;
		}
	}

	// �X�R�A�����o��
	WriteScore();
}
//=============================================================================================================
// �����L���O�̃e�N�X�`��
//=============================================================================================================
void RankingTexture(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {}; // �e���̐������i�[����z��
	int aData = NUM_DIGITS_8;     // 8���̐��l���擾���邽�߂̏���
	int aData2 = NUM_DIGITS_7;    // 7���ڂ��擾���邽�߂̏���

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// �����L���O�X�R�A�̏���
	for (int nCntScore = 0; nCntScore < MAX_RANK; nCntScore++)
	{
		// �e�X�R�A�������Ƃɕ���
		for (int nCntScore1 = 0; nCntScore1 < MAX_DIGIT; nCntScore1++)
		{
			if (nCntScore1 == 0) // 1���ځi�ŏ�ʌ��j�̏���
			{
				aPosTexU[0] = g_RankScore[nCntScore].nScore / aData; // �ŏ�ʌ����擾
			}
			else
			{
				// 2���ڈȍ~�̏���
				aPosTexU[nCntScore1] = g_RankScore[nCntScore].nScore % aData / aData2; // �Ή����錅���擾
				aData = aData / 10;   // ������10�Ŋ����Ď��̌���
				aData2 = aData2 / 10; // 7���ځA6����...�Ə������邽�ߍX�V
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 1.0f);

			// ���_�o�b�t�@�̃|�C���^�����̈ʒu�֐i�߂�
			pVtx += 4;
		}

		// �����̏������i���̃X�R�A�����̂��߁j
		aData = NUM_DIGITS_8;
		aData2 = NUM_DIGITS_7;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRankScore->Unlock();
}
//=========================================================================================================================
// �����L���O�X�R�A�̃A�j���[�V��������
//=========================================================================================================================
void UpdateRankScoreAnim(void)
{
	if (bFlash == true) return;

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// ���ʕ���
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// �C�[�W���O�̃J�E���g
		g_RankScore[nCntRank].nEasingCnt++;

		//�C�[�W���O�̎���
		float time[MAX_RANK] = {};

		// ���Ԃ�ݒ�
		time[nCntRank] = SetEase(g_RankScore[nCntRank].nEasingCnt,180.0f + (nCntRank * 35.0f));

		// �ړI�̒l�ɋ߂Â���
		g_RankScore[nCntRank].pos.x += SetSmoothAprroach(400.0f, g_RankScore[nCntRank].pos.x, EaseInOutQuad(time[nCntRank]));

		// ��������
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[3].pos.z = 0.0f;

			pVtx += 4;
		}

		// �����L���O�̈ړ����I�������
		if (g_RankScore[nCntRank].pos.x <= 410.0f && nCntRank == MAX_RANK - 1)
		{
			bFlash = true;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRankScore->Unlock();
}
//=========================================================================================================================
// �����L���O�X�R�A�̃A�j���[�V�����̃X�L�b�v
//=========================================================================================================================
void SkipRankScoreAnim(void)
{
	if (bFlash == true) return;

	// �X�L�b�v������
	if ((KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_A)))
	{
		bFlash = true;

		// ���_���̃|�C���^
		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N
		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

		// ���ʕ���
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// �����N�X�R�A�̈ʒu��ړI�̈ʒu�ɂ���
			g_RankScore[nCntRank].pos.x = 400.0f;

			// ��������
			for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
			{
				// ���_���̐ݒ�
				pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
				pVtx[3].pos.z = 0.0f;

				pVtx += 4;
			}
		}

		// ���_�o�b�t�@���A�����b�N
		g_pVtxBuffRankScore->Unlock();
	}
}
