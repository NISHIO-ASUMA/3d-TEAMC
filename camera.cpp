//============================================================================================================
//
// �J�������� [camera.cpp]
// Author: TEAM_C
// 
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include "game.h"
#include "tutorial3d.h"
#include "spgauge.h"

//***************************************************************************************************************
// �}�N����`
//***************************************************************************************************************
#define MAX_VIEWUP (3.00f) // �J�����̐���
#define MAX_VIEWDOWN (0.1f) // �J�����̐���
#define MAIN (0) // �v���C���[������J�����̃C���f�b�N�X
#define CRAFT (1) // �N���t�g���
#define MAX_CAMERATEX (256) // �e�N�X�`���ő吔
#define SPCOUNT	(20)


//***************************************************************************************************************
// �v���g�^�C�v�錾
//***************************************************************************************************************
void MouseView(void);       // �Q�[���̎��̃}�E�X�̎��_�ړ�
void MouseEditMode(void); // �ҏW���[�h�̎��̃}�E�X�ړ�


//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
Camera g_camera[CAMERATYPE_MAX];		// �J�������
D3DXVECTOR3 Zoom;						// �z�C�[���̃Y�[��

//===========================================================================================================
// �J�����̏���������
//===========================================================================================================
void InitCamera(void)
{
	// �ϐ��̏�����
	g_camera[MAIN].posV = D3DXVECTOR3(0.0f, 250.0f, -550.0f);			// �J�����̈ʒu

	for (int nCnt = 0; nCnt < CAMERATYPE_MAX; nCnt++)
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
	g_camera[MAIN].Direction = 1.0f; // �ړ���

	g_camera[MAIN].viewport.X = 0; // 2DX���W
	g_camera[MAIN].viewport.Y = 0; // 2DY���W
	g_camera[MAIN].viewport.Width = 1280; // ��
	g_camera[MAIN].viewport.Height = 720; // ����
	g_camera[MAIN].viewport.MinZ = 0.0f;
	g_camera[MAIN].viewport.MaxZ = 1.0f;
	
	g_camera[CRAFT].viewport.X = 300; // 2DX���W
	g_camera[CRAFT].viewport.Y = 200; // 2DY���W
	g_camera[CRAFT].viewport.Width = 650; // ��
	g_camera[CRAFT].viewport.Height = 400; // ����
	g_camera[CRAFT].viewport.MinZ = 0.0f;
	g_camera[CRAFT].viewport.MaxZ = 1.0f;

	// �e�N�X�`���̏�����
	//InitCameraTex();
}
//===========================================================================================================
// �J�����̏I������
//===========================================================================================================
void UninitCamera(void)
{
	// �e�N�X�`���̏I��
	//UninitCameraTex();
}
//===========================================================================================================
// �J�����̍X�V����
//===========================================================================================================
void UpdateCamera(void)
{
	// �v���C���[���擾
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	if (GetEditState() || GetEditStatetuto())
	{
		MouseEditMode();	//�ҏW���[�h���̃J�����ړ�
	}

	if (mode == MODE_TITLE)
	{
		g_camera[MAIN].posV.z = -1500.0f;
		g_camera[MAIN].posV.y = 1500.0f;
		
		g_camera[MAIN].rot.y += 0.005f;		// �J�����̎��_�̏��

		g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * 1800.0f;
		g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * 1800.0f;
		g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * 1800.0f;

	}
		// �Q�[���̎��̃J�����̍X�V
	if (mode != MODE_TITLE && !GetEditState() &&!GetEditStatetuto())
	{
		g_camera[MAIN].fDistance = g_camera[MAIN].oldDistance; // ���������Z�b�g
		
		// �J�����̉E�X�e�B�b�N
		StickCamera();

		//�}�E�X�̎��_�ړ�
		MouseView();

//**************************************************************************************************************************
//      �v���C���[�Ǐ](MAIN�J����)
//**************************************************************************************************************************
		g_camera[MAIN].posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAIN].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera[MAIN].posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		g_camera[MAIN].posR.x += ((g_camera[MAIN].posRDest.x - g_camera[MAIN].posR.x) * 0.3f);
		g_camera[MAIN].posR.y += ((g_camera[MAIN].posRDest.y - g_camera[MAIN].posR.y) * 0.3f);
		g_camera[MAIN].posR.z += ((g_camera[MAIN].posRDest.z - g_camera[MAIN].posR.z) * 0.3f);

//=============================================================================================================================

		/*g_camera[MAIN].posVDest.x = pPlayer->pos.x - sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posVDest.y = pPlayer->pos.y - cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posVDest.z = pPlayer->pos.z - cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

		g_camera[MAIN].posV.x += ((g_camera[MAIN].posVDest.x - g_camera[MAIN].posV.x) * 0.3f);
		g_camera[MAIN].posV.z += ((g_camera[MAIN].posVDest.z - g_camera[MAIN].posV.z) * 0.3f);*/

	}
	
	// �X�y�V�������[�V�����𔭓������Ƃ�
	if (pPlayer->AttackSp && pPlayer->Motion.nKey <= 3)
	{
		//// �J�E���g�p�ϐ�
		//static int nCounter = 0;

		//// �C���N�������g
		//nCounter++;

		//// �E�Ɉړ�
		//if (nCounter >= 0 && nCounter <= 3)
		//{
		//	g_camera[MAIN].Direction = 5.0f;

		//}
		//// ���Ɉړ�
		//if (nCounter >= 3 && nCounter <= 6)
		//{
		//	g_camera[MAIN].Direction = - 5.0f;
		//}
		//if (nCounter >= 6)
		//{
		//	nCounter = 0; // ������
		//}

		int n = rand() % SPCOUNT + 1 - SPCOUNT / 2;

		// �J�������ړ�������
		g_camera[MAIN].posV.z -= sinf(g_camera[MAIN].rot.y)+ n;
		g_camera[MAIN].posV.x += cosf(g_camera[MAIN].rot.y)+ n;

		//g_camera[MAIN].posR.x = sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		//g_camera[MAIN].posR.z = cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
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
//===========================================================================================================
// �J�����̐ݒ菈��
//===========================================================================================================
void SetCamera(int nCnt)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
		6000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCnt].mtxProjection);

	//if (nCnt == MAP)
	//{
	//	// �e�N�X�`���`��
	//	DrawCameraTex();
	//}
	
}
//===========================================================================================================
// �J�����̎擾����
//===========================================================================================================
Camera* GetCamera()
{
	return &g_camera[0];
}
//====================================================================================================================
// �J�����̂��߂̉E�X�e�B�b�N����
//====================================================================================================================
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
			{// �㉺�ړ�
				// �p�x
				g_camera[MAIN].rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// �㉺�ړ�
				// �p�x
				g_camera[MAIN].rot.x += 0.03f;
			}


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

		}
	}

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
	// �J�����̎��_�̏��
	g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

	g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;

	g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

