//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uGameBoard.h"
#include "uMainMenu.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormGame *FormGame;
//---------------------------------------------------------------------------
__fastcall TFormGame::TFormGame(TComponent* Owner)
	: TForm(Owner)
{
	game = new Game();
	hasRolled = true;
	indexPointSel = -2;
	turnStarted = false;
	myColour = ' ';
	isOnlineGame = false;
	inputBuffer = "";
	this->DoubleBuffered = true;
}
//---------------------------------------------------------------------------

void TFormGame::UpdateBoard() {
	const Board& board = game->getBoard();
	for(int i = 0; i < 24; ++i) {
		String compName = "BgPoint" + IntToStr(i + 1);
		TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));

		if (pnl) {
		   BgPoint p = board.getPoint(i);
		   bool isTop = (pnl->Top < pnl->Height);
		   pnl->Caption = "";

		   if (p.isEmpty()) {
			  DrawCheckers(pnl, 0, 'N', isTop);
		   }
		   else {
			  DrawCheckers(pnl, p.getNumber(), p.getColour(), isTop);
		   }
		}
	}

	for(int i = 0; i < 2; ++i) {
		String clr = (i == 1) ? "White" : "Black";
		String barName = "Bar" + clr;
		String homeName = "Home" + clr;
		TPanel* barPnl = static_cast<TPanel*>(FindComponent(barName));
		TPanel* homePnl = static_cast<TPanel*>(FindComponent(homeName));

		if(barPnl) {
			BgPoint p = board.getBarPoint(i);
			bool isTop = (barPnl->Top < barPnl->Height);
			barPnl->Caption = "";

			if(clr == "White") {
				DrawCheckers(barPnl, p.getNumber(), 'W', isTop);
			}
			else {
				DrawCheckers(barPnl, p.getNumber(), 'B', isTop);
			}

		}

		if(homePnl) {
			BgPoint p = board.getHomePoint(i);
			bool isTop = (homePnl->Top < homePnl->Height);
			homePnl->Caption = "";

			if(clr == "White") {
				DrawCheckers(homePnl, p.getNumber(), 'W', isTop);
			}
			else {
				DrawCheckers(homePnl, p.getNumber(), 'B', isTop);
			}
		}

	}

	Dice1->Caption = "";
	Dice2->Caption = "";
	Dice3->Caption = "";
	Dice4->Caption = "";

	std::vector<int> moves = game->getMoves();
	if(moves.size() > 0) {
		Dice1->Caption = IntToStr(moves.at(0));
	}
	if(moves.size() > 1) {
		Dice2->Caption = IntToStr(moves.at(1));
	}
	if(moves.size() > 2) {
		Dice3->Caption = IntToStr(moves.at(2));
	}
	if(moves.size() > 3) {
		Dice4->Caption = IntToStr(moves.at(3));
	}

	Player* pl1 = game->getPlayer(0);
	Player* pl2 = game->getPlayer(1);

	Player1Label->Caption = AnsiString(pl1->getName().c_str());
	Player2Label->Caption = AnsiString(pl2->getName().c_str());
	P1ColourLabel->Caption = (pl1->getColour() == 'W' ? "White" : "Black");
	P2ColourLabel->Caption = (pl2->getColour() == 'W' ? "White" : "Black");

	TurnCounterLabel->Caption = "Turns: " + IntToStr(game->getTurns()/2);

	WinP1Label->Caption = "Wins: " + IntToStr(game->getP1Wins());
	WinP2Label->Caption = "Wins: " + IntToStr(game->getP2Wins());

	char ct = game->getCurrentPlayer();
	if(ct == 'W') {
		Player1Label->StyleElements = Player1Label->StyleElements >> seFont;
		Player1Label->Font->Color = (TColor)0x0050A5FF;
		Player1Label->Font->Style = TFontStyles() << fsBold;

		Player2Label->StyleElements = Player2Label->StyleElements << seFont;
		Player2Label->Font->Style = TFontStyles();
	}
	else {
		Player2Label->StyleElements = Player2Label->StyleElements >> seFont;
		Player2Label->Font->Color = (TColor)0x0050A5FF;
		Player2Label->Font->Style = TFontStyles() << fsBold;

		Player1Label->StyleElements = Player1Label->StyleElements << seFont;
		Player1Label->Font->Style = TFontStyles();
	}

	if (game->hasGameEnded()) {
		BtnRollDice->Enabled = true;
		BtnRollDice->Caption = "New Round";
		if (isOnlineGame) {
			this->Enabled = true;
			if (game->getCurrentPlayer() ==	myColour) {
				consoleLabel->Caption = "You Won!";
			}
			else {
				consoleLabel->Caption = "You Lost!";
			}
		}
		BtnEndTurn->Enabled = false;
		if(ct == 'W') {
			Player1Label->StyleElements = Player1Label->StyleElements >> seFont;
			Player1Label->Font->Color = (TColor)0x0032CD32;
			Player1Label->Font->Style = TFontStyles() << fsBold;

			Player2Label->StyleElements = Player2Label->StyleElements << seFont;
			Player2Label->Font->Style = TFontStyles();
		}
		else {
			Player2Label->StyleElements = Player2Label->StyleElements >> seFont;
			Player2Label->Font->Color = (TColor)0x0032CD32;
			Player2Label->Font->Style = TFontStyles() << fsBold;

			Player1Label->StyleElements = Player1Label->StyleElements << seFont;
			Player1Label->Font->Style = TFontStyles();
		}
	}
	else if(hasRolled) {

		if (!isOnlineGame || myColour == game->getCurrentPlayer()) {
			BtnRollDice->Enabled = false;
			BtnRollDice->Caption = "Rolled Dice";

			if (moves.empty() || !game->hasValidMoves()) {
				BtnEndTurn->Enabled = true;
				BtnEndTurn->Caption = "End Turn";
				BtnEndTurn->Width = 89;
				BtnEndTurn->Left = 1008;
			}
			else {
				BtnEndTurn->Enabled = false;
				BtnEndTurn->Caption = "Moves Still Available";
				BtnEndTurn->Width = 119;
				BtnEndTurn->Left = 993;
			}	
		}
		else {
            BtnRollDice->Enabled = false;
			BtnRollDice->Caption = "Waiting...";

			if (moves.empty() || !game->hasValidMoves()) {
				BtnEndTurn->Enabled = false;
				BtnEndTurn->Caption = "Turn Ended";
				BtnEndTurn->Width = 89;
				BtnEndTurn->Left = 1008;
			}
			else {
				BtnEndTurn->Enabled = false;
				BtnEndTurn->Caption = "Turn Ongoing...";
				BtnEndTurn->Width = 119;
				BtnEndTurn->Left = 993;
			}
        }
	}
	else {
		BtnEndTurn->Enabled = false;
		BtnRollDice->Enabled = true;
		BtnRollDice->Caption = "Roll Dice";
	}
	indexPointSel = -2;
}

