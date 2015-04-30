; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCoserverDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "coserver.h"

ClassCount=3
Class1=CCoserverApp
Class2=CCoserverDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COSERVER_DIALOG

[CLS:CCoserverApp]
Type=0
HeaderFile=coserver.h
ImplementationFile=coserver.cpp
Filter=N

[CLS:CCoserverDlg]
Type=0
HeaderFile=coserverDlg.h
ImplementationFile=coserverDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CCoserverDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=coserverDlg.h
ImplementationFile=coserverDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COSERVER_DIALOG]
Type=1
Class=CCoserverDlg
ControlCount=8
Control1=ID_START,button,1342242817
Control2=ID_STOP,button,1342242816
Control3=IDC_STATIC,static,1342308865
Control4=IDC_PORT,edit,1350631552
Control5=IDC_STATIC,static,1342308865
Control6=IDC_RECV,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_ACK,edit,1350631552

