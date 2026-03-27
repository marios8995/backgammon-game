//---------------------------------------------------------------------------

#ifndef uGameBoardH
#define uGameBoardH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Win.ScktComp.hpp>
#include "game.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <System.Win.ScktComp.hpp>

//---------------------------------------------------------------------------
class TFormGame : public TForm
{
__published:	// IDE-managed Components
	TPanel *BgPoint13;
	TPanel *BgPoint22;
	TPanel *BgPoint23;
	TPanel *BgPoint24;
	TPanel *BgPoint14;
	TPanel *BgPoint15;
	TPanel *BgPoint16;
	TPanel *BgPoint17;
	TPanel *BgPoint18;
	TPanel *BgPoint19;
	TPanel *BgPoint20;
	TPanel *BgPoint21;
	TPanel *BgPoint12;
	TPanel *BgPoint3;
	TPanel *BgPoint2;
	TPanel *BgPoint1;
	TPanel *BgPoint11;
	TPanel *BgPoint10;
	TPanel *BgPoint9;
	TPanel *BgPoint8;
	TPanel *BgPoint7;
	TPanel *BgPoint6;
	TPanel *BgPoint5;
	TPanel *BgPoint4;
	TPanel *BarBlack;
	TPanel *BarWhite;
	TPanel *HomeBlack;
	TPanel *HomeWhite;
	TLabel *Player2Label;
	TLabel *Player1Label;
	TLabel *WinP1Label;
	TLabel *WinP2Label;
	TLabel *Dice1;
	TLabel *Dice2;
	TLabel *P1ColourLabel;
	TLabel *P2ColourLabel;
	TLabel *TurnCounterLabel;
	TLabel *Dice4;
	TLabel *Dice3;
	TButton *BtnEndTurn;
	TButton *BtnRollDice;
	TImage *Image1;
	TImage *Image2;
	TClientSocket *csClient;
	TLabel *consoleLabel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnRollDiceClick(TObject *Sender);
	void __fastcall BtnEndTurnClick(TObject *Sender);
	void __fastcall BgPoint1Click(TObject *Sender);
	void __fastcall BarBlackClick(TObject *Sender);
	void __fastcall HomeWhiteClick(TObject *Sender);
	void __fastcall csClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall csClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall csClientError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
private:	// User declarations
	bool hasRolled;
	int indexPointSel;
	bool turnStarted;
	bool isSameColour(int index);
	std::vector<int> highlightedPoints;
	void UpdateBoard();
	void HighlightMoves();
	void UnhighlightMoves();
	void DrawCheckers(TPanel* pnl, int count, char clr, bool isTopRow);
	void __fastcall DrawTriangle(TObject *Sender);
	void InitBoardGUI();
	char myColour;
	bool isOnlineGame;
	void ProcessNetworkCommand(String cmd);
	void SendNetworkMessage(String msg);
    String inputBuffer;
public:		// User declarations
	__fastcall TFormGame(TComponent* Owner);
	Game* game;
    void ConnectToServer(String ip);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGame *FormGame;
//---------------------------------------------------------------------------
#endif
