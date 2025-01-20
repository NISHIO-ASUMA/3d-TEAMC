//============================
//
// モデル[model.cpp](テンプレート用)
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "model.h"

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_apTextureModel[128] = {};
LPD3DXMESH g_pMeshModel = NULL;//メッシュ(頂点座標)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;//マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;//マテリアルの数
D3DXVECTOR3 g_posModel;//位置
D3DXVECTOR3 g_rotModel;//向き
D3DXVECTOR3 Scal;
D3DXMATRIX g_mtxWorldModel;//ワールドマトリックス

//=============================
//モデルの初期化処理
//=============================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	g_posModel = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scal = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	////Xファイルの読み込み
	//D3DXLoadMeshFromX("data\\MODEL\\3dtest_zako.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatModel,
	//	NULL,
	//	&g_dwNumMatModel,
	//	&g_pMeshModel);

	//D3DXMATERIAL* pMat;//マテリアルへのポインタ

	////マテリアルのデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	//{
	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{
	//		//このファイル名を使用してテクスチャを読み込む
	//			//テクスチャの読み込み
	//		D3DXCreateTextureFromFile(pDevice,
	//			pMat[nCntMat].pTextureFilename,
	//			&g_apTextureModel[nCntMat]);
	//	}
	//}
}
//=============================
//モデルの終了処理
//=============================
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}
//=============================
//モデルの更新処理
//=============================
void UpdateModel(void)
{
	//今はなし
}
//=============================
//モデルの描画処理
//=============================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);


	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}
