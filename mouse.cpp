//============================
//
// マウス [mouse.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// インクルードファイル
//****************************
#include "mouse.h"

//****************************
// グローバル変数宣言
//****************************
#define MAX_MOUSE (32)

//****************************
// グローバル変数宣言
//****************************
LPDIRECTINPUT8 g_pInputInterface = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyDevice = NULL;
DIMOUSESTATE g_MouseState;
DIMOUSESTATE g_MouseOldState;
DIMOUSESTATE g_PrevMouseState;
DIMOUSESTATE g_CurrentMouseState;

//============================
// マウスの初期化処理
//============================
HRESULT InitMouse(HINSTANCE hInstance,HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pInputInterface->CreateDevice(
		GUID_SysMouse,
		&g_pKeyDevice,
		NULL)))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pKeyDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_pKeyDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	//キーボードのアクセス権を獲得
	g_pKeyDevice->Acquire();

	return S_OK;
}
//============================
// マウスの終了処理
//============================
void UninitMouse(void)
{
	//入力デバイス(マウス)の破棄
	if (g_pKeyDevice != NULL)
	{
		g_pKeyDevice->Unacquire();//キーボードへのアクセス権を放棄
		g_pKeyDevice->Release();
		g_pKeyDevice = NULL;
	}

	//DirectInputオブジェクトの攻撃
	if (g_pInputInterface != NULL)
	{
		g_pInputInterface->Release();
		g_pInputInterface = NULL;
	}
}
//============================
// マウスの更新処理
//============================
void UpdateMouse(void)
{
	g_MouseOldState.lX = g_CurrentMouseState.lX;
	g_MouseOldState.lY = g_CurrentMouseState.lY;

	// 更新前に最新マウス情報を保存する
	g_PrevMouseState = g_CurrentMouseState;

	// 最新のマウスの状態を更新
	HRESULT	hr = g_pKeyDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);

	if (FAILED(hr))
	{
		g_pKeyDevice->Acquire();
		hr = g_pKeyDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	}

	// マウス座標を取得する
	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &p);

	g_CurrentMouseState.lX = p.x;
	g_CurrentMouseState.lY = p.y;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();
	pDevice->SetCursorPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, false);
}
//============================
//クリックされた
//============================
bool OnMouseTriggerDown(int button_type)
{
	if (!(g_PrevMouseState.rgbButtons[button_type] & (0x80)) &&
		g_CurrentMouseState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}
	return false;
}
//============================
// クリックをやめた
//============================
bool OnMouseTriggerUp(int button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & (0x80) &&
		!(g_CurrentMouseState.rgbButtons[button_type] & (0x80)))
	{
		return true;
	}

	return false;
}
void SetCursorVisibility(bool visible)
{
	static int cursurCount = 0;

	if(visible)
	{
		//カーソルを表示
		while (cursurCount < 0)
		{
			ShowCursor(TRUE);
			cursurCount++;
		}
	}
	else
	{
		//カーソルを非表示
		while (cursurCount >= 0)
		{
			ShowCursor(FALSE);
			cursurCount--;
		}
	}
}

//============================
// 移動量の取得
//============================
D3DXVECTOR2 GetMouseVelocity()
{
	return D3DXVECTOR2((float)g_CurrentMouseState.lX, (float)g_CurrentMouseState.lY);
}
//============================
// 過去の移動量の取得
//============================
D3DXVECTOR2 GetMouseOldVelocity()
{
	return D3DXVECTOR2((float)g_MouseOldState.lX, (float)g_MouseOldState.lY);
}
//============================
// 過去の移動量の取得
//============================
bool OnMousePress(int button_type)
{
	return (g_CurrentMouseState.rgbButtons[button_type] & 0x80) ? true : false;
}
//============================
// マウスホイールの取得
//============================
BOOL IsMouseWheelPresent(void)
{
	return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0);
}