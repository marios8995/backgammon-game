//---------------------------------------------------------------------------

#ifndef bgServerFormH
#define bgServerFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Win.ScktComp.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMemo *mLog;
	TServerSocket *ssServer;
	void __fastcall ssServerClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ssServerClientRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ssServerClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);


private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
