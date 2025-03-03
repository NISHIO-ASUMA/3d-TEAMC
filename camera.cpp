//============================================================================================================
//
// カメラ処理 [camera.cpp]
// Author: TEAM_C
// 
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "player.h"
#include "game.h"
#include "tutorial3d.h"
#include "spgauge.h"

//***************************************************************************************************************
// マクロ定義
//***************************************************************************************************************
#define MAX_VIEWUP (3.00f) // カメラの制限
#define MAX_VIEWDOWN (0.1f) // カメラの制限
#define MAX_CAMERATEX (256) // テクスチャ最大数
#define SPCOUNT	(20)
#define MAX_WORD (256) // 最大文字数

//***************************************************************************************************************
// プロトタイプ宣言
//***************************************************************************************************************
void MouseView(void);       // ゲームの時のマウスの視点移動
void MouseEditMode(void); // 編集モードの時のマウス移動

void UpdateCameraAnim(void);      // カメラのモーション
void SetCameraAnim(void);
void SaveCameraAnim(int nType);
void LoadCameraAnim(int nType);
void LoadAmimationKey(FILE* pFile, int nType, char* aStr);
int LoadAnimationKeySet(FILE* pFile, int nType, char* aStr);

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
Camera g_camera;		// カメラ情報
bool bWaveCamera = false;
int nCntKey = 0;

