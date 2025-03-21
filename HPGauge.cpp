//=====================================================================================================================
//
// HPゲージのプログラム
// Author;長尾悠成
//
//=====================================================================================================================

//**********************************************************************************************************************
// インクルードファイル
//**********************************************************************************************************************
#include "player.h"
#include "HPGauge.h"
#include "sound.h"
#include "easing.h"
#include "game.h"

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define NUM_HPGAUGE (3) // HPゲージのUIの数
#define NUM_FEVERGAUGE (2) // FEVERGAUGEの数
#define MAX_HPLENGTH (264.0f) // HPゲージの横幅
#define FRAME (0)             // フレームのカウント
#define FEVER_LENGTH (800.0f) // フィーバーゲージの長さ
#define FEVERGAGE_LENGTH (300.0f)

//**********************************************************************************************************************
// グローバル変数宣言
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTexture_Gauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_RedGauge = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Frame = NULL;

LPDIRECT3DTEXTURE9 g_pTexture_Fevframe = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

// 各ゲージの計算用変数
float g_fLength;
float g_fMaxLength;
float g_fPer;
float g_RedLength;
float g_EaseCnt;
float fDestPer;

// フィーバーゲージ用変数
float g_fFeverCharge;
bool g_bFeverON;

//===================================================================================================
// ゲージの初期化処理
//===================================================================================================
void InitGauge(void)
{
	g_fFeverCharge = 0;
	g_bFeverON = false;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// HPのフレームとゲージのテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifeframe.png", &g_pTexture_Frame);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_red.png", &g_pTexture_RedGauge);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\lifebar_gage.png", &g_pTexture_Gauge);

	// フィーバーのフレームとゲージのテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\feverframe2.png", &g_pTexture_Fevframe);
	
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_HPGAUGE * NUM_FEVERGAUGE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);
	VERTEX_2D* pVtx;

	// 位置の調整、都合の良い場所探して下しあ(上からHP枠、HPゲージ、フィーバー枠、フィーバーゲージ)
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();


	float fLeftHP = (float)pPlayer->nLife;
	float fMaxHP = (float)pPlayer->nMaxLife;

	// そこからその割合を計算し長さにする
	float fPer = fLeftHP / fMaxHP;

	float fLength = fPer * MAX_HPLENGTH;

	g_RedLength = 0.0f;
	g_EaseCnt = 0.0f;
	fDestPer = 0.0f;
	g_fLength = 0.0f;
	g_fMaxLength = 0.0f;
	g_fPer = 0.0f;

	// 各ゲージの計算用変数

	g_fFeverCharge = 0.0f;
	g_bFeverON = 0.0f;

	// HPゲージ分回す
	for (int nCnt = 0; nCnt < NUM_HPGAUGE; nCnt++)
	{
		// Frame
		if (nCnt == FRAME)
		{
			// FRAMEの頂点座標
			pVtx[0].pos = D3DXVECTOR3(5.0f, 9.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(390.0f + 5.0f, 9.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(5.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(390.0f + 5.0f, 70, 0.0f);
		}
		// gauge
		else if(nCnt != FRAME)
		{
			// GAUGEの頂点座標
			pVtx[0].pos = D3DXVECTOR3(10.0f, 15.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fLength, 15.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(10.0f, 26.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fLength, 26.0f, 0.0f);
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// フィーバーゲージ分回す
	for (int nCnt = 0; nCnt < NUM_FEVERGAUGE; nCnt++)
	{
		// Frame
		if (nCnt == FRAME)
		{
			// FRAMEの頂点座標
			pVtx[0].pos = D3DXVECTOR3(300.0f, 630.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(FEVER_LENGTH, 630.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(300.0f, 710.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(FEVER_LENGTH, 710.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = COLOR_WHITE;
			pVtx[1].col = COLOR_WHITE;
			pVtx[2].col = COLOR_WHITE;
			pVtx[3].col = COLOR_WHITE;
		}
		// gauge
		else if (nCnt != FRAME)
		{
			// GAUGEの頂点座標
			pVtx[0].pos = D3DXVECTOR3(300.0f, 630.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(FEVERGAGE_LENGTH, 630.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(300.0f, 710.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(FEVERGAGE_LENGTH, 710.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = COLOR_WHITE;
			pVtx[1].col = COLOR_WHITE;
			pVtx[2].col = COLOR_WHITE;
			pVtx[3].col = COLOR_WHITE;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffGauge->Unlock();
}

//===================================================================================================
// Uninit処理色々
//===================================================================================================
void UninitGauge(void)
{
	// テクスチャの破棄
	if (g_pTexture_Frame != NULL)
	{
		g_pTexture_Frame->Release();
		g_pTexture_Frame = NULL;
	}
	// テクスチャの破棄
	if (g_pTexture_Gauge != NULL)
	{
		g_pTexture_Gauge->Release();
		g_pTexture_Gauge = NULL;
	}
	// テクスチャの破棄
	if (g_pTexture_RedGauge != NULL)
	{
		g_pTexture_RedGauge->Release();
		g_pTexture_RedGauge = NULL;
	}

	// テクスチャの破棄
	if (g_pTexture_Fevframe != NULL)
	{
		g_pTexture_Fevframe->Release();
		g_pTexture_Fevframe = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//===================================================================================================
// 更新処理色々
//===================================================================================================
void UpdateGauge(void)
{
	// 残りHPと最大HPを測り反映
	float fLeftHP;
	float fMaxHP;

	Player* pPlayer = GetPlayer();

	fLeftHP = (float)pPlayer->nLife;
	fMaxHP = (float)pPlayer->nMaxLife;

	// そこからその割合を計算し長さにする
	g_fPer = fLeftHP / fMaxHP;
	g_fLength = g_fPer * MAX_HPLENGTH;

	if (pPlayer->state == PLAYERSTATE_DAMAGE)
	{
		g_EaseCnt = 0;
	}
	else
	{
		g_EaseCnt++;
	}

	//// テクスチャ座標の割合
	//fDestPer = g_fPer;


	float t = SetEase(g_EaseCnt, 120.0f);

	// 赤ゲージを緑ゲージに近づける
	g_RedLength += (g_fLength - g_RedLength) * EaseInCubic(t);

	// 赤ゲージを緑ゲージに近づける
	fDestPer += (g_fPer - fDestPer) * EaseInCubic(t);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	VERTEX_2D* pVtx;

	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	// FRAMEの頂点座標
	pVtx[4].pos = D3DXVECTOR3(111.0f, 43.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(111.0f + g_RedLength, 43.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(111.0f, 69.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(111.0f + g_RedLength, 69.0f, 0.0f);

	// 赤ゲージテクスチャ座標の更新
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(fDestPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(fDestPer, 1.0f);

	// FRAMEの頂点座標
	pVtx[8].pos = D3DXVECTOR3(111.0f, 43.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(111.0f + g_fLength, 43.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(111.0f, 69.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(111.0f + g_fLength, 69.0f, 0.0f);

	// ゲージのテクスチャ座標の更新
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(g_fPer, 1.0f);

	g_pVtxBuffGauge->Unlock();

	// ここからはフィーバー
	
	// そこからその割合を計算し長さにする、今は100でチャージ完了
	g_fPer = g_fFeverCharge / 100.0f;
	g_fLength = g_fPer * FEVERGAGE_LENGTH;

	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 12;

	// 頂点カラーの設定
	pVtx[4].col = COLOR_DEEPPINK;
	pVtx[5].col = COLOR_DEEPPINK;
	pVtx[6].col = COLOR_DEEPPINK;
	pVtx[7].col = COLOR_DEEPPINK;

	// GAUGEの頂点座標
	pVtx[4].pos = D3DXVECTOR3(470.0f, 660.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(470.0f + g_fLength, 660.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(470.0f, 687.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(470.0f + g_fLength, 687.0f, 0.0f);

	// テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);

	g_pVtxBuffGauge->Unlock();

	GAMESTATE gameState = GetGameState();

	// フィーバーゲージの処理
	if (g_fFeverCharge >= 100.0f && pPlayer->FeverMode == false)
	{
		g_fFeverCharge = 100.0f;
		PlaySound(SOUND_LABEL_FEVER);
		pPlayer->FeverMode = true;
	}
	else if (pPlayer->FeverMode == true && gameState != GAMESTATE_MOVIE)
	{
		g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[4].col = COLOR_YELLOW;
		pVtx[5].col = COLOR_YELLOW;
		pVtx[6].col = COLOR_YELLOW;
		pVtx[7].col = COLOR_YELLOW;

		g_pVtxBuffGauge->Unlock();

		g_fFeverCharge -= 0.2f;
		if (g_fFeverCharge <= 0.0f)
		{
			g_fFeverCharge = 0.0f;
			pPlayer->FeverMode = false;
		}
	}
}

//===================================================================================================
// 描写処理色々
//===================================================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイス取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexture_Frame);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTexture(0, g_pTexture_RedGauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	pDevice->SetTexture(0, g_pTexture_Gauge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

	pDevice->SetTexture(0, g_pTexture_Fevframe);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);

	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
}

//===================================================================================================
// 変更処理色々...でもほぼ同じだしもう要らないかも、一応保管
//===================================================================================================
void SetGauge(void)
{
	float fLeftHP;
	float fMaxHP;
	Player* pPlayer = GetPlayer();
	fLeftHP = (float)pPlayer->nLife;
	fMaxHP = (float)pPlayer->nMaxLife;

	g_fPer = fLeftHP / fMaxHP;
	g_fLength = g_fPer * 940;
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[4].pos = D3DXVECTOR3(20.0f, 15.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_fLength + 20.0f, 15.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f, 26.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_fLength + 20.0f, 26.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(g_fPer, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(g_fPer, 1.0f);
	g_pVtxBuffGauge->Unlock();
}

void AddFever(float Add)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->FeverMode == false)
	{
		g_fFeverCharge += Add;
	}
}
