//==============================================================================================================
//
// タイトル [title.cpp]
// Author: TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "title.h"
#include "input.h"
#include "fade.h"
#include "gameui.h"
#include "sound.h"
#include "mouse.h"
#include "math.h"
#include "easing.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SelectTitle(int select);					   // タイトル画面の選択
void TitleFlash(int state, int nSelect, int nIdx); // タイトルの点滅
void TitleMenuFlash(int nSelect);				   // タイトルメニューの点滅
void TitleMenuAnimation(int nSelect);              // タイトルメニューのアニメーション処理

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// 頂点バッファへのポインタ
TITLE g_Title[TITLETYPE_MAX];							// 構造体変数
bool bModeSet;
int g_EasingCnt = 0;

//==============================================================================================================
// タイトルの初期化処理
//==============================================================================================================
void InitTitle(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TYPE[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// グローバル変数の初期化
	bModeSet = false; 	// 決定ボタンを押したかどうか
	g_EasingCnt = 0;    // イージングカウント

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		g_Title[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_Title[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_Title[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Title[nCnt].bUse = false;						    // 未使用状態
		g_Title[nCnt].fHeight = 0.0f;					    // 横幅
		g_Title[nCnt].fWidth = 0.0f;					    // 高さ
		g_Title[nCnt].nType = TITLETYPE_TITLE;			    // 種類
		g_Title[nCnt].TitleMenu = TITLESELECT_GAME;		    // タイトルのメニュー
		g_Title[nCnt].state = TITLESTATE_NORMAL;		    // 状態

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		// 頂点情報を進める
		pVtx += 4;
	}

	// タイトルをセット
	SetTitle(D3DXVECTOR3(640.0f, 350.0f, 0.0f), TITLETYPE_TITLE, 200.0f, 40.0f);
	SetTitle(D3DXVECTOR3(640.0f, 500.0f, 0.0f), TITLETYPE_TUTO, 200.0f, 40.0f);
	SetTitle(D3DXVECTOR3(640.0f, 650.0f, 0.0f), TITLETYPE_RANKING, 200.0f, 40.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

}
//==============================================================================================================
// タイトルの終了処理
//==============================================================================================================
void UninitTitle(void)
{
	// 音楽をストップ
	StopSound();

	// テクスチャの破棄
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//==============================================================================================================
// タイトルの更新処理
//==============================================================================================================
void UpdateTitle(void)
{
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		switch (g_Title[nCnt].TitleMenu)
		{
		case TITLESELECT_GAME: // ゲームが選ばれている

			if ((KeyboardTrigger(DIK_DOWN) || KeyboardTrigger(DIK_S) || JoypadTrigger(JOYKEY_DOWN)) && bModeSet == false)
			{
				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_TUTO; // メニューチュートリアル

				// イージングカウントを初期化
				g_EasingCnt = 0;
			}
			if ((KeyboardTrigger(DIK_UP) || KeyboardTrigger(DIK_W) || JoypadTrigger(JOYKEY_UP)) && bModeSet == false)
			{
				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_RANKING; // メニューゲーム

				// イージングカウントを初期化
				g_EasingCnt = 0;
			}

			if (g_Title[nCnt].state != TITLESTATE_FLASH)
			{
				TitleMenuFlash(TITLESELECT_GAME); // チュートリアル点滅
			}

			if ((KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_START) || JoypadTrigger(JOYKEY_A) || OnMouseTriggerDown(LEFT_MOUSE)) && bModeSet == false)
			{// Enterキーを押したら
				bModeSet = true;
				// ゲーム画面へ
				SetFade(MODE_GAME);

				// 音楽再生
				PlaySound(SOUND_LABEL_ENTER_SE);

				g_Title[nCnt].state = TITLESTATE_FLASH;
			}
			// 点滅
			FlashGameUI(TITLESELECT_GAME);
			break;

		case TITLESELECT_TUTO: // チュートリアルが選ばれている
			if ((KeyboardTrigger(DIK_DOWN) || KeyboardTrigger(DIK_S) || JoypadTrigger(JOYKEY_DOWN)) && bModeSet == false)
			{
				// イージングカウントを初期化
				g_EasingCnt = 0;

				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_RANKING; // メニューチュートリアル
			}

			if ((KeyboardTrigger(DIK_UP) || KeyboardTrigger(DIK_W) || JoypadTrigger(JOYKEY_UP)) && bModeSet == false)
			{				
				// イージングカウントを初期化
				g_EasingCnt = 0;

				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_GAME; // メニューゲーム
			}

			if (g_Title[nCnt].state != TITLESTATE_FLASH)
			{
				TitleMenuFlash(TITLESELECT_TUTO); // メニューチュートリアル
			}

			if ((KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_START) || JoypadTrigger(JOYKEY_A) || OnMouseTriggerDown(LEFT_MOUSE)) && bModeSet == false)
			{//Enterキーを押したら
				bModeSet = true;
				//チュートリアル画面へ
				SetFade(MODE_TUTORIAL);

				// 音楽再生
				PlaySound(SOUND_LABEL_ENTER_SE);

				g_Title[nCnt].state = TITLESTATE_FLASH; // ゲーム点滅
			}
			// 点滅
			FlashGameUI(TITLESELECT_TUTO);
			break;

		case TITLESELECT_RANKING:
			if ((KeyboardTrigger(DIK_DOWN) || KeyboardTrigger(DIK_S) || JoypadTrigger(JOYKEY_DOWN)) && bModeSet == false)
			{
				// イージングカウントを初期化
				g_EasingCnt = 0;

				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_GAME; // メニューチュートリアル

			}

			if ((KeyboardTrigger(DIK_UP) || KeyboardTrigger(DIK_W) || JoypadTrigger(JOYKEY_UP)) && bModeSet == false)
			{
				// イージングカウントを初期化
				g_EasingCnt = 0;

				// 音楽再生
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_TUTO; // メニューゲーム
			}

			if (g_Title[nCnt].state != TITLESTATE_FLASH)
			{
				TitleMenuFlash(TITLESELECT_RANKING); // メニューチュートリアル
			}

			if ((KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_START) || JoypadTrigger(JOYKEY_A) || OnMouseTriggerDown(LEFT_MOUSE)) && bModeSet == false)
			{// Enterキーを押したら
				// ランキング画面へ
				SetFade(MODE_RANKING);
				// モードを決定した
				bModeSet = true;

				// 音楽再生
				PlaySound(SOUND_LABEL_ENTER_SE);

				g_Title[nCnt].state = TITLESTATE_FLASH; // ゲーム点滅
			}
			// 点滅
			FlashGameUI(TITLESELECT_RANKING);
			break;

		default:
			break;
		}

		// タイトルの点滅処理
		TitleFlash(g_Title[nCnt].state,g_Title[nCnt].TitleMenu,nCnt);

		// タイトルメニューのアニメーション処理
		TitleMenuAnimation(g_Title[nCnt].TitleMenu);
	}

	if (JoypadTrigger(JOYKEY_BACK) == true)
	{// BACKボタンを押した
		// ウィンドウを閉じる
		PostQuitMessage(0);
	}
}
//==============================================================================================================
// タイトルの描画処理
//==============================================================================================================
void DrawTitle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[g_Title[nCnt].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
//==============================================================================================================
// タイトルの設定処理
//==============================================================================================================
void SetTitle(D3DXVECTOR3 pos, int nType,float fWidth, float fHeight)
{	
	// 頂点情報のポインタ
	VERTEX_2D* pVtx; 

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		// 未使用だったら
		if (!g_Title[nCnt].bUse)
		{
			g_Title[nCnt].pos = pos;	 // 座標
			g_Title[nCnt].nType = nType; // 種類
			g_Title[nCnt].bUse = true;	 // 使用判定

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		// 頂点情報を進める
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================================================================================================
// タイトルの選択処理
//==============================================================================================================
void SelectTitle(int select)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (nCnt == select)
		{
			// 頂点カラーの設定 (白)
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		else
		{
			// 頂点カラーの設定(透明度を下げる)
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}

		// 頂点情報を進める
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================================================================================================
// タイトルの点滅処理
//==============================================================================================================
void TitleFlash(int state,int nSelect,int nIdx)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 点滅カウント用のローカル変数
	static int nCounterFlash = 0; 

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (state != TITLESTATE_FLASH)
		{
			continue; // 点滅じゃなかったら飛ばす
		}

		nCounterFlash++; // カウントを加算

		if (nSelect==TITLESELECT_GAME)
		{
			if (nCounterFlash == 5)
			{// カウントが5の時
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}
			else if (nCounterFlash == 10)
			{// カウントが10の時
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// 初期化する
				nCounterFlash = 0;
			}
		}
		else if (nSelect == TITLESELECT_TUTO)
		{
			if (nCounterFlash == 5)
			{// カウントが5の時

				pVtx += 4 * nSelect;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}
			else if (nCounterFlash == 10)
			{// カウントが10の時
				pVtx += 4 * nSelect;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// 初期化
				nCounterFlash = 0;
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================================================================================================
// タイトルのメニューの点滅処理
//==============================================================================================================
void TitleMenuFlash(int nSelect)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// ローカル変数
	static float fA = 1.0f;	  // 透明度
	static bool bAlv = false; // 判定用

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (nSelect == nCnt)
		{
			// falseの時に透明化
			if (fA > 0.3f && !bAlv)
			{
				fA -= 0.005f; // 減算

				if (fA <= 0.3f)
				{
					bAlv = true;
				}
			}
			// trueの時にだんだん見えるようになる
			else if (fA <= 1.0f && bAlv)
			{
				fA += 0.005f; // 加算

				if (fA >= 1.0f)
				{
					bAlv = false;
				}
			}

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
		}
		else
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// 頂点情報を進める
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================================================================================================
// タイトルメニューのアニメーション処理
//==============================================================================================================
void TitleMenuAnimation(int nSelect)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMenu = 0; nCntMenu < TITLETYPE_MAX; nCntMenu++)
	{
		if (nCntMenu == nSelect)
		{
			// イージングのカウント
			g_EasingCnt++;

			// 位置
			D3DXVECTOR3 pos = g_Title[nCntMenu].pos;

			// 横幅
			float fWidth = g_Title[nCntMenu].fWidth;

			// 高さ
			float fHeight = g_Title[nCntMenu].fHeight;

			// イージングの時間
			float  time = SetEase(g_EasingCnt,120);

			// 横幅を拡大
			g_Title[nCntMenu].fWidth += SetSmoothAprroach(250.0f, g_Title[nCntMenu].fWidth, EaseOutQuint(time));

			// 高さを拡大
			g_Title[nCntMenu].fHeight += SetSmoothAprroach(60.0f, g_Title[nCntMenu].fHeight, EaseOutQuint(time));

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
		}
		else
		{
			// 位置
			D3DXVECTOR3 pos = g_Title[nCntMenu].pos;

			// 横幅
			float fWidth = g_Title[nCntMenu].fWidth;

			// 高さ
			float fHeight = g_Title[nCntMenu].fHeight;

			// 横幅を拡大
			g_Title[nCntMenu].fWidth += SetSmoothAprroach(200.0f, g_Title[nCntMenu].fWidth, 0.1f);

			// 高さを拡大
			g_Title[nCntMenu].fHeight += SetSmoothAprroach(40.0f, g_Title[nCntMenu].fHeight, 0.1f);

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
		}

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
