//==========================
//
// カメラ処理 [camera.cpp]
// Author: TEAM_C
// 
//==========================

//****************************
// インクルードファイル宣言
//****************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include "game.h"

//*****************************
// マクロ定義
//*****************************
#define MAX_VIEWUP (3.00f) // カメラの制限
#define MAX_VIEWDOWN (0.1f) // カメラの制限
#define MAIN (0) // プレイヤーを見るカメラのインデックス
#define MAP (1) // ミニマップのカメラのインデックス

//*****************************
// プロトタイプ宣言
//*****************************
void MouseView(void);       // ゲームの時のマウスの視点移動
void MouseEditMode(void); // 編集モードの時のマウス移動

//*****************************
// グローバル変数宣言
//*****************************
Camera g_camera[MAX_CAMERA];		// カメラ情報
//Camera g_camera;						// カメラ情報
D3DXVECTOR3 Zoom;

//=========================
// カメラの初期化処理
//=========================
void InitCamera(void)
{
	// 変数の初期化
	g_camera[MAIN].posV = D3DXVECTOR3(0.0f, 80.0f, -250.0f);			// カメラの位置
	g_camera[MAP].posV = D3DXVECTOR3(0.0f, 700.0f, 0.0f);			// カメラの位置

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		g_camera[nCnt].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの見ている位置
		g_camera[nCnt].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
		g_camera[nCnt].rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // 角度
		g_camera[nCnt].g_CameraMode = CAMERAMODE_PLAYER;						// 初期状態
		Zoom = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fRotx = g_camera[nCnt].posV.x - g_camera[nCnt].posR.x;
		float fRoty = g_camera[nCnt].posV.y - g_camera[nCnt].posR.y;
		float fRotz = g_camera[nCnt].posV.z - g_camera[nCnt].posR.z;

		g_camera[nCnt].fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// 視点から注視点までの距離
		g_camera[nCnt].oldDistance = g_camera[nCnt].fDistance;	// 距離を保存しておく
	}

	g_camera[MAIN].viewport.X = 0.0f; // 2DX座標
	g_camera[MAIN].viewport.Y = 0.0f; // 2DY座標
	g_camera[MAIN].viewport.Width = 1280.0f; // 幅
	g_camera[MAIN].viewport.Height = 720.0f; // 高さ
	g_camera[MAIN].viewport.MinZ = 0.0f;
	g_camera[MAIN].viewport.MaxZ = 1.0f;

	g_camera[MAP].viewport.X = 1000.0f; // 2DX座標
	g_camera[MAP].viewport.Y = 100.0f; // 2DY座標
	g_camera[MAP].viewport.Width = 200.0f; // 幅
	g_camera[MAP].viewport.Height = 200.0f; // 高さ
	g_camera[MAP].viewport.MinZ = 0.0f;
	g_camera[MAP].viewport.MaxZ = 1.0f;
	
	//g_camera[2].viewport.X = 0.0f; // 2DX座標
	//g_camera[2].viewport.Y = 360.0f; // 2DY座標
	//g_camera[2].viewport.Width = 640.0f; // 幅
	//g_camera[2].viewport.Height = 360.0f; // 高さ

	//g_camera[3].viewport.X = 640.0f; // 2DX座標
	//g_camera[3].viewport.Y = 360.0f; // 2DY座標
	//g_camera[3].viewport.Width = 640.0f; // 幅
	//g_camera[3].viewport.Height = 360.0f; // 高さ

	//for (int nCnt = 0; nCnt < 2; nCnt++)
	//{
	//}
}
//=========================
// カメラの終了処理
//=========================
void UninitCamera(void)
{
	// 無し
}
//=========================
// カメラの更新処理
//=========================
void UpdateCamera(void)
{
	// プレイヤーを取得
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	if (GetEditState())
	{
		MouseEditMode();	//編集モード中のカメラ移動
	}

	// ゲームの時のカメラの更新
	if (mode != MODE_TITLE && !GetEditState())
	{
		g_camera[MAIN].fDistance = g_camera[MAIN].oldDistance; // 距離をリセット
		
		// カメラの右スティック
		StickCamera();

		//マウスの視点移動
		//MouseView();

//****************************************
//      プレイヤー追従(MAINカメラ)
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
//      プレイヤー追従(MAPカメラ)
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
	// 視点の旋回
	//******************
	if (GetKeyboardPress(DIK_LEFT) == true || JoypadPress(JOYKEY_LEFT_B))
	{// LEFTキーが押された

		// カメラの回転
		g_camera.rot.y -= 0.03f;

		// 角度の正規化
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true || JoypadPress(JOYKEY_RIGHT_B))
	{// RIGHTキーが押された
		// カメラの回転
		g_camera.rot.y += 0.03f;

		// 角度の正規化
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

	}

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_UP))
	{// UPキーが押された
		// カメラの回転
		g_camera.rot.x += 0.03f;

		// 角度の正規化
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

	}
	if (GetKeyboardPress(DIK_DOWN))
	{// DOWNキーが押された
		// カメラの回転
		g_camera.rot.x -= 0.03f;

		// 角度の正規化
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
	}

