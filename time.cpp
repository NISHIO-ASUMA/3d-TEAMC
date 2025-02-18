//==============================================================================================================
//
// timer [timer.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include"time.h"
#include "boss.h"
#include "game.h"
#include "sound.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_NUM_TIME (2) // タイマーの最大数

//**************************************************************************************************************
// タイマー構造体
//**************************************************************************************************************
typedef struct
{
	int nCountTimeCounter;//0のカウント
	bool bUse;
}TIME;

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTimeMinute = NULL;			// テクスチャへのポインタ分
LPDIRECT3DTEXTURE9 g_pTextureTimeSecond = NULL;			// テクスチャへのポインタ秒
LPDIRECT3DTEXTURE9 g_pTextureTimeCircle = NULL;			// テクスチャへのポインタコロン
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeMinute = NULL;	// 頂点バッファへのポインタ分
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeSecond = NULL;	// 頂点バッファへのポインタ秒
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeCircle = NULL;	// 頂点バッファへのポインタコロン

LPDIRECT3DTEXTURE9 g_pTextureContDown = NULL;			// テクスチャへのポインタコロン
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffContDown = NULL;	// 頂点バッファへのポインタ分

D3DXVECTOR3 g_posTime; // スコアの位置
int g_nCountTime;      // タイムのカウント

int g_nTimerMinute;    // 分
int g_nTimerSecond;    // 秒

TIME g_aTime[MAX_NUM_TIME]; // 最大数分

//=======================================================================================================
//スコアの初期化処理
//=======================================================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイスの取得

	VERTEX_2D* pVtx; // 頂点バッファへのポインタ

	//テクスチャの読み込み(分)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeMinute);

	//テクスチャの読み込み(秒)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureTimeSecond);

	//テクスチャの読み込み(コロン)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\timer_circle.png",
		&g_pTextureTimeCircle);

	//テクスチャの読み込み(コロン)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureContDown);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeMinute, // 分
		NULL);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeSecond, // 秒
		NULL);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeCircle, // コロン
		NULL);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffContDown, // コロン
		NULL);

	// 分のロック
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

	// 頂点バッファのロック
	g_pVtxBuffTimeCircle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 (コロン)
	pVtx[0].pos = D3DXVECTOR3(1090.0f,20.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(1110.0f,20.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(1090.0f,55.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(1110.0f,55.0f,0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffTimeCircle->Unlock();

	// 頂点バッファのロック
	g_pVtxBuffContDown->Lock(0, 0, (void**)&pVtx, 0);

	// 秒の分回す
	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
	g_pVtxBuffContDown->Unlock();

}
//=======================================================================================================
//スコアの終了処理
//=======================================================================================================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTimeMinute != NULL)
	{
		g_pTextureTimeMinute->Release();
		g_pTextureTimeMinute = NULL;
	}

	// 頂点バッファの破棄
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
	// 頂点バッファの破棄
	if (g_pVtxBuffTimeSecond != NULL)
	{
		g_pVtxBuffTimeSecond->Release();
		g_pVtxBuffTimeSecond = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureTimeCircle != NULL)
	{
		g_pTextureTimeCircle->Release();
		g_pTextureTimeCircle = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTimeCircle != NULL)
	{
		g_pVtxBuffTimeCircle->Release();
		g_pVtxBuffTimeCircle = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureContDown != NULL)
	{
		g_pTextureContDown->Release();
		g_pTextureContDown = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffContDown != NULL)
	{
		g_pVtxBuffContDown->Release();
		g_pVtxBuffContDown = NULL;
	}

}
//=======================================================================================================
//スコアの更新処理
//=======================================================================================================
void UpdateTime(void)
{
	VERTEX_2D* pVtx;

	int Min10 = g_nTimerMinute / 10;	  // 分の十の桁
	int Min1 = g_nTimerMinute % 10;		  // 分の一の桁

	int Second10 = g_nTimerSecond / 10;	  // 秒の十の桁
	int Second1 = g_nTimerSecond % 10;	  // 秒の十の桁

	float offpos = 0.1f;				  // テクスチャ座標のオフセット

	GAMESTATE gamestate = GetGameState(); // ゲームの状態を取得

	// ゲームが続いているなら
	if (gamestate != GAMESTATE_END)
	{
		g_nCountTime++; // タイマーカウントを加算

		// 1秒たったら
		if (g_nCountTime >= 60)
		{
			g_nCountTime = 0; // タイマーカウントを初期化

			// 秒が0より大きかったら
			if (g_nTimerSecond > 0)
			{
				g_nTimerSecond--; // 秒数を減らす
			}
			// 秒数が0より小さくなったら
			else if (g_nTimerSecond <= 0)
			{
				// 秒数を59にする
				g_nTimerSecond = 59;

				// 分を一つ減らす
				g_nTimerMinute--;
			}

			// 残り10秒以下なら
			if (g_nTimerMinute <= 0 && g_nTimerSecond <= 10)
			{
				// 音楽再生(仮)
				PlaySound(SOUND_LABEL_SELECT_SE);
			}
		}

		// 60を超えたら
		if (g_nTimerSecond >= 60)
		{
			g_nTimerMinute++; // 分を1増やす

			// 余りが0じゃなかったら(60より上だったら)
			if (g_nTimerSecond % 60 != 0)
			{
				int Time = g_nTimerSecond % 60; // 余りを求める

				g_nTimerSecond = 0; // 0にする
				g_nTimerSecond += Time; // 余りを加算する
			}
			else
			{
				g_nTimerSecond = 0; // 0にする
			}
		}
	}

	// 分のバッファをロック
	g_pVtxBuffTimeMinute->Lock(0, 0, (void**)&pVtx, 0);

	// 残り10秒じゃなかったら

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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

	// 頂点バッファのアンロック
	g_pVtxBuffTimeSecond->Unlock();

	// カウントダウン
	g_pVtxBuffContDown->Lock(0, 0, (void**)&pVtx, 0);

	// 10秒以下になったら
	if (g_nTimerSecond <= 10 && g_nTimerMinute <= 0)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(400.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f, 400.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);

		//頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(400.0f + 250.0f, 200.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(600.0f + 250.0f, 200.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(400.0f + 250.0f, 400.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(600.0f + 250.0f, 400.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 50);

		//頂点カラーの設定
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 50);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 50);

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
	}
	else
	{
		// 秒の分回す
		for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			pVtx += 4;
		}
	}
	// 頂点バッファのアンロック
	g_pVtxBuffContDown->Unlock();

}
//=======================================================================================================
//スコアの描画処理
//=======================================================================================================
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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeSecond, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeCircle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimeCircle);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//プリミティブの種類


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffContDown, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = 0; nCntTime < MAX_NUM_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureContDown);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);//プリミティブの種類
		}
	}


}
//=====================================================================================================================================================================
// タイマーの取得処理
//=====================================================================================================================================================================
int GetTimeMinute(void)
{
	return g_nTimerMinute; // 分の取得
}
//=====================================================================================================================================================================
// タイマーの取得処理
//=====================================================================================================================================================================
int GetTimeSecond(void)
{
	return g_nTimerSecond;
}
//=====================================================================================================================================================================
// タイマー(分)の加算処理
//=====================================================================================================================================================================
void AddTimeMinute(int minute)
{
	g_nTimerMinute += minute;
}
//=====================================================================================================================================================================
// タイマー(秒)の加算処理
//=====================================================================================================================================================================
void AddTimeSecond(int second)
{
	g_nTimerSecond += second;
}