//===========================================================================================================
// カメラの初期化処理
//===========================================================================================================
void InitCamera(void)
{
	// 変数の初期化
	g_camera.posV = D3DXVECTOR3(0.0f, 250.0f, -550.0f);			// カメラの位置

	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの見ている位置
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f);	    // 角度
	g_camera.CameraState = CAMERAMODE_NORMAL;						// 初期状態

	float fRotx = g_camera.posV.x - g_camera.posR.x;
	float fRoty = g_camera.posV.y - g_camera.posR.y;
	float fRotz = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));	// 視点から注視点までの距離
	g_camera.oldDistance = g_camera.fDistance;	// 距離を保存しておく

	//g_camera.bViewCrossing = false;             // モデルと交差したか
	

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

	bWaveCamera = false; // カメラを揺らすかどうか

	g_camera.Direction = 1.0f; // 移動量

	g_camera.viewport.X = 0; // 2DX座標
	g_camera.viewport.Y = 0; // 2DY座標
	g_camera.viewport.Width = 1280; // 幅
	g_camera.viewport.Height = 720; // 高さ
	g_camera.viewport.MinZ = 0.0f;
	g_camera.viewport.MaxZ = 1.0f;
	
	// テクスチャの初期化
	//InitCameraTex();
}
//===========================================================================================================
// カメラの終了処理
//===========================================================================================================
void UninitCamera(void)
{
	// テクスチャの終了
	//UninitCameraTex();
}
//===========================================================================================================
// カメラの更新処理
//===========================================================================================================
void UpdateCamera(void)
{
	// プレイヤーを取得
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	if (GetEditState() || GetEditStatetuto())
	{
		MouseEditMode();	//編集モード中のカメラ移動
	}

	if (mode == MODE_TITLE)
	{
		//g_camera.posV.z = -1500.0f;
		//g_camera.posV.y = 1500.0f;
		//
		//g_camera.rot.y += 0.005f;		// カメラの視点の情報

		//g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * 1800.0f;
		//g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * 1800.0f;
		//g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * 1800.0f;

	}
		// ゲームの時のカメラの更新
	if (mode != MODE_TITLE && !GetEditState() &&!GetEditStatetuto() && g_camera.bEditMode == false)
	{
		g_camera.fDistance = g_camera.oldDistance; // 距離をリセット
		
		// カメラの右スティック
		StickCamera();

		//マウスの視点移動
		MouseView();

//**************************************************************************************************************************
//      プレイヤー追従(MAINカメラ)
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
	// カメラの編集モードがオンだったら
	else if (g_camera.bEditMode == true)
	{
		MouseEditMode();	//編集モード中のカメラ移動
		SetCameraAnim();
	}

	// 編集モードをオンにする
	if (KeyboardTrigger(DIK_F6) == true && g_camera.bEditMode == false)
	{
		g_camera.bEditMode = true;
	}
	// 編集モードをオフにする
	else if (KeyboardTrigger(DIK_F6) == true && g_camera.bEditMode == true)
	{
		g_camera.bEditMode = false;
	}

	// カメラのアニメーションの更新処理
	UpdateCameraAnim();


	if (bWaveCamera == true)
	{
		g_camera.WaveTIme--;
	}

	// スペシャルモーションを発動したとき
	if (g_camera.WaveTIme > 0)
	{
		int n = rand() % SPCOUNT + 1 - SPCOUNT / 2;

		// カメラを移動させる
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
//===========================================================================================================
// カメラの設定処理
//===========================================================================================================
void SetCamera(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
		6000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}
//===========================================================================================================
// カメラの取得処理
//===========================================================================================================
Camera* GetCamera()
{
	return &g_camera;
}
//====================================================================================================================
// カメラのための右スティック処理
//====================================================================================================================
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
				g_camera.rot.y -= 0.03f;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{// 左右移動
				// 角度
				g_camera.rot.y += 0.03f;
			}

			if (pStick->Gamepad.sThumbRY < -10920.0f)
			{// 上下移動
				// 角度
				g_camera.rot.x -= 0.03f;
			}
			else if (pStick->Gamepad.sThumbRY > 10920.0f)
			{// 上下移動
				// 角度
				g_camera.rot.x += 0.03f;
			}


			//if (pStick->Gamepad.sThumbRY < -10920.0f)
			//{// 上下移動
			//	// 角度
			//	g_camera.rot.x -= 0.03f;

			//	// カメラの視点の情報
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}
			//else if (pStick->Gamepad.sThumbRY > 10920.0f)
			//{
			//	// 角度
			//	g_camera.rot.x += 0.03f;

			//	// カメラの視点の情報
			//	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
			//	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

			//}

		}
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

	if (g_camera.rot.x <= D3DX_PI * 0.55f)
	{// カメラの下限

		g_camera.rot.x = D3DX_PI * 0.55f;
	}

	if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// カメラの上限

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
	// カメラの視点の情報
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;

	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;

	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

#endif
}
//===========================================================================================================
// マウスの視点移動処理
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
// 編集モードの時のマウス移動
//==============================================================================================================
void MouseEditMode(void)
{
	if (OnMousePress(RIGHT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//回転量を更新
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		//カメラ座標を更新
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (OnMousePress(LEFT_MOUSE))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//回転量を更新
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//回転量を制限
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//正規化
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
// マウスの視点移動処理
//===========================================================================================================
void MouseWheel(int zDelta)
{
	// 現在のモードを取得
	MODE nMode = GetMode();

	if (GetEditState() || GetEditStatetuto() || g_camera.bEditMode == true)
	{// ゲーム チュートリアル 編集モード
		// TODO : ここのズームの処理変更の可能性あり

		if (zDelta < 0)
		{
			g_camera.fDistance += 10.0f;
		}
		else if (zDelta > 0)
		{
			g_camera.fDistance -= 10.0f;
		}
		// カメラの視点の情報
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
}
//===========================================================================================================
// カメラを揺らす処理
//===========================================================================================================
void WaveCamera(int WaveTime)
{
	g_camera.WaveTIme = WaveTime;
	bWaveCamera = true;
}
//===========================================================================================================
// カメラのアニメーション処理
//===========================================================================================================
void UpdateCameraAnim(void)
{

	Player* pPlayer = GetPlayer();
	if (g_camera.CameraState == CAMERAMODE_ANIMATION)
	{
		// 次のキーを設定
		int nextKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[0].nNumKey;
		int nType = g_camera.AnimType;

		// 変数に代入
		float fPosVX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX;
		float fPosVY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY;
		float fPosVZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ;

		// 変数に代入
		float fPosRX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX;
		float fPosRY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY;
		float fPosRZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ;

		// 変数に代入
		float fRotX = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX;
		float fRotY = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY;
		float fRotZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ;

		// 差分を求める
		float DiffPosVX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVX - fPosVX;
		float DiffPosVY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVY - fPosVY;
		float DiffPosVZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosVZ - fPosVZ;
		float DiffPosRX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRX - fPosRX;
		float DiffPosRY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRY - fPosRY;
		float DiffPosRZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fPosRZ - fPosRZ;

		// 角度の差分を求める
		float DiffRotX = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotX - fRotX;
		float DiffRotY = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotY - fRotY;
		float DiffRotZ = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fRotZ - fRotZ;

		// 距離の差分を求める
		float DiffDictance = g_camera.aAnimInfo[nType].Anim_KeyInfo[nextKey].fDistance - g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fDistance;

		// 角度の正規化
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

		// 割合を計算
		float fRatePosVX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosVY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosVZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRatePosRZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;

		// 向きの割合を代入
		float fRateRotX = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRateRotY = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;
		float fRateRotZ = (float)g_camera.nCounterAnim / (float)g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame;

		// 現在の向きを算出
		float fCurrentRotX = fRotX + DiffRotX * fRateRotX;
		float fCurrentRotY = fRotY + DiffRotY * fRateRotY;
		float fCurrentRotZ = fRotZ + DiffRotZ * fRateRotZ;

		// 追従しない
		if (g_camera.aAnimInfo[nType].bTracking == false)
		{
			// 視点を更新
			g_camera.posV.x = sinf(fCurrentRotY) + fPosVX + DiffPosVX * fRatePosVX;
			g_camera.posV.y = cosf(fCurrentRotX) + fPosVY + DiffPosVY * fRatePosVY;
			g_camera.posV.z = cosf(fCurrentRotY) + fPosVZ + DiffPosVZ * fRatePosVZ;

			// 注視点を更新
			g_camera.posR.x = g_camera.posV.x + sinf(fCurrentRotY);
			g_camera.posR.y = g_camera.posV.y + cosf(fCurrentRotX);
			g_camera.posR.z = g_camera.posV.z + cosf(fCurrentRotY);
		}
		// プレイヤー追従する
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

		// アニメーションが最後まで行った
		if (g_camera.aAnimInfo[nType].bLoopAnimation == false && g_camera.nAnimKey >= g_camera.aAnimInfo[nType].nNumKey - 1
			&& g_camera.nCounterAnim >= g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame)
		{
			g_camera.CameraState = CAMERAMODE_NORMAL;
		}

		// フレームが最大になったら
		if (g_camera.nCounterAnim >= g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame)
		{
			//モーションカウントが最大になったら0に戻す
			g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
			g_camera.nCounterAnim = 0;
		}

		int Motiontype = pPlayer->Motion.motionType;
		int nNumKey = pPlayer->Motion.aMotionInfo[Motiontype].nNumkey - 1;
		int Frame = pPlayer->Motion.aMotionInfo[Motiontype].aKeyInfo[pPlayer->Motion.nKey].nFrame;

		// プレイヤーのモーションが終わったら
		if (CheckMotionBounds(pPlayer->Motion.nKey, pPlayer->Motion.nCountMotion, nNumKey, nNumKey, 0, Frame) == true)
		{
			g_camera.CameraState = CAMERAMODE_NORMAL;
		}
		//モーションカウントを加算
		g_camera.nCounterAnim++;
	}
}
//===========================================================================================================
// カメラのアニメーション設定
//===========================================================================================================
void SetCameraAnim(void)
{
	// 上移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_camera.posV.x += sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.z += cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// 下移動
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_camera.posV.x -= sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// 左移動
	if (GetKeyboardPress(DIK_A) == true)
	{
		//g_Camera.rot.x -= 0.1f;

		g_camera.posV.z += sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.x -= cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	// 右移動
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_camera.posV.z -= sinf(g_camera.rot.y) * 5.0f;
		g_camera.posV.x += cosf(g_camera.rot.y) * 5.0f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	// 種類を代入
	int nType = g_camera.AnimType;

	// 設置する
	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		// 注視点の位置を保存
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX = g_camera.posR.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY = g_camera.posR.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ = g_camera.posR.z;

		// 視点の位置を保存
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX = g_camera.posV.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY = g_camera.posV.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ = g_camera.posV.z;

		// 角度を保存
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX = g_camera.rot.x;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY = g_camera.rot.y;
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ = g_camera.rot.z;

		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fDistance = g_camera.fDistance;

		// 増やす
		g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
	}

	// キーの最大数を増やす
	if (KeyboardTrigger(DIK_O) == true)
	{
		g_camera.aAnimInfo[nType].nNumKey++;
	}
	else if (KeyboardTrigger(DIK_L) == true && g_camera.aAnimInfo[nType].nNumKey > 1)
	{
		g_camera.aAnimInfo[nType].nNumKey--;
	}

	// アニメーションのキーの変更
	if (KeyboardTrigger(DIK_RIGHT) == true)
	{
		// 増やす
		g_camera.nAnimKey = (g_camera.nAnimKey + 1) % g_camera.aAnimInfo[nType].nNumKey;
	}
	else if (KeyboardTrigger(DIK_LEFT) == true && g_camera.nAnimKey > 0)
	{
		// 減らす
		g_camera.nAnimKey--;
	}

	// 種類の変更
	if (KeyboardTrigger(DIK_1) == true && g_camera.AnimType > 0)
	{
		g_camera.AnimType--;
	}
	else if (KeyboardTrigger(DIK_2) == true)
	{
		g_camera.AnimType = (g_camera.AnimType + 1) % CAMERAANIM_MAX;
	}

	// フレーム数の変更
	if (GetKeyboardPress(DIK_UP))
	{
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame++;
	}
	else if (GetKeyboardPress(DIK_DOWN) && g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame > 1)
	{
		g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].nAnimFrame--;
	}

	// アニメーションを保存
	if (KeyboardTrigger(DIK_TAB))
	{
		SaveCameraAnim(nType);
	}

	// ループするかどうか
	if (KeyboardTrigger(DIK_F8) == true && g_camera.aAnimInfo[nType].bLoopAnimation == false)
	{
		g_camera.aAnimInfo[nType].bLoopAnimation = true;
	}
	else if (KeyboardTrigger(DIK_F8) == true && g_camera.aAnimInfo[nType].bLoopAnimation == true)
	{
		g_camera.aAnimInfo[nType].bLoopAnimation = false;
	}

	// カメラの状態を変更
	if (KeyboardTrigger(DIK_F9) == true && g_camera.CameraState == CAMERAMODE_NORMAL)
	{
		g_camera.CameraState = CAMERAMODE_ANIMATION;
	}
	else if (KeyboardTrigger(DIK_F9) == true && g_camera.CameraState == CAMERAMODE_ANIMATION)
	{
		g_camera.CameraState = CAMERAMODE_NORMAL;
	}

	// カメラの状態を変更
	if (KeyboardTrigger(DIK_M) == true && g_camera.aAnimInfo[nType].bTracking == true)
	{
		g_camera.aAnimInfo[nType].bTracking = false;
	}
	else if (KeyboardTrigger(DIK_M) == true && g_camera.aAnimInfo[nType].bTracking == false)
	{
		g_camera.aAnimInfo[nType].bTracking = true;
	}

	// アニメーションの再生
	if (KeyboardTrigger(DIK_V))
	{
		SetAnimation(nType);
	}

	// キーの場所に移動
	if (KeyboardTrigger(DIK_B))
	{
		// 注視点の位置を保存
		g_camera.posR.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRX;
		g_camera.posR.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRY;
		g_camera.posR.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosRZ;
		
		// 視点の位置を保存
		g_camera.posV.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVX;
		g_camera.posV.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVY;
		g_camera.posV.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fPosVZ;
		
		// 角度を保存
		g_camera.rot.x = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotX;
		g_camera.rot.y = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotY;
		g_camera.rot.z = g_camera.aAnimInfo[nType].Anim_KeyInfo[g_camera.nAnimKey].fRotZ;
	}
}
//===========================================================================================================
// カメラワークの保存処理
//===========================================================================================================
void SaveCameraAnim(int nType)
{
	FILE* pFile; // ファイルのポインタ

	pFile = fopen("data\\cameraInfo.txt", "w");

	// pFileがNULLじゃなかったら
	if (pFile != NULL)
	{
		fwrite(&g_camera.nAnimKey, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+================================================+\n"
			"+	          [%d]アニメーションの情報            +\n"
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
		MessageBox(NULL, "ファイルが開けません", "SaveCameraAnim", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//===========================================================================================================
// カメラワークのロード処理
//===========================================================================================================
void LoadCameraAnim(int nType)
{
	FILE* pFile; // ファイルのポインタ

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
			// 一文字づつ読み取る
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
		MessageBox(NULL, "ファイルが開けません", "LoadCameraAnim", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//===========================================================================================================
// カメラワークのキー読み込み処理
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
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// キーの最大数を読み取る
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].nNumKey);
		}
		else if (strcmp(aStr, "LOOP") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// ループするかどうか
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].bLoopAnimation);

		}
		else if (strcmp(aStr, "TRACKING") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// ループするかどうか
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
// キーの設定のロード処理
//===========================================================================================================
int LoadAnimationKeySet(FILE* pFile, int nType, char* aStr)
{
	char Skip[3] = {};

	while (1)
	{
		// 文字を読み取る
		fscanf(pFile, "%s", aStr);

		// コメントを飛ばす
		if (strcmp(aStr, "#") == 0 || strcmp(aStr, "<<") == 0)
		{
			continue;
		}

		// フレームをよみとったら
		if (strcmp(aStr, "FRAME") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// フレームを読み取る
			fscanf(pFile, "%d", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].nAnimFrame);
		}
		// フレームをよみとったら
		else if (strcmp(aStr, "POSV") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 視点の位置を代入
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVY);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosVZ);
		}
		// フレームをよみとったら
		else if (strcmp(aStr, "POSR") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 注視点を代入
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fPosRX);
		}
		// フレームをよみとったら
		else if (strcmp(aStr, "ROT") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			// 向きを代入
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotX);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotY);
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fRotZ);
		}
		// フレームをよみとったら
		else if (strcmp(aStr, "DISTANCE") == 0)
		{
			// [=]を読み飛ばす
			fscanf(pFile, "%s", &Skip[0]);

			//向きを代入
			fscanf(pFile, "%f", &g_camera.aAnimInfo[nType].Anim_KeyInfo[nCntKey].fDistance);
		}

		// フレームをよみとったら
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
// カメラテクスチャの設定処理
//===========================================================================================================
//int SetTexCamera(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
//{
//}
//===========================================================================================================
// テクスチャの番号を指定して消去する処理
//===========================================================================================================
void DeleteTex(int nIdx)
{
	//g_CameraTex[nIdx].bUse = false;
}
//===========================================================================================================
// カメラのアニメーションの設定処理
//===========================================================================================================
void SetAnimation(int nAnimType)
{
	g_camera.AnimType = nAnimType;
	g_camera.nAnimKey = 0;
	g_camera.nCounterAnim = 0;
	g_camera.CameraState = CAMERAMODE_ANIMATION;
}
