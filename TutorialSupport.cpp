//=====================================================================================================================
//
// チュートリアル解説のプログラム [ tutorialsupport.cpp ]
// Author; 長尾悠成
//
//=====================================================================================================================

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "player.h"
#include "TutorialSupport.h"
#include "sound.h"
#include "camera.h"
#include "input.h"
#include "spgauge.h"
#include "gameui.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define NUM_SHIFT_VTX (2) // 既存の4頂点生成に掛ける値 (今回は8頂点ほしいので 4*2 をする)

//**************************************************************************************************************
// 種類の列挙型宣言
//**************************************************************************************************************
typedef enum
{
	TYPE_MOVE = 0, // 移動
	TYPE_JUMP,     // ジャンプ
	TYPE_AVOID,     // 回避
	TYPE_ATTACK,		// 攻撃
	TYPE_SPATTACK,		// SP攻撃
	TYPE_PICKUP,		// 拾う
	TYPE_STOCK,     // ストック
	TYPE_ITEMMIX,     // 合成
	TYPE_8,     // 
	TYPE_9,		// 自由操作
	OK_MARK,    // 丸マーク
	TYPE_MAX
}TYPE;

//**************************************************************************************************************
// テクスチャ列挙型宣言
//**************************************************************************************************************
static const char* TUTOTEX[TYPE_MAX] =
{
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_1.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_4.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_2.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.5.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_key_5.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_6.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_key_5.5.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_key_7.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_8.png",
	"data\\TEXTURE\\OKMark.png",
};

//**************************************************************************************************************
// サポート構造体宣言
//**************************************************************************************************************
typedef struct
{
	int nType; // 種類
}TutoSupport;

//**************************************************************************************************************
// グローバル変数宣言
//************************************************************************************************************** 
LPDIRECT3DTEXTURE9 g_pTexture_Tutorial[TYPE_MAX] = {}; // テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManager = NULL;      // 頂点バッファのポインタ
TutoSupport g_Tutosupport; // 構造体変数
int nSteps = 0;			   // ステップ数のカウント
float fALv2 = 1.0f;
float bAmove = 0.0f;
float fPlusX = 0.0f;