void TFormGame::HighlightMoves() {

	if(turnStarted && indexPointSel	!= -2 && game->hasValidMoves()) {
		bool moveFound = false;
		std::vector<int> moves = game->getMoves();
		const Board& board = game->getBoard();
		for(int i = 0; i < moves.size(); ++i) {
			int dieVal = moves.at(i);
			if (indexPointSel == -1) {

				int to = -1;
				if (game->getCurrentPlayer() == 'W') {
					to = dieVal - 1;
				}
				else {
					to = 24 - dieVal;
				}

				if(board.canPlace(game->getCurrentPlayer(), to)) {
					String compName = "BgPoint" + IntToStr(to + 1);
					TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));
					if (pnl) {
						pnl->ParentBackground = false;
						pnl->StyleElements = pnl->StyleElements >> seClient;
						pnl->Color = (TColor)0x0000D7FF;
					}
					highlightedPoints.push_back(to);
					moveFound = true;
				}
			}
			else {
				int direction = (game->getCurrentPlayer() == 'W') ? 1 : -1;
				int to = indexPointSel + (dieVal * direction);

				if(board.canMove(indexPointSel, to)) {
                    String compName = "BgPoint" + IntToStr(to + 1);
                    TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));
					if (pnl) {
						pnl->ParentBackground = false;
						pnl->StyleElements = pnl->StyleElements >> seClient;
						pnl->Color = (TColor)0x0000D7FF;
                    }
					highlightedPoints.push_back(to);
                    moveFound = true;
				}
				if(board.allCheckersInHome(game->getCurrentPlayer())) {

                    char clr = game->getCurrentPlayer();
					int dist = (clr == 'W') ? (24 - indexPointSel) : (indexPointSel + 1);
					bool canBearOffThis = false;

                    if (dieVal == dist) {
                        canBearOffThis = true;
					}
                    else if (dieVal > dist) {
                        bool isFurthest = true;

						if (clr == 'W') {
							for(int k = 18; k < indexPointSel; ++k) {
                                if(!board.getPoint(k).isEmpty() && board.getPoint(k).getColour() == clr) {
                                    isFurthest = false;
                                    break;
                                }
                            }
                        }
						else {
							for(int k = 5; k > indexPointSel; --k) {
                                if(!board.getPoint(k).isEmpty() && board.getPoint(k).getColour() == clr) {
									isFurthest = false;
									break;
                                }
                            }
						}
						if (isFurthest) {
							canBearOffThis = true;
						}
					}
                    if (canBearOffThis && board.canBearOff(clr, dieVal)) {
                        String homeName = (clr == 'W') ? "HomeWhite" : "HomeBlack";
                        TPanel* homePnl = static_cast<TPanel*>(FindComponent(homeName));
						if (homePnl) {
							homePnl->Color = (TColor)0x0000D7FF;
                        }
                        highlightedPoints.push_back(24);
                        moveFound = true;
                    }
				}
			}
		}

		highlightedPoints.push_back(indexPointSel);
		if (indexPointSel == -1) {
			String clr = (game->getCurrentPlayer() == 'W') ? "White" : "Black";
			String barName = "Bar" + clr;
			TPanel* barPnl = static_cast<TPanel*>(FindComponent(barName));
			if(barPnl) {
				if(moveFound) {
					barPnl->Color = (TColor)0x0032CD32;
				}
				else {
					barPnl->Color = (TColor)0x002222FF;

                }
            }
		}

		else {
			String compName = "BgPoint" + IntToStr(indexPointSel + 1);
			TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));
			if (pnl) {
				pnl->ParentBackground = false;
				pnl->StyleElements = pnl->StyleElements >> seClient;
				if(moveFound) {
					pnl->Color = (TColor)0x0032CD32;
				}
				else {
					pnl->Color = (TColor)0x002222FF;
				}
			}
		}

	}
}

