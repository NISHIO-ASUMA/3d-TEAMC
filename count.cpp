//==============================================================================================================
//
// カウント [count.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "count.h"
#include "mark.h"
#include "game.h"
#include "event.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define ONE_FRAME (60) // 1フレーム
#define MAX_COUNTER (10) // カウンターの最大数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SetColon(int nCnt,D3DXVECTOR3 pos, float fWidth, float fHeight); // コロンの設定処理
void CreateMinute(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight); // 分の作成処理
void CreateSecond(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight); // 秒の作成処理
void DrawSetMinute(int nCnt, LPDIRECT3DDEVICE9 pDevice);                      // 分の描画
void DrawSetSecond(int nCnt, LPDIRECT3DDEVICE9 pDevice);                      // 秒の描画
void DrawSetColon(int nCnt, LPDIRECT3DDEVICE9 pDevice);                       // コロンの描画
void UpdateTextureMinute(int nCnt, int Minute);								  // 分のテクスチャの更新
void UpdateTextureSecond(int nCnt, int Second);								  // 秒のテクスチャの更新
void SetCountDown(int nCnt);												  // カウントダウン
void SetCountUp(int nCnt);                                                    // カウントアップ
void SetEventTimer(int nCnt);												  // イベントのタイマーのカウント
void SetTerritoryTimer(int nCnt);											  // テリトリーのタイマー

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCounter = NULL;//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureColon = NULL;//テクスチャへの
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCounterColon = NULL; // 頂点バッファへのポインタ
Counter g_Counter[MAX_COUNTER];

//==============================================================================================================
// カウントの初期化処理
//==============================================================================================================
void InitCounter(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// テクスチャの読み込みコロン
	D3DXCreateTextureFromFile(pDevice,
		COUNTER_TEXTURE[0],
		&g_pTextureColon);

	// テクスチャの読み込み分、秒
	D3DXCreateTextureFromFile(pDevice,
		COUNTER_TEXTURE[1],
		&g_pTextureCounter);

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COUNTER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCounterColon,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffCounterColon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		g_Counter[nCnt].pos = NULLVECTOR3;
		g_Counter[nCnt].nCountType = COUNTER_COUNTDOWN;
		g_Counter[nCnt].bUse = false;
		g_Counter[nCnt].nMinute = 0;
		g_Counter[nCnt].nSecond = 0;
		g_Counter[nCnt].TimerCnt = 0;
		g_Counter[nCnt].nType = COUNTERTYPE_EVENTTIMER;
		
		//頂点座標の設定
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffCounterColon->Unlock();
}

