//============================
//
// �p�[�e�B�N��2[particle2.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _PARTICLE2_H_
#define _PARTICLE2_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�p�[�e�B�N����ނ̗񋓌^
//****************************
typedef enum
{
	PARTICLETYPE_NORMAL = 0,
	PARTICLETYPE_MAX
}PARTICLETYPE;

//****************************
//�p�[�e�B�N���̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR col;//�F
	int nLife;//����
	int nType;//���
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bLifedecrease; // ���������邩
	float fSize; // �傫��
	int nNumParticle;
	int AngleMax;
	int AngleMin;
	float fSpeed;
	D3DXVECTOR3 dir;
	float decfAlv;
	int EffectTex;
}PARTICLE;

//****************************
//�p�[�e�B�N���̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR col;//�F
	int nLife;//����
	int nType;//���
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bLifedecrease; // ���������邩
	float fSize; // �傫��
	int nNumParticle;
	int ColorType;
	int AngleMax;
	int AngleMin;
	float fSpeed;
	D3DXVECTOR3 dir;
	float decfAlv;
	int EffectTex;
}SETPARTICLE;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitParticleX(void);//�p�[�e�B�N���̏���������
void UninitParticleX(void);//�p�[�e�B�N���̏I������
void UpdateParticleX(void);//�p�[�e�B�N���̍X�V����
void DrawParticleX(void);//�p�[�e�B�N���̕`�揈��
void SetParticleX(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nType, float fSize, int nNumParticle, int AngleMax, int AngleMin, float fSpeed, D3DXVECTOR3 dir, float decfAlv,int EffectTex,bool bLifedecrease);
void SetParticleEdit(SETPARTICLE particle);
#endif
