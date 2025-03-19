//==============================================================================================================
//
// �Q�[���ɕK�v��UI [gameui.h]
// Author: Asuma Nishio
//
//==============================================================================================================

#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// UI�̎��
//**************************************************************************************************************
typedef enum
{
	UITYPE_TITLE = 0,	// �^�C�g����UI
	UITYPE_TITLE2,		// �^�C�g����UI
	UITYPE_FIVER,		// Fever��UI
	UITYPE_SYUTYUSEN,	// �W����
	UITYPE_TUTORIAL,	// �`���[�g���A��
	UITYPE_BLACK,		// sp���̎��̉�ʂ̈Ó]
	UITYPE_KATANA,		// �^�C�g���̓�
	UITYPE_RED,			
	UITYPE_ICONFRAME,	// �����Ă���A�C�e���A�C�R���̃t���[��
	UITYPE_ICONFRAMESTOCK,	// �X�g�b�N�A�C�R���̃t���[��
	UITYPE_KEY,				// �K�E�Z�̃L�[
	UITYPE_DESTORY,			// ���킪��ꂽ����UI
	UITYPE_EVENT,			// �C�x���g����
	UITYPE_SP,				// �X�y�V���������̃L�[
	UITYPE_DAMAGE,			// �_���[�W�󂯂����̉��
	UITYPE_SETENEMYTIME,	// �G���o������UI
	UITYPE_BOSSMANUALEXIT,	// �{�X�̃}�j���A����UI
	UITYPE_POPENEMY, // �G�o��
	UITYPE_CRAFTTIME, // �N���t�g�̎��Ԃɏo��UI
	UITYPE_STOCKKEY,	// �A�C�e�����X�g�b�N����L�[
	UITYPE_CRAFTTIMEMENU,	// �N���t�g�^�C���ɂȂ�����o��UI
	UITYPE_SPINFO,		// SP�Q�[�W�̉��UI
	UITYPE_SPONBREAK,	// �X�|�[���j��UI
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
	"data\\TEXTURE\\icon_frame_1.png",
	"data\\TEXTURE\\icon_framestoku.png",
	"data\\TEXTURE\\tutoirial005.png",
	"data\\TEXTURE\\ui_weponbreak.png",
	"data\\TEXTURE\\ui_event_1.png",
	"data\\TEXTURE\\SP_Key_1.png",
	"data\\TEXTURE\\damage001.png",
	"data\\TEXTURE\\settime.png",
	"data\\TEXTURE\\bossmanualexit.png",
	"data\\TEXTURE\\ui_popenemy.png",
	"data\\TEXTURE\\ui_crafttime.png",
	"data\\TEXTURE\\ui_stock_key.png",
	"data\\TEXTURE\\ui_Manual_Craft.jpg",
	"data\\TEXTURE\\ui_sp_info.png",
	"data\\TEXTURE\\ui_Break_suponer.png",
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
	int nCounterAnim, nPatternAnim; // �e�N�X�`���A�j���[�V����
	float fAlv;						// �����x
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