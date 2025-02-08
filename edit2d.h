//==============================================================================================================
//
// edit2d [edit2d.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

#ifndef _EDIT2D_H_
#define _EDIT2D_H_

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
// edit�̍\����
//****************************
typedef struct
{
	LPDIRECT3DTEXTURE9 g_pTextureEdit2d;  //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;					  // �ʒu
	D3DXVECTOR3 rot;					  // ��]
	D3DXVECTOR3 move;					  // �ړ���
	D3DXMATRIX mtxWorld;				  // ���[���h�}�g���b�N�X
	float fWidth, fHeight,fLength;		  // ���A����
	int nType;							  // ���
	bool bUse;							  // �g�p���
	float fMove;						  // �ړ���
}EDIT_INFO_2D;

//****************************
// �v���g�^�C�v�錾
//****************************
void InitEdit2d(void);   // �G�f�B�b�g2d�̏���������
void UninitEdit2d(void); // �G�f�B�b�g2d�̏I������
void UpdateEdit2d(void); // �G�f�B�b�g2d�̍X�V����
void DrawEdit2d(void);   // �G�f�B�b�g2d�̕`�揈��
void SaveEdit2d(void);   // �G�f�B�b�g2d�̏����o��
void LoadEdit2d(void);   // �G�f�B�b�g2d�̓ǂݍ���
void ReLoadEdit2d(void); // �G�f�B�b�g2d�̍ēǂݍ���
EDIT_INFO_2D* GetEditInfo2D(void); // �G�f�B�b�g2d�̎擾����
int GetNum2d(void);            // �z�u���擾
#endif
