//=================================
//
// メッシュソード[meshsword.h]
// Author:YOSHIDA YUUTO
//
//=================================

//****************************
//インクルードファイル
//****************************
#include"meshsword.h"
#include "input.h"
#include "player.h"
#include "mouse.h"

//****************************
//プロトタイプ宣言
//****************************

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureMeshSword = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshSword = NULL; //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshSword = NULL;//インデックスバッファへのポインタ
MESHSOAD g_MeshSword;
D3DXVECTOR3 g_posOlds[(X + 1) * 2];
int g_nMeshSwordCount;

//===============================
//メッシュフィールドの初期化処理
//===============================
void InitMeshSword(void)
{
	int nCnt = 0;
	g_nMeshSwordCount = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();//デバイスを取得

		//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\orbit.png",
		&g_pTextureMeshSword);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ORBIT_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshSword,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ORBIT_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshSword,
		NULL);

	// 初期化
	g_MeshSword.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshSword.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MeshSword.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	g_MeshSword.Scal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_3D* pVtx = NULL;

	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得

	//頂点バッファをロック
	g_pVtxBuffMeshSword->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	/*pVtx[0].pos = D3DXVECTOR3(pPlayer->SwordMtx._41, pPlayer->SwordMtx._42, pPlayer->SwordMtx._43);

	pVtx[1].pos = D3DXVECTOR3(pPlayer->Motion.aModel[15].mtxWorld._41, pPlayer->Motion.aModel[15].mtxWorld._42, pPlayer->Motion.aModel[15].mtxWorld._43);*/

	for (int nCount = 0; nCount < X + 1; nCount++)
	{
		pVtx[nCount * 2].pos = g_posOlds[nCount * 2];
		pVtx[nCount * 2 + 1].pos = g_posOlds[nCount * 2 + 1];
	}

	////頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点座標の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCount * 2 + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	float Tex = 1.0f / X;

	//頂点座標の設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - Tex * nCount);
		pVtx[nCount * 2 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - Tex * nCount);
	}

	//for (int nCnt = 0; nCnt < ORBIT_VERTEX; nCnt++)
	//{
		////頂点座標の設定
		//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}
	 
		//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);

	for (int nCount = 0; nCount < X; nCount++)
	{
		pVtx[nCount * 2 + 2].tex = D3DXVECTOR2(Tex * nCount,0.0f);
		pVtx[nCount * 2 + 3].tex = D3DXVECTOR2(Tex * nCount, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffMeshSword->Unlock();

	WORD* pIdx;

	//インデックスバッファのロック
	g_pIdxBuffMeshSword->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < ORBIT_INDEX; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}
	//インデックスバッファのアンロック
	g_pIdxBuffMeshSword->Unlock();

}

//===============================
//メッシュフィールドの終了処理
//===============================ccc
void UninitMeshSword(void)
{
	//テクスチャの解放
	if (g_pTextureMeshSword != NULL)
	{
		g_pTextureMeshSword->Release();
		g_pTextureMeshSword = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffMeshSword != NULL)
	{
		g_pVtxBuffMeshSword->Release();
		g_pVtxBuffMeshSword = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffMeshSword != NULL)
	{
		g_pIdxBuffMeshSword->Release();
		g_pIdxBuffMeshSword = NULL;
	}
}

//===============================
//メッシュフィールドの更新処理
//===============================
void UpdateMeshSword(void)
{
	if (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_X))
	{
		ResetMeshSword();
	}
	g_nMeshSwordCount++;
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得
	VERTEX_3D* pVtx;

	int vtxCnt = 0;

	// 頂点バッファをロック
	g_pVtxBuffMeshSword->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->Motion.motionType == MOTIONTYPE_ACTION||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION2||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION3||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION4)
	{
		g_MeshSword.oldvtx[0] = pVtx[0].pos;
		g_MeshSword.oldvtx[1] = pVtx[1].pos;

		for (int nCount = 0; nCount < X; nCount++)
		{
			g_posOlds[(X - nCount) * 2] = g_posOlds[(X - nCount) * 2 - 2];
			g_posOlds[(X - nCount) * 2 + 1] = g_posOlds[(X - nCount) * 2 - 1];
		}

		g_posOlds[0].x = pPlayer->SwordMtx._41;
		g_posOlds[0].y = pPlayer->SwordMtx._42;
		g_posOlds[0].z = pPlayer->SwordMtx._43;

		g_posOlds[1].x = pPlayer->Motion.aModel[15].mtxWorld._41;
		g_posOlds[1].y = pPlayer->Motion.aModel[15].mtxWorld._42;
		g_posOlds[1].z = pPlayer->Motion.aModel[15].mtxWorld._43;

		for (int nCount = 0; nCount < (X + 1) * 2; nCount++)
		{
			pVtx[nCount].pos = g_posOlds[nCount];
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMeshSword->Unlock();
}

//===============================
//メッシュフィールドの描画処理
//===============================
void DrawMeshSword(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得

	pDevice = GetDevice();
	int Min;

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	if ((pPlayer->Motion.motionType == MOTIONTYPE_ACTION ||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION2 ||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION3 ||
		pPlayer->Motion.motionType == MOTIONTYPE_ACTION4) && pPlayer->Motion.nNumModel != 15)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_MeshSword.mtxWorldMeshSword);

		pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshSword.rot.y, g_MeshSword.rot.x, g_MeshSword.rot.z);
		D3DXMatrixMultiply(&g_MeshSword.mtxWorldMeshSword, &g_MeshSword.mtxWorldMeshSword, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshSword.pos.x, g_MeshSword.pos.y, g_MeshSword.pos.z);
		D3DXMatrixMultiply(&g_MeshSword.mtxWorldMeshSword, &g_MeshSword.mtxWorldMeshSword, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_MeshSword.mtxWorldMeshSword);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshSword, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshSword);

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshSword);

		if (g_nMeshSwordCount - 2 < ORBIT_POLYGON)
		{
			Min = g_nMeshSwordCount - 2;
		}
		else
		{
			Min = ORBIT_POLYGON;
		}
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, Min * 2 + 4, 0, Min);
		//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ORBIT_VERTEX, 0, ORBIT_POLYGON);
	}
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void ResetMeshSword(void)
{
	g_nMeshSwordCount = 0;
}

