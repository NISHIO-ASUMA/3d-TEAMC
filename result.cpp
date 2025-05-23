//==============================================================================================================
//
// リザルト処理 [ result.cpp ]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "result.h"
#include "fade.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "resultscore.h"
#include "sound.h"
#include "player.h"
#include "Timer.h"
#include "gameui.h"
#include "mouse.h"

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;// 頂点バッファへのポインタ

//=======================================================================================================
// リザルトの初期化処理
//=======================================================================================================
void InitResult(void)
{
	// UIの初期化
	InitGameUI();
	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// テクスチャのパスを保存
	const char* Texturename = {};

	if (GetPlayer()->bDisp || GetTimer() <= 0)
	{// プレイヤーが生存 or タイマーが0秒以下
		Texturename = "data/TEXTURE/result.png";
	}
	else if (GetPlayer()->bDisp == false)
	{// プレイヤーが死亡
		Texturename = "data/TEXTURE/result1.jpg";
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		Texturename,
		&g_pTextureResult);

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffResult->Unlock();

	// リザルトスコアの初期化処理
	InitResultScore();

	// 音楽を再生
	PlaySound(SOUND_LABEL_RESULT_BGM);
}
//=======================================================================================================
//リザルトの終了処理
//=======================================================================================================
void UninitResult(void)
{
	// 音楽を停止
	StopSound();

	// UIの終了処理
	UninitGameUI();

	// テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// リザルトスコアの終了処理
	UninitResultScore();
}
//=======================================================================================================
// リザルトの更新処理
//=======================================================================================================
void UpdateResult(void)
{
	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_A)==true || OnMouseTriggerDown(LEFT_MOUSE))
	{// Enterキー or パッドのAボタン or 左クリック 
		// ランキングに遷移
		SetFade(MODE_RANKING);
	}

	// リザルトスコアの更新
	UpdateResultScore();
}
//=======================================================================================================
// リザルトの描画処理
//=======================================================================================================
void DrawResult(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// リザルトスコアの描画処理
	DrawResultScore();
}