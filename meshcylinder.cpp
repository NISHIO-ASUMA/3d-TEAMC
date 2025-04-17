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
#include "effectEdit.h"
#include "mark.h"
#include "player.h"
#include <cassert>

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_CYLINDER (200) // シリンダーの数
#define CYLINDER_ALV (0.5f) // α値の基準
#define MAX_WORD (256)      // 最大の文字数

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void UpdateHealCylinder(int CylinderIdx);		// 回復の時のシリンダーの更新処理
void UpdateEventCylinder(int CylinderIdx);		// イベント発生時のシリンダーの設定
void UpdateTrritoryCylinder(int CylinderIdx);	// テリトリーのシリンダーの設定
void UpdateItemCylinder(int CylinderIdx);		// アイテムのシリンダーの設定

//**************************************************************************************************************
// グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinderOrigin[CYLINDERTYPE_MAX] = {};	    // テクスチャへのポインタ
MeshCylinder g_MeshCylinder[MAX_CYLINDER];

//===================================================================================================================
// メッシュシリンダーの初期化処理
//===================================================================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //デバイスへのポインタ

	// 全シリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
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
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
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

#ifdef _DEBUG
	// すべてのシリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// テクスチャがNULLか確認
		assert(g_MeshCylinder[nCnt].g_pTextureMeshCylinder == NULL && "cylinder.cpp正しくテクスチャを破棄できてません");

		// インデックスバッファがNULLか確認
		assert(g_MeshCylinder[nCnt].g_pIdxBuffMeshCylinder == NULL && "cylinder.cpp正しくインデックスバッファを破棄できてません");

		// 頂点バッファがNULLか確認
		assert(g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder == NULL && "cylinder.cpp正しく頂点バッファを破棄できてません");
	}

#endif // DEBUG
}
//===================================================================================================================
// メッシュシリンダーの更新処理
//===================================================================================================================
void UpdateMeshCylinder(void)
{	
	// 全シリンダー分回す
	for (int nCntCylinder = 0; nCntCylinder < MAX_CYLINDER; nCntCylinder++)
	{
		// 未使用だったら
		if (g_MeshCylinder[nCntCylinder].bUse == false)
		{
			// 処理を読み飛ばす
			continue;
		}
		
		// 剣シリンダーの種類
		switch (g_MeshCylinder[nCntCylinder].nType)
		{
		case CYLINDERTYPE_NORMAL:
			break;
		case CYLINDERTYPE_HEAL:
			// 回復シリンダー
			UpdateHealCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_EVENT:
			// イベントのシリンダーの更新
			UpdateEventCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_TERRITORY:
			// テリトリーのシリンダーの更新
			UpdateTrritoryCylinder(nCntCylinder);
			break;
		case CYLINDERTYPE_ITEM:
			UpdateItemCylinder(nCntCylinder);
			break;
		default:
			break;
		}
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
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// 未使用だったら
		if (g_MeshCylinder[nCnt].bUse == false)
		{
			// 処理を読み飛ばす
			continue;
		}

		int nType = g_MeshCylinder[nCnt].nType;

		if (nType != CYLINDERTYPE_NORMAL)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
		}

		if (nType == CYLINDERTYPE_TERRITORY)
		{
			//ゼットテスト
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
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

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================
// メッシュシリンダーの設定処理
//===================================================================================================================
int SetMeshCylinder(D3DXVECTOR3 pos, int nType, int nLife, float fRadius, D3DXCOLOR col, int nNumPosX, int nNumPosZ,float speed,float fHeight) // メッシュシリンダーの設定
{
	int nCnt = 0;

	// 全シリンダー分回す
	for (nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
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
			g_MeshCylinder[nCnt].fHeight = fHeight;
			g_MeshCylinder[nCnt].bUse = true;

			// 代入
			int Vetex = g_MeshCylinder[nCnt].Vertex;
			int Index = g_MeshCylinder[nCnt].Index;

			// シリンダーの作成処理
			CreateMeshCylinder(nCnt, Vetex, Index, nNumPosX, nNumPosZ, fRadius,fHeight);
			break;
		}
	}
	// インデックスを返す
	return nCnt;
}
//===================================================================================================================
// メッシュシリンダーの作成処理
//===================================================================================================================
void CreateMeshCylinder(int nCntCylinder, int Vertex, int Index, int nNumPosX, int nNumPosZ,float fRadius,float fHeight) // メッシュシリンダーの作成処理
{
	int nCnt = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); //デバイスへのポインタ

	//if (g_MeshCylinder[nCntCylinder].nType == CYLINDERTYPE_HEAL)
	//{
	//	int nType = g_MeshCylinder[nCntCylinder].nType;
	//	g_MeshCylinder[nCntCylinder].g_pTextureMeshCylinder = g_pTextureMeshCylinderOrigin[nType];
	//}

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
	D3DXVECTOR3 pos = g_MeshCylinder[nCntCylinder].pos;

	//縦
	for (int nCntV = 0; nCntV <= nNumPosZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= nNumPosX; nCntH++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nNumPosX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				pos.x + sinf(fAngel) * fRadius,
				(nCntV * (fHeight / nNumPosZ)),
				pos.z + cosf(fAngel) * fRadius);

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
#ifdef _DEBUG

		// 配列がオーバーランしてないかを確認
		assert(IdxCnt <= Index && "シリンダーINDEX配列オーバーラン");

#endif // _DEBUG
	}

	//インデックスバッファのアンロック
	g_MeshCylinder[nCntCylinder].g_pIdxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// メッシュシリンダーの消去処理
