//============================
//
// timer [timer.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// インクルードファイル
//****************************
#include"time.h"

//****************************
// マクロ定義
//****************************
#define MAX_NUM_TIME (2) // タイマーの最大数

//****************************
// タイマー構造体
//****************************
typedef struct
{
	int nCountTimeCounter;//0のカウント
	bool bUse;
}TIME;

//****************************
// グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureTimeMinute = NULL;			// テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimeSecond = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeMinute = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeSecond = NULL;	// 頂点バッファへのポインタ

D3DXVECTOR3 g_posTime; // スコアの位置
int g_nCountTime;      // タイムのカウント

int g_nTimerMinute;    // 分
int g_nTimerSecond;    // 秒

TIME g_aTime[MAX_NUM_TIME]; // 最大数分

//=====================
//スコアの初期化処理
//=====================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeMinute);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeSecond);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeMinute,
		NULL);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeSecond,
		NULL);

	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_NUM_TIME; nCnt1++)
	{
		g_aTime[nCnt1].nCountTimeCounter = 0;
		g_aTime[nCnt1].bUse = true;
	}
	g_nTimerMinute = 1; // 分を初期化
	g_nTimerSecond = 40; // 秒を初期化
	g_nCountTime = 0;   // タイムのカウントを初期化

	// 分の頂点座標
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f + 30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1020.0f + nCntTime * 30.0f + 30.0f, 60.0f, 0.0f);
				

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTimeMinute->Unlock();

	// 頂点バッファのロック
	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	// 秒の分回す
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f + 30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1120.0f + nCntTime * 30.0f + 30.0f, 60.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffTimeSecond->Unlock();

}
//=====================
//スコアの終了処理
//=====================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTimeMinute != NULL)
	{
		g_pTextureTimeMinute->Release();
		g_pTextureTimeMinute = NULL;
	}
	if (g_pVtxBuffTimeMinute != NULL)
	{
		g_pVtxBuffTimeMinute->Release();
		g_pVtxBuffTimeMinute = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureTimeSecond != NULL)
	{
		g_pTextureTimeSecond->Release();
		g_pTextureTimeSecond = NULL;
	}
	if (g_pVtxBuffTimeSecond != NULL)
	{
		g_pVtxBuffTimeSecond->Release();
		g_pVtxBuffTimeSecond = NULL;
	}

}
//=====================
//スコアの更新処理
//=====================
void UpdateTime(void)
{
	VERTEX_2D* pVtx;

	int Min10 = g_nTimerMinute / 10;
	int Min1 = g_nTimerMinute % 10;

	int Second10 = g_nTimerSecond / 10;
	int Second1 = g_nTimerSecond % 10;

	float offpos = 0.1f;

	g_nCountTime++;

	if (g_nCountTime >= 60)
	{
		g_nCountTime = 0;

		if (g_nTimerSecond > 0)
		{
			g_nTimerSecond--;
		}
		else if(g_nTimerSecond <= 0)
		{
			g_nTimerSecond = 59;
			g_nTimerMinute--;
		}
	}
	
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(offpos * Min10,0.0f);
	pVtx[1].tex = D3DXVECTOR2(offpos * Min10 + offpos,0.0f);
	pVtx[2].tex = D3DXVECTOR2(offpos * Min10,1.0f);
	pVtx[3].tex = D3DXVECTOR2(offpos * Min10 + offpos,1.0f);
	
	//テクスチャの設定
	pVtx[4].tex = D3DXVECTOR2(offpos * Min1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(offpos * Min1 + offpos, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(offpos * Min1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(offpos * Min1 + offpos, 1.0f);

	g_pVtxBuffTimeMinute->Unlock();

	g_pVtxBuffTimeSecond->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(offpos * Second10, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(offpos * Second10 + offpos, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(offpos * Second10, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(offpos * Second10 + offpos, 1.0f);

	//テクスチャの設定
	pVtx[4].tex = D3DXVECTOR2(offpos * Second1, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(offpos * Second1 + offpos, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(offpos * Second1, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(offpos * Second1 + offpos, 1.0f);

	g_pVtxBuffTimeSecond->Unlock();

}
//=====================
//スコアの描画処理
//=====================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの習得
	pDevice = GetDevice();

	int nCntTime;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeMinute, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTimeMinute);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//プリミティブの種類
		}
	}

	pDevice->SetStreamSource(0, g_pVtxBuffTimeSecond, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTimeSecond);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//プリミティブの種類
		}
	}

}
//=================
// タイマーの取得処理
//=================
int GetTimeMinute(void)
{
	return g_nTimerMinute;
}
//=================
// タイマーの取得処理
//=================
int GetTimeSecond(void)
{
	return g_nTimerSecond;
}