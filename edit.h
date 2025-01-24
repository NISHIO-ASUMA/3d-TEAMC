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
#include "item.h"

//*******************
//�}�N����`
//*******************
#define MAX_TYPE (100) // ��ނ̐�

//*******************
//�G�f�B�b�g�̎��
//*******************
typedef enum
{
	EDITMODE_BLOCK = 0,
	EDITMODE_ITEM,
	EDITMODE_MAX
}EDITMODE;


typedef struct
{
	LPD3DXMESH g_pMeshEdit;//���b�V��(���_���W)�ւ̃|�C���^
	LPD3DXBUFFER g_pBuffMatEdit;//�}�e���A���ւ̃|�C���^
	DWORD g_dwNumMatEdit;//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 g_apTextureEdit[MAX_TEX];
	D3DXVECTOR3 vtxMin, vtxMax;
}EditModel;

//*****************************
//�G�f�B�b�g�̃e�N�X�`���\����
//*****************************
typedef struct
{
	EditModel pModel[MAX_TYPE];
	int nNumModel;
}EditTex;

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
	int EditCategory;
	int ObjCnt;
	int nBlockTypeNumber;
	int nType;
	EditTex Category[EDITMODE_MAX];
	float fMove;
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