//==============================================================================================================
// カウントの終了処理
//==============================================================================================================
void UninitCounter(void)
{
	// テクスチャの破棄
	if (g_pTextureColon != NULL)
	{
		g_pTextureColon->Release();
		g_pTextureColon = NULL;
	}

	// テクスチャの破棄
	if (g_pTextureCounter != NULL)
	{
		g_pTextureCounter->Release();
		g_pTextureCounter = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffCounterColon != NULL)
	{
		g_pVtxBuffCounterColon->Release();
		g_pVtxBuffCounterColon = NULL;
	}

	// 全部のカウンター分回す
	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		// 桁数分回す
		for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
		{
			// 頂点バッファの破棄
			if (g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit] != NULL)
			{
				g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit]->Release();
				g_Counter[nCnt].g_pVtxBuffCounter[nCntDigit] = NULL;
			}
		}
	}
}
//==============================================================================================================
// カウントの更新処理
//==============================================================================================================
void UpdateCounter(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	GAMESTATE gamestate = GetGameState();

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		if (g_Counter[nCnt].bUse == false)
		{
			continue;
		}

		// ゲーム状態が普通の時
		if (gamestate == GAMESTATE_NORMAL)
		{
			g_Counter[nCnt].TimerCnt++; // タイマーカウントを加算
		}

		// カウントの種類
		switch (g_Counter[nCnt].nCountType)
		{
		case COUNTER_COUNTDOWN:
			// カウントダウン
			SetCountDown(nCnt);
			break;
		case COUNTER_COUNTUP:
			// カウントアップ
			SetCountUp(nCnt);
			break;
		default:
			break;
		}

		// タイマーの種類
		switch (g_Counter[nCnt].nType)
		{
		case COUNTERTYPE_EVENTTIMER:
			SetEventTimer(nCnt);
			break;
		case COUNTERTYPE_TERRITORY:
			SetTerritoryTimer(nCnt);
			break;
		default:
			break;
		}
		// 分のテクスチャの更新
		UpdateTextureMinute(nCnt, g_Counter[nCnt].nMinute);

		// 秒のテクスチャの更新
		UpdateTextureSecond(nCnt, g_Counter[nCnt].nSecond);
	}
}
//==============================================================================================================
// カウントの描画処理
//==============================================================================================================
void DrawCounter(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		if (g_Counter[nCnt].bUse == false)
		{
			continue;
		}

		// 分の描画設定
		DrawSetMinute(nCnt, pDevice);

		// 秒の描画設定
		DrawSetSecond(nCnt, pDevice);

		// コロンの描画設定
		DrawSetColon(nCnt, pDevice);
	}
}
//==============================================================================================================
// カウントの設定処理
//==============================================================================================================
void SetCounter(D3DXVECTOR3 pos, int nCountType, int nTime, float fWidth, float fHeight,int nType)
{
	// 全部のカウンター分
	for (int nCnt = 0; nCnt < MAX_COUNTER; nCnt++)
	{
		// 未使用だったら
		if (g_Counter[nCnt].bUse == false)
		{
			g_Counter[nCnt].pos = pos;					// 位置を代入
			g_Counter[nCnt].nCountType = nCountType;    // 種類を代入
			g_Counter[nCnt].nMinute = nTime / 60;		// 分を代入
			g_Counter[nCnt].nSecond = nTime % 60;		// 秒を代入
			g_Counter[nCnt].nType = nType;				// 種類
			g_Counter[nCnt].bUse = true;				// 使用状態にする

			// 分の作成
			CreateMinute(nCnt, pos, fWidth, fHeight);

			// コロンの作成
			SetColon(nCnt,D3DXVECTOR3(4.0f * fWidth + pos.x, pos.y, pos.z), fWidth * 0.5f, fHeight * 0.75f);

			// 秒の作成
			CreateSecond(nCnt, pos, fWidth, fHeight);

			break;
		}
	}
}
//==============================================================================================================
// コロンの設定処理
//==============================================================================================================
void SetColon(int nCnt,D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffCounterColon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
	
	// 頂点バッファのアンロック
	g_pVtxBuffCounterColon->Unlock();
}
//==============================================================================================================
// 分の作成処理
//==============================================================================================================
void CreateMinute(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_Counter[nCnt].g_pVtxBuffCounter[0],
		NULL);

	// 頂点バッファのロック
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Lock(0, 0, (void**)&pVtx, 0);

	// 桁数分回す
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);

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

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Unlock();
}
//==============================================================================================================
// 秒の作成処理
//==============================================================================================================
void CreateSecond(int nCnt, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_Counter[nCnt].g_pVtxBuffCounter[1],
		NULL);

	// 頂点バッファのロック
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Lock(0, 0, (void**)&pVtx, 0);

	// 桁数分回す
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(6.0f * fWidth + pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(6.0f * fWidth + pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(6.0f * fWidth + pos.x - fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(6.0f * fWidth + pos.x + fWidth + (nCntDigit * fWidth * 2.0f), pos.y + fHeight, 0.0f);

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

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Unlock();
}
//==============================================================================================================
 // 分の描画
 //==============================================================================================================
void DrawSetMinute(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_Counter[nCnt].g_pVtxBuffCounter[0], 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 桁数分回す
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCounter);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDigit, 2);
	}
}
//==============================================================================================================
 // 秒の描画
 //==============================================================================================================
void DrawSetSecond(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_Counter[nCnt].g_pVtxBuffCounter[1], 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 桁数分回す
	for (int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCounter);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDigit, 2);
	}
}
//==============================================================================================================
 // コロンの描画
 //==============================================================================================================
void DrawSetColon(int nCnt, LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCounterColon, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureColon);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
}
//==============================================================================================================
 // 分のテクスチャの更新
 //==============================================================================================================
