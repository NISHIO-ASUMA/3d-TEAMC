//=========================================================================================================================
//
// ランキングスコア処理 [ rankingscore.cpp ]
// Author:TEAM_C
//
//=========================================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "RankingScore.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>
#include "Score.h"
#include "game.h"
#include "mouse.h"
#include "math.h"
#include "easing.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_POLYGON (40) // 最大ポリゴン数
#define MAX_DIGIT (8)    // 桁数	
#define MAX_HEIGHT (105.0f) // 高さ
#define MAX_WIDTH (80.0f)	// 横幅
#define NUM_DIGITS_8 (10000000) // 割る最大桁数(8桁)
#define NUM_DIGITS_7 (1000000) // 割る桁数(7桁)

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void UpdateRankScoreAnim(void);		// ランキングスコアのアニメーション処理
void SkipRankScoreAnim(void);		// ランキングスコアのアニメーションのスキップ

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL; // 頂点バッファへのポインタ
RankScore g_RankScore[MAX_RANK];					// ランキングスコア情報
int g_nRankUpdate = -1; // 更新ランクNo,
int g_nTimerRanking;	// ランキング画面表示タイマー
int Avalue;
int Rank;
bool bFlash;
int g_nRankingCount;

//==========================================================================================================
// ランキングスコアの初期化
//==========================================================================================================
void InitRankingScore(void)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	VERTEX_2D* pVtx;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score001.png",
		&g_pTextureRankScore);
	
	// グローバル変数の初期化
	g_nTimerRanking = 0;
	Avalue = 0;
	Rank = MAX_RANK - 1;
	bFlash = false;
	g_nRankingCount = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);

	// 頂点バッファのロック
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// 順位数分回す(5)
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// 構造体変数の初期化
		g_RankScore[nCntRank].pos = D3DXVECTOR3(1400.0f + (nCntRank * 150.0f), 165.0f + (MAX_HEIGHT * nCntRank) + (20.0f * nCntRank), 0.0f);
		g_RankScore[nCntRank].nScore = 0;
		g_RankScore[nCntRank].nEasingCnt = 0;

		// 桁数分回す(8)
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			// 頂点情報の設定
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[3].pos.z = 0.0f;

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			// 頂点情報を進める
			pVtx += 4;
		}
	}

	// アンロック
	g_pVtxBuffRankScore->Unlock();

}
//==========================================================================================================
// ランキングスコアの終了
//==========================================================================================================
void UninitRankingScore(void)
{
	// テクスチャの破棄
	if (g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}
//==========================================================================================================
// ランキングスコアの更新
//==========================================================================================================
void UpdateRankingScore(void)
{
	// カウントを加算
	g_nRankingCount++;

	if ((g_nRankingCount >= 600 || KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_A) || OnMouseTriggerDown(LEFT_MOUSE)) && bFlash == true)
	{// 10秒経過 or Enterキー or Aボタン or 左クリック
		// タイトル画面に遷移
		SetFade(MODE_TITLE);
	}

	// ランキングスコアのアニメーション
	UpdateRankScoreAnim();

	// ランキングスコアのアニメーションスキップ処理
	SkipRankScoreAnim();

	// スキップ
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(LEFT_MOUSE) == true) && bFlash == false)
	{
		VERTEX_2D* pVtx{};       // 頂点情報のポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);


		// 順位分回す
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// 桁数分回す
			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				// 色をもとに戻しておく
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				pVtx += 4;
			}
		}

		bFlash = true; // フラッシュできる

		// 頂点バッファをアンロック
		g_pVtxBuffRankScore->Unlock();
	}
	// ローカル変数
	static int nCounter{};

	// フラッシュできる
	if (g_nRankUpdate != -1 && bFlash == true)
	{
		nCounter++;   // 加算

		// 該当スコアを点滅
		if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       // 頂点情報のポインタ

			// 頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				// 桁数分のカラーを設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);

				// 頂点情報を進める
				pVtx += 4;
			}
			// アンロック
			g_pVtxBuffRankScore->Unlock();
		}
		else if (nCounter == 20)
		{
			VERTEX_2D* pVtx{};       // 頂点情報のポインタ

			// 頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				// 頂点情報を進める
				pVtx += 4;
			}
			// アンロック
			g_pVtxBuffRankScore->Unlock();

			// カウンターを初期化
			nCounter = 0;
		}
	}
}
//==========================================================================================================
// ランキングスコアの描画
//==========================================================================================================
void DrawRankingScore(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < MAX_POLYGON; nCntScore++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRankScore);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//=======================================================================================================
// ランキングのリセット
//=======================================================================================================
void ResetRanking(void)
{
	// ランキングスコア情報の初期設定
	// ファイルを開く
	FILE* pFile;

	pFile = fopen(RANK000, "r");

	//ファイル開けたら
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &g_RankScore[nCnt].nScore);
		}
	}
	else
	{//開けなかったとき
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(RankingScore.cpp)", MB_OK);
		return;
	}

	//ファイル閉じる
	fclose(pFile);

}
///==================================================================================================================
// ランキングの書き出し
//===================================================================================================================
void WriteScore(void)
{
	// ファイルポインタを宣言
	FILE* pFile;

	// ファイルを開く
	pFile = fopen(RANK000, "w");

	// ファイルが開けたら
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			// 順位数分書き出す
			fprintf(pFile, "%d\n", g_RankScore[nCnt].nScore);
		}
	}
	else
	{// 開けなかったとき
		// メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー", MB_OK);
		return;
	}

	//ファイル閉じる
	fclose(pFile);
}
//======================================================================================================
// ランキングの設定
//======================================================================================================
void SetRanking(int nScore)
{// ランキングスコアの並び替え→指定のスコアがランクインしたらg_nRankUpdateを更新
	// スコア保存用の変数
	int nData = {};

	// 順位を初期化
	g_nRankUpdate = -1;

	//5番目のスコアと比較
	if (g_RankScore[MAX_RANK - 1].nScore < nScore)
	{
		g_RankScore[MAX_RANK - 1].nScore = nScore;

		g_nRankUpdate = MAX_RANK - 1;
	}
	
	// 桁数分回す
	for (int nCntRank = 0; nCntRank < MAX_DIGIT; nCntRank++)
	{
		// 順位分
		for (int nCntScore = nCntRank + 1; nCntScore < MAX_RANK; nCntScore++)
		{
			if (g_RankScore[nCntRank].nScore <= g_RankScore[nCntScore].nScore)
			{
				//上書き前のスコアを保存
				nData = g_RankScore[nCntRank].nScore;

				//スコアを上書き
				g_RankScore[nCntRank].nScore = g_RankScore[nCntScore].nScore;

				//上書きしたスコアを保存
				g_RankScore[nCntScore].nScore = nData;
			}
		}
	}
	for (int nCntRankSet = 0; nCntRankSet < MAX_RANK; nCntRankSet++)
	{
		if (nScore == g_RankScore[nCntRankSet].nScore)
		{// 順位のスコアを更新
			g_nRankUpdate = nCntRankSet;

			break;
		}
	}

	// スコア書き出し
	WriteScore();
}
//=============================================================================================================
// ランキングのテクスチャ
//=============================================================================================================
void RankingTexture(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {}; // 各桁の数字を格納する配列
	int aData = NUM_DIGITS_8;     // 8桁の数値を取得するための除数
	int aData2 = NUM_DIGITS_7;    // 7桁目を取得するための除数

	// 頂点バッファをロック
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// ランキングスコアの処理
	for (int nCntScore = 0; nCntScore < MAX_RANK; nCntScore++)
	{
		// 各スコアを桁ごとに分解
		for (int nCntScore1 = 0; nCntScore1 < MAX_DIGIT; nCntScore1++)
		{
			if (nCntScore1 == 0) // 1桁目（最上位桁）の処理
			{
				aPosTexU[0] = g_RankScore[nCntScore].nScore / aData; // 最上位桁を取得
			}
			else
			{
				// 2桁目以降の処理
				aPosTexU[nCntScore1] = g_RankScore[nCntScore].nScore % aData / aData2; // 対応する桁を取得
				aData = aData / 10;   // 除数を10で割って次の桁へ
				aData2 = aData2 / 10; // 7桁目、6桁目...と処理するため更新
			}

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 1.0f);

			// 頂点バッファのポインタを次の位置へ進める
			pVtx += 4;
		}

		// 除数の初期化（次のスコア処理のため）
		aData = NUM_DIGITS_8;
		aData2 = NUM_DIGITS_7;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffRankScore->Unlock();
}
//=========================================================================================================================
// ランキングスコアのアニメーション処理
//=========================================================================================================================
void UpdateRankScoreAnim(void)
{
	if (bFlash == true) return;

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロック
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	// 順位分回す
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// イージングのカウント
		g_RankScore[nCntRank].nEasingCnt++;

		//イージングの時間
		float time[MAX_RANK] = {};

		// 時間を設定
		time[nCntRank] = SetEase(g_RankScore[nCntRank].nEasingCnt,180.0f + (nCntRank * 35.0f));

		// 目的の値に近づける
		g_RankScore[nCntRank].pos.x += SetSmoothAprroach(400.0f, g_RankScore[nCntRank].pos.x, EaseInOutQuad(time[nCntRank]));

		// 桁数分回す
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			// 頂点情報の設定
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
			pVtx[3].pos.z = 0.0f;

			pVtx += 4;
		}

		// ランキングの移動が終わったら
		if (g_RankScore[nCntRank].pos.x <= 410.0f && nCntRank == MAX_RANK - 1)
		{
			bFlash = true;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffRankScore->Unlock();
}
//=========================================================================================================================
// ランキングスコアのアニメーションのスキップ
//=========================================================================================================================
void SkipRankScoreAnim(void)
{
	if (bFlash == true) return;

	// スキップしたら
	if ((KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_A)))
	{
		bFlash = true;

		// 頂点情報のポインタ
		VERTEX_2D* pVtx;

		// 頂点バッファをロック
		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

		// 順位分回す
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// ランクスコアの位置を目的の位置にする
			g_RankScore[nCntRank].pos.x = 400.0f;

			// 桁数分回す
			for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
			{
				// 頂点情報の設定
				pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_WIDTH * 0.5f;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_WIDTH * 0.5f + (MAX_WIDTH * nCntScore);
				pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_WIDTH * 0.5f;
				pVtx[3].pos.z = 0.0f;

				pVtx += 4;
			}
		}

		// 頂点バッファをアンロック
		g_pVtxBuffRankScore->Unlock();
	}
}
