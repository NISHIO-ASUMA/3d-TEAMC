//=================================
//
// ���b�V���\�[�h[meshsword.h]
// Author: NAGAO & YOSHIDA
//
//=================================

#ifndef _MESHSWORD_H_
#define _MESHSWORD_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�}�N����`
//****************************
#define SOAD_WIDTH (10.0f) //����
#define SOAD_HEIGHT (10.0f) //����

#define X (8) // X
#define Z (1) // Z

#define ORBIT_VERTEX ((X + 1) * (Z + 1)) //���_��
#define ORBIT_INDEX (ORBIT_VERTEX) // �C���f�b�N�X
#define ORBIT_POLYGON (ORBIT_VERTEX - 2) // �|���S����

//****************************
//���̋O���̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//���̋O���̈ʒu
	D3DXVECTOR3 rot;//���̋O���̌���
	D3DXCOLOR col;//�F
	D3DXMATRIX mtxWorldMeshSword;//���̋O���̃��[���h�}�g���b�N�X
	D3DXVECTOR3 Scal;
	D3DXVECTOR3 oldvtx[ORBIT_VERTEX]; // �ۑ��p
}MESHSOAD;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitMeshSword(void);//���̋O���̏���������
void UninitMeshSword(void);//���̋O���̏I������
void UpdateMeshSword(void);//���̋O���̍X�V����
void DrawMeshSword(void);//���̋O���̕`�揈��
void ResetMeshSword(void);
#endif