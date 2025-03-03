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
#define MAX_CAMERATEX (256) // �e�N�X�`���ő吔
#define SPCOUNT	(20)
#define MAX_WORD (256) // �ő啶����

//***************************************************************************************************************
// �v���g�^�C�v�錾
//***************************************************************************************************************
void MouseView(void);       // �Q�[���̎��̃}�E�X�̎��_�ړ�
void MouseEditMode(void); // �ҏW���[�h�̎��̃}�E�X�ړ�

void UpdateCameraAnim(void);      // �J�����̃��[�V����
void SetCameraAnim(void);
void SaveCameraAnim(int nType);
void LoadCameraAnim(int nType);
void LoadAmimationKey(FILE* pFile, int nType, char* aStr);
int LoadAnimationKeySet(FILE* pFile, int nType, char* aStr);

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
Camera g_camera;		// �J�������
bool bWaveCamera = false;
int nCntKey = 0;

//===========================================================================================================
// �J�����̏���������
//===========================================================================================================
void InitCamera(void)
{
	// �ϐ��̏�����
	g_camera.posV = D3DXVECTOR3(0.0f, 250.0f, -550.0f);			// �J�����̈ʒu

	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̌��Ă���ʒu
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ������x�N�g��
	g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // �p�x
	g_camera.CameraState = CAMERAMODE_NORMAL;						// �������

	float fRotx = g_camera.posV.x - g_camera.posR.x;
	float fRoty = g_camera.posV.y - g_camera.posR.y;
	float fRotz = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// ���_���璍���_�܂ł̋���
	g_camera.oldDistance = g_camera.fDistance;	// ������ۑ����Ă���

	//g_camera.bViewCrossing = false;             // ���f���ƌ���������
	

	nCntKey = 0;
	g_camera.WaveTIme = 0;

	for (int nCntAnim = 0; nCntAnim < CAMERAANIM_MAX; nCntAnim++)
	{
		//g_camera.aAnimInfo[nCntAnim].nNumKey = 1;
		//g_camera.AnimType = CAMERAANIM_ONE;

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			g_camera.aAnimInfo[nCntAnim].Anim_KeyInfo[nCntKey].nAnimFrame = 40;
		}
		LoadCameraAnim(nCntAnim);
	}

	bWaveCamera = false; // �J������h�炷���ǂ���

	g_camera.Direction = 1.0f; // �ړ���

	g_camera.viewport.X = 0; // 2DX���W
	g_camera.viewport.Y = 0; // 2DY���W
	g_camera.viewport.Width = 1280; // ��
	g_camera.viewport.Height = 720; // ����
	g_camera.viewport.MinZ = 0.0f;
	g_camera.viewport.MaxZ = 1.0f;
	
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
		//g_camera.posV.z = -1500.0f;
		//g_camera.posV.y = 1500.0f;
		//
		//g_camera.rot.y += 0.005f;		// �J�����̎��_�̏��

		//g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * 1800.0f;
		//g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * 1800.0f;
		//g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * 1800.0f;

	}
		// �Q�[���̎��̃J�����̍X�V
	if (mode != MODE_TITLE && !GetEditState() &&!GetEditStatetuto() && g_camera.bEditMode == false)
	{
		g_camera.fDistance = g_camera.oldDistance; // ���������Z�b�g
		
		// �J�����̉E�X�e�B�b�N
		StickCamera();

		//�}�E�X�̎��_�ړ�
		MouseView();

//**************************************************************************************************************************
//      �v���C���[�Ǐ](MAIN�J����)
//**************************************************************************************************************************
		g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
		g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);
		g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);

