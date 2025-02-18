//=====================================================================================================
//
// �p�[�e�B�N��2[particle2.cpp]
// Author:YOSHIDA YUUTO
//
//=====================================================================================================

//*****************************************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************************************
#include "bullet.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "effect2.h"
#include "particle2.h"

//*****************************************************************************************************
//�}�N����`
//*****************************************************************************************************
#define BULLET_WIDTH (25.0f) //����
#define BULLET_HEIGHT (25.0f) //����
#define MAX_PARTICLE (256) //�p�[�e�B�N���̍ő吔
#define MAX_MOVE (5.0f) //�ړ���

//*****************************************************************************************************
//�O���[�o���ϐ��錾
//*****************************************************************************************************
PARTICLE g_aParticle[MAX_PARTICLE];

//=====================================================================================================
//�p�[�e�B�N���̏���������
//=====================================================================================================
void InitParticleX(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		g_aParticle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aParticle[nCnt].nLife = 0;
		g_aParticle[nCnt].nNumParticle = 1;
		g_aParticle[nCnt].nType = 0;
		g_aParticle[nCnt].AngleMax = 628;
		g_aParticle[nCnt].AngleMin = -314;
		g_aParticle[nCnt].fSize = 0.0f;
		g_aParticle[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCnt].bUse = false;
		g_aParticle[nCnt].bLifedecrease = true;
		g_aParticle[nCnt].EffectTex = EFFECT_NORMAL;
	}
}
//=====================================================================================================
//�p�[�e�B�N���̏I������
//=====================================================================================================
void UninitParticleX(void)
{

}
//=====================================================================================================
//�p�[�e�B�N���̍X�V����
//=====================================================================================================
void UpdateParticleX(void)
{
	int nCntParticle, nCntApper;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	D3DXVECTOR3 fScal;
	float fSize;

	Player* pPlayer = GetPlayer();

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{//�g�p���Ă���
			continue;
		}

		//�p�[�e�B�N������
		for (nCntApper = 0; nCntApper < g_aParticle[nCntParticle].nNumParticle/*���������������q�̐�*/; nCntApper++)
		{
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			int MAX = g_aParticle[nCntParticle].AngleMax;
			int MIN = g_aParticle[nCntParticle].AngleMin;

			float fAngleX = (float)(rand() % MAX - MIN) * 0.01f;
			float fAngleY = (float)(rand() % MAX - MIN) * 0.01f;
			float fAngleZ = (float)(rand() % MAX - MIN) * 0.01f;

			float fMoveX = ((float)(rand() % 1) + 1.0f);
			float fMoveY = ((float)(rand() % 1) + 1.0f);
			float fMoveZ = ((float)(rand() % 1) + 1.0f);

			//�ʒu�̐ݒ�pos
			pos.x = g_aParticle[nCntParticle].pos.x /*+ (float)(rand() % 50 - 50.0f)*/;
			pos.y = g_aParticle[nCntParticle].pos.y /*+ (float)(rand() % 50 - 50.0f)*/;
			pos.z = g_aParticle[nCntParticle].pos.z /*+ (float)(rand() % 50 - 50.0f)*/;

			//pos.x += (float)(rand() % 100 - 100.0f);
			//pos.y += (float)(rand() % 100 - 100.0f);
			//pos.z += (float)(rand() % 100 - 100.0f);

			//�ړ��ʂ̐ݒ�
			move.x = cosf(fAngleX) * g_aParticle[nCntParticle].dir.x * fMoveX;
			move.y = sinf(fAngleY) * g_aParticle[nCntParticle].dir.y * fMoveY;
			move.z = cosf(fAngleZ) * g_aParticle[nCntParticle].dir.z * fMoveZ;

			//�F�̐ݒ�col
			col = g_aParticle[nCntParticle].col;

			fSize = g_aParticle[nCntParticle].fSize;

			int nLife = (rand() % g_aParticle[nCntParticle].nLife) + 60;

			/*int nLife = (rand() % 120) + 20;*/
			
			//�G�t�F�N�g�̐ݒ�SetEffect
			SetEffectX(pos, move, col, nLife, fSize,g_aParticle[nCntParticle].decfAlv,g_aParticle[nCntParticle].EffectTex);
		}

		if (g_aParticle[nCntParticle].bLifedecrease)
		{
			g_aParticle[nCntParticle].nLife--;
		}

		//�����̐ݒ�
		if (g_aParticle[nCntParticle].nLife <= 0)//�������s����
		{
			g_aParticle[nCntParticle].bUse = false;
		}
		
	}

}
//=====================================================================================================
//�p�[�e�B�N���̕`�揈��
//=====================================================================================================
void DrawParticleX(void)
{

}
//=====================================================================================================
//�p�[�e�B�N���̕`�揈��
//=====================================================================================================
void SetParticleX(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nType, float fSize, int nNumParticle, int AngleMax, int AngleMin, float fSpeed, D3DXVECTOR3 dir, float decfAlv, int EffectTex, bool bLifedecrease)
{
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == false)
		{
			g_aParticle[nCnt].pos = pos; // �ʒu
			g_aParticle[nCnt].move = move; // �ړ���
			g_aParticle[nCnt].fSize = fSize; // �傫��
			g_aParticle[nCnt].nType = nType; // ���
			g_aParticle[nCnt].col = col; // �F
			g_aParticle[nCnt].nNumParticle = nNumParticle; // �p�[�e�B�N���̗��q�̐�
			g_aParticle[nCnt].nLife = nLife; // ����
			g_aParticle[nCnt].AngleMax = AngleMax; // �A���O���̍ő�
			g_aParticle[nCnt].AngleMin = AngleMin; // �A���O���̍ŏ�
			g_aParticle[nCnt].fSpeed = fSpeed; // �ړ����x
			g_aParticle[nCnt].dir = dir; // �ړ�����
			g_aParticle[nCnt].decfAlv = decfAlv; // ���l�̌����l
			g_aParticle[nCnt].EffectTex = EffectTex; // �G�t�F�N�g�̎��
			g_aParticle[nCnt].bLifedecrease = bLifedecrease; // ���������邩
			g_aParticle[nCnt].bUse = true;

			break;
		}
	}
}
//=====================================================================================================
// �p�[�e�B�N���̐ݒ菈��
//=====================================================================================================
void SetParticleEdit(SETPARTICLE particle)
{
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse == true)
		{
			g_aParticle[nCnt].pos = particle.pos; // �ʒu
			g_aParticle[nCnt].move = particle.move; // �ړ���
			g_aParticle[nCnt].fSize = particle.fSize; // �傫��
			g_aParticle[nCnt].nType = particle.nType; // ���
			g_aParticle[nCnt].col = particle.col; // �F
			g_aParticle[nCnt].nNumParticle = particle.nNumParticle; // �p�[�e�B�N���̗��q�̐�
			g_aParticle[nCnt].nLife = particle.nLife; // ����
			g_aParticle[nCnt].AngleMax = particle.AngleMax; // �A���O���̍ő�
			g_aParticle[nCnt].AngleMin = particle.AngleMin; // �A���O���̍ŏ�
			g_aParticle[nCnt].fSpeed = particle.fSpeed; // �ړ����x
			g_aParticle[nCnt].dir = particle.dir; // �ړ�����
			g_aParticle[nCnt].decfAlv = particle.decfAlv; // ���l�̌����l
			g_aParticle[nCnt].EffectTex = particle.EffectTex; // �G�t�F�N�g�̎��

			g_aParticle[nCnt].bLifedecrease = particle.bLifedecrease; // ���������邩

			break;
		}
	}
}
