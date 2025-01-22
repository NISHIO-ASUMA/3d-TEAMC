//=================================
//
// ���b�V���t�B�[���h[meshfield.h]
// Author: TEAM_C
//
//=================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
//�}�N����`
//****************************
#define MAX_WIDTH (1000.0f) //����
#define MAX_HEIGHT (1000.0f) //����

#define XVTX (5) //X�̒l
#define ZVTX (5) //y�̒l

#define VERTEX ((XVTX + 1) * (ZVTX + 1)) //���_��
#define INDEX (POLYGON + 2)  //�C���f�b�N�X
#define POLYGON (((XVTX * ZVTX) * 2)) + (4 * (ZVTX - 1)) //�|���S����

//****************************
//���b�V���t�B�[���h�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos;//���b�V���t�B�[���h�̈ʒu
	D3DXVECTOR3 rot;//���b�V���t�B�[���h�̌���
	D3DXMATRIX mtxWorldMeshField;//���b�V���t�B�[���h�̃��[���h�}�g���b�N�X
	D3DXVECTOR3 vtxPos[2];
}MESHFILED;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitMeshField(void);//���b�V���t�B�[���h�̏���������
void UninitMeshField(void);//���b�V���t�B�[���h�̏I������
void UpdateMeshField(void);//���b�V���t�B�[���h�̍X�V����
void DrawMeshField(void);//���b�V���t�B�[���h�̕`�揈��
bool CollisionField(void);
#endif