//=============================================================================================================================

		//g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		//g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
		//g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);

	}
	// �J�����̕ҏW���[�h���I����������
	else if (g_camera.bEditMode == true)
	{
		MouseEditMode();	//�ҏW���[�h���̃J�����ړ�
		SetCameraAnim();
	}

	// �ҏW���[�h���I���ɂ���
	if (KeyboardTrigger(DIK_F6) == true && g_camera.bEditMode == false)
	{
		g_camera.bEditMode = true;
	}
	// �ҏW���[�h���I�t�ɂ���
	else if (KeyboardTrigger(DIK_F6) == true && g_camera.bEditMode == true)
	{
		g_camera.bEditMode = false;
	}

	// �J�����̃A�j���[�V�����̍X�V����
	UpdateCameraAnim();


	if (bWaveCamera == true)
	{
		g_camera.WaveTIme--;
	}

	// �X�y�V�������[�V�����𔭓������Ƃ�
	if (g_camera.WaveTIme > 0)
	{
		int n = rand() % SPCOUNT + 1 - SPCOUNT / 2;

		// �J�������ړ�������
		g_camera.posV.z -= sinf(g_camera.rot.y)+ n;
		g_camera.posV.x += cosf(g_camera.rot.y)+ n;

		//g_camera.posR.x = sinf(g_camera.rot.y) * g_camera.fDistance;
		//g_camera.posR.z = cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else
	{
		g_camera.WaveTIme = -1;
		bWaveCamera = false;
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
void SetCamera(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		6000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}
//===========================================================================================================
// �J�����̎擾����
//===========================================================================================================
Camera* GetCamera()
{
	return &g_camera;
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
				g_camera.rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// ���E�ړ�
				// �p�x
				g_camera.rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// �㉺�ړ�
				// �p�x
				g_camera.rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// �㉺�ړ�
				// �p�x
				g_camera.rot.x += 0.03f;
			}


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// �㉺�ړ�
			//	// �p�x
			//	g_camera.rot.x -= 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// �p�x
			//	g_camera.rot.x += 0.03f;

			//	// �J�����̎��_�̏��
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}

		}
	}

	// �p�x�̐��K��
	if (g_camera.rot.y > D3DX_PI)
	{// D3DX_PI���傫���Ȃ�����
		g_camera.rot.y -= D3DX_PI * 2.0f;
	}

	// �p�x�̐��K��
	if (g_camera.rot.y < -D3DX_PI)
	{// D3DX_PI��菬�����Ȃ�����
		g_camera.rot.y += D3DX_PI * 2.0f;
	}

	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// �J�����̉���

		g_camera.rot.x = D3DX_PI * 0.55f;
	}

	if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// �J�����̏��

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
	// �J�����̎��_�̏��
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;

	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;

	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

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

		const float mouseSensitivity = 0.00045f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		g_camera.rot.y += X;
		g_camera.rot.x += Y;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= Y;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
		
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
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		//�J�������W���X�V
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//��]�ʂ��X�V
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//���K��
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2.0f;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x += -D3DX_PI * 2.0f;
	}

}
//===========================================================================================================
// �}�E�X�̎��_�ړ�����
//===========================================================================================================
void MouseWheel(int zDelta)
{
	// ���݂̃��[�h���擾
	MODE nMode = GetMode();

	if (GetEditState() || GetEditStatetuto() || g_camera.bEditMode == true)
	{// �Q�[�� �`���[�g���A�� �ҏW���[�h
		// TODO : �����̃Y�[���̏����ύX�̉\������

		if (zDelta < 0)
		{
			g_camera.fDistance += 10.0f;
		}
		else if (zDelta > 0)
		{
			g_camera.fDistance -= 10.0f;
		}
		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
}
//===========================================================================================================
// �J������h�炷����
//===========================================================================================================
void WaveCamera(int WaveTime)
{
	g_camera.WaveTIme = WaveTime;
	bWaveCamera = true;
}
//===========================================================================================================
// �J�����̃A�j���[�V��������
//===========================================================================================================
void UpdateCameraAnim(void)
{

	Player* pPlayer = GetPlayer();
	if (g_camera.CameraState == CAMERAMODE_ANIMATION)
	{
		// ���̃L�[��ݒ�
		int nextKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[0].nNumKey;
		int nType = g_camera.AnimType;

		// �ϐ��ɑ��
		float fPosVX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX;
		float fPosVY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY;
		float fPosVZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ;

		// �ϐ��ɑ��
		float fPosRX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX;
		float fPosRY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY;
		float fPosRZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ;

		// �ϐ��ɑ��
		float fRotX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX;
		float fRotY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY;
		float fRotZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ;

		// ���������߂�
		float DiffPosVX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVX - fPosVX;
		float DiffPosVY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVY - fPosVY;
		float DiffPosVZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVZ - fPosVZ;
		float DiffPosRX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRX - fPosRX;
		float DiffPosRY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRY - fPosRY;
		float DiffPosRZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRZ - fPosRZ;

		// �p�x�̍��������߂�
		float DiffRotX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotX - fRotX;
		float DiffRotY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotY - fRotY;
		float DiffRotZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotZ - fRotZ;

		// �����̍��������߂�
		float DiffDictance = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fDistance - g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fDistance;

		// �p�x�̐��K��
		if (DiffRotX > D3DX_PI)
		{
			DiffRotX += -D3DX_PI * 2.0f;
		}
		else if (DiffRotX < -D3DX_PI)
		{
			DiffRotX += D3DX_PI * 2.0f;
		}

		if (DiffRotY > D3DX_PI)
		{
			DiffRotY += -D3DX_PI * 2.0f;
		}
		else if (DiffRotY < -D3DX_PI)
		{
			DiffRotY += D3DX_PI * 2.0f;
		}

		if (DiffRotZ > D3DX_PI)
		{
			DiffRotZ += -D3DX_PI * 2.0f;
		}
		else if (DiffRotZ < -D3DX_PI)
		{
			DiffRotZ += D3DX_PI * 2.0f;
		}

		// �������v�Z
		float fRatePosVX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosVY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosVZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;

		// �����̊�������
		float fRateRotX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRateRotY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRateRotZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;

		// ���݂̌������Z�o
		float fCurrentRotX = fRotX + DiffRotX * fRateRotX;
		float fCurrentRotY = fRotY + DiffRotY * fRateRotY;
		float fCurrentRotZ = fRotZ + DiffRotZ * fRateRotZ;

		// �Ǐ]���Ȃ�
		if (g_camera.aAnimInfo[nType].bTracking == false)
		{
			// ���_���X�V
			g_camera.posV.x = sinf(fCurrentRotY) + fPosVX + DiffPosVX * fRatePosVX;
			g_camera.posV.y = cosf(fCurrentRotX) + fPosVY + DiffPosVY * fRatePosVY;
			g_camera.posV.z = cosf(fCurrentRotY) + fPosVZ + DiffPosVZ * fRatePosVZ;

			// �����_���X�V
			g_camera.posR.x = g_camera.posV.x + sinf(fCurrentRotY);
			g_camera.posR.y = g_camera.posV.y + cosf(fCurrentRotX);
			g_camera.posR.z = g_camera.posV.z + cosf(fCurrentRotY);
		}
		// �v���C���[�Ǐ]����
		else if (g_camera.aAnimInfo[nType].bTracking == true)
		{
			float fRateDistance = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;

			float fCurrentDistance = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fDistance + DiffDictance * fRateDistance;

			g_camera.posV.x = g_camera.posR.x + sinf(fCurrentRotX) * sinf(fCurrentRotY) * fCurrentDistance;
			g_camera.posR.y = pPlayer->pos.y + cosf(fCurrentRotX);
			g_camera.posV.z = g_camera.posR.z + sinf(fCurrentRotX) * cosf(fCurrentRotY) * fCurrentDistance;

			g_camera.posRDest.x = pPlayer->pos.x + sinf(fCurrentRotY) * 1.0f;
			g_camera.posRDest.y = pPlayer->pos.y + cosf(fCurrentRotY) * 1.0f;
			g_camera.posRDest.z = pPlayer->pos.z + cosf(fCurrentRotY) * 1.0f;

			g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
			g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);
			g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);
		}

		// �A�j���[�V�������Ō�܂ōs����
		if (g_camera.aAnimInfo[nType].bLoopAnimation == false && g_camera.nAnimKey >= g_camera.aAnimInfo[nType].nNumKey - 1
			&& g_camera.nCounterAnim >= g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame)
		{
			g_camera.CameraState = CAMERAMODE_NORMAL;
		}

		// �t���[�����ő�ɂȂ�����
		if (g_camera.nCounterAnim >= g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame)
		{
			//���[�V�����J�E���g���ő�ɂȂ�����0�ɖ߂�
			g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
			g_camera.nCounterAnim = 0;
		}

		int Motiontype = pPlayer->Motion.motionType;
		int nNumKey = pPlayer->Motion.aMotionInfo[Motiontype].nNumkey - 1;
		int Frame = pPlayer->Motion.aMotionInfo[Motiontype].aKeyInfo[pPlayer->Motion.nKey].nFrame;

		// �v���C���[�̃��[�V�������I�������
		if (CheckMotionBounds(pPlayer->Motion.nKey, pPlayer->Motion.nCountMotion, nNumKey, nNumKey, 0, Frame) == true)
		{
			g_camera.CameraState = CAMERAMODE_NORMAL;
		}
		//���[�V�����J�E���g�����Z
		g_camera.nCounterAnim++;
	}
}
//===========================================================================================================
// �J�����̃A�j���[�V�����ݒ�
//===========================================================================================================
void SetCameraAnim(void)
{
	// ��ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_camera.posV.x += sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.z += cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// ���ړ�
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_camera.posV.x -= sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// ���ړ�
	if (GetKeyboardPress(DIK_A) == true)
	{
		//g_Camera.rot.x -= 0.1f;

		g_camera.posV.z += sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.x -= cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// �E�ړ�
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_camera.posV.z -= sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.x += cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	// ��ނ���
	int nType = g_camera.AnimType;

	// �ݒu����
	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		// �����_�̈ʒu��ۑ�
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX = g_camera.posR.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY = g_camera.posR.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ = g_camera.posR.z;

		// ���_�̈ʒu��ۑ�
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX = g_camera.posV.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY = g_camera.posV.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ = g_camera.posV.z;

		// �p�x��ۑ�
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX = g_camera.rot.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY = g_camera.rot.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ = g_camera.rot.z;

		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fDistance = g_camera.fDistance;

		// ���₷
		g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
	}

	// �L�[�̍ő吔�𑝂₷
	if (KeyboardTrigger(DIK_O) == true)
	{
		g_camera.aAnimInfo[nType].nNumKey++;
	}
	else if (KeyboardTrigger(DIK_L) == true && g_camera.aAnimInfo[nType].nNumKey > 1)
	{
		g_camera.aAnimInfo[nType].nNumKey--;
	}

	// �A�j���[�V�����̃L�[�̕ύX
	if (KeyboardTrigger(DIK_RIGHT) == true)
	{
		// ���₷
		g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
	}
	else if (KeyboardTrigger(DIK_LEFT) == true && g_camera.nAnimKey > 0)
	{
		// ���炷
		g_camera.nAnimKey--;
	}

	// ��ނ̕ύX
	if (KeyboardTrigger(DIK_1) == true && g_camera.AnimType > 0)
	{
		g_camera.AnimType--;
	}
	else if (KeyboardTrigger(DIK_2) == true)
	{
		g_camera.AnimType = (g_camera.AnimType + 1) % CAMERAANIM_MAX;
	}

	// �t���[�����̕ύX
	if (GetKeyboardPress(DIK_UP))
	{
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame++;
	}
	else if (GetKeyboardPress(DIK_DOWN) && g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame > 1)
	{
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame--;
	}

	// �A�j���[�V������ۑ�
	if (KeyboardTrigger(DIK_TAB))
	{
		SaveCameraAnim(nType);
	}

	// ���[�v���邩�ǂ���
	if (KeyboardTrigger(DIK_F8) == true && g_camera.aAnimInfo[nType].bLoopAnimation == false)
	{
		g_camera.aAnimInfo[nType].bLoopAnimation = true;
	}
	else if (KeyboardTrigger(DIK_F8) == true && g_camera.aAnimInfo[nType].bLoopAnimation == true)
	{
		g_camera.aAnimInfo[nType].bLoopAnimation = false;
	}

	// �J�����̏�Ԃ�ύX
	if (KeyboardTrigger(DIK_F9) == true && g_camera.CameraState == CAMERAMODE_NORMAL)
	{
		g_camera.CameraState = CAMERAMODE_ANIMATION;
	}
	else if (KeyboardTrigger(DIK_F9) == true && g_camera.CameraState == CAMERAMODE_ANIMATION)
	{
		g_camera.CameraState = CAMERAMODE_NORMAL;
	}

	// �J�����̏�Ԃ�ύX
	if (KeyboardTrigger(DIK_M) == true && g_camera.aAnimInfo[nType].bTracking == true)
	{
		g_camera.aAnimInfo[nType].bTracking = false;
	}
	else if (KeyboardTrigger(DIK_M) == true && g_camera.aAnimInfo[nType].bTracking == false)
	{
		g_camera.aAnimInfo[nType].bTracking = true;
	}

	// �A�j���[�V�����̍Đ�
	if (KeyboardTrigger(DIK_V))
	{
		SetAnimation(nType);
	}

	// �L�[�̏ꏊ�Ɉړ�
	if (KeyboardTrigger(DIK_B))
	{
		// �����_�̈ʒu��ۑ�
		g_camera.posR.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX;
		g_camera.posR.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY;
		g_camera.posR.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ;
		
		// ���_�̈ʒu��ۑ�
		g_camera.posV.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX;
		g_camera.posV.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY;
		g_camera.posV.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ;
		
		// �p�x��ۑ�
		g_camera.rot.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX;
		g_camera.rot.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY;
		g_camera.rot.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ;
	}
}
//===========================================================================================================
// �J�������[�N�̕ۑ�����
//===========================================================================================================
void SaveCameraAnim(int nType)
{
	FILE* pFile; // �t�@�C���̃|�C���^

	pFile = fopen("data\\cameraInfo.txt", "w");

	// pFile��NULL����Ȃ�������
	if (pFile != NULL)
	{
		fwrite(&g_camera.nAnimKey, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+================================================+\n"
			"+	          [%d]�A�j���[�V�����̏��            +\n"
			"+================================================+\n", g_camera.AnimType);

		fprintf(pFile, "ANIMATIONSET\n");
		fprintf(pFile, "	LOOP = %d \n", g_camera.aAnimInfo[nType].bLoopAnimation);
		fprintf(pFile, "	NUM_KEY = %d \n\n", g_camera.aAnimInfo[nType].nNumKey);
		fprintf(pFile, "	TRACKING = %d \n\n", g_camera.aAnimInfo[nType].bTracking);

		for (int nCnt = 0; nCnt < g_camera.aAnimInfo[nType].nNumKey; nCnt++)
		{
			fprintf(pFile, "	KEYSET		# --- << KEY  %d / %d >> --- \n", nCnt, g_camera.aAnimInfo[nType].nNumKey);

			fprintf(pFile, "		FRAME = %d \n", g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].nAnimFrame);

			fprintf(pFile, "		POSV = %.2f %.2f %.2f \n", g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosVX, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosVY, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosVZ);

			fprintf(pFile, "		POSR = %.2f %.2f %.2f \n", g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosRX, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosRY, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fPosRZ);

			fprintf(pFile, "		ROT = %.2f %.2f %.2f \n", g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fRotX, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fRotY, g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fRotZ);

			fprintf(pFile, "		DISTANCE = %.2f \n", g_camera.aAnimInfo[nType].Anim_KeyInfo[nCnt].fDistance);

			fprintf(pFile, "	END_KEYSET\n\n");
		}

		fprintf(pFile, "END_ANIMATIONSET\n");
	}
	else
	{
		MessageBox(NULL, "�t�@�C�����J���܂���", "SaveCameraAnim", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//===========================================================================================================
// �J�������[�N�̃��[�h����
//===========================================================================================================
void LoadCameraAnim(int nType)
{
	FILE* pFile; // �t�@�C���̃|�C���^

	switch (nType)
	{
	case 0:
		pFile = fopen("data\\cameraMotion000.txt", "r");
		break;
	case 1:
		pFile = fopen("data\\cameraMotion001.txt", "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	char aStr[MAX_WORD] = {};
	char Skip[3] = {};

	if (pFile != NULL)
	{
		while (1)
		{
			// �ꕶ���Âǂݎ��
			int nData = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "#") == 0 || strcmp(&aStr[0], "<<") == 0)
			{
				continue;
			}

			if (strcmp(aStr, "ANIMATIONSET") == 0)
			{
				LoadAmimationKey(pFile, nType, &aStr[0]);
			}

			if (nData == EOF)
			{
				nCntKey = 0;
				break;
			}
		}
	}
	else
	{
		MessageBox(NULL, "�t�@�C�����J���܂���", "LoadCameraAnim", MB_OK);
		return;
	}

	// �t�@�C�������
	fclose(pFile);
}
//===========================================================================================================
// �J�������[�N�̃L�[�ǂݍ��ݏ���
//===========================================================================================================
void LoadAmimationKey(FILE* pFile, int nType, char* aStr)
{
	char Skip[3] = {};

	while (1)
	{
		fscanf(pFile, "%s", aStr);

		if (strcmp(aStr, "#") == 0 || strcmp(aStr, "<<") == 0)
		{
			continue;
		}

		if (strcmp(aStr, "NUM_KEY") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �L�[�̍ő吔��ǂݎ��
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].nNumKey);
		}
		else if (strcmp(aStr, "LOOP") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩�ǂ���
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].bLoopAnimation);

		}
		else if (strcmp(aStr, "TRACKING") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���[�v���邩�ǂ���
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].bTracking);

		}
		if (strcmp(aStr, "KEYSET") == 0)
		{
			if (LoadAnimationKeySet(pFile, nType, aStr) == -1)
			{
				break;
			}
		}
	}

}
//===========================================================================================================
// �L�[�̐ݒ�̃��[�h����
//===========================================================================================================
int LoadAnimationKeySet(FILE* pFile, int nType, char* aStr)
{
	char Skip[3] = {};

	while (1)
	{
		// ������ǂݎ��
		fscanf(pFile, "%s", aStr);

		// �R�����g���΂�
		if (strcmp(aStr, "#") == 0 || strcmp(aStr, "<<") == 0)
		{
			continue;
		}

		// �t���[������݂Ƃ�����
		if (strcmp(aStr, "FRAME") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �t���[����ǂݎ��
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].nAnimFrame);
		}
		// �t���[������݂Ƃ�����
		else if (strcmp(aStr, "POSV") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ���_�̈ʒu����
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVY);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVZ);
		}
		// �t���[������݂Ƃ�����
		else if (strcmp(aStr, "POSR") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// �����_����
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
		}
		// �t���[������݂Ƃ�����
		else if (strcmp(aStr, "ROT") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			// ��������
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotY);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotZ);
		}
		// �t���[������݂Ƃ�����
		else if (strcmp(aStr, "DISTANCE") == 0)
		{
			// [=]��ǂݔ�΂�
			fscanf(pFile, "%s", &Skip[0]);

			//��������
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fDistance);
		}

		// �t���[������݂Ƃ�����
		if (strcmp(aStr, "END_KEYSET") == 0)
		{
			nCntKey++;
		}

		if (nCntKey >= g_camera.aAnimInfo[nType].nNumKey)
		{
			return -1;
		}
	}
	return 0;
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
//===========================================================================================================
// �J�����̃A�j���[�V�����̐ݒ菈��
//===========================================================================================================
void SetAnimation(int nAnimType)
{
	g_camera.AnimType = nAnimType;
	g_camera.nAnimKey = 0;
	g_camera.nCounterAnim = 0;
	g_camera.CameraState = CAMERAMODE_ANIMATION;
}