void UpdateTextureMinute(int nCnt,int Minute)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 分の十の桁
	int MinTensPlace = Minute / 10;

	// 分の一の桁
	int MinFirstPlace = Minute % 10;

	// 頂点バッファのロック
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.1f * MinTensPlace, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * MinTensPlace + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * MinTensPlace, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * MinTensPlace + 0.1f, 1.0f);

	// テクスチャ設定
	pVtx[4].tex = D3DXVECTOR2(0.1f * MinFirstPlace, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.1f * MinFirstPlace + 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.1f * MinFirstPlace, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.1f * MinFirstPlace + 0.1f, 1.0f);


	// 頂点バッファのアンロック
	g_Counter[nCnt].g_pVtxBuffCounter[0]->Unlock();
}
//==============================================================================================================
 // 秒のテクスチャの更新
 //==============================================================================================================
void UpdateTextureSecond(int nCnt, int Second)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 秒の十の桁
	int SecondTensPlace = Second / 10;

	// 秒の一の桁
	int SecondFirstPlace = Second % 10;

	// 頂点バッファのロック
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.1f * SecondTensPlace, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * SecondTensPlace + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * SecondTensPlace, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * SecondTensPlace + 0.1f, 1.0f);

	// テクスチャ設定
	pVtx[4].tex = D3DXVECTOR2(0.1f * SecondFirstPlace, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.1f * SecondFirstPlace + 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.1f * SecondFirstPlace, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.1f * SecondFirstPlace + 0.1f, 1.0f);

	// 頂点バッファのアンロック
	g_Counter[nCnt].g_pVtxBuffCounter[1]->Unlock();
}
//==============================================================================================================
// カウントダウンの設定
//==============================================================================================================
void SetCountDown(int nCnt)
{
	// 1秒たったら
	if (g_Counter[nCnt].TimerCnt >= 60)
	{
		g_Counter[nCnt].TimerCnt = 0; // タイマーカウントを初期化

		// 秒が0より大きかったら
		if (g_Counter[nCnt].nSecond > 0)
		{
			g_Counter[nCnt].nSecond--; // 秒数を減らす
		}
		// 秒数が0より小さくなったら
		else if (g_Counter[nCnt].nSecond <= 0 && g_Counter[nCnt].nMinute != 0)
		{
			// 秒数を59にする
			g_Counter[nCnt].nSecond = 59;

			// 分を一つ減らす
			g_Counter[nCnt].nMinute--;
		}
	}

	// 60を超えたら
	if (g_Counter[nCnt].nSecond >= 60)
	{
		g_Counter[nCnt].nMinute++; // 分を1増やす

		// 余りが0じゃなかったら(60より上だったら)
		if (g_Counter[nCnt].nSecond % 60 != 0)
		{
			int Time = g_Counter[nCnt].nSecond % 60; // 余りを求める

			g_Counter[nCnt].nSecond = 0; // 0にする
			g_Counter[nCnt].nSecond += Time; // 余りを加算する
		}
		else
		{
			g_Counter[nCnt].nSecond = 0; // 0にする
		}
	}	
}
//==============================================================================================================
// カウントアップの設定
//==============================================================================================================
void SetCountUp(int nCnt)
{
	// 1秒たったら
	if (g_Counter[nCnt].TimerCnt >= 60)
	{
		g_Counter[nCnt].TimerCnt = 0; // タイマーカウントを初期化

		// 秒が0より大きかったら
		if (g_Counter[nCnt].nSecond < 59)
		{
			g_Counter[nCnt].nSecond++; // 秒数を減らす
		}
		// 秒数が0より小さくなったら
		else if (g_Counter[nCnt].nSecond >= 59 && g_Counter[nCnt].nMinute != 599)
		{
			// 秒数を59にする
			g_Counter[nCnt].nSecond = 0;

			// 分を一つ減らす
			g_Counter[nCnt].nMinute++;
		}
	}
}
//==============================================================================================================
// イベントのタイマーのカウント
//==============================================================================================================
void SetEventTimer(int nCnt)
{
	if (g_Counter[nCnt].nType == COUNTERTYPE_EVENTTIMER)
	{
		if (EnableEvent() == false)
		{
			g_Counter[nCnt].bUse = false;
		}
	}
}
//==============================================================================================================
// テリトリーのタイマー
//==============================================================================================================
void SetTerritoryTimer(int nCnt)
{
	// カウントが終了したら
	if (g_Counter[nCnt].nMinute <= 0 && g_Counter[nCnt].nSecond <= 0)
	{
		// 未使用にスル
		g_Counter[nCnt].bUse = false;
	}
}
