//==========================
//
// 3Dゲーム基盤 [main.cpp]
// Author: TEAM_C
//
//==========================

//****************************
// インクルードファイル宣言
//****************************
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

//*****************************
// プロトタイプ宣言
//*****************************
void DrawMode(void); // 現在の画面の表示
void DrawOperation(void); // 操作方法
void DrawEditMode(void);

//*****************************
// グローバル変数宣言
//*****************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;				// 現在のモード
bool g_isFullscreen = false;			// ウィンドウを切り替えるためのフラグ
RECT g_windowRect;						// ウィンドウを切り替えるための変数
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = 0;					// FPSカウンタ
int nGetNumber = 0;						// 番号

//************************************************
// ウィンドウをフルスクリーンに変える処理
//************************************************
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
//================================
// メイン関数
//================================
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

	// TODO : 初期状態をフルスクリーンにしたいとき
	//ShowWindow(hWnd, nCmdShow);			//ウインドウの表示状態の設定
	//UpdateWindow(hWnd);					//クライアント領域の更新

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
//=======================================
// ウインドウプロシージャ
//=======================================
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
//=========================
// 初期化処理
//=========================
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

	//=================
	// 各種初期化処理
	//=================

	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	if (FAILED(InitMouse(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	// デバッグ表示用のフォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// フェードの初期化
	InitFade(g_mode);

	// モードの設定
	SetMode(g_mode);

	return S_OK;
}
//==================
// 終了処理
//==================
void Uninit(void)
{
	//==================
	// 各種終了処理
	//==================
	
	// キーボードの終了
	UninitKeyboard();

	// ジョイパッドの終了
	UninitJoypad();

	//マウスの終了
	UninitMouse();

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
//===================
// 更新処理
//===================
void Update(void)
{
	//==========================
	// 各種オブジェクト更新処理
	//==========================

	// キーボードの更新
	UpdateKeyboard();

	// ジョイパッドの更新
	UpdateJoypad();

	//マウスの更新
	UpdateMouse();

	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UpdateTitle3d();
		break;

	case MODE_TUTORIAL://チュートリアル画面
		UpdateTutorial3d();
		break;

	case MODE_GAME://ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT://リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING://ランキング画面
		UpdateRanking();
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
//===================
// 描画処理
//===================
void Draw(void)
{
	Camera* pCamera = GetCamera();

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		g_pD3DDevice->SetViewport(&pCamera[nCnt].viewport); // ビューポートの設定
		
		//画面クリア(バックバッファ&Zバッファのクリア)
		g_pD3DDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//描画開始
		if (SUCCEEDED(g_pD3DDevice->BeginScene()))
		{
			SetCamera(nCnt); // カメラをセット

			// 描画成功時
			//================
			// 描画処理
			//================
			// 現在の画面の終了
			switch (g_mode)
			{
			case MODE_TITLE: // タイトル画面
				DrawTitle3d();
				break;

			case MODE_TUTORIAL:// チュートリアル画面
				DrawTutorial3d();
				break;

			case MODE_GAME: // ゲーム画面
				DrawGame();
				break;
			case MODE_RESULT: // リザルト
				DrawResult();
				break;

			case MODE_RANKING: // ランキング画面
				DrawRanking();
				break;
			}

#ifdef _DEBUG

			if (!GetEditState())
			{
				//現在の画面の表示
				DrawMode();

				//操作方法
				DrawOperation();
			}
			else if (GetEditState())
			{
				DrawEditMode();
			}
			// プレイヤーの座標表示
			//DrawDebugPlayerPos();

#endif

			// フェードの描画
			DrawFade();

			// 描画終了
			g_pD3DDevice->EndScene();

			if (g_mode == MODE_TITLE || g_mode == MODE_TUTORIAL)
			{
				break; // 一つ目をセットしたら抜ける
			}
		}
	}
		// バックバッファとフロントバッファの入れ替え
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//======================
// FPSの表示
//======================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//文字列に代入
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//============================
// 敵のデバッグ表示
//============================
void DrawEnemySet(int g_nNumEnemy)
{
	// ローカル変数
	RECT rect = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// 文字列に代入
	wsprintf(&aString[0], "敵の総数:%d\n", g_nNumEnemy);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
//==============================
// プレイヤーの座標表示
//==============================
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
//========================
// カメラ
//========================
void DrawCameraPos(void)
{
	// ローカル変数
	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];

	// 取得
	Camera* pCamera = GetCamera();

	// 文字列に代入
	sprintf(&aString[0], "カメラのrot.y:%.2f\n", pCamera->rot.y);
	sprintf(&aString1[0], "カメラのrot.x:%.2f\n", pCamera->rot.x);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//========================
// カメラの種類
//========================
void DrawCameraType(void)
{
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];
	char aString1[256];

	// 取得
	Camera* pCamera = GetCamera();

	if (pCamera->g_CameraMode == CAMERAMODE_NONE)
	{
		strcpy(&aString1[0], "NORMAL");
	}
	else if (pCamera->g_CameraMode == CAMERAMODE_PLAYER)
	{
		strcpy(&aString1[0], "PLAYER");
	}

	//文字列に代入
	wsprintf(&aString[0], "現在のモード[F2] : %s\n",&aString1[0]);

	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

}
//======================
// モードの設定
//======================
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
	}
}
//======================
// デバイスの取得
//======================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//====================
// モードの取得
//====================
MODE GetMode(void)
{
	return g_mode; // 変数を返す
}
//=====================
// ワイヤーフレームON
//=====================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=====================
// ワイヤーフレームOFF
//=====================
void offWireFrame() 
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}


// FPSの表示
//DrawFPS();

//// カメラ
//DrawCameraPos();

//// プレイヤーの座標
//DrawDebugPlayerPos();

//// カメラの種類
//DrawCameraType();

//===========================
// 現在の画面の表示
//===========================
void DrawMode(void)
{
	RECT rectState = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectMode = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128];
	char aStrState[256];

	wsprintf(&aStrState[0],
		"+=================================================+\n"
		"+   現在のモード[プレイモード]::編集モード[F2]    +\n"
		"+=================================================+\n");

	switch (g_mode)
	{
	case MODE_TITLE:
		//文字列に代入
		wsprintf(&aStr[0], "現在のモード[ タイトル ]\n");
		break;
	case MODE_TUTORIAL:
		//文字列に代入
		wsprintf(&aStr[0], "現在のモード[ チュートリアル ]\n");
		break;
	case MODE_GAME:
		//文字列に代入
		wsprintf(&aStr[0], "現在のモード[ ゲーム ]\n");
		break;
	case MODE_RESULT:
		//文字列に代入
		wsprintf(&aStr[0], "現在のモード[ リザルト ]\n");
		break;
	case MODE_RANKING:
		//文字列に代入
		wsprintf(&aStr[0], "現在のモード[ ランキング ]\n");
		break;
	default:
		break;
	}

	g_pFont->DrawText(NULL, &aStrState[0], -1, &rectState, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr[0], -1, &rectMode, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//===========================
// 操作方法表示
//===========================
void DrawOperation(void)
{
	RECT rectPlayer = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectThrow = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[128];
	char aStrthrow[128];

	wsprintf(&aStr[0], "移動 [ WASD ]\n");
	wsprintf(&aStrthrow[0],"ブロック :拾う[ L ]:飛ばす[ ENTER ]:置く[ O ]\n");

	g_pFont->DrawText(NULL, &aStrthrow[0], -1, &rectThrow, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//===========================
// エディットモード
//===========================
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

	wsprintf(&aStrGame[0],
		"+=================================================+\n"
		"+   現在のモード[編集モード]::ゲームモード[F2]    +\n"
		"+=================================================+\n");

	wsprintf(&aStrType[0], "設置するブロック変更:[ %d種類目 ]:[ + F / - G ]\n",pEdit[nNumBlock].nType);
	
	wsprintf(&aStrNum[0], "ブロックの設置数[%d]\n", nNumBlock);
	wsprintf(&aStrKill[0], "ブロックを消去:[ BACKSPACE ]\n");
	wsprintf(&aStrSet[0], "ブロックを設置:[ ENTER ]\n");
	wsprintf(&aStrScal[0], "ブロックの大きさ変更:[ + V / - B ]\n");
	wsprintf(&aStrMove[0], "移動:[ WASD ]:高さ変更:[ +↑ / -↓ ]\n");
	wsprintf(&aStrSave[0], "セーブ[ F7 ]:前回の配置物読み込み[ F8 ] <data/saveEdit.txt>\n");
	wsprintf(&aStrCategory[0], "カテゴリー変更:[ 現在のカテゴリー **%d番目** ][ + Y / - U ]\n",pEdit[nNumBlock].EditCategory);
	wsprintf(&aStrCamera[0], "注視点移動:[ 右クリック ]:視点移動:[ 左クリック ]:ズーム:[ ホイール ]\n");
	sprintf(&aStrpMove[0], "移動量変更:[ %3.2f ]:[ + H / -J ]\n",pEdit[nNumBlock].fMove);

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
}
