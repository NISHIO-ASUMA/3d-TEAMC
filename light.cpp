//============================================================================================================
//
// ���C�g���� [light.cpp]
// Author:TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "light.h"
#include "HPGauge.h"
#include "player.h"

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
D3DLIGHT9 g_light[MAX_LIGHT];		// ���C�g���
D3DXVECTOR3 g_vecDir[MAX_LIGHT];	// ���C�g�̐�

//===========================================================================================================
//�@�J�����̏���������
//===========================================================================================================
void InitLight(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾
	pDevice = GetDevice();


	// ���C�g�̃N���A����(�[���������[�֐�)
	ZeroMemory(&g_light, sizeof(g_light));

	// ���C�g�̎�ނ�ݒ�
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ���C�g�̕����x�N�g��
		g_vecDir[nCnt];

		g_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;	// ���s����

		// ���C�g�̊g�U��
		g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ���C�g�̕����ݒ�
	g_vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	g_vecDir[1] = D3DXVECTOR3(-0.6f, 0.1f, 0.4f);
	g_vecDir[2] = D3DXVECTOR3(1.0f,-0.5f,0.4f);

	for (int nCnt1 = 0; nCnt1 < MAX_LIGHT; nCnt1++)
	{	// ���K������
		D3DXVec3Normalize(&g_vecDir[nCnt1], &g_vecDir[nCnt1]); // �x�N�g���̑傫����1�ɂ���

		g_light[nCnt1].Direction = g_vecDir[nCnt1];

		// ���C�g�̐ݒ�
		pDevice->SetLight(nCnt1, &g_light[nCnt1]);

		// ���C�g��L����
		pDevice->LightEnable(nCnt1, TRUE);

	}
}
//===========================================================================================================
//�@���C�g�̏I������
//===========================================================================================================
void UninitLight(void)
{
	// ��U������
}
//===========================================================================================================
//�@���C�g�̍X�V����
//===========================================================================================================
void UpdateLight(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (pPlayer->FeverMode)
	{
		g_light[0].Diffuse = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);

		// ����������(�傫���ʒu�̃x�N�g���ɂ���)
		D3DXVec3Normalize(&g_vecDir[0], &g_vecDir[0]);

		// ���C�g��ݒ肷��
		pDevice->SetLight(0, &g_light[0]);

		// ���C�g��L���ɂ���(ON,OFF)
		pDevice->LightEnable(0, TRUE);
	}
	else if(!pPlayer->FeverMode && !pPlayer->AttackSp)
	{
		for (int nCnt1 = 0; nCnt1 < MAX_LIGHT; nCnt1++)
		{
			g_light[nCnt1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ����������(�傫���ʒu�̃x�N�g���ɂ���)
			D3DXVec3Normalize(&g_vecDir[nCnt1], &g_vecDir[nCnt1]);

			// ���C�g��ݒ肷��
			pDevice->SetLight(nCnt1, &g_light[nCnt1]);

			// ���C�g��L���ɂ���(ON,OFF)
			pDevice->LightEnable(nCnt1, TRUE);
		}
	}
}

