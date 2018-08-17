#define MyAppName "testApp"
#define MyAppVersion "1.1.1"
#define MyAppURL "https://github.com"
#define MyAppPublisher "PublisherName"
#define OutputName "mysetup"
#define OutputPath "output"

#include "../isx/isx.iss"

[Setup]
AppId={{24E5B62B-2BD7-4B77-9E26-1C072865294A}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir={#OutputPath}
OutputBaseFilename={#OutputName}
Compression=lzma
SolidCompression=yes

DisableReadyPage=yes
DisableReadyMemo=no
DisableWelcomePage=yes
DisableDirPage=yes

[code]

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup();
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
    MemoDependenciesInfo: string;
    tmpPath: string;
    thirdPath: string;
    i: Integer;
begin

    tmpPath := ExpandConstant('{tmp}');
    thirdPath := tmpPath + '\3rdParty\';

    ISX_ClearProducts();

    i := ISX_CreateProduct('My Product1');
    i := ISX_CreateProduct('My Product2');

    MemoDependenciesInfo := ISX_GetReadyMemo(Space, NewLine);
    if MemoUserInfoInfo <> '' then
        Result := Result + MemoUserInfoInfo + NewLine + NewLine;
    if MemoDirInfo <> '' then
        Result := Result + MemoDirInfo + NewLine + NewLine;
    if MemoTypeInfo <> '' then
        Result := Result + MemoTypeInfo + NewLine + NewLine;
    if MemoComponentsInfo <> '' then
        Result := Result + MemoComponentsInfo + NewLine + NewLine;
    if MemoGroupInfo <> '' then
        Result := Result + MemoGroupInfo + NewLine + NewLine;
    if MemoTasksInfo <> '' then
        Result := Result + MemoTasksInfo + NewLine + NewLine;
    if MemoDependenciesInfo <> '' then
        Result := Result + MemoDependenciesInfo + NewLine + NewLine;
end;

function PrepareToInstall(var NeedsRestart: boolean): String;
begin
    ISX_Wait(2000);
    Result := ISX_Run();
end;