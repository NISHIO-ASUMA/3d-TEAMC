//==============================================================================================================
//
// �C�x���g [event.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "event.h"
#include "mark.h"
#include "meshcylinder.h"
#include "boss.h"
#include "game.h"
#include "gameui.h"
#include "count.h"
#include "sound.h"
#include "time.h"
#include "camera.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define START_EVENTTIME (9999) // �C�x���g���n�܂�܂ł̎���
#define POSITIONONE_RADIUS (700.0f) // 0�Ԗڂ̃C�x���g�̔��a

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
Event g_Event[EVENTPOSITION_MAX]; // �C�x���g�̏��
int g_nStartTime = 0;
bool bEvent = false;
int g_EventPos = 0;

//==============================================================================================================
// �C�x���g�̏���������
//==============================================================================================================
void InitEvent(void)
{
	// �C�x���g�̏ꏊ����
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		g_Event[nCnt].IventPosition = nCnt;   // �C�x���g�̏ꏊ
		g_Event[nCnt].nTime = 0;              // �C�x���g�̎���
		g_Event[nCnt].bEvent = false;         // �C�x���g���������Ă��邩
	}

	// �C�x���g���n�܂�܂ł̎���
	g_nStartTime = 0;
	bEvent = false;
	g_EventPos = 0;
}

//==============================================================================================================
// �C�x���g�̐ݒ菈��
//==============================================================================================================
void SetEvent(D3DXVECTOR3 Eventpos, int IventPosition, int nTime)
{
	// �C�x���g�̏ꏊ����
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// ���g�p��������
		if (g_Event[nCnt].bEvent == false)
		{
			g_Event[nCnt].Eventpos = Eventpos; // �ʒu
			g_Event[nCnt].IventPosition = IventPosition;   // �C�x���g�̏ꏊ
			g_Event[nCnt].nTime = nTime;				   // �C�x���g�̎���
			g_Event[nCnt].bEvent = true;				   // �C�x���g������

			break;
		}
	}
}
//==============================================================================================================
// �C�x���g�̍X�V����
//==============================================================================================================
void UpdateEvent(void)
{
	//GAMESTATE gamestate = GetGameState();

	//if (bEvent == false)
	//{
	//	// �C���N�������g
	//	g_nStartTime++;
	//}

	//int Minute = GetTimeMinute();
	//int Second = GetTimeSecond();

	//// 10�b������������
	//if (Minute <= 0 && Second <= 10)
	//{
	//	// �֐��𔲂���
	//	return;
	//}

	//// �C�x���g�̏ꏊ����
	//for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	//{
	//	// �g�p����������
	//	if (g_Event[nCnt].bEvent == true)
	//	{
	//		continue;
	//	}

	//	// �C�x���g���n�܂�
	//	if (g_nStartTime >= START_EVENTTIME)
	//	{
	//		// �C�x���g�̏ꏊ�����߂�
	//		int EventPos = rand() % EVENTPOSITION_MAX;

	//		// �C�x���g�̏ꏊ����
	//		g_EventPos = EventPos;

	//		// �C�x���g�̏ꏊ
	//		switch (EventPos)
	//		{
	//		case EVENTPOSITION_ONE:
	//			// �C�x���g�̃p�����[�^�[�̐ݒ�
	//			SetEventParam(EVENTPOS_ONE, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_TWO:
	//			// �C�x���g�̃p�����[�^�[�̐ݒ�
	//			SetEventParam(EVENTPOS_TWO, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_THREE:
	//			// �C�x���g�̃p�����[�^�[�̐ݒ�
	//			SetEventParam(EVENTPOS_THREE, EventPos, nCnt);
	//			break;
	//		case EVENTPOSITION_FOUR:
	//			// �C�x���g�̃p�����[�^�[�̐ݒ�
	//			SetEventParam(EVENTPOS_FOUR, EventPos, nCnt);
	//			break;
	//		default:
	//			break;
	//		}

	//		g_nStartTime = 0;

	//		break;
	//	}
	//}

	//// �C�x���g�̏ꏊ����
	//for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	//{
	//	// ���g�p��������
	//	if (g_Event[nCnt].bEvent == false)
	//	{
	//		continue;
	//	}

	//	bEvent = true;

	//	if (gamestate == GAMESTATE_NORMAL)
	//	{
	//		g_Event[nCnt].nTime--;
	//	}

	//	// �C�x���g���I�����
	//	if (g_Event[nCnt].nTime <= 0)
	//	{
	//		DeleteCylinder(g_Event[nCnt].nCylinderIdx);
	//		g_Event[nCnt].bEvent = false;
	//		bEvent = false;
	//	}
	//}

}
//==============================================================================================================
// �C�x���g�̏�Ԏ擾
//==============================================================================================================
bool EnableEvent(void)
{
	return bEvent;
}
//==============================================================================================================
// �C�x���g�̔����n�_�̎擾
//==============================================================================================================
int GetEventPos(void)
{
	return g_EventPos;
}
//==============================================================================================================
// �C�x���g�̋����I��
//==============================================================================================================
void SetEndEvent(bool bEnableEvent)
{
	// �C�x���g�̏ꏊ����
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// �g�p����������
		if (g_Event[nCnt].bEvent == true)
		{
			// �^�C���J�E���g�����Z�b�g
			g_Event[nCnt].nTime = 0;

			// �V�����_�[������
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);

			// �g�p��Ԃ����
			g_Event[nCnt].bEvent = bEvent;

			// �C�x���g���I��点��
			bEvent = false;

			return;
		}
	}
}
//==============================================================================================================
// �C�x���g�̃p�����[�^�[�̐ݒ�
//==============================================================================================================
void SetEventParam(D3DXVECTOR3 pos)
{
	// ���y�Đ�
	PlaySound(SOUND_LABEL_EVENTSE);
	
	//// �C�x���g�̐ݒ�
	//SetEvent(pos, eventpos, 2700);

	//// �V�����_�[�̐ݒ�
	//g_Event[nCnt].nCylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);

	// �{�X��ݒ�
	SetBoss(pos, 3.0f, 10000);

	// ���[�r�[��ݒ�
	SetMovie(300);

	// ���[�r�[��L���ɂ���
	EnableMovie(true);

	// UI��ݒ�
	SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);

	//// �^�C�}�[��ݒ�
	//SetCounter(D3DXVECTOR3(1080.0f, 670.0f, 0.0f), COUNTER_COUNTDOWN, 45, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
}
