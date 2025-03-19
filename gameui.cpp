//==============================================================================================================
//
// ゲームに必要なUI [gameui.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "gameui.h"
#include "HPGauge.h"
#include "player.h"
#include "math.h"
#include "easing.h"
#include "event.h"
#include "game.h"
#include "boss.h"
#include "manual.h"
#include "title.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define DEST_WIDTHEXPANSION (120.0f) // 目標の拡大率(横幅)
#define DEST_WIDTHREDUCTION (100.0f) // 目標の縮小率(横幅)

#define DEST_HEIGHTEXPANSION (45.0f) // 目標の拡大率(縦幅)
#define DEST_HEIGHTREDUCTION (25.0f) // 目標の縮小率(縦幅)

#define MAX_UI (120)				 // 最大UI数

#define UITYPE_SPWIDTH (55.0f)		 // スペシャルUIの横幅
#define UITYPE_SPHEIGHT (50.0f)		 // スペシャルUIの高さ

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void UIFlash(int nType);				// 点滅処理
void UpdateDestroyUI(int nCnt);			// 武器が壊れた時のUI
void SetEventUIAnimation(int nCnt);		// イベントのUIの設定
void SetTutoUIAnimation(int nCnt);		// チュートリアルUIのアニメーション
void SetDamageUIAnimation(int nCnt);	// ダメージUIの設定
void SetTerritoryTimeUI(int nCnt);		// テリトリーが出るまでの時間
void SetBossManual(int nCnt);			// ボスのマニュアルの設定
void UpdateUIFlash(int nCnt, float* pAlv,float Maxtime);	// UIの点滅処理(使いまわせるやつ)
void SetTerritoryEnemyUI(int nCnt);		// テリトリーに敵が出た時のUIの設定
void UIScalAnimation(int nCnt,float widthEx,float widthdec,float heightEx,float heigthDec, float MaxtimeWidth,float MaxtimeHeight); // 拡大縮小のアニメーション
void UpdateCraftTimeUI(int nCnt);		// クラフトの時間中のUI
void UpdateCraftTimeMenuUI(int nCnt);	// クラフトメニューのUIの設定

