; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCoclientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "coclient.h"

ClassCount=3
Class1=CCoclientApp
Class2=CCoclientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COCLIENT_DIALOG

[CLS:CCoclientApp]
Type=0
HeaderFile=coclient.h
ImplementationFile=coclient.cpp
Filter=N

[CLS:CCoclientDlg]
Type=0
HeaderFile=coclientDlg.h
ImplementationFile=coclientDlg.cpp
Filter=D
LastObject=IDC_IPADDRESS
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=coclientDlg.h
ImplementationFile=coclientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COCLIENT_DIALOG]
Type=1
Class=CCoclientDlg
ControlCount=11
Control1=IDC_CONNECT,button,1342242816
Control2=IDC_DISC,button,1342242816
Control3=IDC_STATIC,static,1342308865
Control4=IDC_PORT,edit,1350631552
Control5=IDC_STATIC,static,1342308865
Control6=IDC_SENDATA,edit,1350631552
Control7=IDC_SEND,button,1342242816
Control8=IDC_STATIC,static,1342308865
Control9=IDC_ACK,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_IPADDRESS,SysIPAddress32,1342242816

