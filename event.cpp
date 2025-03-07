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

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define START_EVENTTIME (900) // �C�x���g���n�܂�܂ł̎���
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
	g_Event[EVENTPOSITION_ONE].Eventpos = EVENTPOS_ONE; // �ʒu�̏�����

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
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT,-1, POSITIONONE_RADIUS,COLOR_YELLOW,8,1,0.0f,2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_TWO:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_THREE:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
				break;
			case EVENTPOSITION_FOUR:
				SetEvent(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), EventPos, 1800);
				g_Event[nCnt].nCylinderIdx = SetMeshCylinder(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), CYLINDERTYPE_EVENT, -1, POSITIONONE_RADIUS, COLOR_YELLOW, 8, 1, 0.0f, 2000.0f);
				SetBoss(D3DXVECTOR3(-616.0f, 0.0f, -690.0f), 3.0f, 10000); // �{�X���Z�b�g
				SetMovie(300);
				EnableMovie(true);
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

		g_Event[nCnt].nTime--;

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