//============================
//
// edit [edit.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _EDIT_H_
#define _EDIT_H_

//***********************
//�C���N���[�h�t�@�C��
//***********************

#include"main.h"
#include"block.h"

//*******************
//�G�f�B�b�g�̎��
//*******************
typedef enum
{
	EDITMODE_BLOCK = 0,
	EDITMODE_WALL,
	EDITMODE_MAX
}EDITMODE;

//*******************
//�G�f�B�b�g�̍\����
//*******************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 Scal;
	bool bUse;
	int nEditMode;
	int ObjCnt;
	int nBlockTypeNumber;
	int nType;
	BLOCKTEX ModelTex[BLOCKTYPE_MAX];
}EDIT_INFO;

//*********************
//�v���g�^�C�v�錾
//*********************
void InitEdit(void);//edit�̏���������
void UninitEdit(void);//edit�̏I������
void UpdateEdit(void);//edit�̍X�V����
void DrawEdit(void);//edit�̐ݒ菈��
void SaveEdit(void);
void LoadEdit(void);
void ReLoadEdit(void);
EDIT_INFO* GetEdit(void);
int GetNumobj(void);
#endif
