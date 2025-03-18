//============================
//
//�G�t�F�N�g
//Author:YOSHIDA YUUTO
//
//
//============================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//�G�t�F�N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXCOLOR col;//�F
	float fRadius;//���a(�傫��)
	float fWidth;
	float fHeight;
	int nLife;//����(�\������)
	bool bUse;//�g�p���Ă��邩�ǂ���
}Effect;

void InitEffect(void);//�G�t�F�N�g�̏���������
void UninitEffect(void);//�G�t�F�N�g�̏I������
void UpdateEffect(void);//�G�t�F�N�g�̍X�V����
void DrawEffect(void);//�G�t�F�N�g�̕`�揈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);
Effect* GetEffect(void);
#endif
