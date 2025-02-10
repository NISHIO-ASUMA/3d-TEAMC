//======================================
//
// メッシュの球型の処理 [mashfan.h]
// Author: Asuma Nishio
//
//======================================

//***************************
// インクルードファイル宣言
//***************************
#include "meshfan.h"

//***************************
// グローバル変数宣言
//***************************
LPDIRECT3DTEXTURE9 g_pTexturemeshwall = NULL; //メッシュドームのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffmeshwall = NULL; //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffmeshwall = NULL; //インデックスバッファへのポインタ
D3DXVECTOR3 g_posmeshwall; //メッシュドームの位置
D3DXVECTOR3 g_rotmeshwall; //メッシュフドームの向き
D3DXMATRIX g_mtxWorldmeshwall; //メッシュドームのワールドマトリックス

//======================================
// メッシュドーム初期化処理
//======================================
void InitmeshFan(void)
{
	// デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice,
        "data\\TEXTURE\\feild.png",
        &g_pTexturemeshwall);

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * meshwall_VERTEX_NUM,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffmeshwall,
        NULL);

	// 頂点バッファのポインタ
    VERTEX_3D* pVtx = NULL;
	
	// 頂点バッファをロック
    g_pVtxBuffmeshwall->Lock(0, 0, (void**)&pVtx, 0);

    // 半球の半径
    float hemisphereRadius = 3000.0f; 

    for (int nCntV = 0; nCntV <= meshwall_Z_BLOCK; nCntV++) 
	{
        for (int nCntH = 0; nCntH <= meshwall_X_BLOCK; nCntH++) 
		{
            // nCntH番目の角度を計算 (1回転を分割して計算)
            float angleH = (360.0f / meshwall_X_BLOCK) * nCntH;

            // nCntV番目の角度を計算 (半球の高さ方向に)
            float angleV = 90.0f * ((float)nCntV / meshwall_Z_BLOCK);

            // 頂点の位置を設定
            pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].pos = D3DXVECTOR3(
                sinf(D3DXToRadian(angleH)) * hemisphereRadius * cosf(D3DXToRadian(angleV)), // X座標
                 hemisphereRadius * sinf(D3DXToRadian(angleV)),                             // Y座標
                cosf(D3DXToRadian(angleH)) * hemisphereRadius * cosf(D3DXToRadian(angleV))  // Z座標
            );

            // 法線を計算 (頂点座標からの単位ベクトル)
            D3DXVECTOR3 normal = D3DXVECTOR3(
                pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].pos.x / hemisphereRadius,
                pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].pos.y / hemisphereRadius,
                pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].pos.z / hemisphereRadius
            );
			
			// 正規化する
            D3DXVec3Normalize(&normal, &normal);

            pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].nor = normal;

            // 頂点カラーを設定
            pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

            // テクスチャ座標を設定（球面に沿って展開）
            pVtx[nCntV * (meshwall_X_BLOCK + 1) + nCntH].tex = D3DXVECTOR2(
                (float)nCntH / meshwall_X_BLOCK,                           // U座標（円周方向）
                1.0f - (float)nCntV / meshwall_Z_BLOCK                     // V座標（高さ方向）
            );
        }
    }
	
	// グローバル変数の初期化
    g_posmeshwall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_rotmeshwall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // 頂点バッファをアンロック
    g_pVtxBuffmeshwall->Unlock();

    // インデックスバッファの生成
    pDevice->CreateIndexBuffer(sizeof(WORD) * meshwall_INDEX_NUM,
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &g_pIdxBuffmeshwall,
        NULL);

    // インデックスバッファをロック
    WORD* pIdx = NULL;
    g_pIdxBuffmeshwall->Lock(0, 0, (void**)&pIdx, 0);

    // インデックスの設定
    int idx = 0;
    for (int nCntV = 0; nCntV < meshwall_Z_BLOCK; nCntV++) {
        for (int x = 0; x < meshwall_X_BLOCK; x++) {
            int baseIndex = nCntV * (meshwall_X_BLOCK + 1) + x;

            // 三角形1
            pIdx[idx++] = baseIndex;
            pIdx[idx++] = baseIndex + meshwall_X_BLOCK + 1;
            pIdx[idx++] = baseIndex + 1;

            // 三角形2
            pIdx[idx++] = baseIndex + 1;
            pIdx[idx++] = baseIndex + meshwall_X_BLOCK + 1;
            pIdx[idx++] = baseIndex + meshwall_X_BLOCK + 2;
        }
    }

    // インデックスバッファをアンロック
    g_pIdxBuffmeshwall->Unlock();

}
//==========================================
// メッシュドーム終了処理
//===========================================
void UninitmeshFan(void)
{
	// テクスチャの破棄
    if (g_pTexturemeshwall != NULL) 
    {

        g_pTexturemeshwall->Release();
        g_pTexturemeshwall = NULL;
    }

	// 頂点バッファの破棄
    if (g_pVtxBuffmeshwall != NULL) 
    {
        g_pVtxBuffmeshwall->Release();
        g_pVtxBuffmeshwall = NULL;
    }

	// メッシュの破棄
    if (g_pIdxBuffmeshwall != NULL) 
    {
        g_pIdxBuffmeshwall->Release();
        g_pIdxBuffmeshwall = NULL;
    }
}
//==========================================
// メッシュドーム更新処理
//===========================================
void UpdatemeshFan(void)
{
    //必要に応じて追加
}
//==========================================
// メッシュドーム描画処理
//===========================================
void DrawmeshFan(void)
{
	// デバイスへのポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 計算用のマトリックス
    D3DXMATRIX mtxRot, mtxTrans;

    // ワールドマトリックスの初期化
    D3DXMatrixIdentity(&g_mtxWorldmeshwall);

    // 向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotmeshwall.y, g_rotmeshwall.x, g_rotmeshwall.z);
    D3DXMatrixMultiply(&g_mtxWorldmeshwall, &g_mtxWorldmeshwall, &mtxRot);

    // 位置を反映
    D3DXMatrixTranslation(&mtxTrans, g_posmeshwall.x, g_posmeshwall.y, g_posmeshwall.z);
    D3DXMatrixMultiply(&g_mtxWorldmeshwall, &g_mtxWorldmeshwall, &mtxTrans);

    // ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldmeshwall);

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffmeshwall, 0, sizeof(VERTEX_3D));

    // インデックスバッファをデータストリームに設定
    pDevice->SetIndices(g_pIdxBuffmeshwall);

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);

    // テクスチャの設定
    pDevice->SetTexture(0, g_pTexturemeshwall);

    // ポリゴンの描画
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,meshwall_VERTEX_NUM, 0, meshwall_INDEX_NUM);
}