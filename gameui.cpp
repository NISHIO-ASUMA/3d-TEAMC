//============================
//
// ゲームに必要なUI [gameui.cpp]
// Author:TEAM_C
//
//============================

//****************************
//インクルードファイル
//****************************
#include "gameui.h"
#include "HPGauge.h"
#include "player.h"

//****************************
//マクロ定義
//****************************

//****************************
//プロトタイプ宣言
//****************************
void UIFlash(int nType);

//****************************
//グローバル変数
//****************************
LPDIRECT3DTEXTURE9 g_pTextureGameUI[UITYPE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;//頂点バッファへのポインタ
Gameui g_GameUI[UITYPE_MAX];
int g_nPatternAnim, g_nCounterAnim;

//============================
// UIの初期化処理
//============================
void InitGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			UITYPE_INFO[nCnt],
			&g_pTextureGameUI[nCnt]);
	}

	//頂点バッファの生成・頂点情報の設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameUI,
		NULL);

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

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

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

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
//============================
// UIの終了処理
//============================
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
//============================
// UIの更新処理
//============================
void UpdateGameUI(void)
{
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (!g_GameUI[nCnt].bUse)
		{
			continue;
		}

		switch (g_GameUI[nCnt].nType)
		{
		case UITYPE_TITLE:

			if (g_GameUI[nCnt].pos.y <= 160.0f)
			{
				g_GameUI[nCnt].pos.y += 5.0f; // 下に移動
			}

			break;
		case UITYPE_FIVER:
		{
			static bool bUp = false;

			// 上昇しているとき
			if (bUp)
			{
				g_GameUI[nCnt].pos.y -= 2.0f; // 下に下げる

				if (g_GameUI[nCnt].pos.y < 630.0f)
				{
					bUp = false; // 下に下げる
				}
			}
			else if (!bUp)
			{
				g_GameUI[nCnt].pos.y += 2.0f;

				if (g_GameUI[nCnt].pos.y > 650.0f)
				{
					bUp = true; // 上に上げる
				}
			}

			if (!GetFeverMode())
			{
				g_GameUI[nCnt].bUse = false;
			}

			break;
		}
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
		
			if (!GetFeverMode())
			{
				g_GameUI[nCnt].bUse = false;
			}

			break;
		case UITYPE_BLACK:
		{
			static float fcolorA = 0.0f; // アルファ値

			if (fcolorA >= 0.9f)
			{
				fcolorA = 0.9f; // α値を固定
			}
			else
			{
				fcolorA += 0.01f; // インクリメント
			}

			if (pPlayer->WeponMotion == MOTION_SP && pPlayer->Motion.nKey == 4)
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
		default:
			break;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);

		pVtx += 4;
	}

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//============================
// UIの描画処理
//============================
void DrawGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

    //デバイスの習得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureGameUI[g_GameUI[nCnt].nType]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);//プリミティブの種類
		}
	}
}
//============================
// UIの設定処理
//============================
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	VERTEX_2D* pVtx;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (!g_GameUI[nCnt].bUse)
		{
			g_GameUI[nCnt].pos = pos;
			g_GameUI[nCnt].nType = nType;
			g_GameUI[nCnt].fWidth = fWidth;
			g_GameUI[nCnt].fHeight = fHeight;
			g_GameUI[nCnt].nUseTime = nUseTime;
			g_GameUI[nCnt].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//============================
// UI選択処理
//============================
void FlashGameUI(int nSelect)
{
	VERTEX_2D* pVtx;

	//頂点ロック
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].nType == UITYPE_TITLE2)
		{
			if (nSelect == TITLESELECT_GAME)
			{
				g_GameUI[nCnt].pos.y = 450.0f;
			}
			else if (nSelect == TITLESELECT_TUTO)
			{
				g_GameUI[nCnt].pos.y = 600.0f;
			}
		}
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);

		pVtx += 4;
	}

	//頂点ロック解除
	g_pVtxBuffGameUI->Unlock();
}
//============================
// UIの点滅処理
//============================
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
