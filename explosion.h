//============================
//
// ����[explosion.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
// �����̎��
//****************************
typedef enum
{
	EXPLOSION_MOVE = 0,
	EXPLOSION_LANDING,
	EXPLOSION_HIT,
	EXPLOSION_MAX
}EXPLOSIONTYPE;

//****************************
// �����̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	D3DXCOLOR col;//�F
	int nCounterAnim;
	int nPatternAnim;
	int nIdxShadow;//�ԍ�
	int nLife;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
	float speed, radius;
	float fWidth, fHeight;//��,����
	int nType; // ���
}EXPLOSION;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitExplosion(void);//�����̏���������
void UninitExplosion(void);//�����̏I������
void UpdateExplosion(void);//�����̍X�V����
void DrawExplosion(void);//�����̕`�揈��
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col,int nLife,float fWidth,float fHeight,int nType);//�����̐ݒ菈��
#endif
