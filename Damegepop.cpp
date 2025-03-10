//=====================================================================================================================
//
//ダメージ表示関連のプログラム
//Author;長尾悠成
//
//=====================================================================================================================

//*********************************************************************************************************************
// インクルードファイル宣言
//*********************************************************************************************************************
#include "Main.h"
#include "Damagepop.h"

//*********************************************************************************************************************
// マクロ定義
//*********************************************************************************************************************
#define MAXDAMEGE (200) // 表示する最大数
#define MAXDIGI (6)		// 桁数
#define SIZEY (25.0f)	// 文字の高さ
#define SIZEX (13.0f)	// 文字の横幅
#define INTERX (30)		// 間隔

//***********************
// 構造体
//***********************
typedef struct
{
	D3DXVECTOR3 pos;	// 座標
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;		// カラー
	D3DXMATRIX mtxWorld;// ワールドマトリックス
	int nDamege;		// ダメージの数値
	int nLife;			// 表示フレーム
	bool bUse;			// 存在するか否か
	bool bHeal;			// 回復か否か
}Damege;

//*********************************************************************************************************************
// グローバル変数宣言
//*********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTexture_Damege = NULL;
LPDIRECT3DTEXTURE9 g_pTexture_Heal = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamege = NULL;

Damege g_aDamege[MAXDAMEGE];
int g_aPosTexU[MAXDIGI];

