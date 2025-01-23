//===================================
//
//�G�t�F�N�g�֘A�̃w�b�_�[
//Author;�����I��
//
//===================================

// �d���m�F�ƃC���N���[�h
#include "main.h"
#ifndef _EFFECT_H_
#define _EFFECT_H_

// �\���́A���S�̏ォ������A�ړ����x�A�傫���A���݂��Ă邩�ۂ�
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 dir;
	D3DXCOLOR col;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nLife;
	float fSpeed;
	float fSize;
	bool bUse;
}Effect;

// �����̐F�X
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 dir, int nLife, D3DXCOLOR col, float fSpeed, float fSize);
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fSpeed, float fSize);

#endif