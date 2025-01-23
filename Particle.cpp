//======================
//
//Author:�����I��
//�p�[�e�B�N���\���̃v���O����
//
//======================

#include "Particle.h"
#include "main.h"
#include "Effect.h"

// �\������ő吔
#define MAX_PARTICLE (128)

// �\���́A�ォ��ꏊ�A�ړ��A�p�x�����_���͈̔́A�p�x�A�F�A�G�t�F�N�g�ړ�(�P�������̂�)
// �傫���A�ړ����x�A�o���ʒu�̌덷�͈́A�G�t�F�N�g�̎����A���g�̎����A1F�ŏo�����ʁA���݂��邩�ۂ��A�V���v�������邩�ۂ�(�V���v������MOVE�Ō���A�����Ŗ����Ȃ�p�x�Ƒ��x�Ō���)
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 randrot;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXVECTOR3 Emove;
	float fRadius;
	float fSpeed;
	float fRandR;
	int nELife;
	int nLife;
	int nAmount;
	bool bUse;
	bool bSimple;
}particle;
particle g_aParticle[MAX_PARTICLE];

//=================
// Init�����F�X
//=================
void InitParticle(void)
{
	//�����퐶��
	//srand(time(NULL));
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	pDevice = GetDevice();
	
	// ������
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].randrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 1.0f;
		g_aParticle[nCntParticle].fSpeed = 0.0f;
		g_aParticle[nCntParticle].nELife = 10;
		g_aParticle[nCntParticle].nLife = 10;
		g_aParticle[nCntParticle].nAmount = 1;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].bSimple = false;
	}
}

//=================
// Uninit�����͋����
//=================
void UninitParticle(void)
{
}

//=================
// �X�V�����̐F�X
//=================
void UpdateParticle(void)
{
	// �u�������╨�ʂ̃J�E���g��
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	D3DXVECTOR3 rot;
	int nCntParticle;
	int nCntAppear;
	float fmove;
	float fRadius;

	// �S�Č��񂵂�
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// �����Ă���
		if (g_aParticle[nCntParticle].bUse == true)
		{
			// ���G�ȏ����Ŏ��s���Ă�Ȃ炱����
			if (g_aParticle[nCntParticle].bSimple == false)
			{
				// ���̃p�[�e�B�N����move�ňړ�������
				g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

				// -1.00 ���� 1.00 �͈̔͂Ń����_���ȕ��������_���𐶐�
				float random_value;

				// ����͈̔́A�p�x�ɏo�������鏈��
				for (nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nAmount; nCntAppear++)
				{
					// �����ʒu����fRandR�̑傫���ɂ��ꏊ�Ƀ����_������^����
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos = g_aParticle[nCntParticle].pos;
					pos.x += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.y += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.z += g_aParticle[nCntParticle].fRandR * random_value;

					// �����ʒu����randrot�̑傫���ɂ��p�x�Ƀ����_������^����
					rot = g_aParticle[nCntParticle].rot;
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.x += g_aParticle[nCntParticle].randrot.x * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.y += g_aParticle[nCntParticle].randrot.y * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					rot.z += g_aParticle[nCntParticle].randrot.z * random_value;

					// �����ʒu����fRandR�̑傫���ɂ��T�C�Y�Ƀ����_������^���A����Ɛ�������
					col = g_aParticle[nCntParticle].col;
					SetEffect(pos, rot, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}
			// �����łȂ��V���v���ȏ����ɂ��Ă�Ȃ�
			else
			{
				// ���̃p�[�e�B�N����move�ňړ�������
				g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

				// �����Z�b�g�ϐ�
				float random_value;

				// ����͈̔͂ɏo�������鏈��
				for (nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nAmount; nCntAppear++)
				{
					// �����ʒu����fRandR�̑傫���ɂ��ꏊ�Ƀ����_������^����
					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos = g_aParticle[nCntParticle].pos;
					pos.x += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.y += g_aParticle[nCntParticle].fRandR * random_value;

					random_value = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
					pos.z += g_aParticle[nCntParticle].fRandR * random_value;

					// �����ʒu����fRandR�̑傫���ɂ��T�C�Y�Ƀ����_������^���A����Ɛ�������
					col = g_aParticle[nCntParticle].col;
					SetEffect2(pos, g_aParticle[nCntParticle].Emove, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}

			// �����������Ȃ�Ǝ���
			g_aParticle[nCntParticle].nLife -= 1;
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//=================
// �`�ʏ����������
//=================
void DrawParticle(void)
{
}

//=================
// �z�u�����F�X
//=================

// ������ʒu�A�p�x�A�p�x�����̑傫���A���g�̈ړ��B�F�A�傫���A�����A�G�t�F�N�g�̎����A1F�̕��ʁA�G�t�F�N�g�ړ����x�A�ꏊ�����̑傫���A�V���v�������邩�ۂ��A(�V���v��������Ȃ�G�t�F�N�g�̈ړ�)
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove)
{
	int nCntParticle;
	//�S�Č��񂵂�
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�����ĂȂ��z�ɌĂт�����
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].randrot = randrot;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].Emove = Emove;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].fRandR = fRandR;
			g_aParticle[nCntParticle].nELife = nELife;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nAmount = nAmount;
			g_aParticle[nCntParticle].fSpeed = fSpeed;
			g_aParticle[nCntParticle].bSimple = bSimple;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}
