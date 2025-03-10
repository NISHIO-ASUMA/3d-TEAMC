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

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define START_EVENTTIME (1800) // �C�x���g���n�܂�܂ł̎���
#define POSITIONONE_RADIUS (700.0f) // 0�Ԗڂ̃C�x���g�̔��a

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
	GAMESTATE gamestate = GetGameState();

	if (bEvent == false)
	{
		// �C���N�������g
		g_nStartTime++;
	}

	// �C�x���g�̏ꏊ����
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// ���g�p��������
		if (g_Event[nCnt].bEvent == true)
		{
			continue;
		}

		// �C�x���g���n�܂�
		if (g_nStartTime >= START_EVENTTIME)
		{
			// �C�x���g�̏ꏊ�����߂�
			int EventPos = rand() % EVENTPOSITION_MAX;

			// �C�x���g�̏ꏊ����
			g_EventPos = EventPos;

			// �C�x���g�̏ꏊ
			switch (EventPos)
			{
			case EVENTPOSITION_ONE:
				// ���y�Đ�
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_ONE, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_ONE, CYLINDERTYPE_EVENT,-1, POSITIONONE_RADIUS,COLOR_YELLOW,16,1,0.0f,2000.0f);
				SetBoss(EVENTPOS_ONE, 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_TWO:
				// ���y�Đ�
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_TWO, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_TWO, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_TWO, 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_THREE:
				// ���y�Đ�
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_THREE, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_THREE, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_THREE, 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);
				break;
			case EVENTPOSITION_FOUR:
				// ���y�Đ�
				PlaySound(SOUND_LABEL_EVENTSE);

				SetEvent(EVENTPOS_FOUR, EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(EVENTPOS_FOUR, CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 16, 1, 0.0f, 2000.0f);
				SetBoss(EVENTPOS_FOUR, 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				SetGameUI(D3DXVECTOR3(620.0f, 400.0f, 0.0f), UITYPE_EVENT, 1280.0f, 1280.0f, false, 0);
				SetCounter(D3DXVECTOR3(1080.0f, 645.0f, 0.0f), COUNTER_COUNTDOWN, 30, 20.0f, 25.0f, COUNTERTYPE_EVENTTIMER);

				break;
			default:
				break;
			}

			g_nStartTime = 0;

			break;
		}
	}

	// �C�x���g�̏ꏊ����
	for (int nCnt = 0; nCnt < EVENTPOSITION_MAX; nCnt++)
	{
		// ���g�p��������
		if (g_Event[nCnt].bEvent == false)
		{
			continue;
		}

		bEvent = true;

		if (gamestate == GAMESTATE_NORMAL)
		{
			g_Event[nCnt].nTime--;
		}

		// �C�x���g���I�����
		if (g_Event[nCnt].nTime <= 0)
		{
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);
			g_Event[nCnt].bEvent = false;
			bEvent = false;
		}
	}

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
			g_Event[nCnt].nTime = 0;
			DeleteCylinder(g_Event[nCnt].nCylinderIdx);
			g_Event[nCnt].bEvent = bEvent;
			bEvent = false;

			return;
		}
	}
}