float fcolorA;

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameUI[UITYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;	  // 頂点バッファへのポインタ
Gameui g_GameUI[MAX_UI];
int g_nPatternAnim, g_nCounterAnim;

//==============================================================================================================
// UIの初期化処理
//==============================================================================================================
void InitGameUI(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			UITYPE_INFO[nCnt],
			&g_pTextureGameUI[nCnt]);
	}

	// 頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameUI,
		NULL);

	// 頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	fcolorA = 0.0f;
	g_nPatternAnim = 0;
	g_nCounterAnim = 0;

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		g_GameUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].fHeight = 0.0f;
		g_GameUI[nCnt].fWidth = 0.0f;
		g_GameUI[nCnt].nUseTime = 0;
		g_GameUI[nCnt].bUse = false;
		g_GameUI[nCnt].nType = UITYPE_TITLE;
		g_GameUI[nCnt].col = COLOR_WHITE;
		g_GameUI[nCnt].nEaseCnt = 0;
		g_GameUI[nCnt].bLife = false;
		g_GameUI[nCnt].nCounterAnim = 0;
		g_GameUI[nCnt].nPatternAnim = 0;
		g_GameUI[nCnt].fAlv = 0.0f;

		// 頂点座標の設定
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
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UIの終了処理
//==============================================================================================================
void UninitGameUI(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_pTextureGameUI[nCnt] != NULL)
		{
			g_pTextureGameUI[nCnt]->Release();
			g_pTextureGameUI[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGameUI != NULL)
	{
		g_pVtxBuffGameUI->Release();
		g_pVtxBuffGameUI = NULL;
	}
}
//==============================================================================================================
// UIの更新処理
//==============================================================================================================
void UpdateGameUI(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 拡大する
	static bool bExpansion = true;

	// 
	float a = (float)(1 % 10) * 0.1f;
	
	// 頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == true)
		{
			switch (g_GameUI[nCnt].nType)
			{
			case UITYPE_TITLE:

				if (g_GameUI[nCnt].pos.y <= 140.0f)
				{
					g_GameUI[nCnt].pos.y += 5.0f; // 下に移動
				}

				break;
			case UITYPE_FIVER:

				if (g_GameUI[nCnt].fWidth >= 150.0f)
				{
					bExpansion = false;
				}
				else if (g_GameUI[nCnt].fWidth <= 100.0f)
				{
					bExpansion = true;
				}
				if (bExpansion == true)
				{
					g_GameUI[nCnt].fWidth += SetSmoothAprroach(160.0f, g_GameUI[nCnt].fWidth, 0.1f);
					g_GameUI[nCnt].fHeight += SetSmoothAprroach(70.0f, g_GameUI[nCnt].fHeight, 0.1f);
				}
				else if (bExpansion == false)
				{
					g_GameUI[nCnt].fWidth += SetSmoothAprroach(90.0f, g_GameUI[nCnt].fWidth, 0.1f);
					g_GameUI[nCnt].fHeight += SetSmoothAprroach(30.0f, g_GameUI[nCnt].fHeight, 0.1f);
				}

				if (pPlayer->FeverMode == false)
				{
					g_GameUI[nCnt].bUse = false;
				}

				//頂点カラーの設定
				pVtx[0].col = COLOR_WHITE;
				pVtx[1].col = COLOR_WHITE;
				pVtx[2].col = COLOR_WHITE;
				pVtx[3].col = COLOR_WHITE;

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_SYUTYUSEN:

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);

				SetTextureAnimation(2, 1, 2, &g_GameUI[nCnt].nCounterAnim, &g_GameUI[nCnt].nPatternAnim, pVtx, g_pVtxBuffGameUI, nCnt);

				if (!pPlayer->FeverMode)
				{
					g_GameUI[nCnt].bUse = false;
				}

				break;
			case UITYPE_TUTORIAL:
				SetTutoUIAnimation(nCnt);
				break;
			case UITYPE_BLACK:
			{
				if (fcolorA >= 0.9f)
				{
					fcolorA = 0.9f; // α値を固定
				}
				else
				{
					fcolorA += 0.01f; // インクリメント
				}

				if (pPlayer->AttackSp && pPlayer->Motion.nKey == 4)
				{
					g_GameUI[nCnt].bUse = false; // 消す
					fcolorA = 0.0f; // 初期化
				}

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
			}
			break;

			case UITYPE_KATANA:

				if (g_GameUI[nCnt].pos.x >= 650.0f)
				{
					g_GameUI[nCnt].pos.x -= 10.0f; // 左に移動
				}

				break;
			case UITYPE_RED:

				if (pPlayer->nLife <= 150)
				{
					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
				}
				else
				{
					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				break;
			case UITYPE_DESTORY:

				UpdateDestroyUI(nCnt);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[1].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[2].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[3].col = D3DXCOLOR(g_GameUI[nCnt].col);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_EVENT:
				// イベントUIの設定
				SetEventUIAnimation(nCnt);

				//頂点カラーの設定
				pVtx[0].col = COLOR_WHITE;
				pVtx[1].col = COLOR_WHITE;
				pVtx[2].col = COLOR_WHITE;
				pVtx[3].col = COLOR_WHITE;

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_SP:
				// テクスチャアニメーションの設定処理
				SetTextureAnimation(2, 1, 10, &g_GameUI[nCnt].nCounterAnim, &g_GameUI[nCnt].nPatternAnim, pVtx, g_pVtxBuffGameUI, nCnt);

				// spがたまっていなかったら大きさを0にして見えなくする
				g_GameUI[nCnt].fWidth = pPlayer->SpMode ? UITYPE_SPWIDTH : 0.0f;

				// spがたまっていなかったら大きさを0にして見えなくする
				g_GameUI[nCnt].fHeight = pPlayer->SpMode ? UITYPE_SPHEIGHT : 0.0f;
				break;

			case UITYPE_DAMAGE:
				// ダメージUIの設定
				SetDamageUIAnimation(nCnt);
				break;
			case UITYPE_SETENEMYTIME:
				// テリトリー出るまでの情報のUI
				SetTerritoryTimeUI(nCnt);
				break;
			case UITYPE_BOSSMANUALEXIT:
				SetBossManual(nCnt);
				break;
			case UITYPE_POPENEMY:
				SetTerritoryEnemyUI(nCnt);
				break;
			case UITYPE_CRAFTTIME:
				UpdateCraftTimeUI(nCnt);
				break;
			case UITYPE_CRAFTTIMEMENU:
				UpdateCraftTimeMenuUI(nCnt);
				break;
			}

			// 寿命がある
			if (g_GameUI[nCnt].bLife == true)
			{
				// 寿命を減らす
				g_GameUI[nCnt].nUseTime--;

				// 使用時間が終わった
				if (g_GameUI[nCnt].nUseTime <= 0)
				{
					// 消す
					g_GameUI[nCnt].bUse = false;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		}
		pVtx += 4;
	}

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UIの描画処理
//==============================================================================================================
void DrawGameUI(void)
{
	// 現在のモードを取得
	MODE mode = GetMode();

	LPDIRECT3DDEVICE9 pDevice; // デバイスへのポインタ

    // デバイスの習得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == false)
		{
			continue;
		}

		// 種類
		int nType = g_GameUI[nCnt].nType;

		// ダメージUIじゃなかったら
		if (nType != UITYPE_DAMAGE)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureGameUI[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
		// ダメージUIだったら
		else
		{
			pDevice->SetTexture(0, g_pTextureGameUI[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
		}
	}
}
//==============================================================================================================
// UIの設定処理
//==============================================================================================================
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, bool bLife,int nUseTime)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == false)
		{
			g_GameUI[nCnt].pos = pos;
			g_GameUI[nCnt].nType = nType;
			g_GameUI[nCnt].fWidth = fWidth;
			g_GameUI[nCnt].fHeight = fHeight;
			g_GameUI[nCnt].nUseTime = nUseTime;
			g_GameUI[nCnt].bLife = bLife;
			g_GameUI[nCnt].bUse = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// 頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// 選択時のUI点滅処理
//==============================================================================================================
void FlashGameUI(int nSelect)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].nType == UITYPE_TITLE2)
		{
			if (nSelect == TITLESELECT_GAME)
			{
				g_GameUI[nCnt].pos.y = 350.0f;
			}
			else if (nSelect == TITLESELECT_TUTO)
			{
				g_GameUI[nCnt].pos.y = 500.0f;
			}
			else if (nSelect == TITLESELECT_RANKING)
			{
				g_GameUI[nCnt].pos.y = 650.0f;
			}
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);

		pVtx += 4;
	}

	// 頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UIの点滅処理