//==============================================================================================================
// チュートリアルサポートの初期化処理
//==============================================================================================================
void InitManager(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// グローバル変数の初期化
	nSteps = 0;
	fALv2 = 1.0f;
	bAmove = 0.0f;
	g_Tutosupport.nType = 0;

	// テクスチャの設定
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TUTOTEX[nCnt],
			&g_pTexture_Tutorial[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SHIFT_VTX,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffManager, 
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_SHIFT_VTX; nCnt++)
	{// 今回はポリゴン二枚分の頂点バッファ生成 (〇と2Dポリゴン)をしているためその分を設定する
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 〇の頂点バッファのためにずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffManager->Unlock();
}
//==============================================================================================================
// チュートリアルサポートの終了処理
//==============================================================================================================
void UninitManager(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (g_pTexture_Tutorial[nCnt] != NULL)
		{
			g_pTexture_Tutorial[nCnt]->Release();
			g_pTexture_Tutorial[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffManager != NULL)
	{
		g_pVtxBuffManager->Release();
		g_pVtxBuffManager = NULL;
	}
}
//==============================================================================================================
// チュートリアルサポートの更新処理
//==============================================================================================================
void UpdateManager(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// アイテムの取得
	Item* pItem = GetItem();

	// カメラの取得
	Camera* pCamera = GetCamera();

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 110.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f + fPlusX, 110.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 280.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f + fPlusX, 280.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

	//〇の処理
	pVtx += 4;

	// 〇の頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 70.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1100.0f + fPlusX, 70.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 220.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1100.0f + fPlusX, 220.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

	// アンロックする
	g_pVtxBuffManager->Unlock();

	// 移動量を加算
	fALv2 += bAmove;

	if (fALv2 > 1.0f)
	{
		fALv2 = 1.0f;
		bAmove = 0.0f;
	}
	else if (fALv2 < 0.0f)
	{
		fALv2 = 0.0f;
		nSteps++;
		bAmove *= -1.0f;
	}

	fPlusX += bAmove * -100;

	if (nSteps > 3)
	{
		// SPゲージを加算する
		AddSpgauge(1.0f);
	}

	if (nSteps == TYPE_MOVE && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_MOVE && fALv2 == 1.0f)
	{// 最初
		PlaySound(SOUND_LABEL_TUTOCLEARSE);
		bAmove = -0.01f;
	}
	else if (nSteps == TYPE_JUMP && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_JUMP && fALv2 == 1.0f)
	{// 2番目
		PlaySound(SOUND_LABEL_TUTOCLEARSE);
		bAmove = -0.01f;
	}
	else if (nSteps == TYPE_AVOID && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_AVOID && fALv2 == 1.0f)
	{// 3番目
		PlaySound(SOUND_LABEL_TUTOCLEARSE);
		bAmove = -0.01f;
	}
	else if (nSteps == TYPE_ATTACK && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{// 4番目
		if (pPlayer->AttackSp == false)
		{
			PlaySound(SOUND_LABEL_TUTOCLEARSE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == TYPE_SPATTACK && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{// 5番目
		if (pPlayer->AttackSp == true)
		{
			PlaySound(SOUND_LABEL_TUTOCLEARSE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == TYPE_PICKUP && pItem[pPlayer->ItemIdx].nType == ITEMTYPE_BAT && fALv2 == 1.0f)
	{// 6番目
		PlaySound(SOUND_LABEL_TUTOCLEARSE);
		bAmove = -0.01f;
	}
	else if (nSteps == TYPE_STOCK && pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && fALv2 == 1.0f)
	{// 7番目
		PlaySound(SOUND_LABEL_TUTOCLEARSE);
		bAmove = -0.01f;
	}
	else if (nSteps == TYPE_ITEMMIX && fALv2 == 1.0f)
	{// 8番目

		// プレイヤーが持っているアイテムがレシピと一致するか確認
		const bool CheckMatItem = CheckMixItemMat(pItem[pPlayer->ItemIdx].nType, pItem[pPlayer->StockItemIdx].nType, pPlayer->ItemIdx, pPlayer->StockItemIdx) == true;

		if (CheckMatItem == true)
		{
			PlaySound(SOUND_LABEL_TUTOCLEARSE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 8 && fALv2 == 1.0f)
	{// 9番目
		if (pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONEBAT)
		{
			PlaySound(SOUND_LABEL_TUTOCLEARSE);
			bAmove = -0.01f;
		}
	}

	// UIをセットできるか
	static bool bSetUI = true;

	// スペシャル攻撃の説明だったら
	if (nSteps == TYPE_SPATTACK && bSetUI == true)
	{
		SetGameUI(D3DXVECTOR3(640.0f, 500.0f, 0.0f), UITYPE_SPINFO, 0.0f, 100.0f, false, 0);
		bSetUI = false;
	}
	else if (nSteps != TYPE_SPATTACK)
	{
		bSetUI = true;
	}

	static bool bSetRecipe = true;

	// スペシャル攻撃の説明だったら
	if (nSteps == 9 && bSetRecipe == true)
	{
		SetGameUI(D3DXVECTOR3(1500.0f, 600.0f, 0.0f), UITYPE_CRAFTRECIPE, 200.0f, 100.0f, false, 0);
		bSetRecipe = false;
	}
	else if (nSteps != 9)
	{
		bSetRecipe = true;
	}

}
//==============================================================================================================
// チュートリアルサポートの描画処理
//==============================================================================================================
void DrawManager(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffManager, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 種類を代入
	g_Tutosupport.nType = nSteps;

	// テクスチャを設定
	pDevice->SetTexture(0, g_pTexture_Tutorial[g_Tutosupport.nType]);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (bAmove < 0.0f)
	{
		// テクスチャを設定
		pDevice->SetTexture(0, g_pTexture_Tutorial[10]);

		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	}
}
//=====================================================================================================================
// チュートリアルのステップ取得処理
//=====================================================================================================================
int GetStep(void)
{
	return nSteps;
}
