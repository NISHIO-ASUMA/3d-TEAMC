//============================
//
// 敵[enemy.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "enemy.h"
#include "player.h"
#include <stdio.h>
#include "Score.h"
#include "input.h"
#include "damagepop.h"
#include "block.h"
#include "item.h"
#include "Shadow.h"
#include "Particle.h"
#include"item.h"
#include "HPGauge.h"
#include "sound.h"
#include "spgauge.h"

//****************************
//マクロ定義
//****************************
#define ENEMY_WORD (128) // 敵の最大数
#define SWORD_MTX (4) // 剣のマトリックスの数
#define TYPEONE_MOVE (2.5f) //敵0の移動量
#define TYPETWO_MOVE (1.5f) //敵1の移動量
#define TYPETHREE_MOVE (1.0f) //敵2の移動量
#define MAX_TEXENEMY (128) //テクスチャの最大数
#define MAX_ENEMYMOVE (1.0f) // 敵の移動量
#define SHADOWSIZEOFFSET (20.0f) // 影のサイズのオフセット
#define SHADOW_A (1.0f) // 影のアルファ
#define WAVE_ENEMY (15) // 敵の出現数

//****************************
//プロトタイプ宣言
//****************************
void LoadEnemy(int nType);											// 読み込み処理
bool AgentRange(float plrange, float playerrange, int nCntEnemy);   // ホーミングの範囲にいるかどうか
void AgentEnemy(int nCntEnemy);										// 敵のホーミング処理
void CollisionToEnemy(int nCntEnemy);								// 敵と敵の当たり判定

//****************************
//グローバル変数宣言
//****************************
ENEMY g_Enemy[MAX_ENEMY];
ENEMY g_LoadEnemy[ENEMYTYPE_MAX];
int g_nNumEnemy;//敵の総数カウント