//==============================================================================================================
void UIFlash(int nType)
{
	VERTEX_2D* pVtx;
	static float fA = 1.0f;
	static bool bAlv = false;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (nType == nCnt)
		{
			// falseの時に透明化
			if (fA > 0.01f && !bAlv)
			{
				fA -= 0.01f; // 減算

				if (fA <= 0.01f)
				{
					bAlv = true;
				}
			}
			//trueの時にだんだん見えるようになる
			else if (fA <= 1.0f && bAlv)
			{
				fA += 0.01f; // 加算

				if (fA >= 1.0f)
				{
					bAlv = false;
				}
			}

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
		}
		else
		{

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		pVtx += 4;
	}

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UIの点滅処理
//==============================================================================================================
void UpdateDestroyUI(int nCnt)
{
	g_GameUI[nCnt].nUseTime--;
	static int WidthEase = 0;
	static int HeightEase = 0;

	HeightEase++;

	// イージングを設定
	float Widthtime = SetEase(WidthEase,120);

	static bool bWIdthExpansion = false;
	static bool bHeightExpansion = false;

	//// 拡大のカウント
	//static int nExpansionCnt = 0;

	//// 縮小のカウント
	//static int nReductionCnt = 0;

	// 横幅が300.0fを超えたら
	if (g_GameUI[nCnt].fWidth >= DEST_WIDTHEXPANSION - 5.0f)
	{
		WidthEase = 0;
		// 縮小開始
		bWIdthExpansion = false;
	}
	// 横幅が160を下回ったら
	else if(g_GameUI[nCnt].fWidth <= DEST_WIDTHREDUCTION + 5.0f)
	{
		WidthEase = 0;
		// 拡大開始
		bWIdthExpansion = true;
	}

	// 拡大できる
	if (bWIdthExpansion == true)
	{
		// 拡大する
		g_GameUI[nCnt].fWidth += SetSmoothAprroach(DEST_WIDTHEXPANSION, g_GameUI[nCnt].fWidth, 0.1f);
	}
	// 拡大できない
	else if (bWIdthExpansion == false)
	{
		g_GameUI[nCnt].fWidth += SetSmoothAprroach(DEST_WIDTHREDUCTION, g_GameUI[nCnt].fWidth, 0.1f);
	}

	// 横幅が300.0fを超えたら
	if (g_GameUI[nCnt].fHeight >= DEST_HEIGHTEXPANSION - 5.0f)
	{
		// 縮小開始
		bHeightExpansion = false;
	}
	// 横幅が160を下回ったら
	else if (g_GameUI[nCnt].fHeight <= DEST_HEIGHTREDUCTION + 5.0f)
	{
		// 拡大開始
		bHeightExpansion = true;
	}

	// 拡大できる
	if (bHeightExpansion == true)
	{
		// 拡大する
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(DEST_HEIGHTEXPANSION, g_GameUI[nCnt].fHeight, 0.1f);
	}
	// 拡大できない
	else if (bHeightExpansion == false)
	{
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(DEST_HEIGHTREDUCTION, g_GameUI[nCnt].fHeight, 0.1f);
	}

	// 使用時間が0になったら消す
	if (g_GameUI[nCnt].nUseTime <= 0)
	{
		g_GameUI[nCnt].bUse = false;
	}
}
//==============================================================================================================
// イベントのUIの設定
//==============================================================================================================
void SetEventUIAnimation(int nCnt)
{
	GAMESTATE gamestate = GetGameState();
	static int fWidthEasing = 0;
	static int fHeightEasing = 0;

	if (gamestate == GAMESTATE_MOVIE)
	{
		fWidthEasing++;
		fHeightEasing++;

		float timeWidth = SetEase(fWidthEasing, 180);
		float timeHeight = SetEase(fHeightEasing, 160);

		g_GameUI[nCnt].fWidth += SetSmoothAprroach(250.0f, g_GameUI[nCnt].fWidth, EaseInSine(timeWidth));
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(80.0f, g_GameUI[nCnt].fHeight, EaseInSine(timeHeight));
	}
	// ゲーム状態が普通の時
	if (gamestate == GAMESTATE_NORMAL)
	{
		fWidthEasing = 0;
		fHeightEasing = 0;

		g_GameUI[nCnt].fWidth += SetSmoothAprroach(100.0f, g_GameUI[nCnt].fWidth, 0.1f);
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(30.0f, g_GameUI[nCnt].fHeight, 0.1f);

		g_GameUI[nCnt].pos.x += SetSmoothAprroach(1150.0f, g_GameUI[nCnt].pos.x, 0.1f);

		g_GameUI[nCnt].pos.y += SetSmoothAprroach(230.0f, g_GameUI[nCnt].pos.y, 0.1f);
	}

	// ボスの数の取得
	int NumBoss = GetNumBoss();

	// ボスがいなかったら
	if (NumBoss <= 0)
	{
		g_GameUI[nCnt].bUse = false;
	}
}
//==============================================================================================================
// チュートリアルUIのアニメーション
//==============================================================================================================
void SetTutoUIAnimation(int nCnt)
{
	VERTEX_2D* pVtx;

	// 減少させるフラグ
	static bool bDec = true;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	// アルファ値
	static float fA = 1.0f;
	
	// アルファ値の減る量
	float DecAlv = 1.0f / 120.0f;

	// 減らす状態だったら
	if (bDec == true)
	{
		// アルファ値を減少指せる
		fA -= DecAlv;
	}
	// 増やす状態だったら
	else
	{
		// アルファ値を増やす
		fA += DecAlv;
	}

	// 透明度が1.0fになったら
	if (fA >= 1.0f)
	{
		// 減少させる
		bDec = true;
	}
	// 透明度が0.0fになったら
	else if (fA <= 0.0f)
	{
		// 増やす
		bDec = false;
	}
	pVtx += 4 * nCnt;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// ダメージUIの設定
//==============================================================================================================
void SetDamageUIAnimation(int nCnt)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	// 目的のアルファ値
	static float fDestAlv = 0.0f;

	// 現在のアルファ値
	static float fAlv = 0.0f;

	// 状態がダメージだったら
	const bool isDamage = pPlayer->state == PLAYERSTATE_DAMAGE;

	// ダメージ状態だったら
	fDestAlv = isDamage ? 0.5f : 0.0f;

	// 目的の値に知被ける
	fAlv += SetSmoothAprroach(fDestAlv, fAlv, 0.1f);

	pVtx += 4 * nCnt;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// テリトリーが出るまでの時間
//==============================================================================================================
void SetTerritoryTimeUI(int nCnt)
{
	// テリトリーの数の取得
	int NumTerritory = GetNumTeriitory();

	// テリトリーの数が0胃かだったら
	const bool isSetUI = NumTerritory <= 0;

	// 最初のクラフトモードだったら
	const bool NotCraftTime = GetFirstCraftTIme() == true;

	// 大きさを設定
	g_GameUI[nCnt].fWidth = NotCraftTime ?  0.0f : (isSetUI ? 40.0f : 0.0f);
	g_GameUI[nCnt].fHeight = NotCraftTime ?  0.0f : (isSetUI ? 25.0f : 0.0f);
}
//==============================================================================================================
// ボスのマニュアルの設定
//==============================================================================================================
void SetBossManual(int nCnt)
{
	VERTEX_2D* pVtx;
	 
	// アルファ値
	static float fAlv = 1.0f;

	// アルファ値の減少フラグ
	static bool isDecAlv = true;

	// 減少できる
	if (isDecAlv == true)
	{
		fAlv -= 1.0f / 120.0f;
	}
	else
	{
		fAlv += 1.0f / 120.0f;
	}

	if (fAlv >= 1.0f)
	{
		isDecAlv = true;
	}
	else if (fAlv <= 0.0f)
	{
		isDecAlv = false;
	}

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UIの点滅処理(使いまわせるやつ)
//==============================================================================================================
void UpdateUIFlash(int nCnt,float *pAlv,float Maxtime)
{
	VERTEX_2D* pVtx;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// テリトリーに敵が出た時のUIの設定
//=============================================================================================================
void SetTerritoryEnemyUI(int nCnt)
{
	VERTEX_2D* pVtx;

	// 大きくなったり小さくなったりするアニメーション
	UIScalAnimation(nCnt, 200.0f, 150.0f, 80.0f, 40.0f, 40.0f, 40.0f);

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// 拡大縮小のアニメーション
//==============================================================================================================
void UIScalAnimation(int nCnt, float widthEx, float widthdec, float heightEx, float heigthDec,float MaxtimeWidth, float MaxtimeHeight)
{
	// 横幅の拡大
	static bool isWidthEasing = true;

	// 高さの拡大
	static bool isHeightEasing = true;

	// 横幅の拡大率
	static float Widthscal = widthEx / MaxtimeWidth;

	// 高さの拡大率
	static float Heightscal = heightEx / MaxtimeHeight;

	// 大きさが目的の大きさになったら
	if (g_GameUI[nCnt].fWidth >= widthEx)
	{
		// 縮小
		isWidthEasing = false;
		isHeightEasing = false;
	}
	// 小ささが目的の小ささになったら
	else if (g_GameUI[nCnt].fWidth <= widthdec)
	{
		// 拡大
		isWidthEasing = true;
		isHeightEasing = true;

	}

	//if (g_GameUI[nCnt].fHeight >= heightEx)
	//{
	//	
	//}
	//else if (g_GameUI[nCnt].fHeight <= heigthDec)
	//{
	//}

	// 横幅の拡大率
	Widthscal = isWidthEasing ? (widthEx / MaxtimeWidth) : (-widthdec / MaxtimeWidth);
	Heightscal = isHeightEasing ? (heightEx / MaxtimeHeight) : (-heigthDec / MaxtimeHeight);

	// 目的の値に近づける
	g_GameUI[nCnt].fWidth += Widthscal;

	// 目的の値に近づける
	g_GameUI[nCnt].fHeight += Heightscal;
}
//==============================================================================================================
// クラフトの時間中のUI
//==============================================================================================================
void UpdateCraftTimeUI(int nCnt)
{
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();

	// クラフトの時間が終わったら
	if (GetFirstCraftTIme() == false)
	{
		g_GameUI[nCnt].bUse = false;
	}

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// 目的の値に近づける
	g_GameUI[nCnt].pos.x += SetSmoothAprroach(640.0f, g_GameUI[nCnt].pos.x, 0.1f);	

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// クラフトメニューのUIの設定
//==============================================================================================================
void UpdateCraftTimeMenuUI(int nCnt)
{
	VERTEX_2D* pVtx;

	static float fAlv = 0.0f;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// 三秒
	fAlv += 1.0f / 120.0f;

	// カウンター用変数
	static int nCounter = 0;

	// 1.0fに制限する
	if (fAlv >= 1.0f)
	{
		fAlv = 1.0f;

		// 2秒たったら
		if (nCounter >= 120)
		{
			// 目的の位置に近づける
			g_GameUI[nCnt].pos.x += SetSmoothAprroach(1050.0f, g_GameUI[nCnt].pos.x, 0.1f);
			g_GameUI[nCnt].pos.y += SetSmoothAprroach(500.0f, g_GameUI[nCnt].pos.y, 0.1f);
		}
		else
		{
			// インクリメント
			nCounter++;
		}
	}

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();

	// 最初のクラフトの時間が終わったら
	if (GetFirstCraftTIme() == false)
	{
		// リセットする
		fAlv = 0.0f;
		nCounter = 0;

		// 未使用にする
		g_GameUI[nCnt].bUse = false;
	}
}
