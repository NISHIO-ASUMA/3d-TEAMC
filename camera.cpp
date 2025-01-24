//==========================
//
// �J�������� [camera.cpp]
// Author: TEAM_C
// 
//==========================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************
#define MAX_VIEWUP (3.00f) // �J�����̐���
#define MAX_VIEWDOWN (0.1f) // �J�����̐���
#define MAIN (0) // �v���C���[������J�����̃C���f�b�N�X
#define MAP (1) // �~�j�}�b�v�̃J�����̃C���f�b�N�X

//*****************************
// �v���g�^�C�v�錾
//*****************************
void MouseView(void);       // �Q�[���̎��̃}�E�X�̎��_�ړ�
void MouseEditMode(void); // �ҏW���[�h�̎��̃}�E�X�ړ�

//*****************************
// �O���[�o���ϐ��錾
//*****************************
Camera g_camera[MAX_CAMERA];		// �J�������
//Camera g_camera;						// �J�������
D3DXVECTOR3 Zoom;

//=========================
// �J�����̏���������
//=========================
void InitCamera(void)
{
	// �ϐ��̏�����
	g_camera[MAIN].posV = D3DXVECTOR3(0.0f, 80.0f, -250.0f);			// �J�����̈ʒu
	g_camera[MAP].posV = D3DXVECTOR3(0.0f, 700.0f, 0.0f);			// �J�����̈ʒu

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		g_camera[nCnt].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̌��Ă���ʒu
		g_camera[nCnt].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
		g_camera[nCnt].rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // �p�x
		g_camera[nCnt].g_CameraMode = CAMERAMODE_PLAYER;						// �������
		Zoom = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fRotx = g_camera[nCnt].posV.x - g_camera[nCnt].posR.x;
		float fRoty = g_camera[nCnt].posV.y - g_camera[nCnt].posR.y;
		float fRotz = g_camera[nCnt].posV.z - g_camera[nCnt].posR.z;

		g_camera[nCnt].fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// ���_���璍���_�܂ł̋���
		g_camera[nCnt].oldDistance = g_camera[nCnt].fDistance;	// ������ۑ����Ă���
	}

	g_camera[MAIN].viewport.X = 0.0f; // 2DX���W
	g_camera[MAIN].viewport.Y = 0.0f; // 2DY���W
	g_camera[MAIN].viewport.Width = 1280.0f; // ��
	g_camera[MAIN].viewport.Height = 720.0f; // ����
	g_camera[MAIN].viewport.MinZ = 0.0f;
	g_camera[MAIN].viewport.MaxZ = 1.0f;

	g_camera[MAP].viewport.X = 1000.0f; // 2DX���W
	g_camera[MAP].viewport.Y = 100.0f; // 2DY���W
	g_camera[MAP].viewport.Width = 200.0f; // ��
	g_camera[MAP].viewport.Height = 200.0f; // ����
	g_camera[MAP].viewport.MinZ = 0.0f;
	g_camera[MAP].viewport.MaxZ = 1.0f;
	
	//g_camera[2].viewport.X = 0.0f; // 2DX���W
	//g_camera[2].viewport.Y = 360.0f; // 2DY���W
	//g_camera[2].viewport.Width = 640.0f; // ��
	//g_camera[2].viewport.Height = 360.0f; // ����

	//g_camera[3].viewport.X = 640.0f; // 2DX���W
	//g_camera[3].viewport.Y = 360.0f; // 2DY���W
	//g_camera[3].viewport.Width = 640.0f; // ��
	//g_camera[3].viewport.Height = 360.0f; // ����

	//for (int nCnt = 0; nCnt < 2; nCnt++)
	//{
	//}
}
//=========================
// �J�����̏I������
//=========================
void UninitCamera(void)
{
	// ����
}
//=========================
// �J�����̍X�V����
//=========================
void UpdateCamera(void)
{
	// �v���C���[���擾
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	if (GetEditState())
	{
		MouseEditMode();	//�ҏW���[�h���̃J�����ړ�
	}

	// �Q�[���̎��̃J�����̍X�V
	if (mode != MODE_TITLE && !GetEditState())
	{
		g_camera[MAIN].fDistance = g_camera[MAIN].oldDistance; // ���������Z�b�g
		
		// �J�����̉E�X�e�B�b�N
		StickCamera();

		//�}�E�X�̎��_�ړ�
		//MouseView();

//****************************************
//      �v���C���[�Ǐ](MAIN�J����)
//****************************************
		g_camera[MAIN].posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAIN].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAIN].posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		g_camera[MAIN].posVDest.x = pPlayer->pos.x - sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posVDest.y = pPlayer->pos.y - cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posVDest.z = pPlayer->pos.z - cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

		g_camera[MAIN].posR.x += ((g_camera[MAIN].posRDest.x - g_camera[MAIN].posR.x) * 0.3f);
		g_camera[MAIN].posR.y += ((g_camera[MAIN].posRDest.y - g_camera[MAIN].posR.y) * 0.3f);
		g_camera[MAIN].posR.z += ((g_camera[MAIN].posRDest.z - g_camera[MAIN].posR.z) * 0.3f);

		g_camera[MAIN].posV.x += ((g_camera[MAIN].posVDest.x - g_camera[MAIN].posV.x) * 0.3f);
		g_camera[MAIN].posV.z += ((g_camera[MAIN].posVDest.z - g_camera[MAIN].posV.z) * 0.3f);

