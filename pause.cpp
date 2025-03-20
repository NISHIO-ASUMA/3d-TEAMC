//==============================================================================================================
//
// ポーズ [pause.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"pause.h"
#include"input.h"
#include"game.h"
#include"fade.h"
#include "mouse.h"
#include "mark.h"
#include "math.h"
#include "tutorial3d.h"
#include "sound.h"

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SelectPauseMenuPos(int nSelect);	// 選択中のメニュー
void SetPauseUIAnim(int nType);			// ポーズ中のアニメーション
void CreateUITexPos(int nType);			// UIのテクスチャ座標の作成
void CreateUIPos(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight);			// UIの座標の作成
void CreateUIColor(int nType);			// UIの色設定

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_TEXTURE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ
Pause g_Pause[PAUSE_TEXTURE_MAX];//ポーズメニュー
PAUSE_MENU g_PauseMenu; // ポーズメニュー
bool bSelect;

//=====================================================================================================
//ポーズの初期化処理
//=====================================================================================================
void InitPause(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// ポーズのテクスチャ
	for (int nCnt = 0; nCnt < PAUSE_TEXTURE_MAX; nCnt++)
	{
		// テクスチャ1の読み込み
		D3DXCreateTextureFromFile(pDevice,
			PAUSE_TEXTURE[nCnt],
			&g_apTexturePause[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_TEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	bSelect = false;

	// 頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	
	g_PauseMenu = PAUSE_MENU_CONTINUE; // ポーズメニューの初期化

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		g_Pause[nCntPause].pos = NULLVECTOR3;			 // 位置
		g_Pause[nCntPause].fWidth = 0.0f;				 // 横幅
		g_Pause[nCntPause].fHeight = 0.0f;				 // 高さ

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; // 頂点データのポインタを4つ分増やす
	}
	// アンロック
	g_pVtxBuffPause->Unlock();

	// 背景の設定
	SetPause(D3DXVECTOR3(640.0f, 360.0f, 0.0f), PAUSE_TEXTURE_BG, 640.0f, 360.0f);

	// ポーズメニュー１の設定
	SetPause(D3DXVECTOR3(240.0f, 160.0f, 0.0f), PAUSE_TEXTURE_CONTINUE, 300.0f, 35.0f);

	// ポーズメニュー2の設定
	SetPause(D3DXVECTOR3(240.0f, 360.0f, 0.0f), PAUSE_TEXTURE_RETRY, 300.0f, 35.0f);

	// ポーズメニュー3の設定
	SetPause(D3DXVECTOR3(240.0f, 560.0f, 0.0f), PAUSE_TEXTURE_QUIT, 300.0f, 35.0f);

	// ポーズ中UIの設定
	SetPause(D3DXVECTOR3(1100.0f, 660.0f, 0.0f), PAUSE_TEXTURE_PAUSEUI, 100.0f, 35.0f);

	// クラフトのレシピの設定
	SetPause(D3DXVECTOR3(900.0f, 350.0f, 0.0f), PAUSE_TEXTURE_RECIPE, 200.0f, 200.0f);
}
//=====================================================================================================
//ポーズの終了処理
//=====================================================================================================
void UninitPause(void)
{
	// 音楽を停止
	StopSound();

	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=====================================================================================================
//ポーズの更新処理
//=====================================================================================================
void UpdatePause(void)
{
	switch (g_PauseMenu)
	{
	case PAUSE_MENU_CONTINUE:
		
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN)==true || KeyboardTrigger(DIK_S) == true) && bSelect == false)
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_RETRY; // リトライ選択
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true) && bSelect == false)
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_QUIT; // やめる選択
		}
		SelectPause(PAUSE_MENU_CONTINUE);
		SelectPauseMenuPos(PAUSE_MENU_CONTINUE);

		break;
	case PAUSE_MENU_RETRY:
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true && bSelect == false))
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_QUIT; // やめる選択
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true && bSelect == false))
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_CONTINUE; // 続ける選択	
		}
		SelectPause(PAUSE_MENU_RETRY);
		SelectPauseMenuPos(PAUSE_MENU_RETRY);
		break;

	case PAUSE_MENU_QUIT:
		
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true) && bSelect == false)
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_CONTINUE; // 続ける選択
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true) && bSelect == false)
		{
			// サウンド再生
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_RETRY; // リトライ選択
		}
		SelectPause(PAUSE_MENU_QUIT);
		SelectPauseMenuPos(PAUSE_MENU_QUIT);
		break;
	}

	// 決定したら
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(LEFT_MOUSE) == true) && bSelect == false)
	{// Enterキー or Aボタン or 左クリック

		// サウンド再生
		PlaySound(SOUND_LABEL_ENTER_SE);

		switch (g_PauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			bSelect = false;
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:
			bSelect = true; // 選択した
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			bSelect = true; // 選択した
			SetFade(MODE_TITLE);
			break;
		}
	}

	// ポーズ中のテクスチャアニメーション
	SetPauseUIAnim(PAUSE_TEXTURE_PAUSEUI);
}
//=====================================================================================================
//ポーズの描画処理
//=====================================================================================================
void DrawPause(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポーズのテクスチャ分回す
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		// プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//プリミティブの種類
	}
}
//=====================================================================================================
//ポーズの選択処理
//=====================================================================================================
void SelectPause(int nNumSelect)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// ポーズのテクスチャ分回す
	for (int nCntPause = PAUSE_MENU_CONTINUE; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (nCntPause == nNumSelect)
		{
			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// 頂点カラーの設定
			pVtx[0].col = COLOR_KHAKI;
			pVtx[1].col = COLOR_KHAKI;
			pVtx[2].col = COLOR_KHAKI;
			pVtx[3].col = COLOR_KHAKI;

			// アンロック
			g_pVtxBuffPause->Unlock();
		}
		else
		{
			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// アンロック
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// ポーズの設定処理
//==============================================================================================================
void SetPause(D3DXVECTOR3 pos, int nMenu, float fWidth, float fHeight)
{
	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		if (nMenu == nCntPause)
		{
			g_Pause[nCntPause].pos = pos;			 // 位置
			g_Pause[nCntPause].fWidth = fWidth;		 // 横幅
			g_Pause[nCntPause].fHeight = fHeight;	 // 高さ

			// 座標の作成
			CreateUIPos(nMenu, pos, fWidth, fHeight);

			// 色の作成
			CreateUIColor(nMenu);

			// テクスチャ座標の作成
			CreateUITexPos(nMenu);

			break;
		}
	}
}
//==============================================================================================================
// 選択中のメニュー
//==============================================================================================================
void SelectPauseMenuPos(int nSelect)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	for (int nCntPause = PAUSE_MENU_CONTINUE; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// 位置
		D3DXVECTOR3 pos = g_Pause[nCntPause].pos;

		// 横幅
		float width = g_Pause[nCntPause].fWidth;

		// 高さ
		float height = g_Pause[nCntPause].fHeight;

		if (nCntPause == nSelect)
		{
			pos.x += SetSmoothAprroach(740.0f, pos.x,0.1f);

			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// アンロック
			g_pVtxBuffPause->Unlock();
		}
		else
		{
			pos.x += SetSmoothAprroach(240.0f, pos.x, 0.1f);

			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// アンロック
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// ポーズ中のアニメーション
//==============================================================================================================
void SetPauseUIAnim(int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		if (nType == nCntPause)
		{
			static int nPatternAnim = 0;
			static int nCounterAnim = 0;

			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			// テクスチャアニメーション
			SetTextureAnimation(3, 1, 35, &nCounterAnim, &nPatternAnim, pVtx, g_pVtxBuffPause, nCntPause);

			// アンロック
			g_pVtxBuffPause->Unlock();

			// パターンが0のときずらす
			if (nPatternAnim == 0) g_Pause[nCntPause].pos.x = 1100.0f;

			// パターンが1のときずらす
			if(nPatternAnim == 1) g_Pause[nCntPause].pos.x = 1106.0f;

			// パターンが2のときずらす
			if (nPatternAnim == 2) g_Pause[nCntPause].pos.x = 1116.0f;

			// 位置
			D3DXVECTOR3 pos = g_Pause[nCntPause].pos;

			// 横幅
			float width = g_Pause[nCntPause].fWidth;

			// 高さ
			float height = g_Pause[nCntPause].fHeight;

			// 頂点バッファのロック
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nType;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// アンロック
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// UIのテクスチャ座標の作成
//==============================================================================================================
void CreateUITexPos(int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	if (nType == PAUSE_TEXTURE_PAUSEUI)
	{
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.333f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.333f, 1.0f);
	}
	else
	{
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// アンロック
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================
// UIの座標の作成
//==============================================================================================================
void CreateUIPos(int nType,D3DXVECTOR3 pos,float fWidth,float fHeight)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	// アンロック
	g_pVtxBuffPause->Unlock();

}
//==============================================================================================================
// UIの色設定
//==============================================================================================================
void CreateUIColor(int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	if (nType == PAUSE_TEXTURE_BG)
	{
		// 頂点座標の設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	}
	else
	{
		// 頂点座標の設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	// アンロック
	g_pVtxBuffPause->Unlock();

}
