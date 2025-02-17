//==============================================================================================================
//
// �G�t�F�N�gedit[effectedit.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _EFFECTEDIT_H_
#define _EFFECTEDIT_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"
#include"particle2.h"

//**************************************************************************************************************
// �G�t�F�N�g�̐F�̕ύX�̎��
//**************************************************************************************************************
typedef enum
{
	COLORCHANGE_R = 0,
	COLORCHANGE_G,
	COLORCHANGE_B,
	COLORCHANGE_A,
	COLORCHANGE_MAX
}COLORCHANGE;

//**************************************************************************************************************
// �G�t�F�N�g�̍\����
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 fScal;
	float fRadius;
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
	int nType;
	float fSize;			// �傫��
	float fMove;			// �ړ�
	D3DXVECTOR3 fAngle;		// �A���O��
	int nAngleValue;		// �p�x�͈̔�
	int nNumParticle;		// ���q�̐�
	int Max_Move;			// �ړ���		
	int ColorType;			// �J���[�ύX�̎��
}EFFECTEDIT;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void InitEffectEdit(void);		// �G�t�F�N�g�̏���������
void UninitEffectEdit(void);	// �G�t�F�N�g�̏I������
void UpdateEffectEdit(void);	// �G�t�F�N�g�̍X�V����
void DrawEffectEdit(void);		// �G�t�F�N�g�̕`�揈��
void SaveEffect(void);			// �G�t�F�N�g�̃Z�[�u
void LoadEffect(int nType, D3DXVECTOR3 pos);
SETPARTICLE* GetEditEffect(void);
#endif
