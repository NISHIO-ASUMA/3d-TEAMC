//==============================================================================================================
//
// �Q�[���ɕK�v��UI [gameui.h]
// Author: TEAM_C
//
//==============================================================================================================

#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"
#include"title.h"

//**************************************************************************************************************
// UI�̎��
//**************************************************************************************************************
typedef enum
{
	UITYPE_TITLE = 0,
	UITYPE_TITLE2,
	UITYPE_FIVER,
	UITYPE_SYUTYUSEN,
	UITYPE_TUTORIAL,
	UITYPE_BLACK,
	UITYPE_KATANA,
	UITYPE_RED,
	UITYPE_ICONFRAME,
	UITYPE_ICONFRAMESTOCK,
	UITYPE_KEY,
	UITYPE_DESTORY,
	UITYPE_EVENT,
	UITYPE_MAX
}UITYPE;

//**************************************************************************************************************
// UI�̃p�X��ۑ�
//**************************************************************************************************************
static const char* UITYPE_INFO[UITYPE_MAX] =
{
	"data\\TEXTURE\\title_logo.png",
	"data\\TEXTURE\\select.png",
	"data\\TEXTURE\\faver.png",
	"data\\TEXTURE\\syutyu.png",
    "data\\TEXTURE\\UI_enter.png",
	"data\\TEXTURE\\black.png",
	"data\\TEXTURE\\title_katana.png",
	"data\\TEXTURE\\hpred.png",
	"data\\TEXTURE\\icon_frame.png",
	"data\\TEXTURE\\icon_framestoku.png",
	"data\\TEXTURE\\tutoirial005.png",
	"data\\TEXTURE\\ui_weponbreak.png",
	"data\\TEXTURE\\ui_event.png",
};

//**************************************************************************************************************
// �Q�[��UI�̍\����
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ����
	int nUseTime;		// �o�Ă��������܂ł̎���
	int nType;			// ���
	float fWidth, fHeight; // ���A����
	bool bUse;			// �g�p���
	bool bLife;         // ���������邩���Ȃ���
	D3DXCOLOR col;      // �F
	int nEaseCnt;       // �C�[�W���O�̃J�E���g
}Gameui;

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void InitGameUI(void);	 // UI�̏���������
void UninitGameUI(void); // UI�̏I������
void UpdateGameUI(void); // UI�̍X�V����
void DrawGameUI(void);	 // UI�̕`�揈��
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, bool bLife, int nUseTime);
void FlashGameUI(int nSelect);

#endif