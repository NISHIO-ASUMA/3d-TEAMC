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
#include"math.h"
#include "easing.h"
#include "event.h"
#include "game.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define DEST_WIDTHEXPANSION (120.0f) // 目標の拡大率(横幅)
#define DEST_WIDTHREDUCTION (100.0f) // 目標の縮小率(横幅)

#define DEST_HEIGHTEXPANSION (45.0f) // 目標の拡大率(縦幅)
#define DEST_HEIGHTREDUCTION (25.0f) // 目標の縮小率(縦幅)

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void UIFlash(int nType);	// 点滅処理
void UpdateDestroyUI(int nCnt); // 武器が壊れた時のUI
void SetEventUIAnimation(int nCnt);   // イベントのUIの設定
void SetTutoUIAnimation(int nCnt); // チュートリアルUIのアニメーション
float fcolorA;

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameUI[UITYPE_MAX] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;      // 頂点バッファへのポインタ
Gameui g_GameUI[UITYPE_MAX];
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITYPE_MAX,
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

		if (g_GameUI[nCnt].nType == UITYPE_SYUTYUSEN)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);
		}
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
				g_nCounterAnim++;

				if (g_nCounterAnim > 2)
				{
					g_nCounterAnim = 0;

					g_nPatternAnim++;//パターンナンバーを更新

				}

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);

				//頂点座標の更新
				pVtx[0].tex = D3DXVECTOR2(0.0f + g_nPatternAnim * 0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + g_nPatternAnim * 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + g_nPatternAnim * 0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + g_nPatternAnim * 0.5f, 1.0f);

				if (g_nPatternAnim > 2)
				{
					g_nPatternAnim = 0;
				}

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

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameUI[nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // プリミティブの種類
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

		g_GameUI[nCnt].pos.x += SetSmoothAprroach(950.0f, g_GameUI[nCnt].pos.x, 0.1f);

		g_GameUI[nCnt].pos.y += SetSmoothAprroach(670.0f, g_GameUI[nCnt].pos.y, 0.1f);
	}

	// イベントが終わった
	if (EnableEvent() == false)
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
