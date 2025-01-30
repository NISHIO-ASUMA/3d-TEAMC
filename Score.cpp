//============================
//
// スコア処理 [score.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include"Score.h"
#include "ranking.h"
#include <stdio.h>
//****************************
//マクロ定義
//****************************
#define MAX_NUM_SCORE (8)

//****************************
//スコアの構造体
//****************************
typedef struct
{
	int nCountScoreCounter;//0のカウント
	bool bUse;
}SCORE;

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore=NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;//スコアの位置
int g_nScore;//スコアの値
SCORE g_aScore[MAX_NUM_SCORE];

//=====================
//スコアカウンター
//=====================
int ScoreCounter(int nScore)
{
	int nNum = nScore;//スコアの０の部分をカウント
	int nKeepNum = 0;//スコアの桁数

	while (nNum != 0)//割り切れなくなるまで繰り返す(０まで)
	{
		nNum /= 10;//nNumを10で割っていく
		nKeepNum++;//桁数を加算

	}
	if (nScore == 0)
	{
		//1桁目に0が入っているとき
		nKeepNum = 1;
	}
	return nKeepNum;//スコアの桁数を返す
}
//=====================
//スコアの初期化処理
//=====================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score000.png",
		&g_pTextureScore);

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_NUM_SCORE; nCnt1++)
	{
		g_aScore[nCnt1].nCountScoreCounter = 0;
		g_aScore[nCnt1].bUse = false;
	}
		g_nScore = 0;//値を初期化する

	for (nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(850.0f+(nCntScore*50),600.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(900.0f+(nCntScore*50),600.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(850.0f+(nCntScore*50) ,650.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(900.0f+(nCntScore*50),650.0f, 0.0f);
	
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
	g_pVtxBuffScore->Unlock();
}
//=====================
//スコアの終了処理
//=====================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//=====================
//スコアの更新処理
//=====================
void UpdateScore(void)
{
	int nData;//桁数保存用のローカル変数
	int nData1 = 0;//ローカル変数

	//求めたスコアの桁数を保存
	nData = ScoreCounter(g_nScore);

	for (nData1 = 0; nData1 < MAX_NUM_SCORE; nData1++)
	{
		if (nData1 >= MAX_NUM_SCORE - nData)
		{
			//MAX_NUM_SCORE-nDataで０が表示されているとき(nData1より桁数の値が小さいとき)
			g_aScore[nData1].bUse = true;//上の条件を有効化する
		}
		else
		{
			//それ以外の状態のとき
			g_aScore[nData1].bUse = false;
		}
	}	
}
//=====================
//スコアの描画処理
//=====================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	int nCntScore;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{		
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntScore*4 ,2);//プリミティブの種類
	}
}
//=================
//スコアの設定処理
//=================
void SetScore(int nScore)
{
	int aPosTexU[MAX_NUM_SCORE] = {};//各桁の数字を格納
	int aData = 10000000;
	int aData2 = 1000000;

	g_nScore = nScore;

	VERTEX_2D* pVtx;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
		if (nCntScore == 0)//0番目
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			//0番目以外のとき
			aPosTexU[nCntScore] = g_nScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffScore->Unlock();

}
//=================
//スコアの加算処理
//=================
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_NUM_SCORE] = {};//各桁の数字を格納
	int aData = 10000000;
	int aData2 = 1000000;

	g_nScore += nValue;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	for (int nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
	{
		if (nCntScore == 0)//0番目
		{
			aPosTexU[0] = g_nScore / aData;
		}
		else
		{
			//0番目以外のとき
			aPosTexU[nCntScore] = g_nScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffScore->Unlock();
}
//=================
//スコアの取得処理
//=================
int GetScore(void)
{
	return g_nScore;
}
//=================
//スコアの保存処理
//=================
void SaveScore(void)
{
	// ファイルポインタを宣言
	FILE* pFile;

	// ファイルを開く
	pFile = fopen("data/LastScore.txt", "w");

	if (pFile != NULL)
	{// ファイルが開けたら
		// 書き出す
		fprintf(pFile,"%d",g_nScore);
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Score.cpp)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}