//===================================================================================================================
//
// メッシュシリンダー[meshcylinder.cpp]
// Author: YOSHIDA YUTO
//
//===================================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "meshcylinder.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_CILINDER (10) // シリンダーの数
#define CYLINDER_ALV (0.6f) // α値の基準

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
MeshCylinder g_MeshCylinder[MAX_CILINDER];

//===================================================================================================================
// メッシュシリンダーの初期化処理
//===================================================================================================================
void InitMeshCylinder(void)
{
	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CILINDER; nCnt++)
	{
		g_MeshCylinder[nCnt].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_MeshCylinder[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshCylinder[nCnt].nType = CYLINDERTYPE_NORMAL;
		g_MeshCylinder[nCnt].nLife = 0;
		g_MeshCylinder[nCnt].fRadius = 0.0f;
		g_MeshCylinder[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_MeshCylinder[nCnt].nNumPosX = 0;
		g_MeshCylinder[nCnt].nNumPosZ = 0;
		g_MeshCylinder[nCnt].Index = 0;
		g_MeshCylinder[nCnt].Vertex = 0;
		g_MeshCylinder[nCnt].polygon = 0;
		g_MeshCylinder[nCnt].Speed = 0.0f;
		g_MeshCylinder[nCnt].decAlv = 0.0f;
		g_MeshCylinder[nCnt].bUse = false;
	}
}
//===================================================================================================================
// メッシュシリンダーの終了処理
//===================================================================================================================
void UninitMeshCylinder(void)
{
	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CILINDER; nCnt++)
	{
		// テクスチャの解放
		if (g_MeshCylinder[nCnt].g_pTextureMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pTextureMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pTextureMeshCylinder = NULL;
		}
		// インデックスバッファの解放
		if (g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder = NULL;
		}
		// 頂点バッファの解放
		if (g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder != NULL)
		{
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Release();
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder = NULL;
		}
	}
}
//===================================================================================================================
// メッシュシリンダーの更新処理
//===================================================================================================================
void UpdateMeshCylinder(void)
{
	VERTEX_3D* pVtx = NULL;

	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CILINDER; nCnt++)
	{
		// 未使用だったら
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			// 処理を読み飛ばす
			continue;
		}

		////頂点バッファをロック
		//g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	}
}
//===================================================================================================================
// メッシュシリンダーの描画処理
//===================================================================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CILINDER; nCnt++)
	{
		// 未使用だったら
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			// 処理を読み飛ばす
			continue;
		}

		if (g_MeshCylinder[nCnt].nType == CYLINDERTYPE_HEAL)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
		}

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_MeshCylinder[nCnt].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder[nCnt].rot.y, g_MeshCylinder[nCnt].rot.x, g_MeshCylinder[nCnt].rot.z);
		D3DXMatrixMultiply(&g_MeshCylinder[nCnt].mtxWorld, &g_MeshCylinder[nCnt].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder[nCnt].pos.x, g_MeshCylinder[nCnt].pos.y, g_MeshCylinder[nCnt].pos.z);
		D3DXMatrixMultiply(&g_MeshCylinder[nCnt].mtxWorld, &g_MeshCylinder[nCnt].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder[nCnt].mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshCylinder[nCnt].Vertex, 0, g_MeshCylinder[nCnt].polygon);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================
// メッシュシリンダーの設定処理
//===================================================================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, int nType, int nLife, float fRadius, D3DXCOLOR col, int nNumPosX, int nNumPosZ,float speed,float fHeight) // メッシュシリンダーの設定
{
	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CILINDER; nCnt++)
	{
		// 未使用だったら
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			g_MeshCylinder[nCnt].Vertex = (nNumPosX + 1) * (nNumPosZ + 1);
			g_MeshCylinder[nCnt].polygon = ((nNumPosX * nNumPosZ * 2)) + (4 * nNumPosZ - 1);
			g_MeshCylinder[nCnt].Index = g_MeshCylinder[nCnt].polygon + 2;
			g_MeshCylinder[nCnt].pos = pos;
			g_MeshCylinder[nCnt].nType = nType;
			g_MeshCylinder[nCnt].nLife = nLife;
			g_MeshCylinder[nCnt].fRadius = fRadius;
			g_MeshCylinder[nCnt].col = col;
			g_MeshCylinder[nCnt].col.a = CYLINDER_ALV;
			g_MeshCylinder[nCnt].nNumPosX = nNumPosX;
			g_MeshCylinder[nCnt].nNumPosZ = nNumPosZ;
			g_MeshCylinder[nCnt].decAlv = CYLINDER_ALV / nLife;
			g_MeshCylinder[nCnt].Speed = speed;
			g_MeshCylinder[nCnt].fHeight= fHeight;
			g_MeshCylinder[nCnt].bUse = true;

			// 代入
			int Vetex = g_MeshCylinder[nCnt].Vertex;
			int Index = g_MeshCylinder[nCnt].Index;

			// シリンダーの作成処理
			CreateMeshCylinder(nCnt, Vetex, Index, nNumPosX, nNumPosZ, fRadius,fHeight);
			break;
		}
	}
}
//===================================================================================================================
// メッシュシリンダーの作成処理
//===================================================================================================================
void CreateMeshCylinder(int nCntCylinder, int Vertex, int Index, int nNumPosX, int nNumPosZ,float fRadius,float fHeight) // メッシュシリンダーの作成処理
{
	int nCnt = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();//デバイスを取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * Index,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / nNumPosX;
	float fTexY = 1.0f / nNumPosZ;

	D3DXVECTOR3 nor;
	D3DXVECTOR3 pos = g_MeshCylinder[nCnt].pos;

	//縦
	for (int nCntV = 0; nCntV <= nNumPosZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= nNumPosX; nCntH++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nNumPosX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / nNumPosZ)),
				cosf(fAngel) * fRadius);

			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			nor = pVtx[nCnt].pos - pos;

			D3DXVec3Normalize(&pVtx[nCnt].nor, &nor);

			//頂点カラーの設定
			pVtx[nCnt].col = g_MeshCylinder[nCntCylinder].col;


			//テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, nCntV * fTexY);

			nCnt++;//加算
		}
	}

	g_MeshCylinder[nCntCylinder].g_pVtxBuffMeshCylinder->Unlock();

	WORD* pIdx;

	//インデックスバッファのロック
	g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	int IndxCount3 = nNumPosX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nNumPosZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nNumPosX; IndxCount2++, IndxCount3++, Num++)
		{
			pIdx[IdxCnt] = IndxCount3;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		if (IndxCount1 < nNumPosZ - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxCount3;
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder->Unlock();
}
