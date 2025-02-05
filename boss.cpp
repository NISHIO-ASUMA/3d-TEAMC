//============================
//
// ボス [boss.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// インクルードファイル
//****************************
#include "boss.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "player.h"
#include "Shadow.h"
#include "block.h"
#include "Damagepop.h"
#include "Particle.h"
#include "explosion.h"
#include "HPGauge.h"
#include "spgauge.h"
#include "Score.h"
#include "time.h"

//****************************
// マクロ定義
//****************************
#define MAX_WORD (256)			 // 最大の文字数
#define SHADOWSIZEOFFSET (40.0f) // 影の大きさのオフセット
#define SHADOW_A (1.0f)          // 影の濃さの基準
#define NUM_MTX (8)
#define MAX_BOSS (10)

//****************************
// プロトタイプ宣言
//****************************
void LoadBoss(void);				// ボスを読み込む
void colisionSword(int nCntBoss);   // 剣とボスの当たり判定
void CollisionToBoss(int nCntBoss); // ボスとボスの当たり判定

//****************************
// グローバル変数宣言
//****************************
Boss g_Boss[MAX_BOSS]; // ボスの情報
MOTION g_LoadBoss;

//=============================
// ボスの初期化処理
//=============================
void InitBoss(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_Boss[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_Boss[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Boss[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_Boss[nCnt].bUse = false;						 // 未使用状態
		g_Boss[nCnt].nLife = 20;							 // 体力
		g_Boss[nCnt].state = BOSSSTATE_NORMAL;			 // 状態
		g_Boss[nCnt].Speed = 5.0f;						 // 足の速さ
		g_Boss[nCnt].AttackState = BOSSATTACK_NO;			 // 攻撃状態
	}
	LoadBoss(); // ボスのロード

	D3DXMATERIAL* pMat; // マテリアルへのポインタ

	for (int nCntModel = 0; nCntModel < g_LoadBoss.nNumModel; nCntModel++)
	{
		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_LoadBoss.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_LoadBoss.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_LoadBoss.aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}

	// 頂点座標の取得
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE* pVtxBuff; //頂点バッファへのポインタ

	for (int nCntModel = 0; nCntModel < g_LoadBoss.nNumModel; nCntModel++)
	{
		// 頂点数の取得
		nNumVtx = g_LoadBoss.aModel[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_LoadBoss.aModel[nCntModel].pMesh->GetFVF());

		// 頂点バッファのロック
		g_LoadBoss.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標を比較してボスの最小値,最大値を取得
			if (vtx.x < g_LoadBoss.aModel[nCntModel].vtxMin.x)
			{
				g_LoadBoss.aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_LoadBoss.aModel[nCntModel].vtxMin.y)
			{
				g_LoadBoss.aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_LoadBoss.aModel[nCntModel].vtxMin.z)
			{
				g_LoadBoss.aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_LoadBoss.aModel[nCntModel].vtxMax.x)
			{
				g_LoadBoss.aModel[nCntModel].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_LoadBoss.aModel[nCntModel].vtxMax.y)
			{
				g_LoadBoss.aModel[nCntModel].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_LoadBoss.aModel[nCntModel].vtxMax.z)
			{
				g_LoadBoss.aModel[nCntModel].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			// サイズに代入
			g_LoadBoss.aModel[nCntModel].Size.x = g_LoadBoss.aModel[nCntModel].vtxMax.x - g_LoadBoss.aModel[nCntModel].vtxMin.x;
			g_LoadBoss.aModel[nCntModel].Size.y = g_LoadBoss.aModel[nCntModel].vtxMax.y - g_LoadBoss.aModel[nCntModel].vtxMin.y;
			g_LoadBoss.aModel[nCntModel].Size.z = g_LoadBoss.aModel[nCntModel].vtxMax.z - g_LoadBoss.aModel[nCntModel].vtxMin.z;
		}

		// 頂点バッファのアンロック
		g_LoadBoss.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}
//=============================
// ボスの終了処理
//=============================
void UninitBoss(void)
{
	for (int nCntModel = 0; nCntModel < g_LoadBoss.nNumModel; nCntModel++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < (int)g_LoadBoss.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_LoadBoss.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_LoadBoss.aModel[nCntModel].pMesh != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pMesh->Release();
			g_LoadBoss.aModel[nCntModel].pMesh = NULL;
		}

		// マテリアルの破棄
		if (g_LoadBoss.aModel[nCntModel].pBuffMat != NULL)
		{
			g_LoadBoss.aModel[nCntModel].pBuffMat->Release();
			g_LoadBoss.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	// 全ボス分回す
	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		// モデル分回す
		for (int nCntModel = 0; nCntModel < g_Boss[nCntBoss].Motion.nNumModel; nCntModel++)
		{
			// テクスチャの破棄
			for (int nCntTex = 0; nCntTex < (int)g_Boss[nCntBoss].Motion.aModel[nCntModel].dwNumMat; nCntTex++)
			{
				if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
				{
					g_Boss[nCntBoss].Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
				}
			}

			// メッシュの破棄
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの破棄
			if (g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat != NULL)
			{
				g_Boss[nCntBoss].Motion.aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}

}
//=============================
// ボスの更新処理
//=============================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		// 使用状態のみ
		if (!g_Boss[nCnt].bUse)
		{
			continue;
		}
		switch (g_Boss[nCnt].state)
		{
		case BOSSSTATE_NORMAL:
			break;
		case BOSSSTATE_AGENT:
			break;
		case BOSSSTATE_ATTACK:
			break;
		case BOSSSTATE_DAMAGE:
			g_Boss[nCnt].nCounterState--;

			if (g_Boss[nCnt].nCounterState <= 0)
			{
				g_Boss[nCnt].state = BOSSSTATE_NORMAL; // 敵の状態をノーマルにする
			}
			break;
		default:
			break;
		}

		// ボスの攻撃モーション
		if (g_Boss[nCnt].Motion.motionType == MOTIONTYPE_ACTION)// ドス突きの状態で
		{
			if (g_Boss[nCnt].Motion.nKey == 0 || g_Boss[nCnt].Motion.nKey == 1)// 溜める動作中に
			{
				// 向きをプレイヤーに向ける
				float fAngle = atan2f(pPlayer->pos.x - g_Boss[nCnt].pos.x, pPlayer->pos.z - g_Boss[nCnt].pos.z);

				// ボスの向き代入
				g_Boss[nCnt].rot.y = fAngle + D3DX_PI;
			}
			else if (g_Boss[nCnt].Motion.nKey == 2)// 溜まり切ったモーションになって
			{
				if (g_Boss[nCnt].Motion.nCountMotion == 1)// その最初にエフェクトを出す
				{
					SetParticle(D3DXVECTOR3(g_Boss[nCnt].pos.x, g_Boss[nCnt].pos.y + g_Boss[nCnt].Size.y / 1.5f, g_Boss[nCnt].pos.z),
						D3DXVECTOR3(1.57f, g_Boss[nCnt].rot.y, 1.57f),
						D3DXVECTOR3(0.2f, 3.14f, 0.2f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
						2.0f, 3, 20, 150, 4.0f, 50.0f,
						false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
			}
			else if (g_Boss[nCnt].Motion.nKey == 3)// 突進モーション中の			
			{
				if (g_Boss[nCnt].Motion.nCountMotion == 1)// 最初は加速させて
				{
					g_Boss[nCnt].move.x = sinf(g_Boss[nCnt].rot.y + D3DX_PI) * 70;
					g_Boss[nCnt].move.z = cosf(g_Boss[nCnt].rot.y + D3DX_PI) * 70;
				}
				else // その後はエフェクトを纏いながら移動する
				{
					g_Boss[nCnt].pos += g_Boss[nCnt].move;
					SetParticle(D3DXVECTOR3(g_Boss[nCnt].pos.x, g_Boss[nCnt].pos.y + g_Boss[nCnt].Size.y / 1.5f, g_Boss[nCnt].pos.z),
						g_Boss[nCnt].rot,
						D3DXVECTOR3(1.0f, 1.0f, 1.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
						2.0f, 4, 60, 40, 6.0f, 60.0f,
						false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}
			}
		}

		// 移動量の減衰
		g_Boss[nCnt].move.x += (0.0f - g_Boss[nCnt].move.x) * 0.25f;
		g_Boss[nCnt].move.z += (0.0f - g_Boss[nCnt].move.z) * 0.25f;

		// 前回の位置を代入
		g_Boss[nCnt].posOld = g_Boss[nCnt].pos;

		// 位置の更新
		g_Boss[nCnt].pos += g_Boss[nCnt].move;

		// ブロックの判定
		CollisionBlock(&g_Boss[nCnt].pos, &g_Boss[nCnt].posOld, &g_Boss[nCnt].move, &g_Boss[nCnt].Size);

		// 影の位置の更新
		SetPositionShadow(g_Boss[nCnt].nIdxShadow, g_Boss[nCnt].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Boss[nCnt].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Boss[nCnt].pos.y / 30.0f));

		// 範囲に入ったら(どこにいても追いかけてくるが一応円で取る)
		if (sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 20000.0f) && g_Boss[nCnt].Motion.motionType != MOTIONTYPE_ACTION)
		{
			// モデル情報を代入
			D3DXVECTOR3 HootR(g_Boss[nCnt].Motion.aModel[11].mtxWorld._41, g_Boss[nCnt].Motion.aModel[11].mtxWorld._42, g_Boss[nCnt].Motion.aModel[11].mtxWorld._43);
			D3DXVECTOR3 HootL(g_Boss[nCnt].Motion.aModel[14].mtxWorld._41, g_Boss[nCnt].Motion.aModel[14].mtxWorld._42, g_Boss[nCnt].Motion.aModel[14].mtxWorld._43);

			// モーションがムーブの時1キーの1フレーム目
			if (g_Boss[nCnt].Motion.motionType == MOTIONTYPE_MOVE &&
				g_Boss[nCnt].Motion.nKey == 1 &&
				g_Boss[nCnt].Motion.nCountMotion == 1)
			{
				SetExplosion(HootR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
			}
			// モーションがムーブの時3キーの1フレーム目
			else if (g_Boss[nCnt].Motion.motionType == MOTIONTYPE_MOVE &&
				g_Boss[nCnt].Motion.nKey == 3 &&
				g_Boss[nCnt].Motion.nCountMotion == 1)
			{
				SetExplosion(HootL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 40.0f, 40.0f, EXPLOSION_MOVE);
			}

			g_Boss[nCnt].Motion.motionType = MOTIONTYPE_MOVE; // モーションの種類を移動にする

			// ボスの向きをプレイヤーの位置を向くようにする
			float fAngle = atan2f(pPlayer->pos.x - g_Boss[nCnt].pos.x, pPlayer->pos.z - g_Boss[nCnt].pos.z);

			// ボスの向き代入
			g_Boss[nCnt].rot.y = fAngle + D3DX_PI;

			// プレイヤーの位置を算出
			D3DXVECTOR3 Dest = pPlayer->pos - g_Boss[nCnt].pos;

			// 正規化
			D3DXVec3Normalize(&Dest, &Dest);

			// 移動量に代入
			g_Boss[nCnt].move.x = Dest.x * g_Boss[nCnt].Speed;
			g_Boss[nCnt].move.z = Dest.z * g_Boss[nCnt].Speed;

		}
		else
		{
			if (g_Boss[nCnt].Motion.motionType != MOTIONTYPE_ACTION)
			{
				g_Boss[nCnt].Motion.motionType = MOTIONTYPE_NEUTRAL; // 攻撃してない
			}
		}

		// 攻撃範囲に入ったら
		if (sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 30.0f) && g_Boss[nCnt].AttackState != BOSSATTACK_ATTACK)
		{
			// モーションを攻撃にする
			SetMotion(&g_Boss[nCnt].Motion, // モーション構造体のアドレス
				MOTIONTYPE_ACTION,    // モーションタイプ
				MOTIONTYPE_NEUTRAL,   // ブレンドモーションタイプ
				true,                 // ブレンドするかしないか
				10);				  // ブレンドのフレーム

			g_Boss[nCnt].AttackState = BOSSATTACK_ATTACK; // 攻撃している
		}

		// 攻撃範囲に入った
		if (sphererange(&pPlayer->pos, &g_Boss[nCnt].pos, 50.0f, 20.0f) &&
			pPlayer->state != PLAYERSTATE_DAMAGE &&
			g_Boss[nCnt].Motion.nKey >= 4 && !pPlayer->AttackSp)
		{
			HitPlayer(50);
		}

		colisionSword(nCnt);   // 剣との当たり判定
		CollisionToBoss(nCnt); // ボスとボスの当たり判定

		// ループしないモーションが最後まで行ったら
		if (!g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].bLoop && g_Boss[nCnt].Motion.nKey >= g_Boss[nCnt].Motion.aMotionInfo[g_Boss[nCnt].Motion.motionType].nNumkey - 1)
		{
			g_Boss[nCnt].AttackState = BOSSATTACK_NO;					// ボスの攻撃状態を攻撃してない状態にする
		}

		// モーションの更新処理
		UpdateMotion(&g_Boss[nCnt].Motion);
	}
}
//=============================
// ボスの描画処理
//=============================
void DrawBoss(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef; // 現在のマテリアル保存用

	D3DXMATERIAL* pMat;	 // マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (!g_Boss[nCnt].bUse)
		{
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss[nCnt].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss[nCnt].rot.y, g_Boss[nCnt].rot.x, g_Boss[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Boss[nCnt].pos.x, g_Boss[nCnt].pos.y, g_Boss[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Boss[nCnt].mtxWorld, &g_Boss[nCnt].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss[nCnt].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_Boss[nCnt].Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform; // 計算用
			D3DXMATRIX mtxParent;				  // 親のマトリックス

			// パーツのマトリックスの初期化
			D3DXMatrixIdentity(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss[nCnt].Motion.aModel[nCntModel].rot.y, g_Boss[nCnt].Motion.aModel[nCntModel].rot.x, g_Boss[nCnt].Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// パーツの位置(オフセット)を反映
			D3DXMatrixTranslation(&mtxTransform, g_Boss[nCnt].Motion.aModel[nCntModel].pos.x, g_Boss[nCnt].Motion.aModel[nCntModel].pos.y, g_Boss[nCnt].Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			// パーツの[親のマトリックス]を設定
			if (g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				// 親モデルがある場合
				mtxParent = g_Boss[nCnt].Motion.aModel[g_Boss[nCnt].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = g_Boss[nCnt].mtxWorld;
			}

			// 算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld,
				&mtxParent); // 自分,自分,親

			// パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss[nCnt].Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss[nCnt].Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Boss[nCnt].Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				// カラー変更用の変数
				D3DXMATERIAL color;

				if (g_Boss[nCnt].state != BOSSSTATE_DAMAGE)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss[nCnt].state == BOSSSTATE_DAMAGE)
				{
					// カラーを代入
					color = pMat[nCntMat];

					// カラーを変更
					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					// マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_Boss[nCnt].Motion.aModel[nCntModel].pTexture[nCntMat]);

				// モデル(パーツ)の描画
				g_Boss[nCnt].Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			// マテリアルの設定
			pDevice->SetMaterial(&matDef);
		}
		
	}
}
//=============================
// ボスの設定処理
//=============================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (!g_Boss[nCnt].bUse)
		{// 未使用なら
			g_Boss[nCnt].Motion = g_LoadBoss;
			g_Boss[nCnt].pos = pos;	  // 位置を代入
			//g_Boss.Speed = speed; // 足の速さ
			g_Boss[nCnt].nLife = nLife; // 体力を挿入
			g_Boss[nCnt].bUse = true;   // 使用状態にする

			g_Boss[nCnt].nIdxShadow = SetShadow(g_Boss[nCnt].pos, g_Boss[nCnt].rot, 40.0f);

			break;
		}
	}
}
//=============================
// ボスのヒット処理
//=============================
void HitBoss(int nCntBoss,int nDamage)
{
	Player* pPlayer = GetPlayer();

	g_Boss[nCntBoss].nLife -= nDamage;

	// ダメージを設定
	SetDamege(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), // 位置
		nDamage,	// ダメージ																								
		20,			// 寿命
		false);

	if (g_Boss[nCntBoss].nLife <= 0)
	{
		// 死んだらパーティクルを出す
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z),
			g_Boss[nCntBoss].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			4.0f, 8, 15, 20, 5.0f, 0.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// 消す
		g_Boss[nCntBoss].bUse = false;

		// 影から消す
		KillShadow(g_Boss[nCntBoss].nIdxShadow);

		if (pPlayer->FeverMode)
		{
			AddScore(30000);		// スコアを取得
			AddSpgauge(2.5f);   // SPゲージを取得
		}
		else if (!pPlayer->FeverMode)
		{
			AddFever(10.0f);		// フィーバーポイントを取得
			AddScore(15000);		// スコアを取得
			AddSpgauge(2.0f);   // SPゲージを取得
		}
		AddTimeSecond(15); // 15秒増やす
	}
	else
	{
		// パーティクルをセット
		SetParticle(D3DXVECTOR3(g_Boss[nCntBoss].pos.x, g_Boss[nCntBoss].pos.y + g_Boss[nCntBoss].Size.y / 1.5f, g_Boss[nCntBoss].pos.z), g_Boss[nCntBoss].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 4.0f, 1, 20, 30, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Boss[nCntBoss].state = ENEMYSTATE_DAMAGE;
		g_Boss[nCntBoss].nCounterState = 20;
		AddSpgauge(1.0f);   // SPゲージを取得
	}
}
//=============================
// ボスの読み込み処理
//=============================
void LoadBoss(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ファイルポインタを宣言
	FILE* pFile; 

	// ファイルを開く
	pFile = fopen("data\\MOTION\\boss.txt", "r");

	// ローカル変数宣言-----------------------------
	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;
	//----------------------------------------------

	if (pFile != NULL)
	{//　NULL じゃない
		// 文字列
		char aString[MAX_WORD];

		while (1)
		{
			// 読み飛ばし
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{// SCRIPTを読み取ったら
				while (1)
				{
					// 読み飛ばし
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// NUM_MODELを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 値を代入
							fscanf(pFile, "%d", &g_LoadBoss.nNumModel);
						}
					}

					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						// MODEL_FILENAMEを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 代入
							// 文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							const char* MODEL_FILE = {};	// モデルファイル名格納用の変数

							// 読み込んだ文字列を保存する
							MODEL_FILE = aString;

							//Xファイルの読み込み
							D3DXLoadMeshFromX(MODEL_FILE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_LoadBoss.aModel[nCnt].pBuffMat,
								NULL,
								&g_LoadBoss.aModel[nCnt].dwNumMat,
								&g_LoadBoss.aModel[nCnt].pMesh);

							nCnt++; // nCntをインクリメント
						}
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{
						while (1)
						{
							// 文字列を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// NUM_PARTSを読み取ったら

								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// 値を代入
									fscanf(pFile, "%d", &nNumParts);
								}
							}

							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{
								while (1)
								{
									// 文字列を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											fscanf(pFile, "%d", &nIdx);
										}
									}

									if (strcmp(&aString[0], "PARENT") == 0)
									{// PARENTを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											// ペアネント
											fscanf(pFile, "%d", &g_LoadBoss.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 座標を代入
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 角度を代入
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_LoadBoss.aModel[nIdx].rot.z);
										}
									}

									if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// END_PARTSSETを読み取ったら
										// whileを抜ける
										break;
									}

								}// while文末
							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// MOTIONSETを読み取ったら


						while (1)
						{
							// 文字を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "LOOP") == 0)
							{// LOOP を読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEYを読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_LoadBoss.aMotionInfo[nCntMotion].nNumkey)
								{
									// 文字を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(aString, "KEYSET") == 0)
									{// KEYSETを読み取ったら


										while (1)
										{
											// 文字を読み飛ばす
											fscanf(pFile, "%s", &aString[0]);


											if (strcmp(&aString[0], "FRAME") == 0)
											{// FRAMEを読み取ったら
												// 文字を読み飛ばす
												fscanf(pFile, "%s", &aString[0]);

												if (strcmp(&aString[0], "=") == 0)
												{// 値を代入
													fscanf(pFile, "%d", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
													break;
												}
											}

										}

										while (1)
										{
											// 文字を読み飛ばす
											fscanf(pFile, "%s", &aString[0]);
											if (strcmp(&aString[0], "KEY") == 0)
											{// KEYを読みとったら
												while (1)
												{
													// 文字を読み飛ばす
													fscanf(pFile, "%s", &aString[0]);

													if (strcmp(&aString[0], "POS") == 0)
													{// POSを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROTを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_LoadBoss.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
															nCntRotkey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "END_KEY") == 0)
													{// END_KEYを読み取ったら
														break;
													}
												}
											}
											else if (strcmp(&aString[0], "END_KEYSET") == 0)
											{// END_KEYSETを読み取ったら
												nCntRotkey = 0;
												nCntPosKey = 0;
												nCntKey++;			// インクリメント
												break;
											}


										}

									}

								}
							}

							if (strcmp(&aString[0], "END_MOTIONSET") == 0)
							{// END_MOTIONSETを読み取ったら
								nCntMotion++;		// モーションの更新
								nCntKey = 0;		// 0から始める
								break;
							}
						}
					}

					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else
					{
						continue;
					}
				}// while文末

				break;
			}
		}// while文末
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Boss.cpp)", MB_OK);
		return;
	}
	// ファイルを閉じる
	fclose(pFile);
}
//=========================
// 剣の当たり判定
//=========================
void colisionSword(int nCntBoss)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	D3DXVECTOR3 mtxDis, SwordPos;

	if (pPlayer->Motion.nNumModel == 16 && !pPlayer->AttackSp)
	{
		//剣の長さを求める
		mtxDis.x = (pPlayer->SwordMtx._41 - pPlayer->Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (pPlayer->SwordMtx._42 - pPlayer->Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (pPlayer->SwordMtx._43 - pPlayer->Motion.aModel[15].mtxWorld._43);

		// マトリクスの数分だけ回す
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// 剣の位置を全て求める
			SwordPos.x = pPlayer->Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = pPlayer->Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = pPlayer->Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // 距離算出用

			DisPos.x = g_Boss[nCntBoss].pos.x - SwordPos.x; // 距離Xを求める
			DisPos.y = g_Boss[nCntBoss].pos.y - SwordPos.y; // 距離Yを求める
			DisPos.z = g_Boss[nCntBoss].pos.z - SwordPos.z; // 距離Zを求める

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // 距離を求める

			float Radius1, Radius2; // 半径

			Radius1 = 15.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // 半径を求める

			fRadius = (fRadius * fRadius); // 半径を求める

			if (fDistance <= fRadius && g_Boss[nCntBoss].state != BOSSSTATE_DAMAGE && pPlayer->Combostate != COMBO_NO)
			{
				HitBoss(nCntBoss,pPlayer->nDamage * 5);

				pItem[pPlayer->ItemIdx].durability--;

				if (pItem[pPlayer->ItemIdx].durability <= 0)
				{
					pPlayer->Itembreak[pPlayer->ItemIdx] = true;
				}
				break;
			}
		}
	}
	else if (pPlayer->Motion.nNumModel == 15 && !pPlayer->AttackSp)
	{
		// モデルの位置を変数に代入
		D3DXVECTOR3 ModelPos(pPlayer->Motion.aModel[4].mtxWorld._41, pPlayer->Motion.aModel[4].mtxWorld._42, pPlayer->Motion.aModel[4].mtxWorld._43);

		// 円の範囲
		if (sphererange(&ModelPos, &g_Boss[nCntBoss].pos, 30.0f, 65.0f) && pPlayer->Combostate != COMBO_NO && g_Boss[nCntBoss].state != ENEMYSTATE_DAMAGE)
		{
			if (pPlayer->Motion.motionType == MOTIONTYPE_ACTION && pPlayer->Motion.nKey >= 2)
			{
				HitBoss(nCntBoss,pPlayer->nDamage * 3); // 敵に当たった
			}
		}
	}
	else if (pPlayer->Motion.nNumModel == 16 && pPlayer->AttackSp)
	{
		//剣の長さを求める
		mtxDis.x = (pPlayer->SwordMtx._41 - pPlayer->Motion.aModel[15].mtxWorld._41);
		mtxDis.y = (pPlayer->SwordMtx._42 - pPlayer->Motion.aModel[15].mtxWorld._42);
		mtxDis.z = (pPlayer->SwordMtx._43 - pPlayer->Motion.aModel[15].mtxWorld._43);

		// マトリクスの数分だけ回す
		for (int nCnt = 0; nCnt < NUM_MTX; nCnt++)
		{
			// 剣の位置を全て求める
			SwordPos.x = pPlayer->Motion.aModel[15].mtxWorld._41 + mtxDis.x * 0.125f * nCnt;
			SwordPos.y = pPlayer->Motion.aModel[15].mtxWorld._42 + mtxDis.y * 0.125f * nCnt;
			SwordPos.z = pPlayer->Motion.aModel[15].mtxWorld._43 + mtxDis.z * 0.125f * nCnt;

			D3DXVECTOR3 DisPos; // 距離算出用

			DisPos.x = g_Boss[nCntBoss].pos.x - SwordPos.x; // 距離Xを求める
			DisPos.y = g_Boss[nCntBoss].pos.y - SwordPos.y; // 距離Yを求める
			DisPos.z = g_Boss[nCntBoss].pos.z - SwordPos.z; // 距離Zを求める

			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z); // 距離を求める

			float Radius1, Radius2; // 半径

			Radius1 = 200.0f;
			Radius2 = 50.0f;

			float fRadius = Radius1 + Radius2; // 半径を求める

			fRadius = (fRadius * fRadius); // 半径を求める

			if (fDistance <= fRadius && g_Boss[nCntBoss].state != ENEMYSTATE_DAMAGE && pPlayer->Combostate != COMBO_NO && pPlayer->Motion.nKey >= 3)
			{
				HitBoss(nCntBoss,pPlayer->nDamage * 50);
				break;
			}
		}
	}

}
//=========================
// ボスとボスの当たり判定
//========================
void CollisionToBoss(int nCntBoss)
{
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		if (nCntBoss != nCnt)
		{
			if (sphererange(&g_Boss[nCntBoss].pos, &g_Boss[nCnt].pos, 50.0f, 50.0f))
			{
				D3DXVECTOR3 vector = g_Boss[nCntBoss].pos - g_Boss[nCnt].pos;

				D3DXVec3Normalize(&vector, &vector);
				g_Boss[nCntBoss].move.x -= vector.x * g_Boss[nCntBoss].Speed;
				g_Boss[nCntBoss].move.z -= vector.z * g_Boss[nCntBoss].Speed;
			}
		}
	}
}
//======================
// ボスの取得処理
//======================
Boss* Getboss(void)
{
	return &g_Boss[0];
}