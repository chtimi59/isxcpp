#include "isx/isx.iss"

[Setup]
AppName=testApp
AppVersion=1.0.0
DefaultDirName={pf}\testApp

DisableReadyPage=yes
DisableReadyMemo=no
DisableWelcomePage=yes
DisableDirPage=yes

[code]

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
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
    ISX_AddDeleteTask(i, 'C:\\dev\\isxcpp\\sdf\\\\\\\\      ', true);
    ISX_AddExecuteTask(i, 'c:\\dev', 'notepad.exe', '', true);

    i := ISX_CreateProduct('My Product2');
    ISX_AddFakeTask(i, '2.1');
    ISX_AddFakeTask(i, '2.2');

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
    ISX_Wait(10); // for test
    Result := ISX_Run();
end;

function InitializeUninstall(): Boolean;
begin
  Result := ISX_InitializeUninstall(false);
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
    i: Integer;
begin
  case CurUninstallStep of
    usUninstall: begin
        i := ISX_CreateProduct('My Product1');
        ISX_AddDeleteTask(i, 'C:\\dev\\isxcpp\\sdf\\\\\\\\      ', false);

        i := ISX_CreateProduct('My Product2');
        ISX_AddFakeTask(i, '2.1');
        ISX_AddFakeTask(i, '2.2');

        ISX_Run();
        ISX_Terminate();
    end;
    
  end;
end;