//============================================================================================================
//
// 3D�Q�[����� [main.cpp]
// Author: TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//**************************************************************************************************************
#include "main.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "camera.h"
#include "light.h"
#include "input.h"
#include "fade.h"
#include "title3d.h"
#include "tutorial3d.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "mouse.h"
#include "edit.h"
#include "sound.h"
#include "player.h"
#include "edit2d.h"
#include "craftui.h"
#include"effectEdit.h"
#include"Particle2.h"

//***************************************************************************************************************
// �v���g�^�C�v�錾
//***************************************************************************************************************
void DrawMode(void); // ���݂̉�ʂ̕\��
void DrawEditMode(void);  // �ҏW���[��
void DrawPlayerInfo(void); // �v���C���[�̏��
void DrawEditmode2d(void);
void DrawEffectEditMode(void);
void DrawCameraEdit(void);

//***************************************************************************************************************
// �O���[�o���ϐ��錾
//***************************************************************************************************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;				// ���݂̃��[�h
bool g_isFullscreen = false;			// �E�B���h�E��؂�ւ��邽�߂̃t���O
RECT g_windowRect;						// �E�B���h�E��؂�ւ��邽�߂̕ϐ�
LPD3DXFONT g_pFont = NULL;				// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;					// FPS�J�E���^
int nGetNumber = 0;						// �ԍ�

//**********************************************************************************************************************************
// �E�B���h�E���t���X�N���[���ɕς��鏈��
//**********************************************************************************************************************************
void ToggleFullscreen(HWND hWnd)
{
	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
		g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}
