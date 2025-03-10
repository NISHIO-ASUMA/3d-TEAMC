//============================================================================================================
//
// 3Dゲーム基盤 [main.h]
// Author: TEAM_C
//
//============================================================================================================

#ifndef _MAIN_H_  // このマクロ定義がされてなかったら
#define _MAIN_H_  // 2重インクルード防止のマクロ定義

//***************************
// インクルードファイル宣言
//***************************
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)// ビルド時警告対処用のマクロ
#include "d3dx9.h"					// 描画処理に必要
#include "dinput.h"					// 入力処理に必要
#include "Xinput.h"					// ジョイパッド処理に必要
#include "xaudio2.h"

//**************************
// ライブラリのリンク宣言
//**************************
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")	// ジョイパッド処理に必要
#pragma comment(lib,"Winmm.lib")

//**************************
// マクロ定義
//**************************
#define CLASS_NAME "WindowClass"      // ウインドウクラスの名前
#define WINDOW_NAME "抜刀制裁!"		  // ウインドウの名前(キャプションに表示)
#define SCREEN_WIDTH (1280)           // ウインドウの幅
#define SCREEN_HEIGHT (720)           // ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) // 頂点フォーマット[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1) // 座標,法線,カラー,テクスチャ

//**************************
// マクロ定義(色設定)
//**************************
#define COLOR_INDIANRED D3DCOLOR_RGBA(205,92,92,255)				 // インディアンレッド
#define COLOR_LIGHTCORAL D3DCOLOR_RGBA(240,128,128,255)				 // ライトコーラル
#define COLOR_SALMON D3DCOLOR_RGBA(250,128,114,255)					 // サーモンカラー
#define COLOR_DARKSALMON D3DCOLOR_RGBA(233,150,122,255)				 // 黒いサーモンカラー
#define COLOR_LIGHTSALMON D3DCOLOR_RGBA(255,160,122,255)			 // 明るいサーモンカラー
#define COLOR_CRIMSON D3DCOLOR_RGBA(220,20,60,255)					 // クリムゾン
#define COLOR_RED D3DCOLOR_RGBA(255,0,0,255)						 // 赤
#define COLOR_FIREBRICK D3DCOLOR_RGBA(178,34,34,255)				 // ファイアブリック
#define COLOR_DARKRED D3DCOLOR_RGBA(139,0,0,255)					 // 暗めの赤
#define COLOR_PINK D3DCOLOR_RGBA(255,192,203,255)					 // ピンク
#define COLOR_LIGHTPINK D3DCOLOR_RGBA(255,182,193,255)				 // 明るいピンク
#define COLOR_HOTPINK D3DCOLOR_RGBA(255,105,180,255)				 // 濃いピンク
#define COLOR_DEEPPINK D3DCOLOR_RGBA(255,20,147,255)				 // さらに濃いピンク
#define COLOR_MEDIUMVIOLETRED D3DCOLOR_RGBA(199,21,133,255)			 // もっと濃いピンク
#define COLOR_PALEVIOLETRED D3DCOLOR_RGBA(219,112,147,255)			 // 暗めのホットピンク
#define COLOR_CORAL D3DCOLOR_RGBA(255,127,80,255)					 // コーラル
#define COLOR_TOMATO D3DCOLOR_RGBA(255,99,71,255)					 // トマト色
#define COLOR_ORANGERED D3DCOLOR_RGBA(255,69,0,255)					 // 赤めのオレンジ
#define COLOR_DARKORANGE D3DCOLOR_RGBA(255,140,0,255)				 // 暗めのオレンジ
#define COLOR_ORANGE D3DCOLOR_RGBA(255,165,0,255)					 // オレンジ
#define COLOR_GOLD D3DCOLOR_RGBA(255,215,0,255)						 // 金色
#define COLOR_YELLOW D3DCOLOR_RGBA(255,255,0,255)					 // 黄色
#define COLOR_LIGHTYELLOW D3DCOLOR_RGBA(255,255,224,255)			 // クリーム色
#define COLOR_LEMONCHIFFON D3DCOLOR_RGBA(255,250,205,255)			 // レモンシフォン
#define COLOR_LIGHTGOLDENRODYELLOW D3DCOLOR_RGBA(250,250,210,255)	 // ゴールドイエロー
#define COLOR_PAPAYAWHIP D3DCOLOR_RGBA(255,239,213,255)				 // パパイヤポップ
#define COLOR_MOCCASIN D3DCOLOR_RGBA(255,228,181,255)				 // モカシン
#define COLOR_PEACHPUFF D3DCOLOR_RGBA(255,218,185,255)				 // ピーチパフ
#define COLOR_PALEGOLDENROD D3DCOLOR_RGBA(238,232,170,255)			 // ペールゴールデンロッド
#define COLOR_KHAKI D3DCOLOR_RGBA(240,230,140,255)					 // カーキ
#define COLOR_DARKHAKI D3DCOLOR_RGBA(189,183,107,255)				 // ダークカーキ
#define COLOR_LAVENDER D3DCOLOR_RGBA(230,230,250,255)				 // ラベンダー
#define COLOR_THISTLE D3DCOLOR_RGBA(216,191,216,255)				 // ティスル
#define COLOR_PLUM D3DCOLOR_RGBA(221,160,221,255)					 // プラム
#define COLOR_VIOLET D3DCOLOR_RGBA(238,130,238,255)					 // ヴァイオレット
#define COLOR_ORCHID D3DCOLOR_RGBA(218,112,214,255)					 // オーキッド
#define COLOR_FUCHSIA D3DCOLOR_RGBA(255,0,255,255)					 // フクシア
#define COLOR_MAGENTA D3DCOLOR_RGBA(255,0,255,255)					 // マゼンタ
#define COLOR_MIDIUMORCHID D3DCOLOR_RGBA(186,85,211,255)			 // ミディアムオーキッド
#define COLOR_MIDIUMPURPLE D3DCOLOR_RGBA(147,112,219,255)			 // ミディアムパープル
#define COLOR_AMETHYST D3DCOLOR_RGBA(153,102,204,255)				 // アメジスト
#define COLOR_BLUEVIOLET D3DCOLOR_RGBA(138,43,226,255)				 // ブルーヴァイオレット
#define COLOR_DARKVIOLET D3DCOLOR_RGBA(148,0,211,255)				 // ダークヴァイオレット
#define COLOR_DARKORCHID D3DCOLOR_RGBA(153,50,204,255)				 // ダークオーキッド
#define COLOR_DARKMAGENTA D3DCOLOR_RGBA(139,0,139,255)				 // ダークマゼンタ
#define COLOR_PURPLE D3DCOLOR_RGBA(128,0,128,255)					 // パープル
#define COLOR_INDIGO D3DCOLOR_RGBA(75,0,130,255)					 // 藍色
#define COLOR_SLATEBLUE D3DCOLOR_RGBA(106,90,205,255)				 // スレートブルー
#define COLOR_DARKSLATEBLUE D3DCOLOR_RGBA(72,61,139,255)			 // ダークスレートブルー
#define COLOR_GREENYELLOW D3DCOLOR_RGBA(173,255,47,255)				 // グリーンイエロー
#define COLOR_CHARTREUSE D3DCOLOR_RGBA(127,255,0,255)				 // シャルトルーズ
#define COLOR_LAWNGREEN D3DCOLOR_RGBA(124,252,0,255)				 // ローングリーン
#define COLOR_LIME D3DCOLOR_RGBA(0,255,0,255)					     // ライム
#define COLOR_LIMEGREEN D3DCOLOR_RGBA(50,205,50,255)				 // ライムグリーン
#define COLOR_PALEGREEN D3DCOLOR_RGBA(152,251,152,255)				 // ペールグリーン
#define COLOR_LIGHTGREEN D3DCOLOR_RGBA(144,238,144,255)				 // ライトグリーン
#define COLOR_MEDIUMSPRINGGREEN D3DCOLOR_RGBA(0,250,154,255)		 // ミディアムスプリンググリーン
#define COLOR_SPRINGGREEN D3DCOLOR_RGBA(0,255,127,255)				 // スプリンググリーン
#define COLOR_MEDIUMSEAGREEN D3DCOLOR_RGBA(60,179,113,255)			 // ミディアムシーグリーン
#define COLOR_SEAGREEN D3DCOLOR_RGBA(46,139,87,255)					 // シーグリーン
#define COLOR_FORESTGREEN D3DCOLOR_RGBA(34,139,34,255)				 // フォレストグリーン
#define COLOR_GREEN D3DCOLOR_RGBA(0,128,0,255)						 // 緑
#define COLOR_DARKGREEN D3DCOLOR_RGBA(0,100,0,255)					 // ダークグリーン
#define COLOR_YELLOWGREEN D3DCOLOR_RGBA(154,205,50,255)				 // イエローグリーン
#define COLOR_OLIVEDRAB D3DCOLOR_RGBA(107,142,35,255)				 // オリーブドラブ
#define COLOR_OLIVE D3DCOLOR_RGBA(128,128,0,255)					 // オリーブ
#define COLOR_DARKOLIVEGREEN D3DCOLOR_RGBA(85,107,47,255)			 // ダークオリーブグリーン
#define COLOR_MEDIUMAQUAMARINE D3DCOLOR_RGBA(102,205,170,255)		 // ミディアムアクアマリン
#define COLOR_DARKSEAGREEN D3DCOLOR_RGBA(143,188,143,255)			 // ダークシーグリーン
#define COLOR_LIGHTSEAGREEN D3DCOLOR_RGBA(32,178,170,255)			 // ライトシーグリーン
#define COLOR_DARKCYAN D3DCOLOR_RGBA(0,139,139,255)					 // ダークシアン
#define COLOR_TEAL D3DCOLOR_RGBA(0,128,128,255)						 // テール
#define COLOR_AQUA D3DCOLOR_RGBA(0,255,255,255)						 // アクア
#define COLOR_CYAN D3DCOLOR_RGBA(0,255,255,255)						 // シアン
#define COLOR_LIGHTCYAN D3DCOLOR_RGBA(224,255,255,255)				 // ライトシアン
#define COLOR_PALETURQUOISE D3DCOLOR_RGBA(175,238,238,255)			 // ペールターコイズ
#define COLOR_AQUAMARINE D3DCOLOR_RGBA(127,255,212,255)				 // アクアマリン
#define COLOR_TURQUOISE D3DCOLOR_RGBA(64,224,208,255)				 // ターコイズ
#define COLOR_MEDIUMTURQUOISE D3DCOLOR_RGBA(72,209,204,255)			 // ミディアムターコイズ
#define COLOR_DARKTURQUOISE D3DCOLOR_RGBA(0,206,209,255)			 // ダークターコイズ
#define COLOR_CADEBLUE D3DCOLOR_RGBA(95,158,160,255)				 // カデットブルー
#define COLOR_STEELBLUE D3DCOLOR_RGBA(70,130,180,255)				 // スティールブルー
#define COLOR_LIGHTSTEELBLUE D3DCOLOR_RGBA(176,196,222,255)			 // ライトスティールブルー
#define COLOR_POWDERBLUE D3DCOLOR_RGBA(176,224,230,255)				 // パウダーブルー
#define COLOR_LIGHTBLUE D3DCOLOR_RGBA(173,216,230,255)				 // ライトブルー
#define COLOR_SKYBLUE D3DCOLOR_RGBA(135,206,235,255)				 // スカイブルー
#define COLOR_LIGHTSKYBLUE D3DCOLOR_RGBA(135,206,250,255)			 // ライトスカイブルー
#define COLOR_DEEPSKYBLUE D3DCOLOR_RGBA(0,191,255,255)				 // ディープスカイブルー
#define COLOR_DODGERBLUE D3DCOLOR_RGBA(30,144,255,255)				 // ドジャーブルー
#define COLOR_CORNFLOWERBLUE D3DCOLOR_RGBA(100,149,237,255)			 // コーンフラワーブルー
#define COLOR_MEDIUMSLATEBLUE D3DCOLOR_RGBA(123,104,238,255)		 // ミディアムスレートブルー
#define COLOR_ROYALBLUE D3DCOLOR_RGBA(65,105,225,255)				 // ロイヤルブルー
#define COLOR_BLUE D3DCOLOR_RGBA(0,0,255,255)						 // 青
#define COLOR_MEDIUMBLUE D3DCOLOR_RGBA(0,0,205,255)					 // ミディアムブルー
#define COLOR_DARKBLUE D3DCOLOR_RGBA(0,0,139,255)					 // ダークブルー
#define COLOR_NAVY D3DCOLOR_RGBA(0,0,128,255)						 // ネービー
#define COLOR_MIDINIGHTBLUE D3DCOLOR_RGBA(25,25,112,255)			 // ミッドナイトブルー
#define COLOR_CORNSILK D3DCOLOR_RGBA(255,248,220,255)				 // コーンシルク
#define COLOR_BLANCHEDALMOND D3DCOLOR_RGBA(255,235,205,255)			 // ブランチドアーモンド
#define COLOR_BISQUE D3DCOLOR_RGBA(255,228,196,255)					 // ビスク
#define COLOR_NAVAJOWHITE D3DCOLOR_RGBA(255,222,173,255)			 // ナバホホワイト
#define COLOR_WHEAT D3DCOLOR_RGBA(245,222,179,255)				     // ウィート
#define COLOR_BURLYWOOD D3DCOLOR_RGBA(222,184,135,255)			     // バーリーウッド
#define COLOR_TAN D3DCOLOR_RGBA(210,180,140,255)					 // テン
#define COLOR_ROSYBROWN D3DCOLOR_RGBA(188,143,143,255)				 // ロージブラウン
#define COLOR_SANDYBROWN D3DCOLOR_RGBA(244,164,96,255)				 // サンディーブラウン
#define COLOR_GOLDENROD D3DCOLOR_RGBA(218,165,32,255)				 // ゴールデンロッド
#define COLOR_DARKGOLDENROD D3DCOLOR_RGBA(184,134,11,255)			 // ダークゴールデンロッド
#define COLOR_PERU D3DCOLOR_RGBA(205,133,63,255)					 // ペる
#define COLOR_CHOCOLATE D3DCOLOR_RGBA(210,105,30,255)				 // チョコレート
#define COLOR_SADDLEBEOWN D3DCOLOR_RGBA(139,69,19,255)				 // サドルブラウン
#define COLOR_SIENNA D3DCOLOR_RGBA(160,82,45,255)					 // シエナ
#define COLOR_BWOWN D3DCOLOR_RGBA(165,42,42,255)					 // ブラウン
#define COLOR_MAROON D3DCOLOR_RGBA(128,0,0,255)						 // マルーン
#define COLOR_WHITE D3DCOLOR_RGBA(255,255,255,255)					 // ホワイト
#define COLOR_SNOW D3DCOLOR_RGBA(255,250,250,255)					 // 雪
#define COLOR_HONEYDEW D3DCOLOR_RGBA(240,255,240,255)				 // マルーン
#define COLOR_MINTCREAM D3DCOLOR_RGBA(245,255,250,255)				 // ミントグリーン
#define COLOR_AZURE D3DCOLOR_RGBA(240,255,255,255)					 // アジュール
#define COLOR_ALICEBLUE D3DCOLOR_RGBA(240,248,255,255)				 // アリスブルー
#define COLOR_GHOSTWHITE D3DCOLOR_RGBA(248,248,255,255)				 // ゴーストホワイト
#define COLOR_WHITESMOKE D3DCOLOR_RGBA(245,245,245,255)				 // ホワイトスモーク
#define COLOR_SEASHELL D3DCOLOR_RGBA(255,245,238,255)				 // セーシェル
#define COLOR_BEIGE D3DCOLOR_RGBA(245,245,220,255)					 // ベイジ
#define COLOR_OLDLACE D3DCOLOR_RGBA(253,245,230,255)				 // オールドレース
#define COLOR_FLORALWHITE D3DCOLOR_RGBA(255,250,240,255)			 // フローラルホワイト
#define COLOR_IVORY D3DCOLOR_RGBA(255,255,240,255)					 // アイボリー
#define COLOR_ANTIQUEWHITE D3DCOLOR_RGBA(250,235,215,255)			 // アンティークホワイト
#define COLOR_LINEN D3DCOLOR_RGBA(250,240,230,255)					 // リネン
#define COLOR_LAVENDERBLUSH D3DCOLOR_RGBA(255,240,245,255)			 // ラベンダーブラッシュ
#define COLOR_MISTYROSE D3DCOLOR_RGBA(255,228,225,255)				 // ミスティーローズ
#define COLOR_GAINSBORO D3DCOLOR_RGBA(220,220,220,255)				 // ダークカーキ
#define COLOR_LIGHTGREY D3DCOLOR_RGBA(211,211,211,255)				 // ゲインズボロ
#define COLOR_SILVER D3DCOLOR_RGBA(192,192,192,255)					 // 銀
#define COLOR_DARKGRAY D3DCOLOR_RGBA(169,169,169,255)				 // ダークグレー
#define COLOR_GRAY D3DCOLOR_RGBA(128,128,128,255)					 // グレー
#define COLOR_DIMGRAY D3DCOLOR_RGBA(105,105,105,255)				 // ディムグレー
#define COLOR_LIGHTSLATEGRAY D3DCOLOR_RGBA(119,136,153,255)			 // ライトスレートグレー
#define COLOR_SLATEGRAY D3DCOLOR_RGBA(112,128,144,255)				 // スレートグレー
#define COLOR_DARKSLATEGRAY D3DCOLOR_RGBA(47,79,79,255)				 // ダークスレートグレー
#define COLOR_BLACK D3DCOLOR_RGBA(0,0,0,255)						 // ブラック
//#define COLOR_DARKBLUE D3DCOLOR_RGBA(35,59,108,255)					 // ダークブルー
//#define COLOR_INDIGO D3DCOLOR_RGBA(22,94,131,255)					 // 紺色
//#define COLOR_BLUISHPURPLE D3DCOLOR_RGBA(65,20,69,255)				 // 藍色
//#define COLOR_BELLFLOWERCOLOR D3DCOLOR_RGBA(106,76,156,255)			 // 紫紺
//#define COLOR_EDOPURPLE D3DCOLOR_RGBA(116,83,153,255)				 // 江戸紫
//#define COLOR_UGLYCOLOR D3DCOLOR_RGBA(145,141,64,255)				 // 鶯色
//#define COLOR_LYECOLOR D3DCOLOR_RGBA(188,176,156,255)				 // 灰汁色
//#define COLOR_FLAXEN D3DCOLOR_RGBA(214,198,175,255)					 // 亜麻色
//#define COLOR_BRIGHTYELLOW D3DCOLOR_RGBA(248,180,0,255)				 // 山吹色
//#define COLOR_REDBEANCOLOR D3DCOLOR_RGBA(152,81,75,255)				 // あずき色
//#define COLOR_DARKRED D3DCOLOR_RGBA(179.66,74,255)					 // えんじ色
//#define COLOR_MADDERRED D3DCOLOR_RGBA(177,53,70,255)				 // 茜色
//#define COLOR_CRIMSON D3DCOLOR_RGBA(177.6,58,255)					 // 深紅


