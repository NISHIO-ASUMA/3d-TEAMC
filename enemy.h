//==============================================================================================================
//
// 敵[enemy.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"main.h"
#include"motion.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_ENEMY (256)
#define ENEMYFILE001 ("data\\enemy.txt")
#define ENEMYFILE002 ("data\\enemy2.txt")
#define ENEMYFILE003 ("data\\enemy3.txt")

//**************************************************************************************************************
//敵の攻撃状態
//**************************************************************************************************************
typedef enum
{
	ENEMYATTACK_NO = 0,
	ENEMYATTACK_ATTACK,
	ENEMYATTACK_MAX
}ENEMYATTACK;

//**************************************************************************************************************
//敵の状態
//**************************************************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_AGENT,
	ENEMYSTATE_ATTACK,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DEATH,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//**************************************************************************************************************
//敵の種類
//**************************************************************************************************************
typedef enum
{
	ENEMYTYPE_ONE = 0, 
	ENEMYTYPE_TWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_FOUR,
	ENEMYTYPE_FIVE,
	ENEMYTYPE_SIX,
	ENEMYTYPE_SEVEN,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//**************************************************************************************************************
//敵の種類構造体
//**************************************************************************************************************
typedef struct
{
	LPD3DXMESH pMeshEnemy;//メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER pBuffMatEnemy;//マテリアルへのポインタ
	DWORD dwNumMatEnemy;//マテリアルの数
	LPDIRECT3DTEXTURE9 pTextureEnemy[128];
	D3DCOLORVALUE Diffuse,FirstDiffuse;
}EnemyModel;

//**************************************************************************************************************
//敵の構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 offpos, offrot;
	D3DXVECTOR3 rot;//向き
	//D3DXVECTOR3 vtxMin,vtxMax;
	//D3DXVECTOR3 Size;
	D3DXMATRIX mtxWorldEnemy;
	float fDistance;
	int nType;
	bool bUse;//使用しているかどうか
	MOTION Motion;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 vtxMin,vtxMax;
	int nLife;
	int nMaxLife;
	int nDamage;
	ENEMYSTATE state;
	int nCounterState;
	int nIdxShadow;
	int nIdxLifeBar,nIdxLifeFrame;
	int nIdxUi;
	EnemyModel EnemyModel[16];
	bool g_bDamage;
	float fMove;//ランダム
	int nCountAction;
	int AttackState;
	float Speed; // 足の速さ
	D3DXVECTOR3 rotDest; // 目的の角度
	D3DXVECTOR3 PosDest; // 目的の位置
	int nIdxMap;
	int HitStopCount;    // ヒットストップ
}ENEMY;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void InitEnemy(void);//敵の初期化処理
void UninitEnemy(void);//敵の終了処理
void UpdateEnemy(void);//敵の更新処理
void DrawEnemy(void);//敵の描画処理
ENEMY* GetEnemy(void);//敵の取得処理
void HitEnemy(int nCnt,int nDamage);//敵のヒット処理
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife,float Speed);//敵の設定処理
void WaveEnemy(int nSpawner);
int GetNumEnemy(void);//敵の総数取得処理
#endif
