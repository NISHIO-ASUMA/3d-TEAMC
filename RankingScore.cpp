//============================
//
// ランキングスコア [rankingscore.cpp]
// Author:TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
#include "RankingScore.h"
#include"ranking.h"
#include"input.h"
#include "fade.h"
#include <stdio.h>
#include"Score.h"
#include "game.h"

//****************************
//マクロ定義
//****************************
#define MAX_POLYGON (40)//最大ポリゴン数
#define MAX_DIGIT (8)//桁数	
#define MAX_HEIGHT (69.0f)
#define MAX_HABA (50.0f)

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;//テクスチャへのポインタ//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL;//頂点バッファへのポインタ
RankScore g_RankScore[MAX_RANK];//ランキングスコア情報
END g_End;
int g_nRankUpdate = -1;//更新ランクNo,
int g_nTimerRanking;//ランキング画面表示タイマー
int g_state;
//========================
//ランキングスコアの初期化
//========================
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み(スコア)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score000.png",
		&g_pTextureRankScore);
	
	//初期化
	//g_nRankUpdate = -1;
	g_nTimerRanking = 0;
	g_state = 0;

	//頂点バッファの生成・頂点情報の設定//スコア
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);


	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	//順位数分回す(5)
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		g_RankScore[nCntRank].pos = D3DXVECTOR3(500.0f, 150.0f + (MAX_HEIGHT * nCntRank) + (20.0f * nCntRank), 0.0f);
		g_RankScore[nCntRank].nScore = 0;

		//桁数分回す(8)
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			pVtx[0].pos.x = g_RankScore[nCntRank].pos.x - MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[0].pos.y = g_RankScore[nCntRank].pos.y - MAX_HABA * 0.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_RankScore[nCntRank].pos.x + MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[1].pos.y = g_RankScore[nCntRank].pos.y - MAX_HABA * 0.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_RankScore[nCntRank].pos.x - MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[2].pos.y = g_RankScore[nCntRank].pos.y + MAX_HABA * 0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_RankScore[nCntRank].pos.x + MAX_HABA * 0.5f + (MAX_HABA * nCntScore);
			pVtx[3].pos.y = g_RankScore[nCntRank].pos.y + MAX_HABA * 0.5f;
			pVtx[3].pos.z = 0.0f;

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffRankScore->Unlock();

}
//========================
//ランキングスコアの終了
//========================
void UninitRankingScore(void)
{
	//テクスチャの破棄
	if (g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}
//========================
//ランキングスコアの更新
//========================
void UpdateRankingScore(void)
{
	static int nCounter{};

	if (g_nRankUpdate != -1)
	{
		nCounter++;   //加算

		//該当スコアを点滅
		if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

			//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx += 4;
			}
			g_pVtxBuffRankScore->Unlock();
		}
		else if (nCounter == 20)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

			//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx += 4;
			}

			g_pVtxBuffRankScore->Unlock();

			nCounter = 0;
		}

	}
	////該当のランキングスコアを点滅→
	//if (g_nRankUpdate != -1)
	//{
	//	//頂点カラー
	//}

	//g_nTimerRanking++;//タイマー加算

	if (g_nTimerRanking >= 300 || KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		SetFade(MODE_TITLE);
	}

	//MODE Mode = GetMode();


	//g_End.GameState = GAMEEND_STAGE1;

}
//========================
//ランキングスコアの描画
//========================
void DrawRankingScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの習得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_POLYGON; nCntScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRankScore);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);//プリミティブの種類
	}
}
//=====================
//ランキングのリセット
//=====================
void ResetRanking(void)
{
	//ランキングスコア情報の初期設定
	//ファイルを開く
	FILE* pFile;

	//MODE Mode = GetMode();

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
///================================
//ランキングの書き出し
//=================================
void WriteScore(void)
{
	FILE* pFile;

	MODE Mode = GetMode();

	//ファイルを開く
	pFile = fopen(RANK000, "w");

	//ファイルが開けたら
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fprintf(pFile, "%d\n", g_RankScore[nCnt].nScore);
		}
	}
	else
	{//開けなかったとき
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー", MB_OK);
		return;
	}
	//ファイル閉じる
	fclose(pFile);
}

//====================
//ランキングの設定
//====================
void SetRanking(int nScore)
{
	int nCntRank;

	int nData = {};

	int nCntScore;
	//ランキングスコアの並び替え→指定のスコアがランクインしたら g_nRankUpdateを更新

	//5番目のスコアと比較
	if (g_RankScore[MAX_RANK - 1].nScore < nScore)
	{
		g_RankScore[MAX_RANK - 1].nScore = nScore;

		g_nRankUpdate = MAX_RANK - 1;
	}

	for (nCntRank = 0; nCntRank < MAX_DIGIT; nCntRank++)
	{
		for (nCntScore = nCntRank + 1; nCntScore < MAX_RANK; nCntScore++)
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
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nScore == g_RankScore[nCntRank].nScore)
		{
			g_nRankUpdate = nCntRank;

			break;
		}
	}
	//スコア書き出し
	WriteScore();
}
//===========================
//ランキングのテクスチャ
//===========================
void RankingTexture(void)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};//各桁の数字を格納
	int aData = 10000000;
	int aData2 = 1000000;

	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RANK; nCntScore++)
	{
		for (int nCntScore1 = 0; nCntScore1 < MAX_DIGIT; nCntScore1++)
		{
			if (nCntScore1 == 0)//０番目
			{
				aPosTexU[0] = g_RankScore[nCntScore].nScore / aData;
			}
			else
			{
				//0番目以外のとき
				aPosTexU[nCntScore1] = g_RankScore[nCntScore].nScore % aData / aData2;
				aData = aData / 10;
				aData2 = aData2 / 10;
			}
			//テクスチャの設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore1]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore1]), 1.0f);

			pVtx += 4;

		}
		aData = 10000000;
		aData2 = 1000000;
	}
	
	g_pVtxBuffRankScore->Unlock();
}
//===========================
//終了判定の取得
//===========================
END* GetStageEnd(void)
{
	return &g_End;
}