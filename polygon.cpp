//==============================================================================================================
//
// ポリゴン[shadow.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "player.h"
#include "camera.h"
#include "input.h"
#include"polygon.h"
#include "boss.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_POLYGON (100)
#define OFFSET (100.0f) /// 矢印ポリゴンのオフセット

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void SetMtxPolygon(int nCntPolygon);

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL; //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon[POLYGON_TYPE_MAX] = {};//テクスチャへのポインタ保存用
Polygon_Info g_Polygon[MAX_POLYGON];//ポリゴンの構造体

//===============================================================================================================
//ポリゴンの初期化処理
//===============================================================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCntPolygon = 0; nCntPolygon < POLYGON_TYPE_MAX; nCntPolygon++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			POLYGON_TYPE[nCntPolygon],
			&g_pTexturePolygon[nCntPolygon]);
	}

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		g_Polygon[nCntPolygon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Polygon[nCntPolygon].nType = 0.0f;
		g_Polygon[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Polygon[nCntPolygon].fWidth = 0.0f;
		g_Polygon[nCntPolygon].fHeight = 0.0f;
		g_Polygon[nCntPolygon].fLength = 0.0f;
		g_Polygon[nCntPolygon].bUse = false;
		g_Polygon[nCntPolygon].fMove = 10.0f;
	}

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	g_pVtxBuffPolygon->Unlock();
}
//===============================================================================================================
//ポリゴンの終了処理
//===============================================================================================================
void UninitPolygon(void)
{
	for (int nCntPolygon = 0; nCntPolygon < POLYGON_TYPE_MAX; nCntPolygon++)
	{
		if (g_pTexturePolygon[nCntPolygon] != NULL)
		{
			g_pTexturePolygon[nCntPolygon]->Release();
			g_pTexturePolygon[nCntPolygon] = NULL;
		}
	}

	//頂点バッファの解放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//===============================================================================================================
//ポリゴンの更新処理
//===============================================================================================================
void UpdatePolygon(void)
{
	Camera* pCamera = GetCamera();
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_Polygon[nCntPolygon].bUse == false)
		{
			continue;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Polygon[nCntPolygon].pos.x - g_Polygon[nCntPolygon].fWidth, 1.0f, g_Polygon[nCntPolygon].pos.z + g_Polygon[nCntPolygon].fHeight);
		pVtx[1].pos = D3DXVECTOR3(g_Polygon[nCntPolygon].pos.x + g_Polygon[nCntPolygon].fWidth, 1.0f, g_Polygon[nCntPolygon].pos.z + g_Polygon[nCntPolygon].fHeight);
		pVtx[2].pos = D3DXVECTOR3(g_Polygon[nCntPolygon].pos.x - g_Polygon[nCntPolygon].fWidth, 1.0f, g_Polygon[nCntPolygon].pos.z - g_Polygon[nCntPolygon].fHeight);
		pVtx[3].pos = D3DXVECTOR3(g_Polygon[nCntPolygon].pos.x + g_Polygon[nCntPolygon].fWidth, 1.0f, g_Polygon[nCntPolygon].pos.z - g_Polygon[nCntPolygon].fHeight);
		pVtx += 4;
	}

	g_pVtxBuffPolygon->Unlock();

}
//===============================================================================================================
//ポリゴンの描画処理
//===============================================================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (!g_Polygon[nCntPolygon].bUse)
		{
			continue;
		}

		int nType = g_Polygon[nCntPolygon].nType;

		if (nType == POLYGON_TYPE_FIVE)
		{
			// ポリゴンのワールドマトリックスの設定
			SetMtxPolygon(nCntPolygon);
		}
		else
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Polygon[nCntPolygon].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Polygon[nCntPolygon].rot.y, g_Polygon[nCntPolygon].rot.x, g_Polygon[nCntPolygon].rot.z);
			D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Polygon[nCntPolygon].pos.x, g_Polygon[nCntPolygon].pos.y, g_Polygon[nCntPolygon].pos.z);
			D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Polygon[nCntPolygon].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			//テクスチャフォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
		}
	}
}
//===============================================================================================================
//ポリゴンの設定処理
//===============================================================================================================
void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nType)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		if (!g_Polygon[nCnt].bUse)
		{
			g_Polygon[nCnt].pos = pos;
			g_Polygon[nCnt].rot = rot;
			g_Polygon[nCnt].fWidth = fWidth;
			g_Polygon[nCnt].fHeight = fHeight;
			g_Polygon[nCnt].nType = nType;
			g_Polygon[nCnt].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth,1.0f, pos.z + fHeight);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, 1.0f, pos.z + fHeight);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, 1.0f, pos.z - fHeight);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, 1.0f, pos.z - fHeight);
			
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffPolygon->Unlock();
}
//===============================================================================================================
// ポリゴンのワールドマトリックス設定
//===============================================================================================================
void SetMtxPolygon(int nCntPolygon)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Boss* pBoss = Getboss();
	Player* pPlayer = GetPlayer();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxParent;

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++,pBoss++)
	{
		if (pBoss->bUse == false)
		{
			continue;
		}

		// 現在のタイプを取得
		int nType = g_Polygon[nCntPolygon].nType;

		// 角度を求める
		float fAngle = atan2f(pPlayer->pos.x - pPlayer->pos.x, pPlayer->pos.z - pPlayer->pos.z) + D3DX_PI;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Polygon[nCntPolygon].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, fAngle, pBoss->rot.x, pBoss->rot.z);
		D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Polygon[nCntPolygon].pos.x, g_Polygon[nCntPolygon].pos.y + 1.1f, g_Polygon[nCntPolygon].pos.z - OFFSET);
		D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxTrans);

		// 親のワールドマトリックスの設定
		mtxParent = pBoss->mtxWorld;

		// 親のワールドマトリックスと自分のワールドマトリックスをかけ合わせる
		D3DXMatrixMultiply(&g_Polygon[nCntPolygon].mtxWorld, &g_Polygon[nCntPolygon].mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Polygon[nCntPolygon].mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// モーションタイプがアクションだったら
		if (pBoss->Motion.motionType == MOTIONTYPE_ACTION)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
		}
	}

}
