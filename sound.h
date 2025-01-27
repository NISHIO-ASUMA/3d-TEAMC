//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Asuma Nishio
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************
// �C���N���[�h�t�@�C���錾
//*****************************
#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE_BGM = 0,	// �^�C�g��BGM
	SOUND_LABEL_TUTORIAL_BGM,	// �`���[�g���A��BGM
	SOUND_LABEL_GAME_BGM,		// �Q�[�����C��BGM
	SOUND_LABEL_RESULT_BGM,		// ���U���gBGM
	SOUND_LABEL_RANKING_BGM,	// �����L���OBGM
	SOUND_LABEL_JUMP_SE,		// �W�����vSE
	SOUND_LABEL_SELECT_SE,		// �I���L�[SE
	SOUND_LABEL_ENTER_SE,		// ���艹SE
	SOUND_LABEL_ACTION_SE,		// ��SE
	SOUND_LABEL_BAT_SE,			// �ؐ��o�b�g�n��SE
	SOUND_LABEL_HAMMER_SE,		// ��^����n��SE
	SOUND_LABEL_ITEM_SE,		// �A�C�e���擾SE
	SOUND_LABEL_DAMAGE_SE,		// �_���[�WSE
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
