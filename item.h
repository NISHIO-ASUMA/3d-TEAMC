//==============================================================================================================
//
// アイテム [item.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _ITEM_H_
#define _ITEM_H_

//***************************
//インクルードファイル
//***************************
#include"main.h"
#include"player.h"
#include"block.h"

//***************************
//マクロ定義
//***************************
//#define MAX_TEX (32)
#define MAX_ITEM (512)
#define PARENT (5)

//***************************
//ブロックの種類
//***************************
typedef enum
{
	ITEMTYPE_BAT = 0,
	ITEMTYPE_GOLF,
	ITEMTYPE_HUNMER,
	ITEMTYPE_STONE,
	ITEMTYPE_WOOD,
	ITEMTYPE_STONEBAT,
	ITEMTYPE_LIGHT,
	ITEMTYPE_LIGHTWOOD,
	ITEMTYPE_HARISEN,
	ITEMTYPE_ICEBLOCK,
	ITEMTYPE_ICEBLOCKSOWRD,
	ITEMTYPE_IRON,
	ITEMTYPE_IRONBAT,
	ITEMTYPE_SURFBOARD,
	ITEMTYPE_TORCH,
	ITEMTYPE_TORCHSWORD,
	ITEMTYPE_BAR,
	ITEMTYPE_HEADSTATUE,
	ITEMTYPE_HEADSTATUTORSO,
	ITEMTYPE_MEGAPHONE,
	ITEMTYPE_RUBBERCUP,
	ITEMTYPE_TELEPHONEPOLE,
	ITEMTYPE_TORSO,
	ITEMTYPE_FLUORESCENTLIGHTMEGAPHONE,
	ITEMTYPE_BONESPEAR,
	ITEMTYPE_FISH,
	ITEMTYPE_HEX,
	ITEMTYPE_HEXMANDOLIN,
	ITEMTYPE_SURFBOARDFISH,
	ITEMTYPE_TUTORIAL,
	ITEMTYPE_KATANA,
	ITEMTYPE_ONIGIRI,
	ITEMTYPE_SPEAR, // 槍単体
	ITEMTYPE_MANDORIN, // マンドリン
	ITEMTYPE_BONE, // 骨単体
	ITEMTYPE_GOLFHUNMER,
	ITEMTYPE_MAX
}ITEMTYPE;

//***************************
//ブロックの状態の種類
//***************************
typedef enum
{
	ITEMSTATE_NORMAL = 0,
	ITEMSTATE_HOLD,
	ITEMSTATE_THROW,
	ITEMSTATE_STOCK,
	ITEMSTATE_RELEASE,
	ITEMSTATE_MAX
}ItemSTATE;

//***************************
//ブロックの属性の種類(演出面)
//***************************
typedef enum
{
	ITEMELEMENT_STANDARD = 0,
	ITEMELEMENT_BLOOD,
	ITEMELEMENT_FIRE,
	ITEMELEMENT_FREEZE,
	ITEMELEMENT_SPARK,
	ITEMELEMENT_AQUA,
	ITEMELEMENT_DARK,
	ITEMELEMENT_MAX
}ITEMELEMENT;

//***************************
// アイテムの種類の情報
//***************************
typedef struct
{
	int nElement;			// 属性
	int nType;				// 種類
	int durability;			// 耐久力
	int Maxdurability;		// 最大の耐久力
	char Itemtag[TAGWORD];  // アイテムのタグ
	int Power;              // 攻撃力
}ITEM_INFO;

//***************************
//ブロックの構造体
//***************************
typedef struct
{
	D3DXVECTOR3 pos,posOld;				//位置
	D3DXVECTOR3 move;					//移動
	D3DXVECTOR3 rot;					//向き
	D3DXVECTOR3 Scal;					//大きさ
	D3DXMATRIX mtxWorldItem;			//ワールドマトリックス
	D3DXVECTOR3 Size;
	bool bUse;
	int nType;
	int nLife;
	ItemSTATE state;					//ブロックの状態
	int nElement;						//アイテムの属性
	float fRadius;
	MODEL_INFO ItemTex[ITEMTYPE_MAX];
	D3DXMATRIX mtxParent;
	int durability;						// 耐久力
	int nIdxShadow;						// 影のインデックス
	bool EnableCraft;					// クラフトが有効なアイテムか否か
	//bool bMixItem[ITEMTYPE_MAX];		// 完成品表示用フラグ
	float grabity;
	int nCounterState;					// 状態カウンター
	int Maxdurability;					// 最大の耐久力
	int nIdxBillboardCount;
	int nEasingCnt;						// イージングのカウント
	int nImpactCount;					// インパクトのカウント
	char Itemtag[TAGWORD];
	ITEM_INFO aItemInfo[ITEMTYPE_MAX];
	int Power;							// 攻撃力
	int nMinimapIdx;					// ミニマップのインデックス
	bool bTracking;						// プレイヤーについていくか
	int nCylinderIdx;					// シリンダーのインデックス
}Item;

//***************************
//プロトタイプ宣言
//***************************
void InitItem(void);							// アイテムの初期化処理
void UninitItem(void);							// アイテムの終了処理
void UpdateItem(void);							// アイテムの更新処理
void DrawItem(void);							// アイテムの描画処理
void SetItem(D3DXVECTOR3 pos, int nType);		// アイテムの設定処理
bool HitThrowItem(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius); // 投げたアイテムとの当たり判定
void Itemchange(int nIdx,int nType);			// アイテムの変更処理
Item* GetItem(void);							// アイテムの取得処理
MODEL_INFO* GetItemOrigin(void);				// アイテムのモデルの取得処理
ITEM_INFO* GetItemInfo(void);					// アイテムの情報取得処理
void EnableCraftIcon(int Item1, int Item2);		// アイテムのアイコン表示処理
void CraftMixItem(int HoldIdx, int StockIdx);   // アイテムのクラフト処理
void SpawonItem(D3DXVECTOR3 pos, int nType);    // アイテムのスポーン処理
void SetFirstCraftItem(D3DXVECTOR3 pos, int nType);  // 最初にクラフトさせるアイテムの設定処理
bool CheckMixItemMat(int pCraftMat, int pStockMat, int HoldIdx, int StockIdx);		// アイテムがクラフトできるかどうか
#endif
