//============================
//
// �^�C�g�� [title.h]
// Author: TEAM_C
//
//============================

#ifndef _TITLE_H_
#define _TITLE_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include"main.h"

//****************************
// �I���̎��
//****************************
typedef enum
{
	TITLESELECT_GAME = 0,
	TITLESELECT_TUTO,
	TITLESELECT_MAX
}TITLESELECT;

//****************************
// �^�C�g���̎��
//****************************
typedef enum
{
	TITLETYPE_TITLE = 0,
	TITLETYPE_TUTO,
	TITLETYPE_MAX
}TITLETYPE;

static const char* TITLE_TYPE[TITLETYPE_MAX] =
{
	"data\\TEXTURE\\title002.png",
	"data\\TEXTURE\\title001.png",
};

//****************************
// �^�C�g���̏��
//****************************
typedef enum
{
	TITLESTATE_NORMAL = 0,
	TITLESTATE_FLASH,
	TITLESTATE_MAX
}TITLESTATE;

//****************************
// �^�C�g���̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	D3DXVECTOR3 rot; //����
	D3DXVECTOR3 move; //�ړ���
	float fWidth, fHeight; //���A����
	int nType; //���
	bool bUse; //�g�p���
	int TitleMenu; // �^�C�g��
	int state;
}TITLE;

//****************************
//�v���g�^�C�v�錾
//****************************
void InitTitle(void);//�^�C�g����ʂ̏���������
void UninitTitle(void);//�^�C�g����ʂ̏I������
void UpdateTitle(void);//�^�C�g����ʂ̍X�V����
void DrawTitle(void);//�^�C�g����ʂ̕`�揈��
void SetTitle(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight);//�^�C�g���̐ݒ菈��
#endif
