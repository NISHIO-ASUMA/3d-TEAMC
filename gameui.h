//============================
//
// �Q�[���ɕK�v��UI [gameui.h]
// Author: TEAM_C
//
//============================

#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include"main.h"
#include"title.h"

//****************************
// UI�̎��
//****************************
typedef enum
{
	UITYPE_TITLE = 0,
	UITYPE_TITLE2,
	UITYPE_MAX
}UITYPE;

//****************************
// UI�̃p�X��ۑ�
//****************************
static const char* UITYPE_INFO[UITYPE_MAX] =
{
	"data\\TEXTURE\\title000.png",
	"data\\TEXTURE\\select.png",
};

//****************************
// �Q�[��UI�̍\����
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // �ʒu
	D3DXVECTOR3 move; // �ړ���
	D3DXVECTOR3 rot; // ����
	int nUseTime; // �o�Ă��������܂ł̎���
	int nType; // ���
	float fWidth, fHeight; // ���A����
	bool bUse; // �g�p���
}Gameui;

//****************************
// �v���g�^�C�v�錾
//****************************
void InitGameUI(void);//UI�̏���������
void UninitGameUI(void);//UI�̏I������
void UpdateGameUI(void);//UI�̍X�V����
void DrawGameUI(void);//UI�̕`�揈��
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime);
void FlashGameUI(int nSelect);
#endif