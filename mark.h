//==============================================================================================================
//
// �}�[�N�����@[mark.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MARK_H_
#define _MARK_H_

//***************************
//�C���N���[�h�t�@�C��
//***************************
#include"main.h"

//***************************
//�}�N����`
//***************************
#define NULLVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) // �������p�}�N���ϐ�

//***************************
// �}�[�N�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;							//����
	D3DXVECTOR3 offpos;							//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 offrot;							//����
	D3DXVECTOR3 rotDest;						//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	bool bUse; // �g�p���
}Mark;

//***************************
//�v���g�^�C�v�錾
//***************************
void InitMark(void);
void UninitMark(void);
void UpdateMark(void);
void DrawMark(void);
void SetMark(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
Mark* GetMark(void);
#endif
