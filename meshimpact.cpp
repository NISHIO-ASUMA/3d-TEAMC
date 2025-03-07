//=====================================================================================================================
//
// メッシュインパクト[meshimpact.cpp]
// Author: YOSHIDA YUTO
//
//=====================================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "input.h"
#include "player.h"
#include "meshimpact.h"
#include "Effect.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MAX_IMPACT (50) // 衝撃波の最大数
#define A_OFFSET (0.7f) // α値の基準

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
MeshImpact g_MeshImpact[MAX_IMPACT];

//=================================================================================================================
//メッシュフィールドの初期化処理
//=================================================================================================================
void InitMeshImpact(void)
{
	// 衝撃波の最大数分回す
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		// 初期化処理
		g_MeshImpact[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 位置
		g_MeshImpact[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向き
		g_MeshImpact[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_MeshImpact[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f); // 色
		g_MeshImpact[nCnt].nImpactPosX = 0;                      // インパクトのXの分割数
		g_MeshImpact[nCnt].Vertex = 0;							 // 頂点数
		g_MeshImpact[nCnt].Polygon = 0;                          // ポリゴン数
		g_MeshImpact[nCnt].Index = 0;							 // インデックス数
		g_MeshImpact[nCnt].decAlv = 0.0f;                        // a値の減少量
		g_MeshImpact[nCnt].speed = 0.0f;                         // 速さ
		g_MeshImpact[nCnt].nLife = 0;                            // 寿命
		g_MeshImpact[nCnt].nType = IMPACTTYPE_NORMAL;            // 種類
		g_MeshImpact[nCnt].OutRadius = 0.0f;                     // 外側の半径
		g_MeshImpact[nCnt].InRadius = 0.0f;                      // 内側の半径
		g_MeshImpact[nCnt].bUse = false;                         // 使用状態
	}

	//g_MeshImpact.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_MeshImpact.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=================================================================================================================
//メッシュフィールドの終了処理
//=================================================================================================================ccc
void UninitMeshImpact(void)
{
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		//テクスチャの解放
		if (g_MeshImpact[nCnt].g_pTextureMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pTextureMeshImpact->Release();
			g_MeshImpact[nCnt].g_pTextureMeshImpact = NULL;
		}

		//頂点バッファの解放
		if (g_MeshImpact[nCnt].g_pVtxBuffMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Release();
			g_MeshImpact[nCnt].g_pVtxBuffMeshImpact = NULL;
		}

		//インデックスバッファの解放
		if (g_MeshImpact[nCnt].g_pIdxBuffMeshImpact != NULL)
		{
			g_MeshImpact[nCnt].g_pIdxBuffMeshImpact->Release();
			g_MeshImpact[nCnt].g_pIdxBuffMeshImpact = NULL;
		}
	}
}

//=================================================================================================================
//メッシュフィールドの更新処理
//=================================================================================================================
void UpdateMeshImpact(void)
{
	VERTEX_3D* pVtx;

	Player* pPlayer = GetPlayer();

	// 衝撃波の最大数分回す
	for (int nCnt = 0 ; nCnt < MAX_IMPACT; nCnt++)
	{
		if (g_MeshImpact[nCnt].bUse == false)
		{
			continue;
		}

		int nCntVertex = 0;

		if (g_MeshImpact[nCnt].nType != IMPACTTYPE_SPKATANA)
		{
			g_MeshImpact[nCnt].OutRadius += g_MeshImpact[nCnt].speed;
			g_MeshImpact[nCnt].InRadius += g_MeshImpact[nCnt].speed;
		}
		else
		{
			g_MeshImpact[nCnt].OutRadius -= g_MeshImpact[nCnt].speed;
			g_MeshImpact[nCnt].InRadius -= g_MeshImpact[nCnt].speed;
		}
		//頂点バッファをロック
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos = g_MeshImpact[nCnt].pos;

		for (int nCntImpact = 0; nCntImpact <= g_MeshImpact[nCnt].nImpactPosX; nCntImpact++)
		{
			float fAngel = (D3DX_PI * 2.0f) / g_MeshImpact[nCnt].nImpactPosX * nCntImpact;

			//頂点座標の設定
			pVtx[nCntVertex].pos = D3DXVECTOR3(
				sinf(fAngel) * g_MeshImpact[nCnt].OutRadius,
				2.0f,
				cosf(fAngel) * g_MeshImpact[nCnt].OutRadius);

			pVtx[nCntVertex].col = g_MeshImpact[nCnt].col;

			nCntVertex++;
		}

		for (int nCntImpact = 0; nCntImpact <= g_MeshImpact[nCnt].nImpactPosX; nCntImpact++)
		{
			float fAngel = (D3DX_PI * 2.0f) / g_MeshImpact[nCnt].nImpactPosX * nCntImpact;

			//頂点座標の設定
			pVtx[nCntVertex].pos = D3DXVECTOR3(
				sinf(fAngel) * g_MeshImpact[nCnt].InRadius,
				2.0f,
				cosf(fAngel) * g_MeshImpact[nCnt].InRadius);

			pVtx[nCntVertex].col = g_MeshImpact[nCnt].col;

			nCntVertex++;
		}

		// 寿命を減らす
		g_MeshImpact[nCnt].nLife--;

		// a値をだんだん減少指せる
		g_MeshImpact[nCnt].col.a -= g_MeshImpact[nCnt].decAlv;

		// 寿命が尽きた
		if (g_MeshImpact[nCnt].nLife <= 0)
		{
			// 未使用にする
			g_MeshImpact[nCnt].bUse = false;
		}
		// 頂点バッファのアンロック
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Unlock();
	}
}
//=================================================================================================================
//メッシュフィールドの描画処理
//=================================================================================================================
void DrawMeshImpact(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntImpact = 0; nCntImpact < MAX_IMPACT; nCntImpact++)
	{
		if (g_MeshImpact[nCntImpact].bUse == false)
		{
			continue;
		}

		////アルファテストを有効
		//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_MeshImpact[nCntImpact].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshImpact[nCntImpact].rot.y, g_MeshImpact[nCntImpact].rot.x, g_MeshImpact[nCntImpact].rot.z);
		D3DXMatrixMultiply(&g_MeshImpact[nCntImpact].mtxWorld, &g_MeshImpact[nCntImpact].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshImpact[nCntImpact].pos.x, g_MeshImpact[nCntImpact].pos.y, g_MeshImpact[nCntImpact].pos.z);
		D3DXMatrixMultiply(&g_MeshImpact[nCntImpact].mtxWorld, &g_MeshImpact[nCntImpact].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshImpact[nCntImpact].mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_MeshImpact[nCntImpact].g_pTextureMeshImpact);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshImpact[nCntImpact].Vertex, 0, g_MeshImpact[nCntImpact].Polygon);
	}

	////aブレンディングをもとに戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=================================================================================================================
// インパクトの作成処理
//=================================================================================================================
void CreateImpact(int nCntImpact, int nImpactPosX, int Vertex, int Index)
{
	float fTexX = 1.0f / nImpactPosX;
	float fTexY = 1.0f / IMPACT_Z;

	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\.png",
	//	&g_pTextureMeshImpact);

		// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact,
		NULL);

	VERTEX_3D* pVtx = NULL;
	int nCnt = 0;

	//頂点バッファをロック
	g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

	// 外側の半径
	float OutRadius = g_MeshImpact[nCntImpact].OutRadius;

	D3DXCOLOR col = g_MeshImpact[nCntImpact].col;

	D3DXVECTOR3 pos = g_MeshImpact[nCntImpact].pos;

	//横
	for (int nCntH = 0; nCntH <= nImpactPosX; nCntH++)
	{
		float fAngel = (D3DX_PI * 2.0f) / nImpactPosX * nCntH;

		//頂点座標の設定
		pVtx[nCnt].pos = D3DXVECTOR3(
			pos.x + sinf(fAngel) * OutRadius,
			2.0f,
			pos.z + cosf(fAngel) * OutRadius);

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = col;

		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, 1.0f);

		nCnt++;//加算
	}

	// 外側の半径
	float InRadius = g_MeshImpact[nCntImpact].InRadius;

	//横
	for (int nCntH = 0; nCntH <= nImpactPosX; nCntH++)
	{
		float fAngel = (D3DX_PI * 2.0f) / nImpactPosX * nCntH;

		//頂点座標の設定
		pVtx[nCnt].pos = D3DXVECTOR3(
			pos.x + sinf(fAngel) * InRadius,
			2.0f,
			pos.z + cosf(fAngel) * InRadius);

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = col;

		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntH, 1.0f);

		nCnt++;//加算
	}

	g_MeshImpact[nCntImpact].g_pVtxBuffMeshImpact->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * Index,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact,
		NULL);

	WORD* pIdx;

	//インデックスバッファのロック
	g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = nImpactPosX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < IMPACT_Z; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nImpactPosX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	g_MeshImpact[nCntImpact].g_pIdxBuffMeshImpact->Unlock();
}
//=================================================================================================================
// インパクトの判定処理
//=================================================================================================================
bool CollisionImpact(D3DXVECTOR3* pPos)
{
	VERTEX_3D* pVtx = NULL;
	Player* pPlayer = GetPlayer();
	bool bHit = false;

	// 衝撃波の最大数分回す
	for (int nCnt = 0; nCnt < MAX_IMPACT; nCnt++)
	{
		// 未使用だったら
		if (g_MeshImpact[nCnt].bUse == false || g_MeshImpact[nCnt].nType == IMPACTTYPE_NORMAL)
		{// 処理を読み飛ばす
			continue;
		}

		//頂点バッファをロック
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Lock(0, 0, (void**)&pVtx, 0);

		// Xの分割数分
		for (int nCntvtx = 0; nCntvtx < g_MeshImpact[nCnt].Vertex; nCntvtx++)
		{
			// 位置の差分を取る
			float DiffPosX = pPos->x - (g_MeshImpact[nCnt].pos.x + pVtx[nCntvtx].pos.x);
			float DiffPosY = pPos->y - (g_MeshImpact[nCnt].pos.y + pVtx[nCntvtx].pos.y);
			float DiffPosZ = pPos->z - (g_MeshImpact[nCnt].pos.z + pVtx[nCntvtx].pos.z);

			// 頂点との距離を求める
			float fDistance = sqrtf((DiffPosX * DiffPosX) + (DiffPosY * DiffPosY) + (DiffPosZ * DiffPosZ));

			// 大きさを求める
			float DiffSize = g_MeshImpact[nCnt].OutRadius - g_MeshImpact[nCnt].InRadius;

			// 距離がインパクトの大きさよりも小さくなったら
			if (fDistance <= DiffSize * 0.5f && g_MeshImpact[nCnt].nType == IMPACTTYPE_PLAYER)
			{	
				// 当たっている
				bHit = true;
			}
			else if (fDistance <= DiffSize * 0.5f && g_MeshImpact[nCnt].nType != IMPACTTYPE_PLAYER)
			{
				// 当たっている
				HitPlayer(20);
			}
		}

		// 頂点バッファのアンロック
		g_MeshImpact[nCnt].g_pVtxBuffMeshImpact->Unlock();
	}

	// 判定を返す
	return bHit;
}
//=================================================================================================================
// インパクトのダメージ設定処理
//=================================================================================================================
int ImpactDamege(int nID)
{
	return g_MeshImpact[nID].nDamege;
}

