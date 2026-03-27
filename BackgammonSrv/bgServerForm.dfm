object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object mLog: TMemo
    Left = 0
    Top = 0
    Width = 624
    Height = 441
    Align = alClient
    TabOrder = 0
  end
  object ssServer: TServerSocket
    Active = False
    Port = 2000
    ServerType = stNonBlocking
    OnClientConnect = ssServerClientConnect
    OnClientDisconnect = ssServerClientDisconnect
    OnClientRead = ssServerClientRead
    Top = 200
  end
end
