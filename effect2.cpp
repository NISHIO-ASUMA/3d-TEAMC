//=====================================================================================================
//
// エフェクト[effect.cpp]
// Author:YOSHIDA YUUTO
//
//=====================================================================================================

//*****************************************************************************************************
//インクルードファイル
//*****************************************************************************************************
#include "camera.h"
#include "player.h"
#include "input.h"
#include "effect2.h"

//*****************************************************************************************************
//マクロ定義
//*****************************************************************************************************
#define MAX_EFFECT (4096) //爆発の最大数
#define XTEX (8) // テクスチャの横幅
#define YTEX (1) // テクスチャの高さ
#define UV (1.0f / XTEX)
#define HV (1.0f / YTEX)

//*****************************************************************************************************
// プロトタイプ宣言
//*****************************************************************************************************
void SetTextureAnim(int nCnt, int XtexPos, int YtexPos, int Animspeed);

//*****************************************************************************************************
//グローバル変数宣言
//*****************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffectX[EFFECT_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectX = NULL; //頂点バッファへのポインタ
EFFECT g_EffectX[MAX_EFFECT];

//=====================================================================================================
// エフェクトの初期化処理
//=====================================================================================================
void InitEffectX(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイスの取得

	// 種類分回す
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			EFFECT_TEXTURE[nCnt],
			&g_pTextureEffectX[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffectX,
		NULL);

	// 頂点バッファのポインタ
	VERTEX_3D* pVtx = NULL;

	// エフェクト分回す
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_EffectX[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 位置
		g_EffectX[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量 
		g_EffectX[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f); // 色 
		g_EffectX[nCnt].nLife = 0; // 寿命
		g_EffectX[nCnt].g_nCounterAnim = 0; // アニメーションカウンター
		g_EffectX[nCnt].g_nPatternAnim = 0; // アニメーションカウンター
		g_EffectX[nCnt].decfAlv = 0.02f; // α値の減少値
		g_EffectX[nCnt].nType = EFFECTTYPE_PLAYER; // 種類
		g_EffectX[nCnt].fRadius = 20.0f; // 半径
		g_EffectX[nCnt].EffectType = EFFECT_NORMAL; // エフェクトの種類
		g_EffectX[nCnt].bUse = false; // 位置
	}
	//頂点バッファをロック
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	//エフェクト分回す
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x - g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y + g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x + g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y + g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x - g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y - g_EffectX[nCnt].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EffectX[nCnt].pos.x + g_EffectX[nCnt].fRadius, g_EffectX[nCnt].pos.y - g_EffectX[nCnt].fRadius, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);//カメラに対して垂直に
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffectX->Unlock();
}
//=====================================================================================================
// エフェクト終了処理
//=====================================================================================================
void UninitEffectX(void)
{
	// テクスチャの解放
	for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
	{
		if (g_pTextureEffectX[nCnt] != NULL)
		{
			g_pTextureEffectX[nCnt]->Release();
			g_pTextureEffectX[nCnt] = NULL;
		}
	}
	//頂点バッファの解放
	if (g_pVtxBuffEffectX != NULL)
	{
		g_pVtxBuffEffectX->Release();
		g_pVtxBuffEffectX = NULL;
	}
}
//=====================================================================================================
//エフェクト更新処理
//=====================================================================================================
void UpdateEffectX(void)
{
	// 頂点バッファへのポインタ
	VERTEX_3D* pVtx;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	//頂点バッファをロック
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// エフェクト分回す
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// 未使用だったら
		if (!g_EffectX[nCnt].bUse)
		{
			//処理を読み飛ばす
			continue;
		}

		// 位置の更新
		g_EffectX[nCnt].pos += g_EffectX[nCnt].move;

		// 寿命を減らす
		g_EffectX[nCnt].nLife--;

		// α値を減少させる
		g_EffectX[nCnt].col.a -= g_EffectX[nCnt].decfAlv;

		float fRadius = g_EffectX[nCnt].fRadius;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRadius, - fRadius, 0.0f);

		//頂点カラー
		pVtx[0].col = g_EffectX[nCnt].col;
		pVtx[1].col = g_EffectX[nCnt].col;
		pVtx[2].col = g_EffectX[nCnt].col;
		pVtx[3].col = g_EffectX[nCnt].col;

		// アニメーションの設定
		if (g_EffectX[nCnt].EffectType == EFFECT_SMORK)
		{
			SetTextureAnim(nCnt, 8, 1,15);
		}
		if (g_EffectX[nCnt].EffectType == EFFECT_SOUL)
		{
			SetTextureAnim(nCnt, 4, 1, 5);
		}
		if (g_EffectX[nCnt].EffectType == EFFECT_WATER)
		{
			SetTextureAnim(nCnt, 5, 1, 5);
		}
		if(g_EffectX[nCnt].EffectType == EFFECT_NORMAL)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 寿命が尽きたら消す
		if (g_EffectX[nCnt].nLife <= 0)
		{
			g_EffectX[nCnt].bUse = false;
		}
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffEffectX->Unlock();
}
//=====================================================================================================
// エフェクトの描画処理
//=====================================================================================================
void DrawEffectX(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイスの取得

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxView, mtxScal;

	// エフェクト分回す
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// エフェクトが使われていたら
		if (g_EffectX[nCnt].bUse == true)
		{
			// 種類を代入
			int EffectType = g_EffectX[nCnt].EffectType;

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			if (EffectType != EFFECT_SMORK && EffectType != EFFECT_WATER)
			{
				//アルファテストを有効
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//ゼットテスト
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//aブレンディング
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_EffectX[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//ビューマトリックスの取得

			//カメラの逆行列の設定
			g_EffectX[nCnt].mtxWorld._11 = mtxView._11;
			g_EffectX[nCnt].mtxWorld._12 = mtxView._21;
			g_EffectX[nCnt].mtxWorld._13 = mtxView._31;
			g_EffectX[nCnt].mtxWorld._21 = mtxView._12;
			g_EffectX[nCnt].mtxWorld._22 = mtxView._22;
			g_EffectX[nCnt].mtxWorld._23 = mtxView._32;
			g_EffectX[nCnt].mtxWorld._31 = mtxView._13;
			g_EffectX[nCnt].mtxWorld._32 = mtxView._23;
			g_EffectX[nCnt].mtxWorld._33 = mtxView._33;
			

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_EffectX[nCnt].pos.x, g_EffectX[nCnt].pos.y, g_EffectX[nCnt].pos.z);
			D3DXMatrixMultiply(&g_EffectX[nCnt].mtxWorld, &g_EffectX[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EffectX[nCnt].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffectX, 0, sizeof(VERTEX_3D));

			//テクスチャフォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffectX[EffectType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

			//ライトを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//aブレンディングをもとに戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//アルファテストを無効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//ゼットテスト
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		}
	}
}
//=====================================================================================================
//エフェクトの設定処理
//=====================================================================================================
void SetEffectX(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, float decfAlv, int EffectType)//エフェクトの設定処理
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// エフェクト分回す
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		// 未使用だったら
		if (g_EffectX[nCnt].bUse == false)
		{
			g_EffectX[nCnt].pos = pos; // 位置
			g_EffectX[nCnt].fRadius = fRadius; // 半径
			g_EffectX[nCnt].move = move; // 移動量
			g_EffectX[nCnt].col = col; // 色
			g_EffectX[nCnt].nLife = nLife; // 寿命
			g_EffectX[nCnt].decfAlv = decfAlv; // α値の減少値
			g_EffectX[nCnt].EffectType = EffectType; // エフェクトの種類
			g_EffectX[nCnt].bUse = true;// 使用状態にする

			switch (EffectType)
			{
			case EFFECT_NORMAL:
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;
			case EFFECT_SMORK:
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
				break;
			case EFFECT_SOUL:
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
				break;
			case EFFECT_WATER:
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
				break;
			default:
				break;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// 頂点バッファのアンロック
	g_pVtxBuffEffectX->Unlock();

}
//=====================================================================================================
// エフェクトのテクスチャアニメーション
//=====================================================================================================
void SetTextureAnim(int nCnt,int XtexPos, int YtexPos,int Animspeed)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);

	// アニメーションカウンターを加算
	g_EffectX[nCnt].g_nCounterAnim++;

	// テクスチャ座標を計算
	float Uv = 1.0f / XtexPos;
	float Hv = 1.0f / YtexPos;

	// アニメーションのスピード
	if (g_EffectX[nCnt].g_nCounterAnim >= Animspeed)
	{
		g_EffectX[nCnt].g_nCounterAnim = NULL;//カウンターを初期値に戻す

		g_EffectX[nCnt].g_nPatternAnim++;//パターンナンバーを更新

		pVtx += 4 * nCnt;

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
		pVtx[1].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
		pVtx[2].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
		pVtx[3].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
	}

	// パターンが最大になったら
	if (g_EffectX[nCnt].g_nPatternAnim > XtexPos)
	{
		g_EffectX[nCnt].g_nPatternAnim = 0;//パターンナンバーを初期値に戻す
	}

	// 頂点バッファのアンロック
	g_pVtxBuffEffectX->Unlock();
}