//=================================================================================================================
// インパクトの設定処理
//=================================================================================================================
void SetImpact(D3DXVECTOR3 pos, D3DXCOLOR col, int nImpactPosX, float OutRadius, float InRadius, float Speed, int nLife, int nType, int nDamege)
{
	// 衝撃波の最大数分回す
	for (int nCntImpact = 0; nCntImpact < MAX_IMPACT; nCntImpact++)
	{
		// 未使用だったら
		if (g_MeshImpact[nCntImpact].bUse == false)
		{
			// 設定処理
			g_MeshImpact[nCntImpact].Vertex = (nImpactPosX + 1) * (IMPACT_Z + 1);   // 頂点数
			g_MeshImpact[nCntImpact].Polygon = g_MeshImpact[nCntImpact].Vertex - 2; // ポリゴン数
			g_MeshImpact[nCntImpact].Index = g_MeshImpact[nCntImpact].Vertex;       // インデックス数
			g_MeshImpact[nCntImpact].nImpactPosX = nImpactPosX;                     // Xの分割数
			g_MeshImpact[nCntImpact].pos = pos;										// 位置
			g_MeshImpact[nCntImpact].OutRadius = OutRadius;							// 外側の半径
			g_MeshImpact[nCntImpact].InRadius = InRadius;							// 内側の半径
			g_MeshImpact[nCntImpact].speed = Speed;							        // 速さ
			g_MeshImpact[nCntImpact].nLife = nLife;							        // 寿命
			g_MeshImpact[nCntImpact].nDamege = nDamege;							    // 攻撃力
			g_MeshImpact[nCntImpact].nType = nType;							        // 種類
			g_MeshImpact[nCntImpact].col = col;										// 色
			g_MeshImpact[nCntImpact].col.a = A_OFFSET;								// あらかじめ薄くしておく
			g_MeshImpact[nCntImpact].decAlv = A_OFFSET / nLife;                     // a値の減少値
			g_MeshImpact[nCntImpact].bUse = true;							        // 使用する

			// インパクトの作成処理
			CreateImpact(nCntImpact, nImpactPosX, g_MeshImpact[nCntImpact].Vertex, g_MeshImpact[nCntImpact].Index);

			break;
		}
	}

}