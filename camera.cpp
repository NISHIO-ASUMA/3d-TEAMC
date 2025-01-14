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

//*****************************
// グローバル変数宣言
//*****************************
//Camera g_camera[2];				// カメラ情報
Camera g_camera;				// カメラ情報

//=========================
// カメラの初期化処理
//=========================
void InitCamera(void)
{
	// 変数の初期化
	//for (int nCnt = 0; nCnt < 2; nCnt++)
	//{
		g_camera.posV = D3DXVECTOR3(0.0f, 65.0f, -250.0f);			// カメラの位置
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの見ている位置
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
		g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // 角度
		g_camera.g_CameraMode = CAMERAMODE_PLAYER;					// 初期状態
	//}

	//g_camera[0].viewport.X = 0.0f; // 2DX座標
	//g_camera[0].viewport.Y = 0.0f; // 2DY座標
	//g_camera[0].viewport.Width = 640.0f; // 幅
	//g_camera[0].viewport.Height = 720.0f; // 高さ
	//g_camera[0].viewport.MinZ = 0.0f;
	//g_camera[0].viewport.MaxZ = 1.0f;

	//g_camera[1].viewport.X = 640.0f; // 2DX座標
	//g_camera[1].viewport.Y = 0.0f; // 2DY座標
	//g_camera[1].viewport.Width = 640.0f; // 幅
	//g_camera[1].viewport.Height = 720.0f; // 高さ
	//g_camera[1].viewport.MinZ = 0.0f;
	//g_camera[1].viewport.MaxZ = 1.0f;
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
		float fRotx = g_camera.posV.x - g_camera.posR.x;
		float fRoty = g_camera.posV.y - g_camera.posR.y;
		float fRotz = g_camera.posV.z - g_camera.posR.z;

		g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// 視点から注視点までの距離
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
	//PLAYER* pPlayer = GetPlayer();

	// カメラの右スティック:::
	StickCamera();
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
void SetCamera(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得
	pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1450.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
	
}
//=========================
// カメラの取得処理
//=========================
Camera* GetCamera()
{
	return &g_camera;
}
//==================================
// カメラのための右スティック処理
//==================================
void StickCamera(void)
{
	// スティック
	XINPUT_STATE* pStick = GetJoyStickAngle();
#if 0
	if (GetJoyStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -10920.0f)
			{
				// 角度
				g_camera.rot.y -= 0.03f;

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * sinf(g_camera.rot.y) * g_camera.fDistance;
				g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y) * cosf(g_camera.rot.y) * g_camera.fDistance;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{
				// 角度
				g_camera.rot.y += 0.03f;

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * sinf(g_camera.rot.y) * g_camera.fDistance;
				g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y) * cosf(g_camera.rot.y) * g_camera.fDistance;

			}


			// 角度の正規化
			if (g_camera.rot.y > D3DX_PI)
			{// D3DX_PIより大きくなったら
				g_camera.rot.y -= D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (g_camera.rot.y < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				g_camera.rot.y += D3DX_PI * 2.0f;
			}
		}
	}
#endif
}