//===================================================================================================================
void DeleteCylinder(int nIdx)
{
	g_MeshCylinder[nIdx].bUse = false;
}
//===================================================================================================================
// メッシュシリンダーの当たり判定
//===================================================================================================================
bool KeepInCylinder(D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx;
	bool bHit = false;

	// 全部のシリンダー分回す
	for (int nCnt = 0; nCnt < MAX_CYLINDER; nCnt++)
	{
		// 使用状態じゃないかつイベントのシリンダーじゃなかったら
		if (g_MeshCylinder[nCnt].bUse == false || g_MeshCylinder[nCnt].nType != CYLINDERTYPE_EVENT)
		{
			// 読み飛ばす
			continue;
		}

		// 全部の頂点分回す
		for (int nCntvtx = 0; nCntvtx < g_MeshCylinder[nCnt].nNumPosX; nCntvtx++)
		{
			//頂点バッファをロック
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点から目標までのベクトル
			D3DXVECTOR3 posDirection = *pPos - (g_MeshCylinder[nCnt].pos + pVtx[nCntvtx].pos);

			D3DXVECTOR3 edgeVector = (pVtx[(nCntvtx + 1) % g_MeshCylinder[nCnt].nNumPosX].pos + g_MeshCylinder[nCnt].pos) - (g_MeshCylinder[nCnt].pos + pVtx[nCntvtx].pos);

			D3DXVECTOR3 Cross = NULLVECTOR3;
			D3DXVec3Cross(&Cross, &edgeVector, &posDirection);
			D3DXVec3Normalize(&Cross, &Cross);

			if (Cross.y <= 0.0f)
			{
				bHit = true;
			}

			//頂点バッファをアンロック
			g_MeshCylinder[nCnt].g_pVtxBuffMeshCylinder->Unlock();
		}
	}
	return bHit;
}
//===================================================================================================================
// メッシュシリンダーの位置設定処理
//===================================================================================================================
void SetPotisionCylinder(int nIdx,D3DXVECTOR3 pos)
{
	if (g_MeshCylinder[nIdx].nType == CYLINDERTYPE_HEAL)
	{
		g_MeshCylinder[nIdx].pos.x = pos.x;
		g_MeshCylinder[nIdx].pos.z = pos.z;
	}
}
//===================================================================================================================
// 回復の時のシリンダーの更新処理
//===================================================================================================================
void UpdateHealCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	// 位置の更新
	g_MeshCylinder[CylinderIdx].pos.y += g_MeshCylinder[CylinderIdx].Speed;

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点のカウント
	int nCnt = 0;

	// 頂点X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// 頂点Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// 半径
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// 高さ
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// 位置を代入
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//縦
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// 角度を求める
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//加算
		}
	}

	// 透明度を上げる
	g_MeshCylinder[CylinderIdx].col.a -= g_MeshCylinder[CylinderIdx].decAlv;

	// 寿命を減らす
	g_MeshCylinder[CylinderIdx].nLife--;

	// 寿命が0になったら
	if (g_MeshCylinder[CylinderIdx].nLife <= 0)
	{
		// 未使用にする
		g_MeshCylinder[CylinderIdx].bUse = false;
	}

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// イベント発生時のシリンダーの設定
//===================================================================================================================
void UpdateEventCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点のカウント
	int nCnt = 0;

	// 頂点X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// 頂点Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// 半径
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// 高さ
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// 位置を代入
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//縦
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// 角度を求める
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//加算
		}
	}

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// テリトリーのシリンダーの設定
//===================================================================================================================
void UpdateTrritoryCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点のカウント
	int nCnt = 0;

	// 頂点X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// 頂点Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// 半径
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// 高さ
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// 位置を代入
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//縦
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// 角度を求める
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			g_MeshCylinder[CylinderIdx].col.a = 0.3f;
			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//加算
		}
	}

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// アイテムのシリンダーの設定
//===================================================================================================================
void UpdateItemCylinder(int CylinderIdx)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点のカウント
	int nCnt = 0;

	// 頂点X
	int vtxX = g_MeshCylinder[CylinderIdx].nNumPosX;

	// 頂点Z
	int vtxZ = g_MeshCylinder[CylinderIdx].nNumPosZ;

	// 半径
	float fRadius = g_MeshCylinder[CylinderIdx].fRadius;

	// 高さ
	float fHeight = g_MeshCylinder[CylinderIdx].fHeight;

	// 位置を代入
	D3DXVECTOR3 pos = g_MeshCylinder[CylinderIdx].pos;

	//縦
	for (int nCntV = 0; nCntV <= vtxZ; nCntV++)
	{
		//横
		for (int nCntH = 0; nCntH <= vtxX; nCntH++)
		{
			// 角度を求める
			float fAngel = (D3DX_PI * 2.0f) / vtxX * nCntH;

			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(
				sinf(fAngel) * fRadius,
				(nCntV * (fHeight / vtxZ)),
				cosf(fAngel) * fRadius);

			g_MeshCylinder[CylinderIdx].col.a = 0.4f;

			pVtx[nCnt].col = g_MeshCylinder[CylinderIdx].col;

			nCnt++;//加算
		}
	}

	//頂点バッファをロック
	g_MeshCylinder[CylinderIdx].g_pVtxBuffMeshCylinder->Unlock();
}
//===================================================================================================================
// シリンダーの消去
//===================================================================================================================
void ResetItemCylinder(void)
{
	// アイテムの取得
	Item* pItem = GetItem();

	// アイテム分回す
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		int nIdx = pItem[nCntItem].nCylinderIdx;

		if (g_MeshCylinder[nIdx].nType != CYLINDERTYPE_ITEM) continue;

		g_MeshCylinder[nIdx].bUse = false;
	}
}
//===================================================================================================================
// シリンダーのインデックス
//===================================================================================================================
void SetItemCylinder(int nHaveIdx)
{
	// アイテムの取得
	Item* pItem = GetItem();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\SetminimapItem.txt", "r");

	int playeritem = -1;
	int minimapItem = -2;

	// ファイルが開けたら
	if (pFile != NULL)
	{
		char aString[MAX_WORD] = {};
		char skip[10] = {};

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// PLAYER_ITEMを読み取ったら
			if (strcmp(&aString[0], "PLAYER_ITEM") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// プレイヤーのアイテムを読み取る
				nData = fscanf(pFile, "%d", &playeritem);
			}

			// MINIMAP_ITEMを読み取ったら
			if (strcmp(&aString[0], "MINIMAP_ITEM") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 表示するアイテムを読み取る
				nData = fscanf(pFile, "%d", &minimapItem);

				// 持っているアイテムが一致していたら
				if (playeritem == pItem[nHaveIdx].nType)
				{
					playeritem = -1;
					break;
				}
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				playeritem = -1;
				minimapItem = -2;

				// ファイルを閉じる
				fclose(pFile);

				// while文を抜ける
				return;
			}
		}

	}
	else
	{
		// メッセージボックスを表示
		MessageBox(NULL, "ファイルが開けません", "LoadMinimapItem", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

	// アイテム分回す
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 未使用だったら
		if (pItem[nCntItem].bUse == false) continue;

		// 持っているアイテムと同じだったら
		if (nCntItem == nHaveIdx) continue;

		// 表示したいアイテムじゃなかったら
		if (pItem[nCntItem].nType != minimapItem) continue;

		// 位置
		D3DXVECTOR3 pos(pItem[nCntItem].pos.x, 0.0f, pItem[nCntItem].pos.z);

		// アイテムの場所に位置を設定
		pItem[nCntItem].nCylinderIdx = SetMeshCylinder(pos, CYLINDERTYPE_ITEM, 0, 10.0f, COLOR_PINK, 8, 1, 0.0f, 50.0f);
	}

	// 初期化
	minimapItem = -2;
}
//===================================================================================================================
// シリンダーの当たり判定
//===================================================================================================================
bool CollisionCylinder(int nIdx, D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx;

	// 全部の頂点分回す
	for (int nCntvtx = 0; nCntvtx < g_MeshCylinder[nIdx].nNumPosX; nCntvtx++)
	{
		if (g_MeshCylinder[nIdx].nType != CYLINDERTYPE_TERRITORY) continue;

		//頂点バッファをロック
		g_MeshCylinder[nIdx].g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点から目標までのベクトル
		D3DXVECTOR3 posDirection = *pPos - (g_MeshCylinder[nIdx].pos + pVtx[nCntvtx].pos);

		D3DXVECTOR3 edgeVector = (pVtx[(nCntvtx + 1) % g_MeshCylinder[nIdx].nNumPosX].pos + g_MeshCylinder[nIdx].pos) - (g_MeshCylinder[nIdx].pos + pVtx[nCntvtx].pos);

		D3DXVECTOR3 Cross = NULLVECTOR3;
		D3DXVec3Cross(&Cross, &edgeVector, &posDirection);
		D3DXVec3Normalize(&Cross, &Cross);

		if (Cross.y <= 0.0f)
		{
			return true;
		}

		//頂点バッファをアンロック
		g_MeshCylinder[nIdx].g_pVtxBuffMeshCylinder->Unlock();
	}
	return false;
}