#endif // DEBUG

	
	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// カメラの下限

		g_camera.rot.x = D3DX_PI * 0.55f;
	}
	else if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// カメラの上限

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
#endif
#if 0

		// カメラの視点の情報
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;



		switch (g_camera.g_CameraMode)
		{// モード切替
		case CAMERAMODE_NONE:
			break;

		case CAMERAMODE_PLAYER:
			// プレイヤーに追従させる
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
	{// モード切替
		if (g_camera.g_CameraMode == CAMERAMODE_NONE)
		{// 追従モード
			g_camera.g_CameraMode = CAMERAMODE_PLAYER;
		}
		else if (g_camera.g_CameraMode == CAMERAMODE_PLAYER)
		{// 通常状態
			g_camera.g_CameraMode = CAMERAMODE_NONE;
		}
	}

#endif // !_DEBUG
}
//=========================
// カメラの設定処理
//=========================
void SetCamera(int nCnt)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得
	pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nCnt].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera[nCnt].mtxView,
		&g_camera[nCnt].posV,
		&g_camera[nCnt].posR,
		&g_camera[nCnt].vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nCnt].mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nCnt].mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera[nCnt].mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1450.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nCnt].mtxProjection);
	
}
//=========================
// カメラの取得処理
//=========================
Camera* GetCamera()
{
	return &g_camera[0];
}
//==================================
// カメラのための右スティック処理
//==================================
void StickCamera(void)
{
	// スティック
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
			{// 左右移動
				// 角度
				g_camera[MAIN].rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// 左右移動
				// 角度
				g_camera[MAIN].rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// 左右移動
				// 角度
				g_camera[MAIN].rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// 左右移動
				// 角度
				g_camera[MAIN].rot.x += 0.03f;
			}

			// カメラの視点の情報
			g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// 上下移動
			//	// 角度
			//	g_camera[MAIN].rot.x -= 0.03f;

			//	// カメラの視点の情報
			//	g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			//	g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// 角度
			//	g_camera[MAIN].rot.x += 0.03f;

			//	// カメラの視点の情報
			//	g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
			//	g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;

			//}

			// 角度の正規化
			if (g_camera[MAIN].rot.y > D3DX_PI)
			{// D3DX_PIより大きくなったら
				g_camera[MAIN].rot.y -= D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (g_camera[MAIN].rot.y < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				g_camera[MAIN].rot.y += D3DX_PI * 2.0f;
			}

			if (g_camera[MAIN].rot.x <= D3DX_PI * 0.55f)
			{// カメラの下限

				g_camera[MAIN].rot.x = D3DX_PI * 0.55f;
			}

			if (g_camera[MAIN].rot.x >= D3DX_PI * 0.9f)
			{// カメラの上限

				g_camera[MAIN].rot.x = D3DX_PI * 0.9f;
			}

		}
	}
#endif
}
//=========================
// マウスの視点移動処理
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
// 編集モードの時のマウス移動
//============================
void MouseEditMode(void)
{
	if (OnMousePress(RIGHT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//回転量を更新
		g_camera[MAIN].rot.y += fAngle.x * 0.01f;
		g_camera[MAIN].rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera[MAIN].rot.x > MAX_VIEWUP)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera[MAIN].rot.x < MAX_VIEWDOWN)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}

		//カメラ座標を更新
		g_camera[MAIN].posR.x = g_camera[MAIN].posV.x + sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.y = g_camera[MAIN].posV.y + cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posR.z = g_camera[MAIN].posV.z + sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//回転量を更新
		g_camera[MAIN].rot.y += fAngle.x * 0.01f;
		g_camera[MAIN].rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera[MAIN].rot.x > MAX_VIEWUP)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera[MAIN].rot.x < MAX_VIEWDOWN)
		{
			g_camera[MAIN].rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}

	//正規化
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
// マウスの視点移動処理
//=========================
void MouseWheel(int zDelta)
{
	// 現在のモードを取得
	MODE nMode = GetMode();

	if (nMode == MODE_TUTORIAL || GetEditState())
	{// ゲーム チュートリアル 編集モード
		// TODO : ここのズームの処理変更の可能性あり

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
		// カメラの視点の情報
		g_camera[MAIN].posV.x = g_camera[MAIN].posR.x - sinf(g_camera[MAIN].rot.x) * sinf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.y = g_camera[MAIN].posR.y - cosf(g_camera[MAIN].rot.x) * g_camera[MAIN].fDistance;
		g_camera[MAIN].posV.z = g_camera[MAIN].posR.z - sinf(g_camera[MAIN].rot.x) * cosf(g_camera[MAIN].rot.y) * g_camera[MAIN].fDistance;
	}
}
