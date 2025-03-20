//============================================================================================================
//
// 3Dゲーム基盤 [main.cpp]
// Author: TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
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
// プロトタイプ宣言
//***************************************************************************************************************
void DrawMode(void); // 現在の画面の表示
void DrawEditMode(void);  // 編集モーど
void DrawPlayerInfo(void); // プレイヤーの情報
void DrawEditmode2d(void);
void DrawEffectEditMode(void);
void DrawCameraEdit(void);

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;				// 現在のモード
bool g_isFullscreen = false;			// ウィンドウを切り替えるためのフラグ
RECT g_windowRect;						// ウィンドウを切り替えるための変数
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = 0;					// FPSカウンタ
int nGetNumber = 0;						// 番号

//**********************************************************************************************************************************
// ウィンドウをフルスクリーンに変える処理
//**********************************************************************************************************************************
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
		g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}
//==================================================================================================================
// メイン関数
//==================================================================================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ

	DWORD dwCurrentTime;			// 現在時刻
	DWORD dwExecLastTime;			// 終了時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						 // ウインドウのスタイル
		WindowProc,						 // ウインドウプロシージャ
		0,								 // 0にする
		0,								 // 0にする
		hInstance,						 // インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),  // タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),      // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),      // クライアント領域の背景色
		NULL,                            // メニューバー
		CLASS_NAME,                      // ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),  // ファイルアイコン
	};

	HWND hWnd = {};  // ウインドウハンドル(識別子)
	MSG msg;		 // メッセージを格納する変数

	// 画面サイズの構造
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// フルスクリーンモード
	BOOL bWindow = FALSE;

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上のX座標
		CW_USEDEFAULT,				// ウインドウの左上のY座標
		(rect.right - rect.left),   // ウインドウ幅
		(rect.bottom - rect.top),   // ウインドウの高さ
		NULL,
		NULL,
		hInstance,					// インスタンスハンドル
		NULL);						// ウインドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// 初期化処理が失敗したとき
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化
	dwExecLastTime = timeGetTime();		// 現在時刻を保存

	// ウインドウの表示
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // ウインドウの表示状態の設定
	UpdateWindow(hWnd);				    // クライアント領域の更新

	// 初期化
	DWORD dwFrameCount;					// フレームカウント
	DWORD dwFPSLastTime;				// 最後にFPSを計測した時刻

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITメッセージを受け取ったらループ抜ける
				break;
			}
			else
			{
				//メッセージ設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換

				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを送出
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

			// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻の取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 保存

				dwFrameCount = 0;				// 0に戻す

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻

				dwFrameCount++;					// フレームカウントを加算

				// 更新処理
				Update();

				// 描画処理
				Draw();

			}
		}

	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//=========================================================================================================================
