//=================================================================================================================
//
// 武器のアイコン処理 [icon.h]
// Author: Asuma Nishio
//
//=================================================================================================================
#ifndef _ICON_H_
#define _ICON_H_

//****************************
// インクルードファイル
//****************************
#include "main.h"

//****************************
// アイコンの種類の列挙型宣言
//****************************
typedef enum
{
	WEPONTYPE_BAT = 0, // バット
	WEPONTYPE_GOLF,	   // ゴルフ
	WEPONTYPE_HUNMER,  // ハンマー
	WEPONTYPE_STONE,   // 石
	WEPONTYPE_WOOD,	   // 木
	WEPONTYPE_STONEBAT,// 石バット
	WEPONTYPE_LIGHT,   // 蛍光灯
	WEPONTYPE_LIGHTWOOD,// 蛍光灯ソード
	WEPONTYPE_HARISEN,	// ハリセン
	WEPONTYPE_ICEBLOCK,	// 氷

	WEPONTYPE_ICEBLOCKSOWRD, // 氷の剣
	WEPONTYPE_IRON,			 // 鉄パイプ
	WEPONTYPE_IRONBAT,		 // 金属バット
	WEPONTYPE_SURFBOARD,	 // サーフボード
	WEPONTYPE_TORCH,		 // トーチ
	WEPONTYPE_TORCHSWORD,	 // トーチソード
	WEPONTYPE_BAR,			 // バール
	WEPONTYPE_HEADSTATUE,	 // 頭像
	WEPONTYPE_HEADSTATUTORSO,// 頭とトルソーの合成
	WEPONTYPE_MEGAPHONE,	 // メガホン
	WEPONTYPE_RUBBERCUP,	 // すっぽん(ラバーカップ)

	WEPONTYPE_TELEPHONEPOLE, // 電柱
	WEPONTYPE_TORSO,		 // トルソー
	WEPONTYPE_FLUORESCENTLIGHTMEGAPHONE, // メガホンの合成武器
	WEPONTYPE_BONESPEAR,	 // 骨の槍
	WEPONTYPE_FISH,			 // 魚
	WEPONTYPE_HEX,			 // 呪物
	WEPONTYPE_HEXMANDOLIN,	 // ハープ (呪物の合成武器)
	WEPONTYPE_SURFBOARDFISH, // 魚とサーフボードの合成
	WEPONTYPE_TUTORIAL,		 // チュートリアル看板
	WEPONTYPE_MAX
}WEPON_TYPE;

//****************************
// アイコンのテクスチャ宣言
//****************************
static const char* ICON_TEXTURE[WEPONTYPE_MAX] =
{
	"data\\TEXTURE\\bat.png", // バット
	"data\\TEXTURE\\golf.png",// ゴルフ
	"data\\TEXTURE\\hunmer.png",// ハンマー
	"data\\TEXTURE\\stone.png", // 石
	"data\\TEXTURE\\wood.png",	// 木
	"data\\TEXTURE\\stonebat.png", // 石バット
	"data\\TEXTURE\\fluorescentlight.png", // 蛍光灯
	"data\\TEXTURE\\fluorescentlightsword.png", // 蛍光灯ソード
	"data\\TEXTURE\\harisen.png", // ハリセン
	"data\\TEXTURE\\iceblock.png", // 氷

	"data\\TEXTURE\\iceblocksword.png", // 氷の剣
	"data\\TEXTURE\\iron.png",			// 鉄パイプ
	"data\\TEXTURE\\ironbat.png",		// 金属バット
	"data\\TEXTURE\\surfboard.png",		// サーフボード
	"data\\TEXTURE\\torch.png",			// トーチ
	"data\\TEXTURE\\torchsword.png",	// トーチソード
	"data\\TEXTURE\\bar.png",			// バール
	"data\\TEXTURE\\headstatue.png",	// 頭像
	"data\\TEXTURE\\headstatutorso.png",// トルソーと頭の合成
	"data\\TEXTURE\\megaphone.png",		// メガホン
	"data\\TEXTURE\\rubbercup.png",		// ラバーカップ

	"data\\TEXTURE\\telephonepole.png", // 電柱
	"data\\TEXTURE\\torso.png",			// トルソー
	"data\\TEXTURE\\fluorescentlightmegaphone.png", // メガホンの合成武器
	"data\\TEXTURE\\bonespear.png",		// 骨の槍
	"data\\TEXTURE\\fish.png",			// 魚
	"data\\TEXTURE\\hex.png",			// 呪物
	"data\\TEXTURE\\hexmandolin.png",	// ハープ
	"data\\TEXTURE\\surfboardfish.png", // サーフボードの合成武器
	"data\\TEXTURE\\tutorial.png",		// チュートリアルの看板
};

//****************************
// アイコンの構造体宣言
//****************************
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	D3DXVECTOR3 move;// 移動量
	int nType;		 // 種類
	float fWidth;	 // 横幅
	float fHeight;	 // 高さ
	bool bUse;		 // 使用しているか
}ICON;

//****************************
// プロトタイプ宣言
//****************************
void InitIcon();	// 初期化処理
void UninitIcon();	// 終了処理
void UpdateIcon();	// 更新処理
void DrawIcon();	// 描画処理
void SetIcon(D3DXVECTOR3 pos, float fWidth,float fHeight,int nType); // 設定処理

#endif