void TFormGame::UnhighlightMoves() {
	if(!highlightedPoints.empty()) {
        const Board& board = game->getBoard();
		for(int i = 0; i < highlightedPoints.size(); ++i) {
			switch(highlightedPoints.at(i)) {
				case -2:
					break;
				case -1:
					if(game->getCurrentPlayer() == 'W') {
						String clr = "White";
						String barName = "Bar" + clr;
						TPanel* barPnl = static_cast<TPanel*>(FindComponent(barName));
						if (barPnl) {
						   barPnl->Color = (TColor)0x00233446;	
						}
					}
					else {
						String clr = "Black";
						String barName = "Bar" + clr;
						TPanel* barPnl = static_cast<TPanel*>(FindComponent(barName));
						if (barPnl) {
							barPnl->Color = (TColor)0x00233446;	
						}
					}
					break;
				case 24:
					if(game->getCurrentPlayer() == 'W') {
						String clr = "White";
						String homeName = "Home" + clr;
						TPanel* homePnl = static_cast<TPanel*>(FindComponent(homeName));
						if (homePnl) {
							homePnl->Color = (TColor)0x00182531;	
						}
					}
					else {
						String clr = "Black";
						String homeName = "Home" + clr;
						TPanel* homePnl = static_cast<TPanel*>(FindComponent(homeName));
						if (homePnl) {
							homePnl->Color = (TColor)0x00182531;
						}
					}
					break;
				default:
					String compName = "BgPoint" + IntToStr(highlightedPoints.at(i) + 1);
					TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));

					if (pnl) {
					   BgPoint p = board.getPoint(highlightedPoints.at(i));
					   pnl->ParentBackground = true;
					   pnl->StyleElements = pnl->StyleElements << seClient;
					}
			}
		}
		highlightedPoints.clear();
	}
}

