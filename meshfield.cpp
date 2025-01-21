//===================================
//
// メッシュフィールド[meshfield.cpp]
// Author: TEAM_C
//
//===================================

//****************************
//インクルードファイル
//****************************
#include "meshfield.h"
#include "input.h"
#include "player.h"

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshFiled = NULL;//インデックスバッファへのポインタ
MESHFILED g_MeshField;

//===============================
//メッシュフィールドの初期化処理
//===============================
void InitMeshField(void)
{
	int nCnt = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\kari.png",
		&g_pTextureMeshField);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshFiled,
		NULL);

	g_MeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	float fTexX = 1.0f / XVTX;
	float fTexY = 1.0f / ZVTX;

	//縦
	for (int nCntZ = 0; nCntZ <= ZVTX; nCntZ++)
	{
		//横
		for (int nCntX = 0; nCntX <= XVTX; nCntX++)
		{
			pVtx[nCnt].pos = D3DXVECTOR3(
				(0.0f + (MAX_WIDTH / XVTX) * nCntX) - (MAX_WIDTH * 0.5f),
				0.0f,
				MAX_HEIGHT - ((MAX_HEIGHT / ZVTX) * nCntZ) - (MAX_HEIGHT * 0.5f));

			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX,nCntZ * fTexY);

			nCnt++;//加算
		}
	}

	g_pVtxBuffMeshField->Unlock();

	WORD* pIdx;

	//インデックスバッファのロック
	g_pIdxBuffMeshFiled->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = XVTX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < ZVTX; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= XVTX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < ZVTX - 1) 
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshFiled->Unlock();
}

//===============================
//メッシュフィールドの終了処理
//===============================ccc
void UninitMeshField(void)
{
	//テクスチャの解放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffMeshFiled != NULL)
	{
		g_pIdxBuffMeshFiled->Release();
		g_pIdxBuffMeshFiled = NULL;
	}
}

//===============================
//メッシュフィールドの更新処理
//===============================
void UpdateMeshField(void)
{

}
//===============================
//メッシュフィールドの描画処理
//===============================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshField.mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);
	D3DXMatrixMultiply(&g_MeshField.mtxWorldMeshField, &g_MeshField.mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorldMeshField);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshFiled);

	//テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,VERTEX,0,POLYGON);
}
//================================
//メッシュフィールドの当たり判定
//================================
bool CollisionField(void)
{
	VERTEX_3D* pVtx;//頂点情報のポインタ

	Player* pPlayer = GetPlayer();//プレイヤーの取得

	bool bLanding = false;

	int nCntPos = 0;
	int nNextWall = 1;


	//if (pPlayer->bDisp == true)
	//{
		D3DXVECTOR3 VecALine;

		D3DXVECTOR3 pos, pos1;

		g_MeshField.vtxPos[0].x = (MAX_WIDTH / XVTX) * 0 - (MAX_WIDTH * 0.5f);
		g_MeshField.vtxPos[0].y = 0.0f;
		g_MeshField.vtxPos[0].z = MAX_HEIGHT - ((MAX_HEIGHT / ZVTX) * 0) - (MAX_HEIGHT * 0.5f);

		g_MeshField.vtxPos[1].x = (MAX_WIDTH / XVTX) * 1 - (MAX_WIDTH * 0.5f);
		g_MeshField.vtxPos[1].y = 0.0f;
		g_MeshField.vtxPos[1].z = MAX_HEIGHT - ((MAX_HEIGHT / ZVTX) * 1) - (MAX_HEIGHT * 0.5f);

		VecALine = g_MeshField.vtxPos[1] - g_MeshField.vtxPos[0];//0-1...

		D3DXVec3Normalize(&VecALine, &VecALine);

		D3DXVECTOR3 PlayerVec;

		D3DXVECTOR3 VecBLine;//壁からプレイヤー

		VecBLine = pPlayer->pos - g_MeshField.vtxPos[0];

		D3DXVec3Normalize(&VecBLine, &VecBLine);

		D3DXVECTOR3 Cross = {};

		D3DXVec3Cross(&Cross, &VecALine, &VecBLine);

		if (Cross.x < 0)
		{
			bLanding = true;
			pPlayer->pos.y = pPlayer->posOld.y;
		}
	//}
	return bLanding;//判定を返す
}

