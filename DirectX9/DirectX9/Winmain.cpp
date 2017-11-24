
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>


#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include "PieceProcessing.h"

#include <random>


//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		//メッセージキューに
		//新しくWM_QUITメッセージを送る

		//いずれメッセージキューから
		//取り出され
		//メインループが終了する
		//(メインループの終了条件を
		//そのように作る)
		break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;

	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex);
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
					//正しくウィンドウの作成ができたら
					//この変数に識別子を代入する
	int width = WindowWidthSize,	//クライアント領域の幅
	int height = WindowHeightSize)	//クライアント領域の高さ
						//クライアント領域はウィンドウ全体から
						//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("将棋"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

								//ウィンドウが正しく作れたかをチェック
								//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}


//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstance,	//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	int nCmdShow)		//ウィンドウの表示状態
{
	//メッセージボックス
	MessageBox(NULL,		//ウィンドウのハンドル 
		TEXT("テスト"),		//本文
		TEXT("テスト-タイトル"),//タイトル
		MB_OK);				//メッセージボックスのタイプ
							//MB_OK  okのボタンが表示

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウクラス設定失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	//正しくウィンドウが作成されれば
	//hWndにウィンドウの識別ハンドルが入る
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device作成成功"),
			TEXT("テスト-タイトル"),
			MB_OK);
	}

	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHATEST);

	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	//駒のサイズ
	int PieceImageSize = BoardImageSize / MapSize - 6;

	//マウスフラグ
	bool MouseFlag;
	//ウィンドウフラグ
	bool WindowFlag;
	//ウィンドウマウス座標
	float WindowMousePosX, WindowMousePosY;
	//駒の切り替えフラグ
	bool TypeChangeFlag;
	//押した瞬間のマウス座標
	int SetMousePosX, SetMousePosY;
	//現在のマウス座標
	int MousePosX, MousePosY;

	//盤面の画像を設定
	Sprite spriteImgBoard;
	Texture imgBoard;
	spriteImgBoard.SetSize(BoardImageSize, BoardImageSize);
	spriteImgBoard.SetPos(BoardImageSize / 2, BoardImageSize / 2);

	imgBoard.Load(_T("Texture/board.png"));

	//ウィンドウの画像を設定
	Sprite spriteImgWindow;
	Texture imgWindow;
	spriteImgWindow.SetSize(300, 150);
	spriteImgWindow.SetPos(350, 300);

	imgWindow.Load(_T("Texture/window.png"));

	//駒の画像を設定
	Sprite spriteImgPiece[MapSize][MapSize];
	Texture imgPiece[MapSize][MapSize];

	//カーソルの画像を設定
	Sprite spriteImgCursor[MapSize][MapSize];
	Texture imgCursor[MapSize][MapSize];

	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			//駒
			spriteImgPiece[yCount][xCount].SetSize(PieceImageSize, PieceImageSize);

			imgPiece[yCount][xCount].Load(_T("Texture/piece.png"));
			imgPiece[yCount][xCount].SetDivide(8, 2);

			//カーソル
			spriteImgCursor[yCount][xCount].SetSize(PieceImageSize, PieceImageSize);

			imgCursor[yCount][xCount].Load(_T("Texture/cursor.png"));
		}
	}

	//初期を演算処理に設定
	Game_Mode Mode = GameStartProcessing;

	//初期の順番をプレイヤー１に設定
	GameOrderMode GOM = Player01;
	//持ち駒の宣言
	OfBoardPieceState OBPS;

	PieceProcessing PP;

	MSG msg = {};

	//メインプログラム-------------------------------------------------
	//quitメッセージが出てくるまでループを繰り返す
	//quitメッセージは上記のウィンドウプロシージャから送信
	//送信の条件などはウィンドウプロシージャを確認
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し
			//それをメッセージキューにポストする
			TranslateMessage(&msg);

			//メッセージの割り当て
			//ウィンドウプロシージャでメッセージを処理
			DispatchMessage(&msg);
		}
		else
		{
			pDi->Update();//キー状態の更新

			switch (Mode)
			{

			//スタート画面
			case Game_Mode::StartScreenProcessing:

				break;

			//初期処理
			case Game_Mode::GameStartProcessing:

				PP.~PieceProcessing();

				MouseFlag = false;
				TypeChangeFlag = false;

				//プレイヤー操作に移動
				Mode = PlayerProcessing;
				break;

			//プレイヤー操作(選択可能処理)
			case Game_Mode::PlayerProcessing:

				if (TypeChangeFlag == true)
				{
					PP.PieceMove(SetMousePosY,SetMousePosX,MousePosY,MousePosX);
					PP.PieceChooseableReset();

					TypeChangeFlag = false;
					MouseFlag = false;
					WindowFlag = false;

					switch (GOM)
					{
					case Player01:
						GOM = Player02;
						break;
					case Player02:
						GOM = Player01;
						break;
					}
					break;
				}
				
				if (MouseFlag == false && pDi->MouseButton(0))
				{
					MouseFlag = true;

					//マウス座標を取得
					Vector2<int> mousePos = pDi->MousePosition();

					SetMousePosX = (mousePos.X() - 32.0) / PieceImageSize;
					SetMousePosY = (mousePos.Y() - 32.0) / PieceImageSize;

					if (MousePosX < 9&&MousePosY < 9)
					{
						if (PP.PS[SetMousePosY][SetMousePosX].PiecesGOM == GOM)
						{
							//選択可能描画
							PP.PieceChooseable
							(SetMousePosY, SetMousePosX,
							 PP.PS[SetMousePosY][SetMousePosX].Type,
							 PP.PS[SetMousePosY][SetMousePosX].Promoted,
							 GOM);
						}
					}
				}
				else if (MouseFlag == true && pDi->MouseButton(0))
				{
					;
				}
				else if (PP.PS[SetMousePosY][SetMousePosX].PiecesGOM == GOM && MouseFlag == true && !(pDi->MouseButton(0)))
				{

					//マウス座標を取得
					Vector2<int> mousePos = pDi->MousePosition();

					MousePosX = (mousePos.X() - 32.0) / PieceImageSize;
					MousePosY = (mousePos.Y() - 32.0) / PieceImageSize;

					if (MousePosX < 9 && MousePosY < 9)
					{
						if (PP.PS[MousePosY][MousePosX].Chooseable == true)
						{
							//駒があれば持ち駒を増やす
							if (PP.PS[MousePosY][MousePosX].Type != Null)
							{
								;
							}

							//成るかどうかの判断を呼び出す
							//それ以上進めなくなる駒は自動的に成る
							if (((PP.PS[SetMousePosY][SetMousePosX].Type == Pawn
							  || PP.PS[SetMousePosY][SetMousePosX].Type == Lance)
							  && ((GOM == Player01 && MousePosY <= 0)
							  || (GOM == Player02 && MousePosY >= MapSize - 1)))

							  || ((PP.PS[SetMousePosY][SetMousePosX].Type == Knight)
							  && ((GOM == Player01 && MousePosY <= 1)
							  || (GOM == Player02 && MousePosY >= MapSize - 2)))

							  && PP.PS[SetMousePosY][SetMousePosX].Promoted == false)
							{
								PP.PS[MousePosY][MousePosX].Promoted = true;
								Mode = PlayerProcessing;
							}
							else if (GOM == Player01 && (MousePosY < 3 || SetMousePosY < 3))
							{
								Mode = PromotedWindowProcessing;
							}
							else if (GOM == Player02 && (MousePosY > 5 || SetMousePosY > 5))
							{
								Mode = PromotedWindowProcessing;
							}
							else
							{
								PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
							}
							
							TypeChangeFlag = true;
						}
						else
						{
							PP.PieceChooseableReset();
							MouseFlag = false;
						}
					}
				}
				else if (MouseFlag == true && !(pDi->MouseButton(0)))
				{
					PP.PieceChooseableReset();
					MouseFlag = false;
				}

				break;

			//盤面処理
			case Game_Mode::BoardProcess:

				//プレイヤー操作(選択可能処理)に移動
				Mode = PlayerProcessing;

				break;

			//成るかどうかの処理（ウィンドウ描画）
			case Game_Mode::PromotedWindowProcessing:

				if (PP.PS[SetMousePosY][SetMousePosX].Type != King
					&& PP.PS[SetMousePosY][SetMousePosX].Type != GoldGeneral
					&& PP.PS[SetMousePosY][SetMousePosX].Promoted == false)
				{
					WindowFlag = true;

					if (MouseFlag == false && pDi->MouseButton(0))
					{
						MouseFlag = true;
						//マウス座標を取得
						Vector2<int> mousePos = pDi->MousePosition();
					}
					else if (MouseFlag == true && pDi->MouseButton(0))
					{
						;
					}
					else if (MouseFlag == true && !(pDi->MouseButton(0)))
					{
						//マウス座標を取得
						Vector2<int> mousePos = pDi->MousePosition();

						WindowMousePosX = mousePos.X();
						WindowMousePosY = mousePos.Y();

						if (WindowMousePosX >=229&& WindowMousePosX < 341
						  && WindowMousePosY >= 312 && WindowMousePosY < 344)
						{
							PP.PS[MousePosY][MousePosX].Promoted = true;
							Mode = PlayerProcessing;
							WindowFlag = false;
						}
						else if (WindowMousePosX >= 356 && WindowMousePosX < 468
							&& WindowMousePosY >= 312 && WindowMousePosY < 344)
						{
							PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
							Mode = PlayerProcessing;
							WindowFlag = false;
						}

						MouseFlag = false;

					}
				}
				else
				{
					PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
					Mode = PlayerProcessing;
				}

				break;

			//ゲーム終了
			case Game_Mode::GameEndProcessing:

				break;

			}

			//描画処理
			//バックバッファのクリア
			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();

				//盤の描画
				spriteImgBoard.Draw(imgBoard);

				//駒の描画
				for (int yCount = 0; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						//取ることが可能であれば選択可能カーソルを描画
						if (PP.PS[yCount][xCount].Chooseable == true)
						{
							//座標設定
							spriteImgCursor[yCount][xCount].SetPos
							((float)xCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0,
								(float)yCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0);

							//描画
							spriteImgCursor[yCount][xCount].Draw(imgCursor[yCount][xCount]);
						}

						//その駒が盤面に出ているかどうか
						//そこ駒の状態が無ではないか
						if (PP.PS[yCount][xCount].Type != Null)
						{
							//その駒が敵だと（90度）反転する
							switch (PP.PS[yCount][xCount].PiecesGOM)
							{
							case Player01:
								spriteImgPiece[yCount][xCount].SetAngle(0);
								break;
							case Player02:
								spriteImgPiece[yCount][xCount].SetAngle(3.1415/1);
								break;;
							}

							switch (PP.PS[yCount][xCount].Promoted)
							{

							//成っていない
							case false:

								//その駒の種類は
								switch (PP.PS[yCount][xCount].Type)
								{
								//歩
								case Pawn:
									imgPiece[yCount][xCount].SetNum(0, 0);
									break;
								//香車
								case Lance:
									imgPiece[yCount][xCount].SetNum(2, 0);
									break;
								//桂馬
								case Knight:
									imgPiece[yCount][xCount].SetNum(4, 0);
									break;
								//銀将
								case SilverGeneral:
									imgPiece[yCount][xCount].SetNum(6, 0);
									break;
								//金将
								case GoldGeneral:
									imgPiece[yCount][xCount].SetNum(0, 1);
									break;
								//飛車
								case Rook:
									imgPiece[yCount][xCount].SetNum(2, 1);
									break;
								//角行
								case Bishop:
									imgPiece[yCount][xCount].SetNum(4, 1);
									break;
								//王,玉
								case King:
									//どちらの駒か
									switch (PP.PS[yCount][xCount].PiecesGOM)
									{
										//王
									case Player01:
										imgPiece[yCount][xCount].SetNum(6, 1);
										break;
										//玉
									case Player02:
										imgPiece[yCount][xCount].SetNum(7, 1);
										break;
									}
									break;
								}

								break;

							//成っている
							case true:

								//その駒の種類は
								switch (PP.PS[yCount][xCount].Type)
								{
								//と金
								case Pawn:
									imgPiece[yCount][xCount].SetNum(1, 0);
									break;
								//成香
								case Lance:
									imgPiece[yCount][xCount].SetNum(3, 0);
									break;
								//成桂
								case Knight:
									imgPiece[yCount][xCount].SetNum(5, 0);
									break;
								//成銀
								case SilverGeneral:
									imgPiece[yCount][xCount].SetNum(7, 0);
									break;
								//龍王（成車）
								case Rook:
									imgPiece[yCount][xCount].SetNum(3, 1);
									break;
								//竜馬（成角）
								case Bishop:
									imgPiece[yCount][xCount].SetNum(5, 1);
									break;
								}

								break;
							}

							//座標設定
							spriteImgPiece[yCount][xCount].SetPos
							((float)xCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0,
								(float)yCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0);

							//描画
							spriteImgPiece[yCount][xCount].Draw(imgPiece[yCount][xCount]);
						}
					}
				}

				//ウィンドウの描画
				if (WindowFlag == true)
				{
					spriteImgWindow.Draw(imgWindow);
				}

				//描画終了の合図
				d3d.EndScene();
				//バックバッファをフロントへ反映
				d3d.Present();
			}
		}
	}

	return 0;
};
