//============================
//
// ポーズ [pause.h]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include"pause.h"
#include"input.h"
#include"game.h"
#include"fade.h"

//****************************
//マクロ定義
//****************************
#define NUM_PAUSE (3)

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_apTexturePause[NUM_PAUSE] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ
PAUSE_MENU g_PauseMenu;//ポーズメニュー

//===================
//ポーズの初期化処理
//===================
void InitPause(void)
{
	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntPause;

	//デバイスの習得
	pDevice = GetDevice();

	//テクスチャ1の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\continue.png",
		&g_apTexturePause[0]);

	//テクスチャ2の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\retry.png",
		&g_apTexturePause[1]);

	//テクスチャ3の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\quit.png",
		&g_apTexturePause[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4* NUM_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(400.0f, (100.0f + nCntPause * 200), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(900.0f, (100.0f + nCntPause * 200), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f, (200.0f + nCntPause * 200), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(900.0f, (200.0f + nCntPause * 200), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分増やす
	}
	g_pVtxBuffPause->Unlock();

	g_PauseMenu = PAUSE_MENU_CONTINUE; // ポーズメニューの初期化
}
//===================
//ポーズの終了処理
//===================
void UninitPause(void)
{
	int nCntPause;

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//テクスチャの破棄
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//===================
//ポーズの更新処理
//===================
void UpdatePause(void)
{
	switch (g_PauseMenu)
	{
	case PAUSE_MENU_CONTINUE:
		
		if (KeyboardTrigger(DIK_DOWN) == true||JoypadTrigger(JOYKEY_DOWN)==true)
		{
			g_PauseMenu = PAUSE_MENU_RETRY; // リトライ選択
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true)
		{
			g_PauseMenu = PAUSE_MENU_QUIT; // やめる選択
		}
		SelectPause(0);
		break;
	case PAUSE_MENU_RETRY:
		if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_PauseMenu = PAUSE_MENU_QUIT; // やめる選択
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTINUE; // 続ける選択	
		}
		SelectPause(1);
		break;

	case PAUSE_MENU_QUIT:
		
		if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTINUE; // 続ける選択
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true)
		{
			g_PauseMenu = PAUSE_MENU_RETRY; // リトライ選択
		}
		SelectPause(2);
		break;
	}

	if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		switch (g_PauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;
		}
	}
}
//===================
//ポーズの描画処理
//===================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの習得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//プリミティブの種類
	}
}
//===================
//ポーズの選択処理
//===================
void SelectPause(int nNumSelect)
{
	VERTEX_2D* pVtx;

	int nCntPause;

	//頂点バッファのロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (nCntPause == nNumSelect)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 140);
		}
		pVtx += 4;
	}
	g_pVtxBuffPause->Unlock();
}


