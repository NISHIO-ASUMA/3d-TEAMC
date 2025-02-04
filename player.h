//============================
//
//プレイヤー
//Author:YOSHIDA YUUTO
//
//============================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************
//インクルードファイル
//**************************
#include"main.h"
#include"model.h"
#include"motion.h"
#include "enemy.h"
#include"item.h"

//**************************
//マクロ定義
//**************************
#define PLAYER_MOVE (1.0f)
#define WIDTH (100.0f) //壁の横幅
#define HEIGHT (100.0f) //壁の高さ
#define MAX_MODEL (16)
#define MAX_ITEM (256)

//****************************
//ライフの種類
//****************************
typedef enum
{
	LIFE_ONE = 0,
	LIFE_TWO,
	LIFE_THREE,
	LIFE_MAX
}LIFE;

//*******************************
// ライフのテクスチャ設定
//*******************************
static const char* LIFE_TYPE[LIFE_MAX] =
{
	"data\\TEXTURE\\Life001.png",
	"data\\TEXTURE\\Life000.png",
	"data\\TEXTURE\\playerLife0000.png",
};

//***************************
//ライフの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
	D3DXCOLOR col;
	int nLife;
	int nType;
	int nDamage;
}LIFEBAR;

//**************************
//プレイヤーの種類
//**************************
typedef enum
{
	PLAYERHOLD_NO = 0,
	PLAYERHOLD_HOLD,
	PLAYERHOLD_MAX
}PLAYERHOLDTATE;

//**************************
//プレイヤーの種類
//**************************
typedef enum
{
	PLAYERTYPE_WEPON = 0,
	PLAYERTYPE_NOHAND,
	PLAYERTYPE_MAX
}PLAYERTYPE;

//**************************
//プレイヤーのコンボ状態
//**************************
typedef enum
{
	COMBO_NO = 0,
	COMBO_ATTACK1,
	COMBO_ATTACK2,
	COMBO_ATTACK3,
	COMBO_ATTACK4,
	COMBO_ATTACK5,
	COMBO_MAX
}COMBOSTATE;

//**************************
//プレイヤーの状態
//**************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_ATTACK,
	PLAYERSTATE_JUMP,
	PLAYERSTATE_LANDING,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_FALL,
	PLAYERSTATE_INVISIBLE,
	PLAYERSTATE_NO_WEPON,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*******************************
//プレイヤーの武器モーションの数
//*******************************
typedef enum
{
	MOTION_KATANA = 0,
	MOTION_BIGWEPON,
	MOTION_DBHAND,
	MOTION_ONE_HAND,
	MOTION_PIERCING,
	MOTION_SP,
	MOTION_SPHAMMER,
	MOTION_ONEHANDBLOW,
	MOTION_SPPIERCING,
	MOTION_MAX
}WEPONMOTION; // 使いまわせるものは使いまわす

//**************************
//プレイヤー構造体
//**************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posDest;
	D3DXVECTOR3 posOld;//前回の位置
	D3DXVECTOR3 Size;//大きさ
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 rotDestPlayer;//目的の角度
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3	vtxMinPlayer;
	D3DXVECTOR3	vtxMaxPlayer;
	D3DXMATRIX mtxWorldPlayer;//ワールドマトリックス
	int nIdxShadow;
	bool bJump;
	bool bDisp;
	float PlayerMove;
	MOTION Motion;
	bool bMove;
	PLAYERSTATE state;
	int nCounterState;
	int nLife;
	int nMaxLife;

	D3DXVECTOR3 SwordOffpos;
	D3DXVECTOR3 Swordpos, Swordrot;
	D3DXMATRIX SwordMtx;//剣先につけるワールドマトリックスのオフセット
	PLAYERHOLDTATE HandState;
	int nCounterAction;
	int PlayerType;
	bool bImpactCollision;
	bool bJumpAttack;
	float fShadowSize;//影の大きさ
	int Combostate; // コンボの状態をもつ変数 
	int ItemIdx;    // アイテムのインデックス保管用変数
	float speed;    // 足の速さ 
	int nDamage;    // プレイヤーの攻撃力 
	int nOldItem;
	int WeponMotion; // 武器ごとのモーション
	int nStockDamage;
	float fStockSpeed;
	bool Itembreak[MAX_ITEM];
	bool FeverMode;
	bool SpMode;
	bool AttackSp;  // sp攻撃をしているか
}Player;

//**************************
//プロトタイプ宣言
//**************************
void InitPlayer(void);//プレイヤーの初期化処理
void UninitPlayer(void);//プレイヤーの終了処理
void UpdatePlayer(void);//プレイヤーの更新処理
void DrawPlayer(void);//プレイヤーの描画処理
Player* GetPlayer(void);//プレイヤーの取得処理
void SetMtxPos(void);//ワールドマトリックスのオフセット設定処理
void HitPlayer(int nDamage);//プレイヤーのヒット処理
bool CollisionItem(int nIdx, float Itemrange, float plrange);
void HitSowrd(ENEMY* pEnemy,int nCntEnemy);
void ThrowItem(void); // アイテムを投げる
void CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove, float PLradius, float ENradius); // プレイヤーと敵の当たり判定
bool sphererange(D3DXVECTOR3* pPos1,D3DXVECTOR3 *pPos2, float radius1, float radius2); // 円の当たり判定
#endif