//**********************
// 頂点情報[2D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	float rhw;					// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
}VERTEX_2D;

//**********************
// 頂点情報[3D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	D3DXVECTOR3 nor;			// 法線ベクトル
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
}VERTEX_3D;

//**********************
// 画面の種類
//**********************
typedef enum
{
	MODE_TITLE = 0,		// タイトル
	MODE_TUTORIAL,		// チュートリアル
	MODE_GAME,			// ゲーム
	MODE_RESULT,		// リザルト
	MODE_EDIT,			// エディット
	MODE_RANKING,		// ランキング
	MODE_EFFECT,        // エフェクトの編集モード
	MODE_MAX,
}MODE;

//**********************
// プロトタイプ宣言
//**********************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//　ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							//　初期化処理
void Uninit(void);																	//　終了処理
void Update(void);																	//　更新処理
void Draw(void);																	//　描画処理
LPDIRECT3DDEVICE9 GetDevice(void);													//　デバイスの取得
void SetMode(MODE mode);															//　モードの設定
MODE GetMode(void);																	//　モードの取得
void DrawFPS(void);																	//　デバッグ表示
void DrawEnemySet(int g_nNumEnemy);													//　敵の総数の表示
void ToggleFullscreen(HWND hWnd);													//　ウィンドウをフルスクリーンにする方法
																					  
void DrawDebugPlayerPos(void);														//　プレイヤーの座標の表示
void DrawCameraPos(void);															//　カメラの座標表示
void DrawCameraType(void);															//　カメラの種類の表示

void onWireFrame();																	// ワイヤーフレーム起動
void offWireFrame();																// ワイヤーフレーム終了

#endif