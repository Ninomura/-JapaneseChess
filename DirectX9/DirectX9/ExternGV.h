#pragma once //インクルードガード

#define WindowWidthSize 960  //ウィンドウのX幅
#define WindowHeightSize 620 //ウィンドウのY幅
#define BoardImageSize 620   //盤面を表す画像サイズ
#define OfBoardImageSize 300
#define MapSize 9            //盤面のサイズ比

//画像ハンドル
extern int
ImgChooseable,  //選択可能の画像
ImgStartScreen, //スタート画面の画像
ImgGameEnd;     //終了画面の画像

//ゲームのモード
enum Game_Mode
{
	StartScreenProcessing,   //スタート画面の設定
	GameStartProcessing,     //初期処理
	PlayerProcessing,        //プレイヤー操作（選択可能処理）
	BoardProcess,            //盤面処理
	PromotedWindowProcessing,//成る画面処理
	GameEndProcessing,       //最終処理
};

//順番の状態、駒の状態
enum GameOrderMode
{
	Player01,
	Player02
};

//盤面の種類
enum PiecesType
{
//--------------------------------
	Pawn,          //歩
	Lance,         //香車
	Knight,        //桂馬
	SilverGeneral, //銀将
	GoldGeneral,   //金将
	Bishop,        //角行
	Rook,          //飛車
	King,          //王,玉
//---------------------------------
	Null           //無
//----------------------------------
};

//盤面上の駒の状態
typedef struct
{
	//駒の座標
	int y, x;
	//誰の駒か
	GameOrderMode PiecesGOM;
	//駒の種類
	PiecesType Type;
	//成っているか
	bool Promoted;
	//盤面上か
	bool OnBoard;
	
} OnBoardPieceState;