//==================================================================================================================
// ���C���֐�
//==================================================================================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O

	DWORD dwCurrentTime;			// ���ݎ���
	DWORD dwExecLastTime;			// �I������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						 // �E�C���h�E�̃X�^�C��
		WindowProc,						 // �E�C���h�E�v���V�[�W��
		0,								 // 0�ɂ���
		0,								 // 0�ɂ���
		hInstance,						 // �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),  // �^�X�N�o�[�A�C�R��
		LoadCursor(NULL,IDC_ARROW),      // �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),      // �N���C�A���g�̈�̔w�i�F
		NULL,                            // ���j���[�o�[
		CLASS_NAME,                      // �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),  // �t�@�C���A�C�R��
	};

	HWND hWnd = {};  // �E�C���h�E�n���h��(���ʎq)
	MSG msg;		 // ���b�Z�[�W���i�[����ϐ�

	// ��ʃT�C�Y�̍\��
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �t���X�N���[�����[�h
	BOOL bWindow = FALSE;

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�C���h�E�̍����X���W
		CW_USEDEFAULT,				// �E�C���h�E�̍����Y���W
		(rect.right - rect.left),   // �E�C���h�E��
		(rect.bottom - rect.top),   // �E�C���h�E�̍���
		NULL,
		NULL,
		hInstance,					// �C���X�^���X�n���h��
		NULL);						// �E�C���h�E�쐬�f�[�^

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// ���������������s�����Ƃ�
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ������
	dwExecLastTime = timeGetTime();		// ���ݎ�����ۑ�

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // �E�C���h�E�̕\����Ԃ̐ݒ�
	UpdateWindow(hWnd);				    // �N���C�A���g�̈�̍X�V

	// ������
	DWORD dwFrameCount;					// �t���[���J�E���g
	DWORD dwFPSLastTime;				// �Ō��FPS���v����������

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windows�̏���
			if (msg.message == WM_QUIT)
			{
				// WM_QUIT���b�Z�[�W���󂯎�����烋�[�v������
				break;
			}
			else
			{
				//���b�Z�[�W�ݒ�
				TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�

				DispatchMessage(&msg);	// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}

		else
		{

			// DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ����̎擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// �ۑ�

				dwFrameCount = 0;				// 0�ɖ߂�

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���

				dwFrameCount++;					// �t���[���J�E���g�����Z

				// �X�V����
				Update();

				// �`�揈��
				Draw();

			}
		}

	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//=========================================================================================================================
// �E�C���h�E�v���V�[�W��
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // �E�C���h�E�̗̈�
	int nID;
	switch (uMsg)
	{
	case WM_ACTIVATE:	// �A�N�e�B�u���F1�@��A�N�e�B�u���F0
		return 0L;

	case WM_DESTROY:// �E�C���h�E�j�����b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// �E�C���h�E�j�����b�Z�[�W

			}
			else
			{
				return 0;			// �Ԃ�
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);	// F11�Ńt���X�N���[��
			break;
		}
	case WM_MOUSEWHEEL:
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		MouseWheel(zDelta);
		break;
	//case WM_SETCURSOR:	// �J�[�\���̐ݒ�
	//	SetCursor(NULL);
	//	return TRUE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏������J��Ԃ�
}
//===========================================================================================================
// ����������
//===========================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	// �f�o�C�X�̃v���[���e�[�V�����̃p�����[�^�[��ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // �p�����[�^�[��0�N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g�p
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V���V�[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A,���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈��,���_������CPU�ōs��)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//===================================================================================================
	// �e�평��������
	//===================================================================================================

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// �W���C�p�b�h�̏���������
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	// �}�E�X�̏���������
	if (FAILED(InitMouse(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	// �f�o�b�O�\���p�̃t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, FW_BOLD, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"MS Gothic", &g_pFont);

	// �T�E���h�̏�����
	InitSound(hWnd);

	// �t�F�[�h�̏�����
	InitFade(g_mode);

	// ���[�h�̐ݒ�
	SetMode(g_mode);

	return S_OK;
}
//====================================================================================================
// �I������
//====================================================================================================
void Uninit(void)
{
	//====================================================================================================
	// �e��I������
	//====================================================================================================
	
	// �L�[�{�[�h�̏I��
	UninitKeyboard();

	// �W���C�p�b�h�̏I��
	UninitJoypad();

	//�}�E�X�̏I��
	UninitMouse();

	// �T�E���h�̏I��
	UninitSound();

	// �t�F�[�h�̏I��
	UninitFade();

	// �f�o�b�O�\���p�̃t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//=====================================================================================================
// �X�V����
//=====================================================================================================
void Update(void)
{
	//============================================================================================================
	// �e��I�u�W�F�N�g�X�V����
	//============================================================================================================

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �W���C�p�b�h�̍X�V
	UpdateJoypad();

	//�}�E�X�̍X�V
	UpdateMouse();

	// �R���g���[���[�̐U���̍X�V
	UpdateVibration();

	switch (g_mode)
	{
	case MODE_TITLE: // �^�C�g�����
		UpdateTitle3d();
		break;

	case MODE_TUTORIAL: // �`���[�g���A�����
		UpdateTutorial3d();
		break;

	case MODE_GAME: // �Q�[�����
		UpdateGame();
		break;

	case MODE_RESULT: // ���U���g���
		UpdateResult();
		break;

	case MODE_RANKING: // �����L���O���
		UpdateRanking();
		break;

	case MODE_EFFECT: // �G�t�F�N�g�ҏW���
		UpdateEffectEdit();
		break;
	}

#if _DEBUG

	if (KeyboardTrigger(DIK_F4) == true)
	{
		onWireFrame(); // ���C���[�t���[��ON
	}
	else if (KeyboardTrigger(DIK_F5) == true)
	{
		offWireFrame(); // ���C���[�t���[��OFF
	}

#endif // !_DEBUG

	// �t�F�[�h�̍X�V
	UpdateFade();

}
//=====================================================================================================
// �`�揈��
//=====================================================================================================
void Draw(void)
{
	// �J�������擾
	Camera* pCamera = GetCamera();
	Player* pPlayer = GetPlayer();

	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetCamera(); // �J�������Z�b�g

		// �`�搬����
		//==================================================================================================
		// �`�揈��
		//==================================================================================================
		// ���݂̉�ʂ̏I��
		switch (g_mode)
		{
		case MODE_TITLE:   // �^�C�g�����(�����Ƀf�o�b�N�\����Draw�����Ȃ��ł�������)
			DrawTitle3d();
			break;
		case MODE_TUTORIAL:// �`���[�g���A�����(�����Ƀf�o�b�N�\����Draw�����Ȃ��ł�������)
			DrawTutorial3d();
			break;
		case MODE_GAME:	   // �Q�[�����(�����Ƀf�o�b�N�\����Draw�����Ȃ��ł�������)
			DrawGame();
			break;
		case MODE_RESULT:  // ���U���g���(�����Ƀf�o�b�N�\����Draw�����Ȃ��ł�������)
			DrawResult();
			break;
		case MODE_RANKING: // �����L���O���(�����Ƀf�o�b�N�\����Draw�����Ȃ��ł�������)
			DrawRanking();
			break;
		case MODE_EFFECT: // �G�t�F�N�g�ҏW���
			DrawEffectEdit();
			break;
		}

#ifdef _DEBUG

		// �f�o�b�O�\���͂��̉��ɏ����Ă�������
		if (GetEditState()==false && GetEdit2d()==false && pCamera->bEditMode==false && g_mode == MODE_GAME)
		{
			// �v���C���[�̏��
			DrawPlayerInfo();

			//// �J�����̈ʒu�\���p�f�o�b�O�t�H���g
			//DrawCameraPos();

		}
		else if (GetEditState())
		{
			if (!GetEdit2d())
			{
				// �G�f�B�^�[��ʂ̕`��3d
				DrawEditMode();
			}
			else if (GetEdit2d())
			{
				// �G�f�B�^�[2d�̕`��
				DrawEditmode2d(); 
			}
		}

		if (pCamera->bEditMode == true)
		{
			DrawCameraEdit();
		}

		if (g_mode == MODE_EFFECT)
		{
			// �G�t�F�N�g�G�f�B�^�[��ʂ̕`��
			DrawEffectEditMode();
		}
#endif
		// �t�F�[�h�̕`��
		DrawFade();

		// �`��I��
		g_pD3DDevice->EndScene();
	}
		// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//========================================================================================================
// FPS�̕\��
//========================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//������ɑ��
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==============================================================================================================
// �G�̃f�o�b�O�\��
//==============================================================================================================
void DrawEnemySet(int g_nNumEnemy)
{
	// ���[�J���ϐ�
	RECT rect = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256] = {};

	// ������ɑ��
	wsprintf(&aString[0], "�G�̑���:%d\n", g_nNumEnemy);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//================================================================================================================
// �v���C���[�̍��W�\��
//================================================================================================================
void DrawDebugPlayerPos(void)
{
#if 0
	// ���[�J���ϐ�
	RECT rect =	 { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];

	// �擾
	PLAYER* pPlayer = GetPlayer();

	// ������ɑ��
	sprintf(&aString[0],  "�v���C���[��pos.x:%.2f\n",  pPlayer->pos.x);
	sprintf(&aString1[0], "�v���C���[��pos.y:%.2f\n", pPlayer->pos.y);
	sprintf(&aString2[0], "�v���C���[��pos.z:%.2f\n", pPlayer->pos.z);
	wsprintf(&aString3[0],"���C���[�t���[�� ON[F3]:OFF[F4]\n");

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect,	  DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==========================================================================================================
// �J����
//==========================================================================================================
void DrawCameraPos(void)
{
	// ���[�J���ϐ�
	RECT rect = { 0,240,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,260,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256] = {};
	char aString1[256] = {};

	// �擾
	Camera* pCamera = GetCamera();

	// ������ɑ��
	snprintf(&aString[0], sizeof(aString),"%.2f %.2f %.2f\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	snprintf(&aString1[0], sizeof(aString1),"%.2f %.2f %.2f\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==========================================================================================================
// �J�����̎��
//==========================================================================================================
void DrawCameraType(void)
{

}
//========================================================================================================
// ���[�h�̐ݒ�
//========================================================================================================
void SetMode(MODE mode)
{
	// ���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		UninitTitle3d();
		break;

	case MODE_TUTORIAL: // �`���[�g���A�����
		UninitTutorial3d();
		break;

	case MODE_GAME:		// �Q�[�����
		UninitGame();
		break;

	case MODE_RESULT:   // ���U���g���
		UninitResult();
		break;

	case MODE_RANKING:  // �����L���O���
		UninitRanking();
		break;

	case MODE_EFFECT: // �G�t�F�N�g�ҏW���
		UninitEffectEdit();
		break;
	}

	g_mode = mode;		// ��ʐ؂�ւ�

	// �V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		InitTitle3d();
		break;

	case MODE_TUTORIAL: // �`���[�g���A�����
		InitTutorial3d();
		break;

	case MODE_GAME:		// �Q�[�����
		InitGame();
		break;

	case MODE_RESULT:	// ���U���g���
		InitResult();
		break;

	case MODE_RANKING:  // �����L���O���
		InitRanking();
		break;

	case MODE_EFFECT: // �G�t�F�N�g�ҏW���
		InitEffectEdit();
		break;
	}
}
//========================================================================================================
// �f�o�C�X�̎擾
//========================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//======================================================================================================
// ���[�h�̎擾
//======================================================================================================
MODE GetMode(void)
{
	return g_mode; // �ϐ���Ԃ�
}
//=======================================================================================================
// ���C���[�t���[��ON
//=======================================================================================================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=======================================================================================================
// ���C���[�t���[��OFF
//=======================================================================================================
void offWireFrame() 
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//=============================================================================================================
// ���݂̉�ʂ̕\��
//=============================================================================================================
void DrawMode(void)
{
	RECT rectState = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectMode = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128] = {};
	char aStrState[256] = {};

	snprintf(&aStrState[0],sizeof(aStrState),
		"+=================================================+\n"
		"+   ���݂̃��[�h[�v���C���[�h]::�ҏW���[�h[F2]    +\n"
		"+=================================================+\n");

	switch (g_mode)
	{
	case MODE_TITLE:
		//������ɑ��
		snprintf(&aStr[0], sizeof(aStr), "���݂̃��[�h[ �^�C�g�� ]\n");
		break;
	case MODE_TUTORIAL:
		//������ɑ��
		snprintf(&aStr[0], sizeof(aStr), "���݂̃��[�h[ �`���[�g���A�� ]\n");
		break;
	case MODE_GAME:
		//������ɑ��
		snprintf(&aStr[0], sizeof(aStr), "���݂̃��[�h[ �Q�[�� ]\n");
		break;
	case MODE_RESULT:
		//������ɑ��
		snprintf(&aStr[0], sizeof(aStr), "���݂̃��[�h[ ���U���g ]\n");
		break;
	case MODE_RANKING:
		//������ɑ��
		snprintf(&aStr[0], sizeof(aStr), "���݂̃��[�h[ �����L���O ]\n");
		break;
	default:
		break;
	}

	g_pFont->DrawText(NULL, &aStrState[0], -1, &rectState, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[0], -1, &rectMode, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//=============================================================================================================
// �G�f�B�b�g���[�h
//=============================================================================================================
void DrawEditMode(void)
{
	EDIT_INFO* pEdit = GetEdit();
	int nNumBlock = GetNumobj();

	RECT rectGameMode = { 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectBlockNum = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectBlockType = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectBlockKill = { 0, 120, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectSet = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectScal = { 0, 160, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectMove = { 0, 180, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectSave = { 0, 220, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectCategory = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectCamera = { 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectpMove = { 0, 260, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectObjPos = { 0, 280, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStrGame[256];
	char aStrType[256];
	char aStrNum[256];
	char aStrKill[256];
	char aStrSet[256];
	char aStrScal[256];
	char aStrMove[256];
	char aStrSave[256];
	char aStrCategory[256];
	char aStrCamera[256];
	char aStrpMove[256];
	char aStrObjPos[256];

	snprintf(&aStrGame[0],sizeof(aStrGame),
		"+=============================================================+\n"
		"+   ���݂̃��[�h[�ҏW���[�h3d:�ύX:[2d:F9]::�Q�[�����[�h[F2]  +\n"
		"+=============================================================+\n");

	snprintf(&aStrType[0], sizeof(aStrType), "�ݒu����u���b�N�ύX:[ %d��ޖ� ]:[ + F / - G ]\n",pEdit[nNumBlock].nType);
	
	snprintf(&aStrNum[0], sizeof(aStrNum), "�u���b�N�̐ݒu��[%d]\n", nNumBlock);
	snprintf(&aStrKill[0], sizeof(aStrKill), "�u���b�N������:[ BACKSPACE ]\n");
	snprintf(&aStrSet[0], sizeof(aStrSet), "�u���b�N��ݒu:[ ENTER ]\n");
	snprintf(&aStrScal[0], sizeof(aStrScal), "�u���b�N�̑傫���ύX:[ + V / - B ]\n");
	snprintf(&aStrMove[0], sizeof(aStrMove), "�ړ�:[ WASD ]:�����ύX:[ +�� / -�� ]\n");
	snprintf(&aStrSave[0], sizeof(aStrSave), "�Z�[�u[ F7 ]:�O��̔z�u���ǂݍ���[ F8 ] <data/saveEdit.txt>\n");
	snprintf(&aStrCategory[0], sizeof(aStrCategory), "�J�e�S���[�ύX:[ ���݂̃J�e�S���[ **%d�Ԗ�** ][ + Y / - U ]\n",pEdit[nNumBlock].EditCategory);
	snprintf(&aStrCamera[0], sizeof(aStrCamera), "�����_�ړ�:[ �E�N���b�N ]:���_�ړ�:[ ���N���b�N ]:�Y�[��:[ �z�C�[�� ]\n");
	snprintf(&aStrpMove[0], sizeof(aStrpMove), "�ړ��ʕύX:[ %3.2f ]:[ + H / -J ]\n",pEdit[nNumBlock].fMove);
	snprintf(&aStrObjPos[0], sizeof(aStrObjPos), "�I�𒆂̃I�u�W�F�N�g�̏ꏊ�ֈړ�:[ F6 ]\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStrType[0], -1, &rectBlockType, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrNum[0], -1, &rectBlockNum, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrKill[0], -1, &rectBlockKill, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrSet[0], -1, &rectSet, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));

	g_pFont->DrawText(NULL, &aStrGame[0], -1, &rectGameMode, DT_LEFT, D3DCOLOR_RGBA(0, 200, 0, 255));
	g_pFont->DrawText(NULL, &aStrScal[0], -1, &rectScal, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrMove[0], -1, &rectMove, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrSave[0], -1, &rectSave, DT_LEFT, D3DCOLOR_RGBA(225, 0, 0, 255));
	g_pFont->DrawText(NULL, &aStrCategory[0], -1, &rectCategory, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrCamera[0], -1, &rectCamera, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrpMove[0], -1, &rectpMove, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrObjPos[0], -1, &rectObjPos, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
}
//=============================================================================================================
// �v���C���[�̏��
//=============================================================================================================
void DrawPlayerInfo(void)
{
	Player* pPlayer = GetPlayer();

	RECT rectPos = { 0, 160, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectState = { 0, 180, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectMotion = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectBlend = { 0, 220, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectmotiontype = { 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStrPos[256] = {};
	char aStrState[256] = {};
	char aStrMotion[256] = {};
	char aStrBlend[256] = {};
	char aStrmotiontype[256] = {};

	int motiontype = pPlayer->Motion.motionType;
	int nNumKey = pPlayer->Motion.aMotionInfo[motiontype].nNumkey;
	int nKey = pPlayer->Motion.nKey;

	snprintf(&aStrPos[0],sizeof(aStrPos), "�v���C���[�̈ʒuX:[ %3.2f ] Y:[ %3.2f ] Z:[ %3.2f ]\n", pPlayer->pos.x,pPlayer->pos.y,pPlayer->pos.z);

	snprintf(&aStrState[0], sizeof(aStrState), "�v���C���[�̏��[%d]", pPlayer->state);
	snprintf(&aStrMotion[0], sizeof(aStrMotion), "���[�V�����̃L�[[ %d / %d ]:���[�V�����̃t���[��:[ %d / %d ]", nKey, nNumKey, pPlayer->Motion.nCountMotion,pPlayer->Motion.aMotionInfo[motiontype].aKeyInfo[nKey].nFrame);
	snprintf(&aStrBlend[0], sizeof(aStrBlend), "���[�V�����u�����h�̃L�[[ %d ]:���[�V�����u�����h�̃t���[��:[ %d ]", pPlayer->Motion.nKeyBlend, pPlayer->Motion.nCounterBlend);
	snprintf(&aStrmotiontype[0], sizeof(aStrmotiontype), "���݂̃��[�V����:[ %d ] / �u�����h���[�V����:[ %d ]", pPlayer->Motion.motionType, pPlayer->Motion.motiontypeBlend);

	g_pFont->DrawText(NULL, &aStrPos[0], -1, &rectPos, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrState[0], -1, &rectState, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrMotion[0], -1, &rectMotion, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrBlend[0], -1, &rectBlend, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrmotiontype[0], -1, &rectmotiontype, DT_LEFT, COLOR_PINK);

}
//==============================================================================================================
// �G�f�B�b�g���[�h2d�̕`��
//==============================================================================================================
void DrawEditmode2d(void)
{
	EDIT_INFO_2D* pEdit2d = GetEditInfo2D(); // �|�C���^�̎擾
	int nNumCnt = GetNum2d();                // �z�u���̎擾

	RECT rectGameMode = { 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectMove = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectNumObj = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectDelet = { 0, 120, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectSet = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectscal = { 0, 160, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectRot = { 0, 180, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectMouse = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectFile = { 0, 220, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectObjpos = { 0, 240, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStrGame[256];
	char aStrMove[256];
	char aStrNumObj[256];
	char aStrDelet[256];
	char aStrSet[256];
	char aStrscal[256];
	char aStrRot[256];
	char aStrMouse[256];
	char aStrFile[256];
	char aStrObj[256];

	wsprintf(&aStrGame[0],
		"+=============================================================+\n"
		"+   ���݂̃��[�h[�ҏW���[�h2d:�ύX:[3d:F9]::�Q�[�����[�h[F2]  +\n"
		"+=============================================================+\n");

	snprintf(&aStrMove[0],sizeof(aStrMove), "�ړ�:[ WASD  Q / E ]");
	snprintf(&aStrNumObj[0], sizeof(aStrMove), "�z�u��:[ %d ]", nNumCnt);
	snprintf(&aStrDelet[0], sizeof(aStrMove), "����:[ BACKSPACE ]");
	snprintf(&aStrSet[0], sizeof(aStrMove), "�ݒu:[ ENTER ]");
	snprintf(&aStrscal[0], sizeof(aStrMove), "�傫���ύX:����:[ �� / ��]:����:[ �� / �� ]");
	snprintf(&aStrRot[0], sizeof(aStrMove), "�p�x�ύX:X:[ Z / C ]:Y:[ V / B ]:Z:[ F / G ]");
	snprintf(&aStrMouse[0], sizeof(aStrMove), "�����_�ړ�:[ �E�N���b�N ]:���_�ړ�:[ ���N���b�N ]:�Y�[��:[ �z�C�[�� ]\n");
	snprintf(&aStrFile[0], sizeof(aStrMove), "�Z�[�u[ F7 ]:�O��̔z�u���ǂݍ���:[ F8 ] <data/saveEdit2d.txt>\n");
	snprintf(&aStrObj[0], sizeof(aStrMove), "�I�𒆂̃I�u�W�F�N�g�̏ꏊ�ֈړ�:[ F6 ]\n");

	g_pFont->DrawText(NULL, &aStrGame[0], -1, &rectGameMode, DT_LEFT, D3DCOLOR_RGBA(0, 200, 0, 255));
	g_pFont->DrawText(NULL, &aStrGame[0], -1, &rectMove, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrNumObj[0], -1, &rectNumObj, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrDelet[0], -1, &rectDelet, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrSet[0], -1, &rectSet, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrscal[0], -1, &rectscal, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrRot[0], -1, &rectRot, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrMouse[0], -1, &rectMouse, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrFile[0], -1, &rectFile, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrObj[0], -1, &rectObjpos, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));

}
//==============================================================================================================
// �G�t�F�N�g�ҏW���[�h�̕`��
//==============================================================================================================
void DrawEffectEditMode(void)
{
	SETPARTICLE* pEdit = GetEditEffect();

	RECT rectAngle = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectSize = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectNum = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectColorType = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectColor = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectMove = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectLife = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectdir = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectdecA = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStrAbgle[256];
	char aStrSize[256];
	char aStrNum[256];
	char aStrColorType[256];
	char aStrColor[256];
	char aStrMove[256];
	char aStrLife[256];
	char aStrdir[256];
	char aStrdecA[256];

	wsprintf(&aStrAbgle[0], "rand()%d + %d: MAX:[ + U / - Y ]:MIN:[ + O / - I ]", pEdit->AngleMax, pEdit->AngleMin);
	sprintf(&aStrSize[0], "�傫���ύX:[ + F / - G ]:[ %3.2f ]", pEdit->fSize);
	wsprintf(&aStrNum[0], "���q�̐��ύX:[ + R / - T ]:[ %d ]", pEdit->nNumParticle);

	switch (pEdit->ColorType)
	{
	case COLORCHANGE_R:
		sprintf(&aStrColorType[0], "�F�ύX : [ + V / - B ] : [ R ]");
		break;
	case COLORCHANGE_G:
		sprintf(&aStrColorType[0], "�F�ύX : [ + V / - B ] : [ G ]");
		break;
	case COLORCHANGE_B:
		sprintf(&aStrColorType[0], "�F�ύX : [ + V / - B ] : [ B ]");
		break;
	case COLORCHANGE_A:
		sprintf(&aStrColorType[0], "�F�ύX : [ + V / - B ] : [ A ]");
		break;
	default:
		break;
	}

	sprintf(&aStrColor[0], "�J���[RGBA[ �� / �� ]:[ %3.2f,%3.2f,%3.2f,%3.2f]", pEdit->col.r, pEdit->col.g, pEdit->col.b, pEdit->col.a);
	sprintf(&aStrMove[0], "�ړ���:[ + J / - H ]:[ %3.2f ]", pEdit->fSpeed);
	wsprintf(&aStrLife[0], "����:[ + K / - L ]:[ %d ]", pEdit->nLife);
	sprintf(&aStrdir[0], "�d��:[ X + 1 Y + 2 Z + 3 / X - 4 Y - 5 Z - 6 ]:[ %3.2f,%3.2f,%3.2f ]", pEdit->dir.x, pEdit->dir.y, pEdit->dir.z);
	sprintf(&aStrdecA[0], "A�l�̌���:[ + N / - M ]:[ %f ]", pEdit->decfAlv);

	g_pFont->DrawText(NULL, &aStrMove[0], -1, &rectMove, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));

	g_pFont->DrawText(NULL, &aStrColor[0], -1, &rectColor, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));

	g_pFont->DrawText(NULL, &aStrColorType[0], -1, &rectColorType, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));

	g_pFont->DrawText(NULL, &aStrNum[0], -1, &rectNum, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrAbgle[0], -1, &rectAngle, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrSize[0], -1, &rectSize, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrLife[0], -1, &rectLife, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrdir[0], -1, &rectdir, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
	g_pFont->DrawText(NULL, &aStrdecA[0], -1, &rectdecA, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
}
//=============================================================================================================
// �J�����̕ҏW���[�h
//=============================================================================================================
void DrawCameraEdit(void)
{
	Camera* pCamera = GetCamera();

	RECT rectBlockComent000 = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectTracking = { 0, 60, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectoperation = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };

	RECT rectBlockComent001 = { 0, 110, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectType = { 0, 170, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectKey = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectFrame = { 0, 230, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectSet = { 0, 260, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectSave = { 0, 320, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rectLoop = { 0, 290, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStrBlockComent[256];
	char aStrBlockComent1[256];
	char aStrType[128];
	char aStrFrame[128];
	char aStrSet[128];
	char aStrSave[128];
	char aStrLoop[128];

	char aStrTracking[128];
	char aStrrectoperation[256];
	char aStrKey[256];

	snprintf(&aStrBlockComent[0],sizeof(aStrBlockComent), "+*******************************************************************+\n"
								  "+                 ���[�h�ؑ�:[F6]       < EDITMODE >                +\n"
								  "+*******************************************************************+\n");

	snprintf(&aStrBlockComent1[0],sizeof(aStrBlockComent1),"+*******************************************************************+\n"
								   "+                       �A�j���[�V�����̐ݒ�                        +\n"
								   "+*******************************************************************+\n");
	int nType = pCamera->AnimType;
	int nKey = pCamera->nAnimKey;

	if (pCamera->aAnimInfo[nType].bTracking == true)
	{
		snprintf(&aStrTracking[0], sizeof(aStrTracking),"[�J�����Ǐ]]:[F3]�y�I���z\n");
	}
	else if (pCamera->aAnimInfo[nType].bTracking == false)
	{
		snprintf(&aStrTracking[0], sizeof(aStrTracking), "[�J�����Ǐ]]:[F3]�y�I�t�z\n");
	}

	snprintf(&aStrrectoperation[0], sizeof(aStrrectoperation), "�ړ�:[ WASD ]:���_�ړ�:[ �}�E�X�E ]:�����_�ړ�:[ �}�E�X�� ]\n");

	snprintf(&aStrType[0], sizeof(aStrType), "��ޑI��:[ 1 / 2 ]  �^�C�v�y %d / %d �z", pCamera->AnimType, CAMERAANIM_MAX);


	snprintf(&aStrKey[0], sizeof(aStrKey), "[ �L�[�ύX ]:[ �� / �� ]�y %d / %d �z\n", pCamera->nAnimKey, pCamera->aAnimInfo[nType].nNumKey);


	snprintf(&aStrFrame[0], sizeof(aStrFrame), "[ �t���[���ύX ]:[ �� / �� ] �y %d �z\n", pCamera->aAnimInfo[nType].Anim_KeyInfo[nKey].nAnimFrame);

	snprintf(&aStrSet[0], sizeof(aStrSet), "[ �ʒu�����肷�� ]:[ RETURN ]");

	snprintf(&aStrSave[0], sizeof(aStrSave), "[ �ۑ����� ]:[ F7 ] << data\\cameraInfo.txt >>");

	if (pCamera->aAnimInfo[nType].bLoopAnimation == true)
	{
		snprintf(&aStrLoop[0], sizeof(aStrLoop), "[ LOOP ]:[ F8 ]�y ���[�v���� �z");
	}
	else if (pCamera->aAnimInfo[nType].bLoopAnimation == false)
	{
		snprintf(&aStrLoop[0], sizeof(aStrLoop), "[ LOOP ]:[ F8 ]�y ���[�v���Ȃ� �z");
	}

	g_pFont->DrawText(NULL, &aStrBlockComent[0], -1, &rectBlockComent000, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	g_pFont->DrawText(NULL, &aStrBlockComent1[0], -1, &rectBlockComent001, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));


	g_pFont->DrawText(NULL, &aStrTracking[0], -1, &rectTracking, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrType[0], -1, &rectType, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrrectoperation[0], -1, &rectoperation, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	g_pFont->DrawText(NULL, &aStrKey[0], -1, &rectKey, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrFrame[0], -1, &rectFrame, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrSet[0], -1, &rectSet, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrSave[0], -1, &rectSave, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStrLoop[0], -1, &rectLoop, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
