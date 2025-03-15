//==============================================================================================================
//
//プレイヤー
//Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************
//インクルードファイル
//**************************
#include"main.h"
#include"model.h"
#include"motion.h"
#include "enemy.h"
#include "item.h"

//**************************
//マクロ定義
//**************************
#define PLAYER_MOVE (1.0f)
#define WIDTH (100.0f) //壁の横幅
#define HEIGHT (100.0f) //壁の高さ
#define MAX_MODEL (16)
#define ATTACKER_ENEMY (0)      // 攻撃してきたのが敵
#define ATTACKER_BOSS (1)       // 攻撃してきたのがボス
#define MAX_GLABITY (1.0f)      // 重力
#define ITEMTYPE_NONEXISTENT (4096) // 存在しないアイテムの種類
#define ITEM_MAX (456)

//**************************************************************************************************************
//ライフの種類
//**************************************************************************************************************
typedef enum
{
	LIFE_ONE = 0,
	LIFE_TWO,
	LIFE_THREE,
	LIFE_MAX
}LIFE;

//*****************************************************************************************************************
// ライフのテクスチャ設定
//*****************************************************************************************************************
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
	PLAYERSTATE_CRAFT,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*****************************************************************************************************************
//プレイヤーの武器モーションの数
//*****************************************************************************************************************
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
	MOTION_SPDOUBLE,
	MOTION_MAX
}WEPONMOTION; // 使いまわせるものは使いまわす

//***************************
//手持ちの属性(演出面)
//***************************
typedef enum
{
	WEPONELEMENT_STANDARD = 0,
	WEPONELEMENT_BLOOD,
	WEPONELEMENT_FIRE,
	WEPONELEMENT_FREEZE,
	WEPONELEMENT_SPARK,
	WEPONELEMENT_AQUA,
	WEPONELEMENT_DARK,
	WEPONELEMENT_MAX
}WEPONELEMENT;

//***************************
// プレイヤーの攻撃状態
//***************************
typedef enum
{
	PLAYERATTACKSTATE_NO,
	PLAYERATTACKSTATE_ATTACK,
	PLAYERATTACKSTATE_MAX
}PLAYERATTACKSTATE;

//**************************
//プレイヤー構造体
//**************************
typedef struct
{
	D3DXVECTOR3 pos;//位置
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
	PLAYERSTATE state;
	int nCounterState;
	int nLife;
	int nMaxLife;
	D3DXVECTOR3 SwordOffpos;
	D3DXMATRIX SwordMtx;//剣先につけるワールドマトリックスのオフセット
	PLAYERHOLDTATE HandState;
	int nCounterAction;
	int PlayerType;
	float fShadowSize;//影の大きさ
	int Combostate; // コンボの状態をもつ変数 
	int ItemIdx;    // アイテムのインデックス保管用変数
	int nElement;   // 手持ちアイテムの属性管理
	float speed;    // 足の速さ 
	int nDamage;    // プレイヤーの攻撃力 
	int nOldItem;
	int WeponMotion; // 武器ごとのモーション
	int nStockDamage;
	float fStockSpeed;
	bool Itembreak[ITEM_MAX];
	bool FeverMode;
	bool SpMode;
	bool AttackSp;
	bool bLandingOBB; // OBBの上に着地下かどうか
	bool bCraft;      // クラフト中かどうか
	int nIdxMap;
	int StockItemIdx; // ストックしているアイテムのインデックス
	int nIdxCylinder;  // シリンダーのインデックス
	int AttackState;   // プレイヤーの攻撃状態
	int nCounterAttack; // 攻撃状態の状態カウンター
	D3DXVECTOR3 avoidMove; // 回避の移動量
	int BlowCounter;       // 吹き飛ぶまでのカウンター
	int AttackerIdx;       // 攻撃してきた敵のインデックス
	bool bstiffness;        // ダメージの硬直時間
	int StockItemType;		// ストックしているアイテムの種類
	int HoldItemType;       // 持っているアイテムの種類
	bool bAvoid;			// 無敵時間
	int HitStopCount;       // ヒットストップのカウント
}Player;

//**************************
//プロトタイプ宣言
//**************************
void InitPlayer(void);																							// プレイヤーの初期化処理
void UninitPlayer(void);																						// プレイヤーの終了処理
void UpdatePlayer(void);																						// プレイヤーの更新処理
void DrawPlayer(void);																							// プレイヤーの描画処理
Player* GetPlayer(void);																						// プレイヤーの取得処理
void SetMtxPos(void);																							// ワールドマトリックスのオフセット設定処理
void HitPlayer(int nDamage, bool SetDamageMotion, int AttackerIdx, int AttackerType);							// プレイヤーのヒット処理
bool CollisionItem(int nIdx, float Itemrange, float plrange);													// アイテムとの判定
void HitSowrd(ENEMY* pEnemy,int nCntEnemy);                                                                     // 剣の当たり判定
void ThrowItem(void);																							// アイテムを投げる
void CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove, float PLradius, float ENradius);					// プレイヤーと敵の当たり判定
void MotionChange(int itemtype, int LoadPlayer);																// モーション変更
void StatusChange(float speed, D3DXVECTOR3 SwordOffpos, int nDamage);											// プレイヤーのステータス変更
bool CheckMotionBounds(int nKey, int nCountFrame, int StartKey, int EndKey, int startFrame, int EndFrame);		// モーションの開始判定処理
float SetAttackerAngle(int AttackerIdx, int AttackerType);														// アタッカーを調べる関数
void LoadItemChange(int nType, float swordLength);																// アイテムの変更する時のステータス読み込み
int LoadPlayerParamCharge(char* aStr, FILE* pFile, float swordLength);											// アイテムのロード
bool isPlayerAttaking(void);																					// プレイヤーが攻撃状態か
#endif
