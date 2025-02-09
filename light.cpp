//============================================================================================================
//
// ライト処理 [light.cpp]
// Author:TEAM_C
//
//============================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "light.h"
#include "HPGauge.h"
#include "player.h"

//***************************************************************************************************************
// グローバル変数宣言
//***************************************************************************************************************
D3DLIGHT9 g_light[MAX_LIGHT];		// ライト情報
D3DXVECTOR3 g_vecDir[MAX_LIGHT];	// ライトの数

//===========================================================================================================
//　カメラの初期化処理
//===========================================================================================================
void InitLight(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得
	pDevice = GetDevice();


	// ライトのクリア処理(ゼロメモリー関数)
	ZeroMemory(&g_light, sizeof(g_light));

	// ライトの種類を設定
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		// ライトの方向ベクトル
		g_vecDir[nCnt];

		g_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;	// 平行光源

		// ライトの拡散光
		g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ライトの方向設定
	g_vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	g_vecDir[1] = D3DXVECTOR3(-0.6f, 0.1f, 0.4f);
	g_vecDir[2] = D3DXVECTOR3(1.0f,-0.5f,0.4f);

	for (int nCnt1 = 0; nCnt1 < MAX_LIGHT; nCnt1++)
	{	// 正規化する
		D3DXVec3Normalize(&g_vecDir[nCnt1], &g_vecDir[nCnt1]); // ベクトルの大きさを1にする

		g_light[nCnt1].Direction = g_vecDir[nCnt1];

		// ライトの設定
		pDevice->SetLight(nCnt1, &g_light[nCnt1]);

		// ライトを有効化
		pDevice->LightEnable(nCnt1, TRUE);

	}
}
//===========================================================================================================
//　ライトの終了処理
//===========================================================================================================
void UninitLight(void)
{
	// 一旦無しで
}
//===========================================================================================================
//　ライトの更新処理
//===========================================================================================================
void UpdateLight(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (pPlayer->FeverMode)
	{
		g_light[0].Diffuse = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);

		// 正視化する(大きさ位置のベクトルにする)
		D3DXVec3Normalize(&g_vecDir[0], &g_vecDir[0]);

		// ライトを設定する
		pDevice->SetLight(0, &g_light[0]);

		// ライトを有効にする(ON,OFF)
		pDevice->LightEnable(0, TRUE);
	}
	else if(!pPlayer->FeverMode && !pPlayer->AttackSp)
	{
		for (int nCnt1 = 0; nCnt1 < MAX_LIGHT; nCnt1++)
		{
			g_light[nCnt1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 正視化する(大きさ位置のベクトルにする)
			D3DXVec3Normalize(&g_vecDir[nCnt1], &g_vecDir[nCnt1]);

			// ライトを設定する
			pDevice->SetLight(nCnt1, &g_light[nCnt1]);

			// ライトを有効にする(ON,OFF)
			pDevice->LightEnable(nCnt1, TRUE);
		}
	}
}