bool TFormGame::isSameColour(int index) {
	const Board& board = game->getBoard();
	BgPoint pnt = board.getPoint(index);
	return game->getCurrentPlayer() == pnt.getColour();
}

//--------------------------------------------------------------------------

void TFormGame::DrawCheckers(TPanel* pnl, int count, char clr, bool isTopRow) {
	for (int i = pnl->ControlCount - 1; i >= 0; --i) {
		if(dynamic_cast<TShape*>(pnl->Controls[i])) {
			delete pnl->Controls[i];
		}
	}

	if (count <= 0) return;

	int size = pnl->Width - 4;
	int height = pnl->Height;
	int step = size + 1;

	if (count > 1) {
		int availableSpace = height - size;
		int maxStep = availableSpace / (count - 1);

		if (step > maxStep) {
			step = maxStep;
		}
	}
	
	for (int i = 0; i < count; ++i) {
		TShape* checker = new TShape(pnl);
		checker->Parent = pnl;
		checker->Shape = stCircle;
		checker->Align = alNone;
		checker->Width = size;
		checker->Height	= size;
		checker->Enabled = false;

		if (clr == 'W') {
			checker->Brush->Color = clWhite;
			checker->Pen->Color = clGray;
		}
		else {
			checker->Brush->Color = clBlack;
			checker->Pen->Color = clWhite;
		}

		int leftPos = 2;
		int topPos = 0;
		
		if (isTopRow) {
			topPos = 2 + (i * step);
		}
		else {
			topPos = (height - size - 2) - (i * step);
		}

		checker->SetBounds(leftPos, topPos, size, size);
		checker->BringToFront();
	}

}

void TFormGame::InitBoardGUI() {	
	for (int i = 0; i < 24; ++i) {
		String compName = "BgPoint" + IntToStr(i + 1);
		TPanel* pnl = static_cast<TPanel*>(FindComponent(compName));

		if (pnl) {
			pnl->DoubleBuffered = true;
			TPaintBox* pb = new TPaintBox(pnl);
			pb->Parent = pnl;
			pb->Align = alClient;
			pb->OnPaint	= DrawTriangle;

			pb->Enabled	= false;
			pb->SendToBack();	
		}
	}

	BarWhite->DoubleBuffered = true;
	BarBlack->DoubleBuffered = true;
	HomeWhite->DoubleBuffered = true;
    HomeBlack->DoubleBuffered = true;
	
	BarWhite->ParentBackground = false;
	BarWhite->StyleElements = BarWhite->StyleElements >> seClient;
	BarBlack->ParentBackground = false;
	BarBlack->StyleElements = BarBlack->StyleElements >> seClient;
	HomeWhite->ParentBackground = false;
	HomeWhite->StyleElements = HomeWhite->StyleElements >> seClient;
	HomeBlack->ParentBackground = false;
	HomeBlack->StyleElements = HomeBlack->StyleElements >> seClient;
	
	BarWhite->Color = (TColor)0x00233446;
    BarBlack->Color = (TColor)0x00233446;
	HomeWhite->Color = (TColor)0x00182531;
	HomeBlack->Color = (TColor)0x00182531;

	BarWhite->BevelOuter = bvNone;
	BarBlack->BevelOuter = bvNone;
	HomeWhite->BevelOuter = bvNone;
	HomeBlack->BevelOuter = bvNone;
}

