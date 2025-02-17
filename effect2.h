//============================
//
// �G�t�F�N�g2[effect.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EFFECT2_H_
#define _EFFECT2_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�G�t�F�N�g��ނ̗񋓌^
//****************************
typedef enum
{
	EFFECT_NORMAL = 0,
	EFFECT_SMORK,
	EFFECT_MAX
}EFFECTTEX;

static const char* EFFECT_TEXTURE[EFFECT_MAX] =
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\smoke000.png",
};
//****************************
//�G�t�F�N�g��ނ̗񋓌^
//****************************
typedef enum
{
	EFFECTTYPE_PLAYER=0,
	EFFECTTYPE_BULLET,
	EFFECTTYPE_MAX
}EFFECTTYPE;

//****************************
//�G�t�F�N�g�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR col;//�F
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	float fRadius;
	int nLife;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nType;
	EFFECTTYPE Type;//���
	int EffectType;
	int g_nCounterAnim, g_nPatternAnim;
	float decfAlv;
}EFFECT;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitEffectX(void);//�G�t�F�N�g�̏���������
void UninitEffectX(void);//�G�t�F�N�g�̏I������
void UpdateEffectX(void);//�G�t�F�N�g�̍X�V����
void DrawEffectX(void);//�G�t�F�N�g�̕`�揈��
void SetEffectX(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float decfAlv,int EffectType);//�G�t�F�N�g�̐ݒ菈��
#endif