//===================================================================================================
// Init処理色々
//===================================================================================================
void InitDamege(void)
{
	// デバイスへのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntDamege;
	int nCnt = 0;

	// ダメージを受けた時のフォント
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\damagepop.png", &g_pTexture_Damege);

	// 回復した時のフォント
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\damagepop.png", &g_pTexture_Heal);

	// 初期化処理
	for (nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
	{
		g_aDamege[nCntDamege].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDamege[nCntDamege].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDamege[nCntDamege].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aDamege[nCntDamege].nLife = 100;
		g_aDamege[nCntDamege].nDamege = 0;
		g_aDamege[nCntDamege].bUse = false;
		g_aDamege[nCntDamege].bHeal = false;
	}

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 6 * MAXDAMEGE * MAXDIGI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffDamege, NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロック
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntDamege = 0; nCntDamege < MAXDAMEGE * MAXDIGI; nCntDamege++)
	{
		// 多分大きさと基準値
		pVtx[0].pos = D3DXVECTOR3(-SIZEX, SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SIZEX, SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-SIZEX, -SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SIZEX, -SIZEY, 0.0f);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);*/

		// テクスチャ、今は10パターンのやつなので10にしている
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 1.0f);

		pVtx += 4;

		nCnt++;

		if (nCnt >= MAXDIGI)
		{
			nCnt = 0;
		}
	}
	g_pVtxBuffDamege->Unlock();
}
//===================================================================================================
// Uninit処理色々
//===================================================================================================
void UninitDamege(void)
{
	// テクスチャの破棄
	if (g_pTexture_Damege != NULL)
	{
		g_pTexture_Damege->Release();
		g_pTexture_Damege = NULL;
	}
	// バッファの破棄
	if (g_pVtxBuffDamege != NULL)
	{
		g_pVtxBuffDamege->Release();
		g_pVtxBuffDamege = NULL;
	}

	// テクスチャの破棄
	if (g_pTexture_Heal != NULL)
	{
		g_pTexture_Heal->Release();
		g_pTexture_Heal = NULL;
	}
}
//===================================================================================================
// 更新処理色々
//===================================================================================================
void UpdateDamege(void)
{
	// デバイスへのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntDamege;
	int nMath;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
	{
		// もし存在するなら
		if (g_aDamege[nCntDamege].bUse == true)
		{
			// 動かしながら透明化していく
			g_aDamege[nCntDamege].pos.y += g_aDamege[nCntDamege].move.y;
			g_aDamege[nCntDamege].col.a -= 0.01f;
			if (g_aDamege[nCntDamege].col.a < 0.0f)
			{
				g_aDamege[nCntDamege].col.a = 0.0f;
			}

			// 桁の数を測るシステム
			g_aPosTexU[0] = g_aDamege[nCntDamege].nDamege / 100000 % 10;
			g_aPosTexU[1] = g_aDamege[nCntDamege].nDamege / 10000 % 10;
			g_aPosTexU[2] = g_aDamege[nCntDamege].nDamege / 1000 % 10;
			g_aPosTexU[3] = g_aDamege[nCntDamege].nDamege / 100 % 10;
			g_aPosTexU[4] = g_aDamege[nCntDamege].nDamege / 10 % 10;
			g_aPosTexU[5] = g_aDamege[nCntDamege].nDamege % 10;

			// 桁数によって数値のTEXTURE座標を変える
			for (int nCnt = 0; nCnt < MAXDIGI; nCnt++)
			{
				nMath = pow(10.0f, 5.0f - nCnt);

				// １の位以外かつ最大の桁数を超えているなら表示しない
				if (g_aDamege[nCntDamege].nDamege >= nMath || nCnt == 5)
				{
					pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
				}

				pVtx[0].col = g_aDamege[nCntDamege].col;
				pVtx[1].col = g_aDamege[nCntDamege].col;
				pVtx[2].col = g_aDamege[nCntDamege].col;
				pVtx[3].col = g_aDamege[nCntDamege].col;

				// 場所を調整する
				pVtx[0].pos = D3DXVECTOR3(-SIZEX + ((nCnt - 4) * INTERX), SIZEY, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SIZEX + ((nCnt - 4) * INTERX), SIZEY, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-SIZEX + ((nCnt - 4) * INTERX), -SIZEY, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SIZEX + ((nCnt - 4) * INTERX), -SIZEY, 0.0f);
				pVtx += 4;
			}

			// 寿命で死ぬ
			g_aDamege[nCntDamege].nLife--;

			if (g_aDamege[nCntDamege].nLife <= 0)
			{
				g_aDamege[nCntDamege].bUse = false;
			}
		}
		else
		{
			// 4×桁数なので24進む
			pVtx += 4 * MAXDIGI;
		}
	}
	// アンロック
	g_pVtxBuffDamege->Unlock();
}
//===================================================================================================
// 描写処理色々
//===================================================================================================
void DrawDamege(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	 
	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 5);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		if (g_aDamege[nCnt].bUse == true)
		{
			// ビルボードの色々
			D3DXMATRIX mtxView;
			D3DXMatrixIdentity(&g_aDamege[nCnt].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aDamege[nCnt].mtxWorld._11 = mtxView._11;
			g_aDamege[nCnt].mtxWorld._12 = mtxView._21;
			g_aDamege[nCnt].mtxWorld._13 = mtxView._31;
			g_aDamege[nCnt].mtxWorld._21 = mtxView._12;
			g_aDamege[nCnt].mtxWorld._22 = mtxView._22;
			g_aDamege[nCnt].mtxWorld._23 = mtxView._32;
			g_aDamege[nCnt].mtxWorld._31 = mtxView._13;
			g_aDamege[nCnt].mtxWorld._32 = mtxView._23;
			g_aDamege[nCnt].mtxWorld._33 = mtxView._33;

			g_aDamege[nCnt].mtxWorld._41 = 0.0f;
			g_aDamege[nCnt].mtxWorld._42 = 0.0f;
			g_aDamege[nCnt].mtxWorld._43 = 0.0f;
			mtxView._14 = g_aDamege[nCnt].mtxWorld._41;
			mtxView._24 = g_aDamege[nCnt].mtxWorld._42;
			mtxView._34 = g_aDamege[nCnt].mtxWorld._43;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aDamege[nCnt].pos.x, g_aDamege[nCnt].pos.y, g_aDamege[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aDamege[nCnt].mtxWorld, &g_aDamege[nCnt].mtxWorld, &mtxTrans);

			pDevice->SetTransform(D3DTS_WORLD, &g_aDamege[nCnt].mtxWorld);
			pDevice->SetStreamSource(0, g_pVtxBuffDamege, 0, sizeof(VERTEX_3D));

			// 回復なら回復用のTEXTURE反映、逆ならダメージ用のTEXTURE
			if (g_aDamege[nCnt].bHeal == true)
			{
				pDevice->SetTexture(0, g_pTexture_Heal);
			}
			else
			{
				pDevice->SetTexture(0, g_pTexture_Damege);
			}
			// 頂点フォーマットの設定

			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			for (int nCnt2 = 0; nCnt2 < MAXDIGI; nCnt2++)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (nCnt * 4 * MAXDIGI) + (nCnt2 * 4), 2);
			}
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//===================================================================================================
// 配置処理色々
//===================================================================================================
// 左から順に場所、数値、寿命、回復かどうか
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);

	// 全て見回して
	for (int nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		// 働いてない奴に呼びかける
		if (g_aDamege[nCnt].bUse == false)
		{
			g_aDamege[nCnt].pos = pos;
			g_aDamege[nCnt].nDamege = nDamege;
			g_aDamege[nCnt].nLife = nLife;
			g_aDamege[nCnt].bHeal = Heal;
			g_aDamege[nCnt].move = D3DXVECTOR3(0.0f, 6.0f, 0.0f);
			g_aDamege[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aDamege[nCnt].bUse = true;
			break;
		}
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffDamege->Unlock();
}