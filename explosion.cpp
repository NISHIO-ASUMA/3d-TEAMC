//============================
//
// 爆発 [explosion.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "explosion.h"
#include "camera.h"
#include "player.h"

//****************************
//マクロ定義
//****************************
#define EXPLOSION_WIDTH (25.0f) //横幅
#define EXPLOSION_HEIGHT (25.0f) //高さ
#define MAX_EXPLOSION (80) //爆発の最大数
#define LANDINGEXPLOSION (8)

//****************************
//プロトタイプ宣言
//****************************

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //頂点バッファへのポインタ
EXPLOSION g_Explosion[MAX_EXPLOSION];

//============================
//爆発の初期化処理
//============================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion.png",
		&g_pTextureExplosion);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_Explosion[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[nCnt].nLife = 0;
		g_Explosion[nCnt].nCounterAnim = 0;
		g_Explosion[nCnt].fHeight = 0;
		g_Explosion[nCnt].fWidth = 0;
		g_Explosion[nCnt].nPatternAnim = 0;
		g_Explosion[nCnt].speed = 10.0f;
		g_Explosion[nCnt].radius = 30.0f;
		g_Explosion[nCnt].nType = EXPLOSION_MOVE;
		g_Explosion[nCnt].bUse = false;
	}

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x - EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y + EXPLOSION_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x + EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y + EXPLOSION_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x - EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y - EXPLOSION_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Explosion[nCnt].pos.x + EXPLOSION_WIDTH, g_Explosion[nCnt].pos.y - EXPLOSION_HEIGHT, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 0.3f);

		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
//============================
//爆発の終了処理
//============================
void UninitExplosion(void)
{
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//============================
//爆発の更新処理
//============================
void UpdateExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	Player* pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_3D* pVtx;//頂点情報のポインタ

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_Explosion[nCntExplosion].nCounterAnim++;

		if (g_Explosion[nCntExplosion].bUse == true)
		{
			g_Explosion[nCntExplosion].nLife--;

			if (g_Explosion[nCntExplosion].nLife <= 0)
			{
				g_Explosion[nCntExplosion].bUse = false;//寿命が尽きた
			}

			//位置の更新
			g_Explosion[nCntExplosion].move.x += (0.0f - g_Explosion[nCntExplosion].move.x) * 0.01f;
			g_Explosion[nCntExplosion].move.z += (0.0f - g_Explosion[nCntExplosion].move.z) * 0.01f;
			   
			g_Explosion[nCntExplosion].pos += g_Explosion[nCntExplosion].move;
			   

			if (g_Explosion[nCntExplosion].nCounterAnim >= 4)
			{
				g_Explosion[nCntExplosion].nCounterAnim = NULL;//カウンターを初期値に戻す

				g_Explosion[nCntExplosion].nPatternAnim++;//パターンナンバーを更新

				pVtx[0].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f);
				pVtx[1].tex = D3DXVECTOR2(0.25f + g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f);
				pVtx[2].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f + 0.333f);
				pVtx[3].tex = D3DXVECTOR2(0.25f + g_Explosion[nCntExplosion].nPatternAnim * 0.25f, (g_Explosion[nCntExplosion].nPatternAnim / 4) * 0.333f + 0.333f);

			}

			if (g_Explosion[nCntExplosion].nPatternAnim > 11)
			{
				g_Explosion[nCntExplosion].nPatternAnim = 0;//パターンナンバーを初期値に戻す
				g_Explosion[nCntExplosion].bUse = false;
			}

			if (g_Explosion[nCntExplosion].nType == EXPLOSION_LANDING)
			{
				for (int nCnt = 0; nCnt < LANDINGEXPLOSION; nCnt++)
				{
					float fAngle = (D3DX_PI * 2.0f) / LANDINGEXPLOSION * nCnt;

					g_Explosion[nCnt].move.x = sinf(fAngle) * 1.0f;
					g_Explosion[nCnt].move.y = 0.3f;
					g_Explosion[nCnt].move.z = cosf(fAngle) * 1.0f;
				}
			}
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}
//============================
//爆発の描画処理
//============================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_Explosion[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Explosion[nCnt].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);//ビューマトリックスの取得

			//カメラの逆行列の設定
			g_Explosion[nCnt].mtxWorld._11 = mtxView._11;
			g_Explosion[nCnt].mtxWorld._12 = mtxView._21;
			g_Explosion[nCnt].mtxWorld._13 = mtxView._31;
			g_Explosion[nCnt].mtxWorld._21 = mtxView._12;
			g_Explosion[nCnt].mtxWorld._22 = mtxView._22;
			g_Explosion[nCnt].mtxWorld._23 = mtxView._32;
			g_Explosion[nCnt].mtxWorld._31 = mtxView._13;
			g_Explosion[nCnt].mtxWorld._32 = mtxView._23;
			g_Explosion[nCnt].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Explosion[nCnt].pos.x, g_Explosion[nCnt].pos.y, g_Explosion[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Explosion[nCnt].mtxWorld, &g_Explosion[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Explosion[nCnt].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//テクスチャフォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
	//アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//============================
//爆発の設定処理
//============================
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife,float fWidth,float fHeight, int nType)
{
	VERTEX_3D* pVtx;//頂点情報のポインタ

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_Explosion[nCnt].bUse == false)
		{
			g_Explosion[nCnt].pos = pos;
			g_Explosion[nCnt].fWidth = fWidth;
			g_Explosion[nCnt].fHeight = fHeight;
			g_Explosion[nCnt].col = col;
			g_Explosion[nCnt].nLife = nLife;
			g_Explosion[nCnt].nType = nType;
			g_Explosion[nCnt].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth,  -fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth,  -fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}
