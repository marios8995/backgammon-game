//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "bgServerForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	ssServer->Port = 2000;
	ssServer->Active = true;
	mLog->Lines->Add("Server started on Port 2000");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ssServerClientConnect(TObject *Sender, TCustomWinSocket *Socket)

{
    mLog->Lines->Add("Client connected: " + Socket->RemoteAddress);
	int count = ssServer->Socket->ActiveConnections;

	if (count == 1) {
		Socket->SendText("ID W@");
		mLog->Lines->Add("White->Client1");
	}
	else if (count == 2) {
		Socket->SendText("ID B@");
		mLog->Lines->Add("Black->Client2");
		ssServer->Socket->Connections[0]->SendText("START@");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ssServerClientRead(TObject *Sender, TCustomWinSocket *Socket)

{
    String msg = Socket->ReceiveText();
	mLog->Lines->Add("Received: " + msg);

	for (int i = 0; i < ssServer->Socket->ActiveConnections; ++i) {
		TCustomWinSocket* client = ssServer->Socket->Connections[i];

		if(client != Socket) {
            client->SendText(msg);
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ssServerClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)

{
	mLog->Lines->Add("Client disconnected: " + Socket->RemoteAddress);

	for (int i = 0; i < ssServer->Socket->ActiveConnections; ++i) {
		TCustomWinSocket* client = ssServer->Socket->Connections[i];
		if (client && client->Connected) {
			 client->SendText("DISCONNECT@");
		}
	}
}
//---------------------------------------------------------------------------
