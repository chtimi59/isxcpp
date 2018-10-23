#define MyAppName "testApp"
#define MyAppVersion "1.1.1"
#define MyAppURL "https://github.com"
#define MyAppPublisher "PublisherName"
#define OutputName "mysetup"
#define OutputPath "output"

#include "isx/isx.iss"

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

{
   read JSON from 'http://localhost:8081/json'
   and push back an altered object to 'http://localhost:8081/api'
}
procedure testRestJson();
var
  pCode1, pCode2: Integer;
  hJson, hArray: Integer;
  data, str: AnsiString;
  number: Integer;
  float: Single;
  boolean: Boolean;
begin
  
  { MsgBox('Attach me!', mbInformation, MB_OK); }

  { Get DATA }
  data := ISX_HttpGet('http://localhost:8081/json', pCode1);
  Log(IntToStr(pCode1));

  { Parse it into JSON }
  if (not ISX_JsonParse(data, hJson)) then exit;

  { object.doubleit = 2 * object.number (integer) }
  if (not ISX_JsonInt(hJson, 'object.number', 0, number)) then exit;
  number := 2*number;
  if (not ISX_JsonInt(hJson, 'object.doubleit', 1, number)) then exit;

  { object.divideit = object.number / 2 (float) }
  if (not ISX_JsonFloat(hJson, 'object.number', 0, float)) then exit;
  float := float / 2;
  if (not ISX_JsonFloat(hJson, 'object.divideit', 1, float)) then exit;
  
  { object.greeting = 'Hello InnoSetup !' }
  str := 'Hello InnoSetup !';
  if (not ISX_JsonString(hJson, 'object.InnoSetup', 1, str)) then exit;

  { Get object.array }
  if (not ISX_JsonObj(hJson, 'object.array', 0, hArray)) then exit;
  { Add object.arraysize }
  number := ISX_JsonSize(hArray);
  ISX_JsonInt(hJson, 'object.arraysize', 1, number);
  { array[1] = array[1] + '!!' }  
  if (not ISX_JsonStringFromIdx(hArray, 1, 0, str)) then exit;
  str := str + ' !!'
  if (not ISX_JsonStringFromIdx(hArray, 1, 1, str)) then exit;

  { Bring back object.array in main JSON Object }
  if (not ISX_JsonObj(hJson, 'object.array', 1, hArray)) then exit;

  { Stringify main JSON Object }
  if (not ISX_JsonStringify(hJson, data)) then exit;
  
  { Post back altered Object }
  ISX_HttpPost('http://localhost:8081/api', 'application/json', data, pCode2);
  Log(IntToStr(pCode2));
end;

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
  testRestJson();
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