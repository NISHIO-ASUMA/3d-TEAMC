//============================
//
// 敵[enemy.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//****************************
//インクルードファイル
//****************************
#include"main.h"
#include"motion.h"

//****************************
//マクロ定義
//****************************
#define MAX_ENEMY (64)
#define ENEMYFILE001 ("data\\enemy.txt")
#define ENEMYFILE002 ("data\\enemy2.txt")
#define ENEMYFILE003 ("data\\enemy3.txt")

//**************************
//プレイヤーの種類
//**************************
typedef enum
{
	ENEMYMOTION_NEUTRAL = 0,
	ENEMYMOTION_AGENT,
	ENEMYMOTION_ATTACK,
	ENEMYMOTION_ATTACK2,
	ENEMYMOTION_ATTACK3,
	ENEMYMOTION_MAX
}ENEMYMOTION;

//****************************
//敵の攻撃状態
//****************************
typedef enum
{
	ENEMYATTACK_NO = 0,
	ENEMYATTACK_ONE,
	ENEMYATTACK_TWO,
	ENEMYATTACK_THREE,
	ENEMYATTACK_MAX
}ENEMYATTACK;

//****************************
//敵の状態
//****************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_AGENT,
	ENEMYSTATE_ATTACK,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DEATH,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//****************************
//敵の種類
//****************************
typedef enum
{
	ENEMYTYPE_ONE = 0, 
	ENEMYTYPE_TWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_FOUR,
	ENEMYTYPE_FIVE,
	//ENEMYTYPE_SEVEN,
	//ENEMYTYPE_ATE,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//****************************
//敵の種類構造体
//****************************
typedef struct
{
	LPD3DXMESH pMeshEnemy;//メッシュ(頂点座標)へのポインタ
	LPD3DXBUFFER pBuffMatEnemy;//マテリアルへのポインタ
	DWORD dwNumMatEnemy;//マテリアルの数
	LPDIRECT3DTEXTURE9 pTextureEnemy[128];
	D3DCOLORVALUE Diffuse,FirstDiffuse;
}EnemyModel;

//****************************
//敵の構造体
//****************************
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
	D3DXVECTOR3 AttackEnemy;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 vtxMin,vtxMax;
	float radius;
	float EnemyRadius;//敵の半径(敵と剣の当たり判定用)
	float fenemycollision;
	float posRadius;//ホーミング範囲
	float AttackRadius;//攻撃範囲
	float radAttackEnemy;//攻撃モーション遷移範囲
	float fcollision;//敵と敵
	int nLife;
	int nDamage;
	ENEMYSTATE state;
	int nCounterState;
	int nIdxShadow;
	int nIdxLifeBar,nIdxLifeFrame;
	int nIdxUi;
	EnemyModel EnemyModel[16];
	bool g_bDamage;
	float fMove;//ランダム
	ENEMYATTACK AttackState;
	float Speed; // 足の速さ
}ENEMY;

//****************************
//プロトタイプ宣言
//****************************
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
