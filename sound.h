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
	SOUND_LABEL_SP_SE,			// SP�U��SE
	SOUND_LABEL_CRAFT,			// ����쐬SE
	SOUND_LABEL_FEVER,			// �t�B�[�o�[�J�nSE
	SPUND_LABEL_WEPONBREAK,     // ����j��SE
	SOUND_LABEL_EVENTSE,        // �C�x���g������
	SOUND_LABEL_AVOIDSE,        // ���SE
	SOUND_LABEL_TUTOCLEARSE,    // �`���[�g���A���N���A��
	SOUND_LABEL_WEPONFIRESE,    // �����퉹
	SOUND_LABEL_NOWEPON,        // ���펝���ĂȂ��Ƃ�
	SOUND_LABEL_WATERWEPON,     // �����퉹
	SOUND_LABEL_SPEARWEPON,     // ���̕��퉹
	SOUND_LABEL_METALWEPON,     // �S�̕��퉹
	SOUND_LABEL_HARISENWEPON,   // �n���Z���̉�
	SOUND_LABEL_TELPHONWEPON,   // �d���̉�
	SOUND_LABEL_MUSICWEPON,     // ���y�n���̉�
	SOUND_LABEL_ICEWEPONSE,     // �X����
	SOUND_LABEL_LIFERECOVERY,   // �񕜉�
	SOUND_LABEL_ITEMTHROW,      // �����A�C�e����
	SOUND_LABEL_FALSEBOSS_SE,   // �{�X���SSE��
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