//****************************************
//      �v���C���[�Ǐ](MAP�J����)
//****************************************
		g_camera[MAP].posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAP].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAP].posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		g_camera[MAP].posVDest.x = pPlayer->pos.x - sinf(g_camera[MAP].rot.y) * g_camera[MAP].fDistance;
		g_camera[MAP].posVDest.y = pPlayer->pos.y - cosf(g_camera[MAP].rot.y) * g_camera[MAP].fDistance;
		g_camera[MAP].posVDest.z = pPlayer->pos.z - cosf(g_camera[MAP].rot.y) * g_camera[MAP].fDistance;

		g_camera[MAP].posR.x += ((g_camera[MAP].posRDest.x - g_camera[MAP].posR.x) * 0.3f);
		g_camera[MAP].posR.y += ((g_camera[MAP].posRDest.y - g_camera[MAP].posR.y) * 0.3f);
		g_camera[MAP].posR.z += ((g_camera[MAP].posRDest.z - g_camera[MAP].posR.z) * 0.3f);
	}
	

#if 0
	//******************
	// ���_�̐���
	//******************
	if (GetKeyboardPress(DIK_LEFT) == true || JoypadPress(JOYKEY_LEFT_B))
	{// LEFT�L�[�������ꂽ

		// �J�����̉�]
		g_camera.rot.y -= 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true || JoypadPress(JOYKEY_RIGHT_B))
	{// RIGHT�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.y += 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

	}

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_UP))
	{// UP�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.x += 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

	}
	if (GetKeyboardPress(DIK_DOWN))
	{// DOWN�L�[�������ꂽ
		// �J�����̉�]
		g_camera.rot.x -= 0.03f;

		// �p�x�̐��K��
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
	}

#endif // DEBUG

	
	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// �J�����̉���

		g_camera.rot.x = D3DX_PI * 0.55f;
	}
	else if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// �J�����̏��

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
#endif
#if 0

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;



		switch (g_camera.g_CameraMode)
		{// ���[�h�ؑ�
		case CAMERAMODE_NONE:
			break;

		case CAMERAMODE_PLAYER:
			// �v���C���[�ɒǏ]������
			g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
			g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

			g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
			g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

			g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
			g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);
			g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);

			g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
			g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);
			
			break;

		default:
			break;
		}
	

	if (KeyboardTrigger(DIK_F2))
	{// ���[�h�ؑ�
		if (g_camera.g_CameraMode == CAMERAMODE_NONE)
		{// �Ǐ]���[�h
			g_camera.g_CameraMode = CAMERAMODE_PLAYER;
		}
		else if (g_camera.g_CameraMode == CAMERAMODE_PLAYER)
		{// �ʏ���
			g_camera.g_CameraMode = CAMERAMODE_NONE;
		}
	}

#endif // !_DEBUG
}
//=========================
// �J�����̐ݒ菈��
//=========================
void SetCamera(int nCnt)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾
	pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nCnt].mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera[nCnt].mtxView,
		&g_camera[nCnt].posV,
		&g_camera[nCnt].posR,
		&g_camera[nCnt].vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nCnt].mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nCnt].mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera[nCnt].mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1450.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCnt].mtxProjection);
	
}
//=========================
// �J�����̎擾����
//=========================
Camera* GetCamera()
{
	return &g_camera[0];
}
//==================================
// �J�����̂��߂̉E�X�e�B�b�N����
//==================================
void StickCamera(void)
{
	// �X�e�B�b�N
	XINPUT_STATE* pStick = GetJoyStickAngle();

#if 1
	if (GetJoyStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera[MAIN].rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera[MAIN].rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera[MAIN].rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera[MAIN].rot.x += 0.03f;
			}

			// �J�����̎��_�̏��
			g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// �㉺�ړ�
			//	// �p�x
			//	g_camera[MAIN].rot.x -= 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			//	g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// �p�x
			//	g_camera[MAIN].rot.x += 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			//	g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

			//}

			// �p�x�̐��K��
			if (g_camera[MAIN].rot.y > D3DX_PI)
			{// D3DX_PI���傫���Ȃ�����
				g_camera[MAIN].rot.y -= D3DX_PI * 2.0f;
			}

			// �p�x�̐��K��
			if (g_camera[MAIN].rot.y < -D3DX_PI)
			{// D3DX_PI��菬�����Ȃ�����
				g_camera[MAIN].rot.y += D3DX_PI * 2.0f;
			}

			if (g_camera[MAIN].rot.x <= D3DX_PI * 0.55f)
			{// �J�����̉���

				g_camera[MAIN].rot.x = D3DX_PI * 0.55f;
			}

			if (g_camera[MAIN].rot.x >= D3DX_PI * 0.9f)
			{// �J�����̏��

				g_camera[MAIN].rot.x = D3DX_PI * 0.9f;
			}

		}
	}