void __fastcall TFormGame::DrawTriangle(TObject *Sender) {
	TPaintBox* pb = static_cast<TPaintBox*>(Sender);
	TPanel* pnl = static_cast<TPanel*>(pb->Parent);
	int w = pb->Width;
	int h = pb->Height;
	TCanvas* c = pb->Canvas;

	String numStr = pnl->Name.SubString(8, pnl->Name.Length());
	int index = StrToIntDef(numStr, 1) - 1;

	if (index % 2 == 0) {
		c->Brush->Color	= (TColor)0x00A0C5E6;
	}
	else {
		c->Brush->Color = (TColor)0x005E8CA6;
	}

	c->Pen->Style = psClear;

	TPoint points[3];

	if (index >= 12) {
		points[0] = Point(0, 0);
		points[1] = Point(w, 0);
		points[2] = Point(w/2, h);    
	}
	else {
		points[0] = Point(0, h);
		points[1] = Point(w, h);
		points[2] = Point(w/2, 0);
	}
	c->Polygon(points, 2);	
}
//---------------------------------------------------------------------------

void TFormGame::ConnectToServer(String ip) {
	game->isOnl = true;
	isOnlineGame = true;
    csClient->Address = ip;
	csClient->Port = 2000;
	csClient->Active = true;
}

void TFormGame::SendNetworkMessage(String msg) {
	if (isOnlineGame && csClient->Active) {
		csClient->Socket->SendText(msg + "@");
	}
}