#endif
}
//===========================================================================================================
// �}�E�X�̎��_�ړ�����
//===========================================================================================================
void MouseView(void)
{
	MODE mode = GetMode();

	DIMOUSESTATE mouseState;

	if (mode != MODE_TITLE && GetMouseState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00025f;

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

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		prevCursorPos.x = (long)SCREEN_WIDTH / (long)1.5f;
		prevCursorPos.y = (long)SCREEN_HEIGHT / (long)1.5f;

		g_camera[MAIN].posR.x = g_camera[MAIN].posV.x + sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.y = g_camera[MAIN].posV.y + cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.z = g_camera[MAIN].posV.z + sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		
	}
}
//==============================================================================================================
// �ҏW���[�h�̎��̃}�E�X�ړ�
//==============================================================================================================
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
//===========================================================================================================
// �}�E�X�̎��_�ړ�����
//===========================================================================================================
void MouseWheel(int zDelta)
{
	// ���݂̃��[�h���擾
	MODE nMode = GetMode();

	if (GetEditState() || GetEditStatetuto())
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
//===========================================================================================================
// �J�����e�N�X�`���̐ݒ菈��
//===========================================================================================================
//int SetTexCamera(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
//{
//}
//===========================================================================================================
// �e�N�X�`���̔ԍ����w�肵�ď������鏈��
//===========================================================================================================
void DeleteTex(int nIdx)
{
	//g_CameraTex[nIdx].bUse = false;
}
