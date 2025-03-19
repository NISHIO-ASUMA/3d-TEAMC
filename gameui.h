//==============================================================================================================
//
// ゲームに必要なUI [gameui.h]
// Author: Asuma Nishio
//
//==============================================================================================================

#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include"main.h"

//**************************************************************************************************************
// UIの種類
//**************************************************************************************************************
typedef enum
{
	UITYPE_TITLE = 0,	// タイトルのUI
	UITYPE_TITLE2,		// タイトルのUI
	UITYPE_FIVER,		// FeverのUI
	UITYPE_SYUTYUSEN,	// 集中線
	UITYPE_TUTORIAL,	// チュートリアル
	UITYPE_BLACK,		// sp刀の時の画面の暗転
	UITYPE_KATANA,		// タイトルの刀
	UITYPE_RED,			
	UITYPE_ICONFRAME,	// 持っているアイテムアイコンのフレーム
	UITYPE_ICONFRAMESTOCK,	// ストックアイコンのフレーム
	UITYPE_KEY,				// 必殺技のキー
	UITYPE_DESTORY,			// 武器が壊れた時のUI
	UITYPE_EVENT,			// イベント発生
	UITYPE_SP,				// スペシャル発動のキー
	UITYPE_DAMAGE,			// ダメージ受けた時の画面
	UITYPE_SETENEMYTIME,	// 敵が出た時のUI
	UITYPE_BOSSMANUALEXIT,	// ボスのマニュアルのUI
	UITYPE_POPENEMY, // 敵出現
	UITYPE_CRAFTTIME, // クラフトの時間に出るUI
	UITYPE_STOCKKEY,	// アイテムをストックするキー
	UITYPE_CRAFTTIMEMENU,	// クラフトタイムになったら出すUI
	UITYPE_SPINFO,		// SPゲージの解説UI
	UITYPE_SPONBREAK,	// スポーン破壊UI
	UITYPE_MAX
}UITYPE;

//**************************************************************************************************************
// UIのパスを保存
//**************************************************************************************************************
static const char* UITYPE_INFO[UITYPE_MAX] =
{
	"data\\TEXTURE\\title_logo.png",
	"data\\TEXTURE\\select.png",
	"data\\TEXTURE\\faver.png",
	"data\\TEXTURE\\syutyu.png",
    "data\\TEXTURE\\UI_enter.png",
	"data\\TEXTURE\\black.png",
	"data\\TEXTURE\\title_katana.png",
	"data\\TEXTURE\\hpred.png",
	"data\\TEXTURE\\icon_frame_1.png",
	"data\\TEXTURE\\icon_framestoku.png",
	"data\\TEXTURE\\tutoirial005.png",
	"data\\TEXTURE\\ui_weponbreak.png",
	"data\\TEXTURE\\ui_event_1.png",
	"data\\TEXTURE\\SP_Key_1.png",
	"data\\TEXTURE\\damage001.png",
	"data\\TEXTURE\\settime.png",
	"data\\TEXTURE\\bossmanualexit.png",
	"data\\TEXTURE\\ui_popenemy.png",
	"data\\TEXTURE\\ui_crafttime.png",
	"data\\TEXTURE\\ui_stock_key.png",
	"data\\TEXTURE\\ui_Manual_Craft.jpg",
	"data\\TEXTURE\\ui_sp_info.png",
	"data\\TEXTURE\\ui_Break_suponer.png",
};

//**************************************************************************************************************
// ゲームUIの構造体
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 向き
	int nUseTime;		// 出てから消えるまでの時間
	int nType;			// 種類
	float fWidth, fHeight; // 幅、高さ
	bool bUse;			// 使用状態
	bool bLife;         // 寿命をつけるかつけないか
	D3DXCOLOR col;      // 色
	int nEaseCnt;       // イージングのカウント
	int nCounterAnim, nPatternAnim; // テクスチャアニメーション
	float fAlv;						// 透明度
}Gameui;

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
void InitGameUI(void);	 // UIの初期化処理
void UninitGameUI(void); // UIの終了処理
void UpdateGameUI(void); // UIの更新処理
void DrawGameUI(void);	 // UIの描画処理
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, bool bLife, int nUseTime);
void FlashGameUI(int nSelect);
#endif