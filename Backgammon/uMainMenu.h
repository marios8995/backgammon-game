//---------------------------------------------------------------------------

#ifndef uMainMenuH
#define uMainMenuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TFormMainMenu : public TForm
{
__published:	// IDE-managed Components
	TLabel *TitleLabel;
	TLabel *NameInputLabel;
	TEdit *NameInput;
	TButton *BtnExit;
	TButton *BtnPlay;
	TImage *Image1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *NameInput2;
	void __fastcall BtnPlayClick(TObject *Sender);
	void __fastcall BtnExitClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMainMenu(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMainMenu *FormMainMenu;
//---------------------------------------------------------------------------
#endif
