//================================================================================================================
//
// �p�[�e�B�N�� [ Particle.cpp ]
// Author:�����I��
//
//================================================================================================================

//****************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//****************************************************************************************************************
#include "Particle.h"
#include "main.h"
#include "Effect.h"

//***************************************************************************************************************
// �}�N����`
//***************************************************************************************************************
#define MAX_PARTICLE (128)		// �\������ő吔

//***************************************************************************************************************
// �p�[�e�B�N���̍\����
//***************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	D3DXVECTOR3 move; // �ړ���
	D3DXVECTOR3 randrot; // �����_���Ȋp�x�̕���
	D3DXVECTOR3 rot; // �p�x
	D3DXCOLOR col;	 // �F
	D3DXVECTOR3 Emove; // �G�t�F�N�g�̈ړ�(�P�������̂�)
	float fRadius; // ���a
	float fSpeed;  // �ړ����x
	float fRandR;  // �o������ʒu�̌덷�͈̔�
	int nELife;	   // �G�t�F�N�g�̎���
	int nLife;	   // ����
	int nAmount;   // 1F�ŏo����
	bool bUse;	   // �g�p���Ă��邩�ۂ�
	bool bSimple;  // �V���v�������邩�ۂ�
}particle;

//*********************************************************************************************************************
// �O���[�o���ϐ��錾
//*********************************************************************************************************************
particle g_aParticle[MAX_PARTICLE];

//=============================================================================================================
// �p�[�e�B�N���̏���������
//=============================================================================================================
void InitParticle(void)
{
	//�����퐶��
	//srand(time(NULL));

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �\���̕ϐ��̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // ���W
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // �p�x
		g_aParticle[nCntParticle].randrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �����_���p�x
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // �ړ���
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F
		g_aParticle[nCntParticle].fRadius = 1.0f;	// ���a
		g_aParticle[nCntParticle].fSpeed = 0.0f;	// �ړ����x
		g_aParticle[nCntParticle].nELife = 10;		// �G�t�F�N�g�̎���
		g_aParticle[nCntParticle].nLife = 10;		// ���g�̎���
		g_aParticle[nCntParticle].nAmount = 1;		// 1F�ŏo���p�[�e�B�N���̗�
		g_aParticle[nCntParticle].bUse = false;		// ���g�p����
		g_aParticle[nCntParticle].bSimple = false;	// �V���v������Ȃ�
	}
}
//==========================================================================================================
// �p�[�e�B�N���̏I������
//==========================================================================================================
void UninitParticle(void)
{
	// ����
}
//==========================================================================================================
// �p�[�e�B�N���̍X�V����
//==========================================================================================================
void UpdateParticle(void)
{
	// �u�������╨�ʂ̃J�E���g��
	D3DXVECTOR3 pos; // ���W
	D3DXVECTOR3 move;// �ړ���
	D3DXCOLOR col;	 // �F
	D3DXVECTOR3 rot; // �p�x
	int nCntAppear;

	//float fmove;
	//float fRadius;

	// �S�Č��񂵂�
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// �p�[�e�B�N�����g���Ă���Ȃ�
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

					// �����ʒu����fRandR�̑傫���ɂ��T�ꏊ�Ƀ����_������^���A����Ɛ�������
					col = g_aParticle[nCntParticle].col;
					SetEffect(pos, rot, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}
			else
			{// �����łȂ��V���v���ȏ����ɂ��Ă�Ȃ�

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

					// �����ʒu����fRandR�̑傫���ɂ��ꏊ�Ƀ����_������^���A����Ɛ�������
					col = g_aParticle[nCntParticle].col;
					SetEffect2(pos, g_aParticle[nCntParticle].Emove, g_aParticle[nCntParticle].nELife, col, g_aParticle[nCntParticle].fSpeed * ((float)(rand() % 100) / 100.0f), g_aParticle[nCntParticle].fRadius);
				}
			}

			// �����������Ȃ�Ǝ���
			g_aParticle[nCntParticle].nLife -= 1;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{// �̗͂�0�ȉ��Ȃ�
				// ���g�p����ɂ���
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}
//===========================================================================================================
// �p�[�e�B�N���̕`�揈��
//===========================================================================================================
void DrawParticle(void)
{
	// ����
}
//=================================================================================================================
// �p�[�e�B�N���̐ݒ�(�z�u)����
//=================================================================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove)
{
	// �S�Č��񂵂�
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// �p�[�e�B�N�������g�p��������
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos; // ���W
			g_aParticle[nCntParticle].rot = rot; // �p�x
			g_aParticle[nCntParticle].randrot = randrot; // �p�x�̗���
			g_aParticle[nCntParticle].move = move;	 // �ړ���
			g_aParticle[nCntParticle].col = col;	 // �F
			g_aParticle[nCntParticle].fRadius = fRadius; // ���a
			g_aParticle[nCntParticle].nLife = nLife; // ���g�̎���
			g_aParticle[nCntParticle].nELife = nELife; // �G�t�F�N�g�̎���
			g_aParticle[nCntParticle].nAmount = nAmount; // 1F�ŏo���p�[�e�B�N���̗�
			g_aParticle[nCntParticle].fSpeed = fSpeed; // �ړ����x
			g_aParticle[nCntParticle].fRandR = fRandR; // �ꏊ����
			g_aParticle[nCntParticle].bSimple = bSimple; // �V���v�����ǂ���
			g_aParticle[nCntParticle].Emove = Emove; // �G�t�F�N�g�̈ړ���
			g_aParticle[nCntParticle].bUse = true; // �g�p���

			break;
		}
	}
}