#endif
}
//=========================
// �}�E�X�̎��_�ړ�����
//=========================
void MouseView(void)
{
	MODE mode = GetMode();

	DIMOUSESTATE mouseState;

	if (mode != MODE_TITLE && GetMouseState(&mouseState))
	{

			static POINT prevCursorPos = { SCREEN_WIDTH / 1.5f,SCREEN_HEIGHT / 1.5f };

			POINT cursorPos;
			GetCursorPos(&cursorPos);

			float X = (float)cursorPos.x - prevCursorPos.x;
			float Y = (float)cursorPos.y - prevCursorPos.y;

			const float mouseSensitivity = 0.009f;

			X *= mouseSensitivity;
			Y *= mouseSensitivity;

			g_camera[MAIN].rot.y += X;
			g_camera[MAIN].rot.x += Y;

			if (g_camera[MAIN].rot.y < -D3DX_PI)
			{
				g_camera[MAIN].rot.y += D3DX_PI * 2.0f;
			}
			else if (g_camera[MAIN].rot.y > D3DX_PI)
			{
				g_camera[MAIN].rot.y += -D3DX_PI * 2.0f;
			}

			if (g_camera[MAIN].rot.x < -D3DX_PI)
			{
				g_camera[MAIN].rot.x += D3DX_PI * 2.0f;
			}
			else if (g_camera[MAIN].rot.x > D3DX_PI)
			{
				g_camera[MAIN].rot.x += -D3DX_PI * 2.0f;
			}

			if (g_camera[MAIN].rot.x > MAX_VIEWUP)
			{
				g_camera[MAIN].rot.x -= Y;
			}
			else if (g_camera[MAIN].rot.x < MAX_VIEWDOWN)
			{
				g_camera[MAIN].rot.x -= Y;
			}
			SetCursorPos((float)SCREEN_WIDTH / 1.5f, (float)SCREEN_HEIGHT / 1.5f);

			prevCursorPos.x = SCREEN_WIDTH / 1.5f;
			prevCursorPos.y = SCREEN_HEIGHT / 1.5f;

			g_camera[MAIN].posR.x = g_camera[MAIN].posV.x + sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posR.y = g_camera[MAIN].posV.y + cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posR.z = g_camera[MAIN].posV.z + sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		
	}
}
//============================
// �ҏW���[�h�̎��̃}�E�X�ړ�
//============================
void MouseEditMode(void)
{
	if (OnMousePress(RIGHT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//��]�ʂ��X�V
		g_camera[MAIN].rot.y += fAngle.x * 0.01f;
		g_camera[MAIN].rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera[MAIN].rot.x > MAX_VIEWUP)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera[MAIN].rot.x < MAX_VIEWDOWN)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}

		//�J�������W���X�V
		g_camera[MAIN].posR.x = g_camera[MAIN].posV.x + sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.y = g_camera[MAIN].posV.y + cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.z = g_camera[MAIN].posV.z + sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//��]�ʂ��X�V
		g_camera[MAIN].rot.y += fAngle.x * 0.01f;
		g_camera[MAIN].rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera[MAIN].rot.x > MAX_VIEWUP)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera[MAIN].rot.x < MAX_VIEWDOWN)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}

	//���K��
	if (g_camera[MAIN].rot.y < -D3DX_PI)
	{
		g_camera[MAIN].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera[MAIN].rot.y > D3DX_PI)
	{
		g_camera[MAIN].rot.y += -D3DX_PI * 2.0f;
	}
	if (g_camera[MAIN].rot.x < -D3DX_PI)
	{
		g_camera[MAIN].rot.x += D3DX_PI * 2.0f;
	}
	else if (g_camera[MAIN].rot.x > D3DX_PI)
	{
		g_camera[MAIN].rot.x += -D3DX_PI * 2.0f;
	}

}
//=========================
// �}�E�X�̎��_�ړ�����
//=========================
void MouseWheel(int zDelta)
{
	// ���݂̃��[�h���擾
	MODE nMode = GetMode();

	if (nMode == MODE_TUTORIAL || GetEditState())
	{// �Q�[�� �`���[�g���A�� �ҏW���[�h
		// TODO : �����̃Y�[���̏����ύX�̉\������

		Zoom = g_camera[MAIN].posV - g_camera[MAIN].posR;
		D3DXVec3Normalize(&Zoom, &Zoom);

		if (zDelta < 0)
		{
			g_camera[MAIN].fDistance += 10.0f;
		}
		else if (zDelta > 0)
		{
			g_camera[MAIN].fDistance -= 10.0f;
		}
		// �J�����̎��_�̏��
		g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}
}
