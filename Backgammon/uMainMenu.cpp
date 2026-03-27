//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMainMenu.h"
#include "uGameBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMainMenu *FormMainMenu;
//---------------------------------------------------------------------------
__fastcall TFormMainMenu::TFormMainMenu(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMainMenu::BtnPlayClick(TObject *Sender)
{

    FormGame = new TFormGame(this);

    FormGame->ConnectToServer("127.0.0.1");
	AnsiString vclNameP1 = NameInput->Text;
	std::string stdNameP1 = vclNameP1.c_str();
	AnsiString vclNameP2 = NameInput2->Text;
	std::string stdNameP2 = vclNameP2.c_str();
	FormGame->game->setPlayerName(0, stdNameP1);
	FormGame->game->setPlayerName(1, stdNameP2);
	FormGame->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormMainMenu::BtnExitClick(TObject *Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