//=============================
//ブロックの初期化処理
//=============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//座標
		g_Enemy[nCntEnemy].AttackEnemy = D3DXVECTOR3(5.0f, 10.0f, 5.0f);//
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//角度
		//g_Enemy[nCntEnemy].nType = ENEMYTYPE_ONE;
		g_Enemy[nCntEnemy].bUse = false;								//未使用状態
		g_Enemy[nCntEnemy].Motion.bLoopMotion = true;					//ループするか否か
		g_Enemy[nCntEnemy].nLife = 20;									//体力
		g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					//状態
		g_Enemy[nCntEnemy].Speed = 0.0f;							    //足の速さ
	}

	//グローバル変数の初期化
	g_nNumEnemy = 0;
	//LoadEnemy(0);

	//敵の読み込み
	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		LoadEnemy(nCntEnemyType);

		//読み込み

		//g_LoadEnemy[nCntEnemyType].
		g_LoadEnemy[nCntEnemyType].nLife = 20;
		//g_LoadEnemy[nCntEnemyType].nType = ENEMYTYPE_ONE;
		g_LoadEnemy[nCntEnemyType].Motion.motionType = MOTIONTYPE_NEUTRAL;//モーションの種類
		g_LoadEnemy[nCntEnemyType].g_bDamage = true;					  //ダメージか否か
		g_LoadEnemy[nCntEnemyType].Motion.bLoopMotion = true;			  //ループか否か
		g_LoadEnemy[nCntEnemyType].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //サイズ
		g_LoadEnemy[nCntEnemyType].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  //座標
		g_LoadEnemy[nCntEnemyType].state = ENEMYSTATE_NORMAL;			  //状態
		g_LoadEnemy[nCntEnemyType].Speed = 0.0f;						  //足の速さ


		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			D3DXMATERIAL* pMat;//マテリアルへのポインタ

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].dwNumMatEnemy; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[nCntMat]);
				}
			}

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse = pMat->MatD3D.Diffuse;

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].FirstDiffuse = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse;

		}
	}

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			//頂点数の取得
			nNumVtx = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetFVF());

			//頂点バッファのロック
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadEnemy[nCntEnemyType].vtxMin.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadEnemy[nCntEnemyType].vtxMin.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadEnemy[nCntEnemyType].vtxMin.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.z = vtx.z;
				}
				else if (vtx.x > g_LoadEnemy[nCntEnemyType].vtxMax.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.x = vtx.x;
				}
				else if (vtx.y > g_LoadEnemy[nCntEnemyType].vtxMax.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.y = vtx.y;
				}
				else if (vtx.z > g_LoadEnemy[nCntEnemyType].vtxMax.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;

			}
			//頂点バッファのアンロック
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->UnlockVertexBuffer();		
		}

		//サイズに代入
		g_LoadEnemy[nCntEnemyType].Size.x = g_LoadEnemy[nCntEnemyType].vtxMax.x - g_LoadEnemy[nCntEnemyType].vtxMin.x;
		g_LoadEnemy[nCntEnemyType].Size.y = g_LoadEnemy[nCntEnemyType].vtxMax.y - g_LoadEnemy[nCntEnemyType].vtxMin.y;
		g_LoadEnemy[nCntEnemyType].Size.z = g_LoadEnemy[nCntEnemyType].vtxMax.z - g_LoadEnemy[nCntEnemyType].vtxMin.z;

	}
}
//=============================
//敵の終了処理
//=============================
void UninitEnemy(void)
{
	// 音楽を止める
	StopSound();

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			//テクスチャの破棄
			for (int TexCnt = 0; TexCnt < (int)g_LoadEnemy[nCntEnemyType].Motion.aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] != NULL)
				{
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt]->Release();
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] = NULL;
				}
			}

			//メッシュの破棄
			if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy != NULL)
			{
				g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->Release();
				g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy = NULL;
			}

			//マテリアルの破棄
			if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy != NULL)
			{
				g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->Release();
				g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy = NULL;
			}
		}
	}

	// 全敵分回す
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 全モデル分合わす
		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
		{
			// テクスチャの最大数分回す
			for (int TexCnt = 0; TexCnt < (int)g_Enemy[nCntEnemy].Motion.aModel[nCntModel].dwNumMat; TexCnt++)
			{
				if (g_Enemy[nCntEnemy].EnemyModel[nCntModel].pTextureEnemy[TexCnt] != NULL)
				{
					g_Enemy[nCntEnemy].EnemyModel[nCntModel].pTextureEnemy[TexCnt] = NULL;
				}
			}

			//メッシュの破棄
			if (g_Enemy[nCntEnemy].EnemyModel[nCntModel].pMeshEnemy != NULL)
			{
				g_Enemy[nCntEnemy].EnemyModel[nCntModel].pMeshEnemy = NULL;
			}

			//マテリアルの破棄
			if (g_Enemy[nCntEnemy].EnemyModel[nCntModel].pBuffMatEnemy != NULL)
			{
				g_Enemy[nCntEnemy].EnemyModel[nCntModel].pBuffMatEnemy = NULL;
			}

		}
	}
}
//=============================
//敵の更新処理
//=============================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{//未使用状態だったら
			//とばしてカウントを進める
			continue;
		}

		switch (g_Enemy[nCntEnemy].state)
		{
		case ENEMYSTATE_NORMAL:
			break;
		case ENEMYSTATE_AGENT:
			break;
		case ENEMYSTATE_ATTACK:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		case ENEMYSTATE_DAMAGE:
			g_Enemy[nCntEnemy].nCounterState--;

			if (g_Enemy[nCntEnemy].nCounterState <= 0)
			{
				g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}
			break;
		default:
			break;
		}

		//移動量の減衰
		g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.5f;
		g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.5f;

		//前回の位置を保存
		g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

		//位置の更新
		g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

		//ブロックの当たり判定
		if (CollisionBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, &g_Enemy[nCntEnemy].Size))
		{

		}

		//アイテムが当たったか
		if (HitThrowItem(&g_Enemy[nCntEnemy].pos,10.0f,40.0f)&& g_Enemy[nCntEnemy].state!=ENEMYSTATE_DAMAGE)
		{
			HitEnemy(nCntEnemy, (float)pPlayer->nDamage * 1.5f);
		}
		// 剣と敵の当たり判定
		HitSowrd(&g_Enemy[nCntEnemy], nCntEnemy);

		// 影の計算
		SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos, SHADOWSIZEOFFSET + SHADOWSIZEOFFSET * g_Enemy[nCntEnemy].pos.y / 200.0f, SHADOW_A / (SHADOW_A + g_Enemy[nCntEnemy].pos.y / 30.0f));


		if (AgentRange(50.0f, 20000.0f, nCntEnemy))
		{
			AgentEnemy(nCntEnemy);
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_MOVE;
		}
		else
		{
			g_Enemy[nCntEnemy].Motion.motionType = MOTIONTYPE_NEUTRAL;
		}

		if (pPlayer->WeponMotion == MOTION_SP && pPlayer->Motion.motionType == MOTIONTYPE_ACTION)
		{
			CollisionPlayer(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].move, 100.0f, 10.0f);
		}
		else
		{
			CollisionPlayer(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].move, 10.0f, 10.0f);
		}
		
		CollisionToEnemy(nCntEnemy); // 敵と敵の当たり判定

		//モーションの更新
		UpdateMotion(&g_Enemy[nCntEnemy].Motion);
	}
}
//=============================
//敵の描画処理
//=============================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	int nNumEnemy = 0; // ローカル変数

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//全モデルパーツの描画
			for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
			{
				D3DXMATRIX mtxRotModel, mtxTransform;//計算用
				D3DXMATRIX mtxParent;//親のマトリックス

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

				//位置を反映
				D3DXMatrixTranslation(&mtxTransform, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

				//パーツの[親のマトリックス]を設定
				if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent != -1)
				{
					//親モデルがある場合
					mtxParent = g_Enemy[nCntEnemy].Motion.aModel[g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
				}
				else
				{//親モデルがない場合
					mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
				}

				//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&mtxParent);//自分自分親

				//パーツのワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].dwNumMatEnemy; nCntMat++)
				{					
					D3DXMATERIAL color;

					if (g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}
					else if (g_Enemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
					{
						color = pMat[nCntMat];

						color.MatD3D.Diffuse.r = 1.0f;
						color.MatD3D.Diffuse.g = 0.0f;
						color.MatD3D.Diffuse.b = 0.0f;
						color.MatD3D.Diffuse.a = 1.0f;

						pDevice->SetMaterial(&color.MatD3D);
					}

					//テクスチャの設定
					pDevice->SetTexture(0, g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pTextureEnemy[nCntMat]);

					//ブロック(パーツ)の描画
					g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pMeshEnemy->DrawSubset(nCntMat);
				}
				//インクリメント
				nNumEnemy++;
			}
		}
		//初期化
		nNumEnemy = 0;
	}
}
//=======================
//敵の取得処理
//=======================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}
//=======================
//敵のヒット処理
//=======================
void HitEnemy(int nCnt,int nDamage)
{
	// アイテム取得
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();
	SetDamege(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z), // 位置
		nDamage,	// ダメージ																								
		20,			// 寿命
		false);		

	g_Enemy[nCnt].nLife -= nDamage;

	if (g_Enemy[nCnt].nLife <= 0)
	{//体力が0以下なら
		SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z),
			g_Enemy[nCnt].rot,
			D3DXVECTOR3(3.14f, 3.14f, 3.14f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			4.0f, 8, 15, 20, 5.0f, 0.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		AddFever(5.0f);
		AddScore(8100);
		AddSpgauge(2.0f);

		g_Enemy[nCnt].state = ENEMYSTATE_DEATH;//敵の状態を死亡状態にする
		KillShadow(g_Enemy[nCnt].nIdxShadow);  // 敵の影を消す
		g_Enemy[nCnt].bUse = false;			   //未使用判定

		g_nNumEnemy--;//デクリメント

		switch (pItem[pPlayer->ItemIdx].nType)
		{
		case ITEMTYPE_BAT:

			// 音楽再生
			PlaySound(SOUND_LABEL_BAT_SE);

			break;

		case ITEMTYPE_HUNMER:

			// 音楽再生
			PlaySound(SOUND_LABEL_HAMMER_SE);

			break;
		default:

			// 音楽再生
			PlaySound(SOUND_LABEL_ACTION_SE);

			break;
		}

	}
	else
	{
		switch (pItem[pPlayer->ItemIdx].nType)
		{
		case ITEMTYPE_BAT:

			// 音楽再生
			PlaySound(SOUND_LABEL_BAT_SE);

			break;

		case ITEMTYPE_HUNMER:

			// 音楽再生
			PlaySound(SOUND_LABEL_HAMMER_SE);

			break;
		default:

			// 音楽再生
			PlaySound(SOUND_LABEL_ACTION_SE);

			break;
		}

		SetParticle(D3DXVECTOR3(g_Enemy[nCnt].pos.x, g_Enemy[nCnt].pos.y + g_Enemy[nCnt].Size.y / 1.5f, g_Enemy[nCnt].pos.z), g_Enemy[nCnt].rot, D3DXVECTOR3(3.14f, 3.14f, 3.14f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 4.0f, 1, 20, 30, 8.0f, 0.0f, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		g_Enemy[nCnt].state = ENEMYSTATE_DAMAGE;//敵の状態をダメージにする

		g_Enemy[nCnt].g_bDamage = false;//ダメージを通らなくする

		g_Enemy[nCnt].nCounterState = 30;//ダメージ状態からノーマルに戻るまでの時間

		AddScore(4300);
	}
}
//=======================
//敵の設定処理
//=======================
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife,float Speed)
{
	MODE mode = GetMode();//現在のモードの取得

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{
			g_Enemy[nCntEnemy] = g_LoadEnemy[nType]; // 情報を代入

			g_Enemy[nCntEnemy].pos = pos;	 //座標
			g_Enemy[nCntEnemy].nType = nType;//種類
			g_Enemy[nCntEnemy].nLife = nLife;//体力
			g_Enemy[nCntEnemy].Speed = Speed;//足の速さ
			g_Enemy[nCntEnemy].bUse = true;  //使用状態

			g_Enemy[nCntEnemy].nIdxShadow = SetShadow(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 1.0f, g_Enemy[nCntEnemy].pos.z), g_Enemy[nCntEnemy].rot, 40.0f);

			g_nNumEnemy++;//インクリメント
			break;
		}
	}
}
//=======================
//敵の出現
//=======================
void WaveEnemy(int nSpawner)
{
	for (int nCnt = 0; nCnt < WAVE_ENEMY; nCnt++)
	{
		// スポナー0
		if (nSpawner == 0)
		{
			SetEnemy(D3DXVECTOR3((float)(rand() % 450 + 400), 0.0f, (float)(rand() % -400 - 680)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.2f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 50 - 500), 0.0f, (float)(rand() % 50 -700.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 + 780.0f), 0.0f, (float)(rand() % 20 + 780.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
		// スポナー1
		else if (nSpawner == 1)
		{
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 -530.0f), 0.0f, (float)(rand() % 20 -780.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.6f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 -531.0f), 0.0f, (float)(rand() % 20 + 30.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.7f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 + 723.0f), 0.0f, (float)(rand() % 20 - 245.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 1 + 1.5f));
		}
		// スポナー2
		else if (nSpawner == 2)
		{
			SetEnemy(D3DXVECTOR3((float)(rand() % 450 + 400), 0.0f, (float)(rand() % -400 - 680)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.0f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 50 - 500), 0.0f, (float)(rand() % 50 - 700.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.5f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 + 780.0f), 0.0f, (float)(rand() % 20 + 780.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.0f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 - 530.0f), 0.0f, (float)(rand() % 20 - 780.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.0f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 - 531.0f), 0.0f, (float)(rand() % 20 + 30.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.5f));
			SetEnemy(D3DXVECTOR3((float)(rand() % 20 + 723.0f), 0.0f, (float)(rand() % 20 - 245.0f)), rand() % ENEMYTYPE_MAX, rand() % 400 + 200, (float)(rand() % 2 + 1.5f));
		}
	}
}
//=======================
//敵の総数取得処理
//=======================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=============================
//敵のロード処理
//=============================
void LoadEnemy(int nType)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ローカル変数
	int nModel = 0;					// モデル数
	int nIdx = 0;					// インデックス
	int NumKey = 0;					// キー数
	int nCnt = 0;					// モデルカウント用
	int nNumParts = 0;				// パーツ数格納用
	int nCntMotion = 0;				// モーションカウント用
	int nCntKey = 0;				// キーカウント用
	int nCntPosKey = 0;				// posカウント
	int nCntRotkey = 0;				// rotカウント

	// ファイルポインタを宣言
	FILE* pFile;

	switch (nType)
	{
	case 0:
		// ファイルを開く
		pFile = fopen("data/MOTION/motionEnemy01.txt", "r");
		break;
	case 1:
		pFile = fopen("data/MOTION/motionEnemy02.txt", "r");
		break;
	case 2:
		pFile = fopen("data/MOTION/motionEnemy03.txt", "r");
		break;
	case 3:
		pFile = fopen("data/MOTION/motionEnemy04.txt", "r");
		break;
	case 4:
		pFile = fopen("data/MOTION/motionEnemy05.txt", "r");
		break;
	default:
		pFile = NULL; //NULLにする
		break;
	}

	if (pFile != NULL)
	{//　NULL じゃない
		char aString[ENEMY_WORD];

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
							fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);
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
								&g_LoadEnemy[nType].EnemyModel[nCnt].pBuffMatEnemy,
								NULL,
								&g_LoadEnemy[nType].EnemyModel[nCnt].dwNumMatEnemy,
								&g_LoadEnemy[nType].EnemyModel[nCnt].pMeshEnemy);

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
											fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 座標を代入
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 角度を代入
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.x);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.y);
											fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.z);
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
									fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEYを読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey)
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
													fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
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
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROTを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
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
		MessageBox(NULL, "ファイルが開けません。", "エラー(enemy.cpp)", MB_OK);
		return;
    }
	// ファイルを閉じる
	fclose(pFile);

}
//================================
// ホーミングの範囲にいるかどうか
//================================
bool AgentRange(float playerrange,float Agentrange,int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	bool bHorming = false; // ホーミングしてくるかしないか

	float fDistanceX = g_Enemy[nCntEnemy].pos.x - pPlayer->pos.x; // 距離Xを求める
	float fDistanceY = g_Enemy[nCntEnemy].pos.y - pPlayer->pos.y; // 距離Yを求める
	float fDistanceZ = g_Enemy[nCntEnemy].pos.z - pPlayer->pos.z; // 距離Zを求める

	// 距離を求める
	float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

	// ホーミングしてくる半径
	float Radius = Agentrange + playerrange;

	// 半径を算出
	Radius = Radius * Radius;

	//範囲内に入った
	if (fDistance <= Radius)
	{
		bHorming = true; // ホーミングしている
	}

	return bHorming;
}
//=============================
// 敵のホーミング処理
//=============================
void AgentEnemy(int nCntEnemy)
{
	Player* pPlayer = GetPlayer();

	D3DXVECTOR3 fDest = pPlayer->pos - g_Enemy[nCntEnemy].pos; // 敵からプレイヤーまでのベクトルを引く
	D3DXVec3Normalize(&fDest, &fDest); // 正規化

	float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z); // 敵からプレイやまでの角度を求める

	g_Enemy[nCntEnemy].rot.y = fAngle + D3DX_PI; // 角度を代入

	// 移動量の更新
	g_Enemy[nCntEnemy].move.x += fDest.x * MAX_ENEMYMOVE * g_Enemy[nCntEnemy].Speed; 
	g_Enemy[nCntEnemy].move.z += fDest.z * MAX_ENEMYMOVE * g_Enemy[nCntEnemy].Speed;
}
//=============================
// 敵と敵の当たり判定
//=============================
void CollisionToEnemy(int nCntEnemy)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_Enemy[nCnt].bUse && nCnt != nCntEnemy)
		{
			float fDistanceX = g_Enemy[nCntEnemy].pos.x - g_Enemy[nCnt].pos.x; // 距離Xを求める
			float fDistanceY = g_Enemy[nCntEnemy].pos.y - g_Enemy[nCnt].pos.y; // 距離Yを求める
			float fDistanceZ = g_Enemy[nCntEnemy].pos.z - g_Enemy[nCnt].pos.z; // 距離Zを求める

			// 距離を求める
			float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

			float Eradius = 15.0f; // 半径を設定

			// ホーミングしてくる半径
			float Radius = Eradius + Eradius;

			// 半径を算出
			Radius = Radius * Radius;

			//範囲内に入った
			if (fDistance <= Radius)
			{
				D3DXVECTOR3 vector = g_Enemy[nCntEnemy].pos - g_Enemy[nCnt].pos; // はじく方向ベクトル
				D3DXVec3Normalize(&vector, &vector);
				g_Enemy[nCnt].move.x -= vector.x * g_Enemy[nCntEnemy].Speed;
				g_Enemy[nCnt].move.z -= vector.z * g_Enemy[nCntEnemy].Speed;
			}
		}
	}
}