void TFormGame::ProcessNetworkCommand(String cmd) {
	if (cmd.Pos("ID") == 1) {

		if (cmd.Pos("W")) {
			myColour = 'W';
			P1ColourLabel->StyleElements = P1ColourLabel->StyleElements >> seFont;
			P1ColourLabel->Font->Color = (TColor)0x0032CD32;
		}
		else {
			myColour = 'B';
			P2ColourLabel->StyleElements = P2ColourLabel->StyleElements >> seFont;
			P2ColourLabel->Font->Color = (TColor)0x0032CD32;
			String myName = game->getPlayer(0)->getName().c_str();
			std::string pName = game->getPlayer(0)->getName().c_str();
			game->setPlayerName(1, pName);
			SendNetworkMessage("NAME " + myName);
		}
		UpdateBoard();
	}

	else if (cmd.Pos("NAME") == 1) {
		std::wstring ws = cmd.SubString(6, cmd.Length()).c_str();
        std::string oppName(ws.begin(), ws.end());
		if (myColour == 'W') {
			game->setPlayerName(1, oppName);
			String myName = game->getPlayer(0)->getName().c_str();
			SendNetworkMessage("NAME " + myName);
		}
		else {
			game->setPlayerName(0, oppName);
		}
		UpdateBoard();
	}

	else if (cmd.Pos("ROLL") == 1) {
		int val1, val2;
		AnsiString aCmd = cmd;
		sscanf(aCmd.c_str(), "ROLL %d %d", &val1, &val2);
		if (game->getCurrentPlayer() == myColour && game->getTurns() > 1) {
			 game->nextTurn();
		}
		game->onlineRoll(val1, val2);
		hasRolled = true;
		UpdateBoard();
	}

	else if (cmd.Pos("MOVE") == 1) {
		int from, to;
		AnsiString aCmd = cmd;
		sscanf(aCmd.c_str(), "MOVE %d %d", &from, &to);
		game->attemptMove(from, to);
		UpdateBoard();
	}

	else if (cmd == "TURN_END") {
		game->nextTurn();
		hasRolled = false;
		turnStarted = false;
		UpdateBoard();
		this->Enabled = true;
		consoleLabel->Caption =	"Your Turn!";
	}

	else if (cmd == "START") {
		game->start();
        UpdateBoard();
		this->Enabled = false;
		char val = game->getCurrentPlayer();
		SendNetworkMessage("SET_TURN " + String(val));
		int val1 = game->getDice(0)->getValue();
		int val2 = game->getDice(1)->getValue();
		SendNetworkMessage("ROLL " + IntToStr(val1) + " " + IntToStr(val2));
		if (myColour == game->getCurrentPlayer()) {
			this->Enabled = true;
			if (consoleLabel) {
				consoleLabel->Caption = "Game Started! Your Turn!";
			}
		} else {
			BtnRollDice->Enabled = false;
			if (consoleLabel) {
				consoleLabel->Caption = "Game Started!";
			}
		}
	}
	else if (cmd.Pos("SET_TURN") == 1) {
		game->start();
        this->Enabled = false;
		game->stop();
		char val;
		AnsiString aCmd = cmd;
		sscanf(aCmd.c_str(), "SET_TURN %c", &val);
		game->setTurn(val);
		if (myColour == game->getCurrentPlayer()) {
			this->Enabled = true;
			if (consoleLabel) {
				consoleLabel->Caption = "Game Started! Your Turn!";
			}
		} else {
			BtnRollDice->Enabled = false;
			if (consoleLabel) {
				consoleLabel->Caption = "Game Started!";
			}
		}
		UpdateBoard();
	}
	else if (cmd == "DISCONNECT") {
		ShowMessage("Opponent Disconnected! Returning to Main Menu.");
		this->Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TFormGame::FormShow(TObject *Sender)
{
	InitBoardGUI();
	UpdateBoard();
	if(hasRolled) {
		turnStarted = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormGame::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (csClient->Active) {
		csClient->Close();
	}
	FormMainMenu->Show();
	FormMainMenu->Repaint();
	FormMainMenu->Update();
	Application->ProcessMessages();
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormGame::BtnRollDiceClick(TObject *Sender)
{

	if (isOnlineGame && game->getCurrentPlayer() != myColour && !game->hasGameEnded()) {
        return;
	}

	if (game->hasGameEnded()) {
		if(isOnlineGame) {
			game->start();
			hasRolled = true;
			turnStarted = true;
			this->Enabled = false;
			char val = game->getCurrentPlayer();
			SendNetworkMessage("SET_TURN " + String(val));
			int val1 = game->getDice(0)->getValue();
			int val2 = game->getDice(1)->getValue();
			SendNetworkMessage("ROLL " + IntToStr(val1) + " " + IntToStr(val2));
			if (myColour == game->getCurrentPlayer()) {
				this->Enabled = true;
				if (consoleLabel) {
					consoleLabel->Caption = "Game Started! Your Turn!";
				}
			} else {
				BtnRollDice->Enabled = false;
				if (consoleLabel) {
					consoleLabel->Caption = "Game Started!";
				}
			}
			UpdateBoard();
		  return;
		}
		else {
			game->start();
			hasRolled = true;
			turnStarted = true;
			UpdateBoard();
			return;
        }
	}

	OnlineDice* d1 = dynamic_cast<OnlineDice*>(game->getDice(0));
	OnlineDice* d2 = dynamic_cast<OnlineDice*>(game->getDice(1));
	if (d1) d1->setFixedValue(0);
	if (d2) d2->setFixedValue(0);

	game->rollDice();
	hasRolled = true;
	UpdateBoard();
	turnStarted = true;

	if (isOnlineGame) {
		int val1 = game->getDice(0)->getValue();
		int val2 = game->getDice(1)->getValue();
		SendNetworkMessage("ROLL " + IntToStr(val1) + " " + IntToStr(val2));
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::BtnEndTurnClick(TObject *Sender)
{

	if (isOnlineGame && game->getCurrentPlayer() != myColour) {
		return;
	}

	turnStarted = false;
	game->nextTurn();
	hasRolled = false;
	UpdateBoard();

	if (isOnlineGame) {
        SendNetworkMessage("TURN_END");
		this->Enabled = false;
		consoleLabel->Caption = "Turn Ended!";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::BgPoint1Click(TObject *Sender)
{

    if (isOnlineGame && game->getCurrentPlayer() != myColour) {
		return;
	}

	TPanel* pnl = nullptr;

	if (dynamic_cast<TPanel*>(Sender)) {
		pnl = static_cast<TPanel*>(Sender);
	}

	if (!pnl) return;

	String name = pnl->Name;
	String numStr = name.SubString(8, name.Length());
	int clickedIndex = StrToInt(numStr) - 1;

	if(indexPointSel == -2 && isSameColour(clickedIndex)) {
		indexPointSel = clickedIndex;
		HighlightMoves();
	}
	else if (indexPointSel == clickedIndex) {
		UnhighlightMoves();
		indexPointSel = -2;
	}
	else if(indexPointSel != -2) {
		bool success = game->attemptMove(indexPointSel, clickedIndex);
		if (success) {
			if (isOnlineGame) {
				SendNetworkMessage("MOVE " + IntToStr(indexPointSel) + " " + IntToStr(clickedIndex));
			}
			UnhighlightMoves();
			UpdateBoard();
			indexPointSel = -2;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::BarBlackClick(TObject *Sender)
{

    if (isOnlineGame && game->getCurrentPlayer() != myColour) {
        return;
	}

	TPanel* barPnl = static_cast<TPanel*>(Sender);
	String name = barPnl->Name;
	String clrStr = name.SubString(4, name.Length());
	char clr = (clrStr == "White") ? 'W' : 'B';
	if (game->getCurrentPlayer() == clr) {
		int clickedIndex = -1;

		if(indexPointSel == -2) {
			indexPointSel = clickedIndex;
			HighlightMoves();
		}
		else if (indexPointSel == clickedIndex) {
			UnhighlightMoves();
			indexPointSel = -2;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::HomeWhiteClick(TObject *Sender)
{

	if (isOnlineGame && game->getCurrentPlayer() != myColour) {
        return;
	}

	TPanel* homePnl = static_cast<TPanel*>(Sender);
	String name = homePnl->Name;
	String clrStr = name.SubString(5, name.Length());
	char clr = (clrStr == "White") ? 'W' : 'B';
	int clickedIndex = 24;

	if (game->getCurrentPlayer() == clr) {
		bool success = game->attemptMove(indexPointSel, clickedIndex);
		if (success) {
			if (isOnlineGame) {
				SendNetworkMessage("MOVE " + IntToStr(indexPointSel) + " " + IntToStr(clickedIndex));
			}
			UnhighlightMoves();
			UpdateBoard();
			indexPointSel = -2;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::csClientConnect(TObject *Sender, TCustomWinSocket *Socket)

{
	consoleLabel->Caption =	"Connected to Server!";
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::csClientRead(TObject *Sender, TCustomWinSocket *Socket)

{
	String newData = Socket->ReceiveText();
	inputBuffer += newData;
	int delimiterPos;
	while ((delimiterPos = inputBuffer.Pos("@")) > 0) {
		String singleCmd = inputBuffer.SubString(1, delimiterPos - 1);
		inputBuffer = inputBuffer.Delete(1, delimiterPos);
        if (!singleCmd.IsEmpty()) {
			ProcessNetworkCommand(singleCmd);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGame::csClientError(TObject *Sender, TCustomWinSocket *Socket,
		  TErrorEvent ErrorEvent, int &ErrorCode)
{
	consoleLabel->Caption =	"Connection failed. Running offline.";
	ErrorCode = 0;
	isOnlineGame = false;
	game->isOnl = false;
	UpdateBoard();
}
//---------------------------------------------------------------------------


