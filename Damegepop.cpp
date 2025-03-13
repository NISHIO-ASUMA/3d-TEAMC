//=====================================================================================================================
//
//ダメージ表示関連処理 [ damegepop.cpp ]
//Author;長尾悠成
//
//=====================================================================================================================

//*********************************************************************************************************************
// インクルードファイル宣言
//*********************************************************************************************************************
#include "Damagepop.h"

//*********************************************************************************************************************
// マクロ定義
//*********************************************************************************************************************
#define MAXDAMEGE (200) // 表示する最大数
#define MAXDIGI (6)		// 桁数
#define SIZEY (25.0f)	// 文字の高さ
#define SIZEX (13.0f)	// 文字の横幅
#define INTERX (30)		// 間隔

//*********************************************************************************************************************
// ポップアップの構造体宣言
//*********************************************************************************************************************
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
LPDIRECT3DTEXTURE9 g_pTexture_Damege = NULL; // テクスチャのポインタ(ダメージ)
LPDIRECT3DTEXTURE9 g_pTexture_Heal = NULL;   // テクスチャのポインタ(回復)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamege = NULL; // 頂点バッファのポインタ
Damege g_aDamege[MAXDAMEGE];   // 構造体変数
int g_aPosTexU[MAXDIGI];

//===================================================================================================
//ダメージポップアップの初期化処理
//===================================================================================================
void InitDamege(void)
{
	// デバイスへのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCnt = 0;

	// ダメージを受けた時のフォント
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\damagepop.png", 
		&g_pTexture_Damege);

	// 回復した時のフォント
	D3DXCreateTextureFromFile(pDevice, 
		"data\\TEXTURE\\damagepop.png", 
		&g_pTexture_Heal);

	// 初期化処理
	for (int nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXDAMEGE * MAXDIGI, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffDamege, 
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロック
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPop = 0; nCntPop < MAXDAMEGE * MAXDIGI; nCntPop++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-SIZEX, SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SIZEX, SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-SIZEX, -SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SIZEX, -SIZEY, 0.0f);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	// アンロック
	g_pVtxBuffDamege->Unlock();
}
//===================================================================================================
// ダメージポップアップの終了処理
//===================================================================================================
void UninitDamege(void)
{
	// テクスチャの破棄
	if (g_pTexture_Damege != NULL)
	{// ダメージテクスチャ
		g_pTexture_Damege->Release();
		g_pTexture_Damege = NULL;
	}

	// テクスチャの破棄
	if (g_pTexture_Heal != NULL)
	{// 回復テクスチャ
		g_pTexture_Heal->Release();
		g_pTexture_Heal = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffDamege != NULL)
	{
		g_pVtxBuffDamege->Release();
		g_pVtxBuffDamege = NULL;
	}

}
//===================================================================================================
// ダメージポップアップの更新処理
//===================================================================================================
void UpdateDamege(void)
{
	// デバイスへのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nMath;

	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	g_pVtxBuffDamege->Lock(0, 0, (void**)&pVtx, 0);

	// 最大数分回す
	for (int nCntDamege = 0; nCntDamege < MAXDAMEGE; nCntDamege++)
	{
		// 使用状態なら
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
				nMath = pow(10.0, 5.0 - nCnt);

				// １の位以外かつ最大の桁数を超えているなら表示しない
				if (g_aDamege[nCntDamege].nDamege >= nMath || nCnt == 5)
				{
					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCnt] / 10.0f + (1.0f / 10.0f), 1.0f);
				}
				else
				{
					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
				}

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-SIZEX + ((nCnt - 4) * INTERX), SIZEY, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SIZEX + ((nCnt - 4) * INTERX), SIZEY, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-SIZEX + ((nCnt - 4) * INTERX), -SIZEY, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SIZEX + ((nCnt - 4) * INTERX), -SIZEY, 0.0f);

				// 頂点カラーの設定
				pVtx[0].col = g_aDamege[nCntDamege].col;
				pVtx[1].col = g_aDamege[nCntDamege].col;
				pVtx[2].col = g_aDamege[nCntDamege].col;
				pVtx[3].col = g_aDamege[nCntDamege].col;

				// 頂点情報のポインタをすすめる
				pVtx += 4;
			}

			// 寿命で死ぬ
			g_aDamege[nCntDamege].nLife--;

			if (g_aDamege[nCntDamege].nLife <= 0)
			{
				// ダメージポップアップを未使用状態にする
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
// ダメージポップアップの描画処理
//===================================================================================================
void DrawDamege(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	 
	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 5);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		if (g_aDamege[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aDamege[nCnt].mtxWorld);

			// ビューマトリックス格納用変数
			D3DXMATRIX mtxView;

			// ビューマトリックスの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// カメラの逆行列を設定
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

			// ビューマトリックに代入
			mtxView._14 = g_aDamege[nCnt].mtxWorld._41;
			mtxView._24 = g_aDamege[nCnt].mtxWorld._42;
			mtxView._34 = g_aDamege[nCnt].mtxWorld._43;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aDamege[nCnt].pos.x, g_aDamege[nCnt].pos.y, g_aDamege[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aDamege[nCnt].mtxWorld, &g_aDamege[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aDamege[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffDamege, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			if (g_aDamege[nCnt].bHeal == true)
			{// 回復が有効なら
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTexture_Heal);
			}
			else
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTexture_Damege);
			}

			for (int nCnt2 = 0; nCnt2 < MAXDIGI; nCnt2++)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 + (nCnt * 4 * MAXDIGI) + (nCnt2 * 4), 2);
			}
		}
	}

	// αテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/

	//　ライトの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//===================================================================================================
// ダメージポップアップの設定処理
//===================================================================================================
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal)
{
	for (int nCnt = 0; nCnt < MAXDAMEGE; nCnt++)
	{
		// 未使用だったら
		if (g_aDamege[nCnt].bUse == false)
		{
			g_aDamege[nCnt].pos = pos; // 座標
			g_aDamege[nCnt].nDamege = nDamege; // ダメージ量
			g_aDamege[nCnt].nLife = nLife; // 寿命
			g_aDamege[nCnt].bHeal = Heal;  // 回復かどうか
			g_aDamege[nCnt].move = D3DXVECTOR3(0.0f, 6.0f, 0.0f); // 移動量
			g_aDamege[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // カラー
			g_aDamege[nCnt].bUse = true; // 使用状態

			break;
		}
	}
}