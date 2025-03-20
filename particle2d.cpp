//===================================================================================================================
//
// �p�[�e�B�N��2d [particle2d.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "particle2d.h"
#include "effect2d.h"
#include "craftui.h"
#include "math.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_PARTICLE2D (30) // �p�[�e�B�N���̍ő吔

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateNormalParticle(int nCntParticle);	// ���ʂ̃p�[�e�B�N���̐ݒ�
void UpdateIconParticle(int nCntParticle);		// �A�C�R���̃p�[�e�B�N���̐ݒ�
void UpdateMixItemParticle(int nCntParticle);	// �N���t�g�������̃p�[�e�B�N��
void UpdateSpModeParticle(int nCntParticle);	// Sp�����̃p�[�e�B�N��

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
Particle2D  g_aParticle2D[MAX_PARTICLE2D];//�e�̏��

//===================================================================================================================
// �p�[�e�B�N��2D�̏���������
//===================================================================================================================
void InitParticle2D(void)
{
	// �p�[�e�B�N���̍ő吔����
	for (int nCnt = 0; nCnt < MAX_PARTICLE2D; nCnt++)
	{
		g_aParticle2D[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aParticle2D[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aParticle2D[nCnt].col = COLOR_WHITE;						// �F
		g_aParticle2D[nCnt].fWidth = 0.0f;							// ����
		g_aParticle2D[nCnt].fHeight = 0.0f;							// ����
		g_aParticle2D[nCnt].fRadius = 0.0f;							// ���a
		g_aParticle2D[nCnt].nLife = 0;								// ����
		g_aParticle2D[nCnt].nType = PARTICLE2D_NORMAL;				// ���
		g_aParticle2D[nCnt].nNumParticle = 0;						// �p�[�e�B�N���̗��q�̐�
		g_aParticle2D[nCnt].speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�[�e�B�N���̈ړ����x
		g_aParticle2D[nCnt].bUse = false;							// �g�p���
	}
}
//===================================================================================================================
// �p�[�e�B�N��2D�̏I������
//===================================================================================================================
void UninitParticle2D(void)
{
}
//===================================================================================================================
// �p�[�e�B�N��2D�̍X�V����
//===================================================================================================================
void UpdateParticle2D(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE2D; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == false) continue;

		//�p�[�e�B�N������
		for (int nCntApper = 0; nCntApper < g_aParticle2D[nCntParticle].nNumParticle; nCntApper++)
		{
			// �m�[�}���p�[�e�B�N���̍X�V
			UpdateNormalParticle(nCntParticle);

			// �A�C�R���̃p�[�e�B�N���̍X�V
			UpdateIconParticle(nCntParticle);

			// �N���t�g�������̃p�[�e�B�N��
			UpdateMixItemParticle(nCntParticle);

			// Sp���[�h�̃p�[�e�B�N��
			UpdateSpModeParticle(nCntParticle);

			g_aParticle2D[nCntParticle].nLife--;

			//�����̐ݒ�
			if (g_aParticle2D[nCntParticle].nLife <= 0)//�������s����
			{
				g_aParticle2D[nCntParticle].bUse = false;
			}
		}
	}
}
//===================================================================================================================
// �p�[�e�B�N��2D�̕`�揈��
//===================================================================================================================
void DrawParticle2D(void)
{
}
//===================================================================================================================
// �p�[�e�B�N��2D�̐ݒ菈��
//===================================================================================================================
void SetParticle2D(D3DXVECTOR3 pos, D3DXVECTOR3 speed, int nLife, D3DXCOLOR col,float fRadius, int nType,int nNumParticle)
{
	// �p�[�e�B�N���̍ő吔����
	for (int nCnt = 0; nCnt < MAX_PARTICLE2D; nCnt++)
	{
		if (g_aParticle2D[nCnt].bUse == false)
		{
			g_aParticle2D[nCnt].pos = pos;						// �ʒu
			g_aParticle2D[nCnt].col = col;						// �F
			g_aParticle2D[nCnt].fRadius = fRadius;				// ���a
			g_aParticle2D[nCnt].nLife = nLife;					// ����
			g_aParticle2D[nCnt].nNumParticle = nNumParticle;	// ���q�̐�
			g_aParticle2D[nCnt].nType = nType;					// ���
			g_aParticle2D[nCnt].speed = speed;					// ����
			g_aParticle2D[nCnt].bUse = true;					// �g�p���

			break;
		}
	}
}
//===================================================================================================================
// ���ʂ̃p�[�e�B�N���̐ݒ�
//===================================================================================================================
void UpdateNormalParticle(int nCntParticle)
{
	// ��ނ��m�[�}������Ȃ�������
	if (g_aParticle2D[nCntParticle].nType != PARTICLE2D_NORMAL) return;

	//�ʒu�̐ݒ�pos
	D3DXVECTOR3 pos = g_aParticle2D[nCntParticle].pos;

	float fAngleX = (float)(rand() % 629 - 314) * 0.01f;
	float fAngleY = (float)(rand() % 629 - 314) * 0.01f;

	// �ړ����x
	D3DXVECTOR3 speed = g_aParticle2D[nCntParticle].speed;

	//�ړ��ʂ̐ݒ�
	g_aParticle2D[nCntParticle].move.x = sinf(fAngleX) * speed.x;
	g_aParticle2D[nCntParticle].move.y = cosf(fAngleY) * speed.y;
	g_aParticle2D[nCntParticle].move.z = 0.0f;

	// �ړ���
	D3DXVECTOR3 move = g_aParticle2D[nCntParticle].move;

	//�F�̐ݒ�col
	D3DXCOLOR col = g_aParticle2D[nCntParticle].col;

	//���a�̐ݒ�fRadius
	float fRadius = ((float)(rand() % (int)g_aParticle2D[nCntParticle].fRadius));

	int nLife = (rand() % g_aParticle2D[nCntParticle].nLife) + 10;

	//�G�t�F�N�g�̐ݒ�SetEffect
	SetEffect2D(pos, move, col, fRadius, nLife);
}
//===================================================================================================================
// �A�C�R���̃p�[�e�B�N���̐ݒ�
//===================================================================================================================
void UpdateIconParticle(int nCntParticle)
{
	// ��ނ��A�C�R���̃p�[�e�B�N������Ȃ�������
	if (g_aParticle2D[nCntParticle].nType != PARTICLE2D_ICON) return;

	//�ʒu�̐ݒ�
	D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
	
	// �p�x�����߂�
	float fAngle = (float)(rand() % 629) * 0.01f;

	// ���������߂�
	float fDistance = (float)(rand() % 300 + 300.0f);

	// �ʒu�̐ݒ�
	pos.x = g_aParticle2D[nCntParticle].pos.x + sinf(fAngle) * fDistance;
	pos.y = g_aParticle2D[nCntParticle].pos.y + cosf(fAngle) * fDistance;

	// �ڕW�܂ł̃x�N�g��
	D3DXVECTOR3 Dest = g_aParticle2D[nCntParticle].pos - pos;

	// ���K������
	D3DXVec3Normalize(&Dest, &Dest);

	// �����x�N�g�����ړ��ʂɑ��
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	move.x = Dest.x * g_aParticle2D[nCntParticle].speed.x;
	move.y = Dest.y * g_aParticle2D[nCntParticle].speed.y;

	// �F�̐ݒ�
	D3DXCOLOR col = g_aParticle2D[nCntParticle].col;

	// ���a�̐ݒ�
	float fRadius = (float)(rand() % (int)g_aParticle2D[nCntParticle].fRadius + 1.0f);

	// �����̐ݒ�
	int nLife = rand() % 60 + g_aParticle2D[nCntParticle].nLife;
	
	//�G�t�F�N�g�̐ݒ�SetEffect
	SetEffect2D(pos, move, col, fRadius, nLife);
}
//===================================================================================================================
// �N���t�g�������̃p�[�e�B�N��
//===================================================================================================================
void UpdateMixItemParticle(int nCntParticle)
{
	// ��ނ��A�C�R���̃p�[�e�B�N������Ȃ�������
	if (g_aParticle2D[nCntParticle].nType != PARTICLE2D_CRAFT) return;

	//�ʒu�̐ݒ�
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

	// �p�x�����߂�
	float fAngle = (float)(rand() % 629) * 0.01f;

	// �ʒu�̐ݒ�
	pos.x = g_aParticle2D[nCntParticle].pos.x;
	pos.y = g_aParticle2D[nCntParticle].pos.y;

	// �ڕW�܂ł̃x�N�g��
	D3DXVECTOR3 Dest = g_aParticle2D[nCntParticle].pos - pos;

	// ���K������
	D3DXVec3Normalize(&Dest, &Dest);

	// �����x�N�g�����ړ��ʂɑ��
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	move.x = sinf(fAngle) * g_aParticle2D[nCntParticle].speed.x;
	move.y = cosf(fAngle) * g_aParticle2D[nCntParticle].speed.y;

	// �F�̐ݒ�
	D3DXCOLOR col = g_aParticle2D[nCntParticle].col;

	// ���a�̐ݒ�
	float fRadius = (float)(rand() % (int)g_aParticle2D[nCntParticle].fRadius + 1.0f);

	// �����̐ݒ�
	int nLife = rand() % 60 + g_aParticle2D[nCntParticle].nLife;

	//�G�t�F�N�g�̐ݒ�SetEffect
	SetEffect2D(pos, move, col, fRadius, nLife);
}
//===================================================================================================================
// Sp�����̃p�[�e�B�N��
//===================================================================================================================
void UpdateSpModeParticle(int nCntParticle)
{
	// ��ނ��A�C�R���̃p�[�e�B�N������Ȃ�������
	if (g_aParticle2D[nCntParticle].nType != PARTICLE2D_SPMODE) return;

	//�ʒu�̐ݒ�
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

	pos.x = g_aParticle2D[nCntParticle].pos.x + (float)(rand() % 10 - 5.0f);	
	pos.y = g_aParticle2D[nCntParticle].pos.y;

	// �ړ���
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);

	move.x = g_aParticle2D[nCntParticle].speed.x;
	move.y = g_aParticle2D[nCntParticle].speed.y;

	// �F�̐ݒ�
	D3DXCOLOR col = g_aParticle2D[nCntParticle].col;

	// ���a�̐ݒ�
	float fRadius = (float)(rand() % (int)g_aParticle2D[nCntParticle].fRadius + 1.0f);

	// �����̐ݒ�
	int nLife = rand() % 60 + g_aParticle2D[nCntParticle].nLife;

	//�G�t�F�N�g�̐ݒ�SetEffect
	SetEffect2D(pos, move, col, fRadius, nLife);

}