// ウインドウプロシージャ
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // ウインドウの領域
	int nID;
	switch (uMsg)
	{
	case WM_ACTIVATE:	// アクティブ時：1　非アクティブ時：0
		return 0L;

	case WM_DESTROY:// ウインドウ破棄メッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// ウインドウ破棄メッセージ

			}
			else
			{
				return 0;			// 返す
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);	// F11でフルスクリーン
			break;
		}
	case WM_MOUSEWHEEL:
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		MouseWheel(zDelta);
		break;
	//case WM_SETCURSOR:	// カーソルの設定
	//	SetCursor(NULL);
	//	return TRUE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を繰り返す
}
//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメーターの0クリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使用
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュシート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
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
	
	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//===================================================================================================
	// 各種初期化処理
	//===================================================================================================

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	// マウスの初期化処理
	if (FAILED(InitMouse(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	// デバッグ表示用のフォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, FW_BOLD, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"MS Gothic", &g_pFont);

	// サウンドの初期化
	InitSound(hWnd);

	// フェードの初期化
	InitFade(g_mode);

	// モードの設定
	SetMode(g_mode);

	return S_OK;
}
//====================================================================================================
// 終了処理
//====================================================================================================
void Uninit(void)
{
	//====================================================================================================
	// 各種終了処理
	//====================================================================================================
	
	// キーボードの終了
	UninitKeyboard();

	// ジョイパッドの終了
	UninitJoypad();

	//マウスの終了
	UninitMouse();

	// サウンドの終了
	UninitSound();

	// フェードの終了
	UninitFade();

	// デバッグ表示用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//=====================================================================================================
// 更新処理
//=====================================================================================================
void Update(void)
{
	//============================================================================================================
	// 各種オブジェクト更新処理
	//============================================================================================================

	// キーボードの更新
	UpdateKeyboard();

	// ジョイパッドの更新
	UpdateJoypad();

	//マウスの更新
	UpdateMouse();

	// コントローラーの振動の更新
	UpdateVibration();

	switch (g_mode)
	{
	case MODE_TITLE: // タイトル画面
		UpdateTitle3d();
		break;

	case MODE_TUTORIAL: // チュートリアル画面
		UpdateTutorial3d();
		break;

	case MODE_GAME: // ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT: // リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING: // ランキング画面
		UpdateRanking();
		break;

	case MODE_EFFECT: // エフェクト編集画面
		UpdateEffectEdit();
		break;
	}

#if _DEBUG

	if (KeyboardTrigger(DIK_F4) == true)
	{
		onWireFrame(); // ワイヤーフレームON
	}
	else if (KeyboardTrigger(DIK_F5) == true)
	{
		offWireFrame(); // ワイヤーフレームOFF
	}

#endif // !_DEBUG

	// フェードの更新
	UpdateFade();

}
//=====================================================================================================
// 描画処理
//=====================================================================================================
void Draw(void)
{
	// カメラを取得
	Camera* pCamera = GetCamera();
	Player* pPlayer = GetPlayer();

	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetCamera(); // カメラをセット

		// 描画成功時
		//==================================================================================================
		// 描画処理
		//==================================================================================================
		// 現在の画面の終了
		switch (g_mode)
		{
		case MODE_TITLE:   // タイトル画面(ここにデバック表示のDraw書かないでください)
			DrawTitle3d();
			break;
		case MODE_TUTORIAL:// チュートリアル画面(ここにデバック表示のDraw書かないでください)
			DrawTutorial3d();
			break;
		case MODE_GAME:	   // ゲーム画面(ここにデバック表示のDraw書かないでください)
			DrawGame();
			break;
		case MODE_RESULT:  // リザルト画面(ここにデバック表示のDraw書かないでください)
			DrawResult();
			break;
		case MODE_RANKING: // ランキング画面(ここにデバック表示のDraw書かないでください)
			DrawRanking();
			break;
		case MODE_EFFECT: // エフェクト編集画面
			DrawEffectEdit();
			break;
		}

#ifdef _DEBUG

		// デバッグ表示はこの下に書いてください
		if (GetEditState()==false && GetEdit2d()==false && pCamera->bEditMode==false && g_mode == MODE_GAME)
		{
			// プレイヤーの情報
			DrawPlayerInfo();

			//// カメラの位置表示用デバッグフォント
			//DrawCameraPos();

		}
		else if (GetEditState())
		{
			if (!GetEdit2d())
			{
				// エディター画面の描画3d
				DrawEditMode();
			}
			else if (GetEdit2d())
			{
				// エディター2dの描画
				DrawEditmode2d(); 
			}
		}

		if (pCamera->bEditMode == true)
		{
			DrawCameraEdit();
		}

		if (g_mode == MODE_EFFECT)
		{
			// エフェクトエディター画面の描画
			DrawEffectEditMode();
		}
#endif
		// フェードの描画
		DrawFade();

		// 描画終了
		g_pD3DDevice->EndScene();
	}
		// バックバッファとフロントバッファの入れ替え
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//========================================================================================================
// FPSの表示
//========================================================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//文字列に代入
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==============================================================================================================
// 敵のデバッグ表示
//==============================================================================================================
void DrawEnemySet(int g_nNumEnemy)
{
	// ローカル変数
	RECT rect = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256] = {};

	// 文字列に代入
	wsprintf(&aString[0], "敵の総数:%d\n", g_nNumEnemy);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//================================================================================================================
// プレイヤーの座標表示
//================================================================================================================
void DrawDebugPlayerPos(void)
{
#if 0
	// ローカル変数
	RECT rect =	 { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];

	// 取得
	PLAYER* pPlayer = GetPlayer();

	// 文字列に代入
	sprintf(&aString[0],  "プレイヤーのpos.x:%.2f\n",  pPlayer->pos.x);
	sprintf(&aString1[0], "プレイヤーのpos.y:%.2f\n", pPlayer->pos.y);
	sprintf(&aString2[0], "プレイヤーのpos.z:%.2f\n", pPlayer->pos.z);
	wsprintf(&aString3[0],"ワイヤーフレーム ON[F3]:OFF[F4]\n");

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect,	  DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==========================================================================================================
// カメラ
//==========================================================================================================
void DrawCameraPos(void)
{
	// ローカル変数
	RECT rect = { 0,240,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,260,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256] = {};
	char aString1[256] = {};

	// 取得
	Camera* pCamera = GetCamera();

	// 文字列に代入
	snprintf(&aString[0], sizeof(aString),"%.2f %.2f %.2f\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	snprintf(&aString1[0], sizeof(aString1),"%.2f %.2f %.2f\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==========================================================================================================
// カメラの種類
//==========================================================================================================
void DrawCameraType(void)
{

}
//========================================================================================================
// モードの設定
//========================================================================================================
void SetMode(MODE mode)
{
	// 現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UninitTitle3d();
		break;

	case MODE_TUTORIAL: // チュートリアル画面
		UninitTutorial3d();
		break;

	case MODE_GAME:		// ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:   // リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:  // ランキング画面
		UninitRanking();
		break;

	case MODE_EFFECT: // エフェクト編集画面
		UninitEffectEdit();
		break;
	}

	g_mode = mode;		// 画面切り替え

	// 新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面
		InitTitle3d();
		break;

	case MODE_TUTORIAL: // チュートリアル画面
		InitTutorial3d();
		break;

	case MODE_GAME:		// ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:	// リザルト画面
		InitResult();
		break;

	case MODE_RANKING:  // ランキング画面
		InitRanking();
		break;

	case MODE_EFFECT: // エフェクト編集画面
		InitEffectEdit();
		break;
	}
}
//========================================================================================================
// デバイスの取得
//========================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//======================================================================================================
// モードの取得
//======================================================================================================
MODE GetMode(void)
{
	return g_mode; // 変数を返す
}
//=======================================================================================================
// ワイヤーフレームON
//=======================================================================================================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=======================================================================================================
// ワイヤーフレームOFF
//=======================================================================================================
void offWireFrame() 
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//=============================================================================================================
// 現在の画面の表示
//=============================================================================================================
void DrawMode(void)
{
	RECT rectState = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectMode = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128] = {};
	char aStrState[256] = {};

	snprintf(&aStrState[0],sizeof(aStrState),
		"+=================================================+\n"
		"+   現在のモード[プレイモード]::編集モード[F2]    +\n"
		"+=================================================+\n");

	switch (g_mode)
	{
	case MODE_TITLE:
		//文字列に代入
		snprintf(&aStr[0], sizeof(aStr), "現在のモード[ タイトル ]\n");
		break;
	case MODE_TUTORIAL:
		//文字列に代入
		snprintf(&aStr[0], sizeof(aStr), "現在のモード[ チュートリアル ]\n");
		break;
	case MODE_GAME:
		//文字列に代入
		snprintf(&aStr[0], sizeof(aStr), "現在のモード[ ゲーム ]\n");
		break;
	case MODE_RESULT:
		//文字列に代入
		snprintf(&aStr[0], sizeof(aStr), "現在のモード[ リザルト ]\n");
		break;
	case MODE_RANKING:
		//文字列に代入
		snprintf(&aStr[0], sizeof(aStr), "現在のモード[ ランキング ]\n");
		break;
	default:
		break;
	}

	g_pFont->DrawText(NULL, &aStrState[0], -1, &rectState, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[0], -1, &rectMode, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//=============================================================================================================
// エディットモード
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
		"+   現在のモード[編集モード3d:変更:[2d:F9]::ゲームモード[F2]  +\n"
		"+=============================================================+\n");

	snprintf(&aStrType[0], sizeof(aStrType), "設置するブロック変更:[ %d種類目 ]:[ + F / - G ]\n",pEdit[nNumBlock].nType);
	
	snprintf(&aStrNum[0], sizeof(aStrNum), "ブロックの設置数[%d]\n", nNumBlock);
	snprintf(&aStrKill[0], sizeof(aStrKill), "ブロックを消去:[ BACKSPACE ]\n");
	snprintf(&aStrSet[0], sizeof(aStrSet), "ブロックを設置:[ ENTER ]\n");
	snprintf(&aStrScal[0], sizeof(aStrScal), "ブロックの大きさ変更:[ + V / - B ]\n");
	snprintf(&aStrMove[0], sizeof(aStrMove), "移動:[ WASD ]:高さ変更:[ +↑ / -↓ ]\n");
	snprintf(&aStrSave[0], sizeof(aStrSave), "セーブ[ F7 ]:前回の配置物読み込み[ F8 ] <data/saveEdit.txt>\n");
	snprintf(&aStrCategory[0], sizeof(aStrCategory), "カテゴリー変更:[ 現在のカテゴリー **%d番目** ][ + Y / - U ]\n",pEdit[nNumBlock].EditCategory);
	snprintf(&aStrCamera[0], sizeof(aStrCamera), "注視点移動:[ 右クリック ]:視点移動:[ 左クリック ]:ズーム:[ ホイール ]\n");
	snprintf(&aStrpMove[0], sizeof(aStrpMove), "移動量変更:[ %3.2f ]:[ + H / -J ]\n",pEdit[nNumBlock].fMove);
	snprintf(&aStrObjPos[0], sizeof(aStrObjPos), "選択中のオブジェクトの場所へ移動:[ F6 ]\n");

	//テキストの描画
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
// プレイヤーの情報
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

	snprintf(&aStrPos[0],sizeof(aStrPos), "プレイヤーの位置X:[ %3.2f ] Y:[ %3.2f ] Z:[ %3.2f ]\n", pPlayer->pos.x,pPlayer->pos.y,pPlayer->pos.z);

	snprintf(&aStrState[0], sizeof(aStrState), "プレイヤーの状態[%d]", pPlayer->state);
	snprintf(&aStrMotion[0], sizeof(aStrMotion), "モーションのキー[ %d / %d ]:モーションのフレーム:[ %d / %d ]", nKey, nNumKey, pPlayer->Motion.nCountMotion,pPlayer->Motion.aMotionInfo[motiontype].aKeyInfo[nKey].nFrame);
	snprintf(&aStrBlend[0], sizeof(aStrBlend), "モーションブレンドのキー[ %d ]:モーションブレンドのフレーム:[ %d ]", pPlayer->Motion.nKeyBlend, pPlayer->Motion.nCounterBlend);
	snprintf(&aStrmotiontype[0], sizeof(aStrmotiontype), "現在のモーション:[ %d ] / ブレンドモーション:[ %d ]", pPlayer->Motion.motionType, pPlayer->Motion.motiontypeBlend);

	g_pFont->DrawText(NULL, &aStrPos[0], -1, &rectPos, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrState[0], -1, &rectState, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrMotion[0], -1, &rectMotion, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrBlend[0], -1, &rectBlend, DT_LEFT, COLOR_PINK);
	g_pFont->DrawText(NULL, &aStrmotiontype[0], -1, &rectmotiontype, DT_LEFT, COLOR_PINK);

}
//==============================================================================================================
// エディットモード2dの描画
//==============================================================================================================
void DrawEditmode2d(void)
{
	EDIT_INFO_2D* pEdit2d = GetEditInfo2D(); // ポインタの取得
	int nNumCnt = GetNum2d();                // 配置数の取得

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
		"+   現在のモード[編集モード2d:変更:[3d:F9]::ゲームモード[F2]  +\n"
		"+=============================================================+\n");

	snprintf(&aStrMove[0],sizeof(aStrMove), "移動:[ WASD  Q / E ]");
	snprintf(&aStrNumObj[0], sizeof(aStrMove), "配置数:[ %d ]", nNumCnt);
	snprintf(&aStrDelet[0], sizeof(aStrMove), "消去:[ BACKSPACE ]");
	snprintf(&aStrSet[0], sizeof(aStrMove), "設置:[ ENTER ]");
	snprintf(&aStrscal[0], sizeof(aStrMove), "大きさ変更:横幅:[ ← / →]:高さ:[ ↑ / ↓ ]");
	snprintf(&aStrRot[0], sizeof(aStrMove), "角度変更:X:[ Z / C ]:Y:[ V / B ]:Z:[ F / G ]");
	snprintf(&aStrMouse[0], sizeof(aStrMove), "注視点移動:[ 右クリック ]:視点移動:[ 左クリック ]:ズーム:[ ホイール ]\n");
	snprintf(&aStrFile[0], sizeof(aStrMove), "セーブ[ F7 ]:前回の配置物読み込み:[ F8 ] <data/saveEdit2d.txt>\n");
	snprintf(&aStrObj[0], sizeof(aStrMove), "選択中のオブジェクトの場所へ移動:[ F6 ]\n");

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
// エフェクト編集モードの描画
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
	sprintf(&aStrSize[0], "大きさ変更:[ + F / - G ]:[ %3.2f ]", pEdit->fSize);
	wsprintf(&aStrNum[0], "粒子の数変更:[ + R / - T ]:[ %d ]", pEdit->nNumParticle);

	switch (pEdit->ColorType)
	{
	case COLORCHANGE_R:
		sprintf(&aStrColorType[0], "色変更 : [ + V / - B ] : [ R ]");
		break;
	case COLORCHANGE_G:
		sprintf(&aStrColorType[0], "色変更 : [ + V / - B ] : [ G ]");
		break;
	case COLORCHANGE_B:
		sprintf(&aStrColorType[0], "色変更 : [ + V / - B ] : [ B ]");
		break;
	case COLORCHANGE_A:
		sprintf(&aStrColorType[0], "色変更 : [ + V / - B ] : [ A ]");
		break;
	default:
		break;
	}

	sprintf(&aStrColor[0], "カラーRGBA[ ↑ / ↓ ]:[ %3.2f,%3.2f,%3.2f,%3.2f]", pEdit->col.r, pEdit->col.g, pEdit->col.b, pEdit->col.a);
	sprintf(&aStrMove[0], "移動量:[ + J / - H ]:[ %3.2f ]", pEdit->fSpeed);
	wsprintf(&aStrLife[0], "寿命:[ + K / - L ]:[ %d ]", pEdit->nLife);
	sprintf(&aStrdir[0], "重力:[ X + 1 Y + 2 Z + 3 / X - 4 Y - 5 Z - 6 ]:[ %3.2f,%3.2f,%3.2f ]", pEdit->dir.x, pEdit->dir.y, pEdit->dir.z);
	sprintf(&aStrdecA[0], "A値の減少:[ + N / - M ]:[ %f ]", pEdit->decfAlv);

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
// カメラの編集モード
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
								  "+                 モード切替:[F6]       < EDITMODE >                +\n"
								  "+*******************************************************************+\n");

	snprintf(&aStrBlockComent1[0],sizeof(aStrBlockComent1),"+*******************************************************************+\n"
								   "+                       アニメーションの設定                        +\n"
								   "+*******************************************************************+\n");
	int nType = pCamera->AnimType;
	int nKey = pCamera->nAnimKey;

	if (pCamera->aAnimInfo[nType].bTracking == true)
	{
		snprintf(&aStrTracking[0], sizeof(aStrTracking),"[カメラ追従]:[F3]【オン】\n");
	}
	else if (pCamera->aAnimInfo[nType].bTracking == false)
	{
		snprintf(&aStrTracking[0], sizeof(aStrTracking), "[カメラ追従]:[F3]【オフ】\n");
	}

	snprintf(&aStrrectoperation[0], sizeof(aStrrectoperation), "移動:[ WASD ]:視点移動:[ マウス右 ]:注視点移動:[ マウス左 ]\n");

	snprintf(&aStrType[0], sizeof(aStrType), "種類選択:[ 1 / 2 ]  タイプ【 %d / %d 】", pCamera->AnimType, CAMERAANIM_MAX);


	snprintf(&aStrKey[0], sizeof(aStrKey), "[ キー変更 ]:[ ← / → ]【 %d / %d 】\n", pCamera->nAnimKey, pCamera->aAnimInfo[nType].nNumKey);


	snprintf(&aStrFrame[0], sizeof(aStrFrame), "[ フレーム変更 ]:[ ↑ / ↓ ] 【 %d 】\n", pCamera->aAnimInfo[nType].Anim_KeyInfo[nKey].nAnimFrame);

	snprintf(&aStrSet[0], sizeof(aStrSet), "[ 位置を決定する ]:[ RETURN ]");

	snprintf(&aStrSave[0], sizeof(aStrSave), "[ 保存する ]:[ F7 ] << data\\cameraInfo.txt >>");

	if (pCamera->aAnimInfo[nType].bLoopAnimation == true)
	{
		snprintf(&aStrLoop[0], sizeof(aStrLoop), "[ LOOP ]:[ F8 ]【 ループする 】");
	}
	else if (pCamera->aAnimInfo[nType].bLoopAnimation == false)
	{
		snprintf(&aStrLoop[0], sizeof(aStrLoop), "[ LOOP ]:[ F8 ]【 ループしない 】");
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
