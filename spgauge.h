//=======================================
//
// SP�Q�[�W���� [spgauge.h]
// Author:YOSHIDA YUTO
//
//=======================================
#ifndef _SPGAUGE_H_
#define _SPGAUGE_H_

//******************************
// �C���N���[�h�t�@�C���錾
//******************************
#include "main.h"

//****************************
// �X�y�V�����Q�[�W�̎��
//****************************
typedef enum
{
	SPGAUGE_FRAME = 0,
	SPGAUGE_GAUGE,
	SPGAUGE_MAX
}SPGAUGE;

//****************************
// �X�y�V�����Q�[�W�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // �ʒu
	int nType; // ���
	float SpGauge;
}SPgauge;

//****************************
// �v���g�^�C�v�錾
//****************************
void InitSPgauge(void);			// �Q�[�W�̏�����
void UninitSPgauge(void);		// �Q�[�W�̏I��
void UpdateSPgauge(void);		// �Q�[�W�̍X�V
void DrawSPgauge(void);			// �Q�[�W�̕`��
void AddSpgauge(float fValue);  // �Q�[�W�̉��Z����
#endif