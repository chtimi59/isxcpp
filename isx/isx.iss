[Files]

; provide isx for uninstall
Source: "isx\bin\*"; DestDir: "{app}\._unins000.isx"; Flags: ignoreversion replacesameversion recursesubdirs createallsubdirs;

; don't provide isx for uninstall
; Source: "isx\bin\*"; DestDir: "{app}\._unins000.isx"; Flags: dontcopy 

[code]

// DLL SIGNATURES

procedure __isx_setuponly_Initialize(isInstall, isQuiet: Boolean; tmpPath, lang: PAnsiChar);
external  'Initialize@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_Initialize(isInstall, isQuiet: Boolean; tmpPath, lang: PAnsiChar);
external  'Initialize@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure  __isx_setuponly_ClearProducts();
external  'ClearProducts@files:isx.dll stdcall setuponly';
procedure  __isx_uninstallonly_ClearProducts();
external  'ClearProducts@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_CreateProduct(ProductName: PAnsiChar): Integer;
external  'CreateProduct@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_CreateProduct(ProductName: PAnsiChar): Integer;
external  'CreateProduct@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure __isx_setuponly_AddDownloadTask(ProductIndex: Integer; url, dst: PAnsiChar);
external  'AddDownloadTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddDownloadTask(ProductIndex: Integer; url, dst: PAnsiChar);
external  'AddDownloadTask@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure __isx_setuponly_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar);
external  'AddExecuteTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar);
external  'AddExecuteTask@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure __isx_setuponly_AddUnZipTask(ProductIndex: Integer; path, dst: PAnsiChar; clear: Bool);
external  'AddUnZipTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddUnZipTask(ProductIndex: Integer; path, dst: PAnsiChar; clear: Bool);
external  'AddUnZipTask@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure __isx_setuponly_AddDeleteTask(ProductIndex: Integer; path: PAnsiChar; exitIfFail: Boolean);
external  'AddDeleteTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddDeleteTask(ProductIndex: Integer; path: PAnsiChar; exitIfFail: Boolean);
external  'AddDeleteTask@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure __isx_setuponly_AddFakeTask(ProductIndex: Integer; name: PAnsiChar);
external  'AddFakeTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddFakeTask(ProductIndex: Integer; name: PAnsiChar);
external  'AddFakeTask@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_GetReadyMemo(Space, NewLine: PAnsiChar): PAnsiChar;
external  'GetReadyMemo@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_GetReadyMemo(Space, NewLine: PAnsiChar): PAnsiChar;
external  'GetReadyMemo@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_Run(hWnd: Integer; matchPrepareToInstallPage: Boolean): PAnsiChar;
external  'Run@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_Run(hWnd: Integer; matchPrepareToInstallPage: Boolean): PAnsiChar;
external  'Run@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure  __isx_setuponly_Wait(ms: Integer);
external  'Wait@files:isx.dll stdcall setuponly';
procedure  __isx_uninstallonly_Wait(ms: Integer);
external  'Wait@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

// PUBLIC DEFINITIONS

var
  isSetup: Boolean;
  isInitDone: Boolean;

function ISX_InitializeSetup(isQuiet: Boolean): Boolean;
{
  Initialize ISX
  Please perfoms this call on InnoSetup's 'InitializeSetup()'
}
begin
  Log('Initialize ISX in SETUP mode')
  if (isInitDone) then RaiseException('ISX Already initialized');
  isSetup := true;
  ExtractTemporaryFiles('{app}\._unins000.isx\*');
  __isx_setuponly_Initialize(isSetup, isQuiet, ExpandConstant('{language}'), ExpandConstant('{tmp}'));
  isInitDone := true;
  Result := true;
end;

function ISX_InitializeUninstall(isQuiet: Boolean): Boolean;
{
  Initialize ISX
  Please make it call on InnoSetup's 'InitializeUninstall()'
}
begin
  Log('Initialize ISX in UNINSTALL mode')
  if (isInitDone) then RaiseException('ISX Already initialized');
  isSetup := false;
  __isx_uninstallonly_Initialize(isSetup, isQuiet, ExpandConstant('{language}'), ExpandConstant('{tmp}'));
  isInitDone := true;
  Result := true;
end;

procedure ISX_ClearProducts();
{
  Clear Product list
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_ClearProducts();
  end else begin 
    __isx_uninstallonly_ClearProducts();
  end;
end;

function  ISX_CreateProduct(ProductName: PAnsiChar): Integer;
{
  Add a product in ISX List
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    Result := __isx_setuponly_CreateProduct(ProductName);
  end else begin 
    Result := __isx_uninstallonly_CreateProduct(ProductName);
  end;
end;

procedure ISX_AddDownloadTask(ProductIndex: Integer; url, dst: PAnsiChar);
{
  Add a Download Task to a product
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddDownloadTask(ProductIndex, url, dst);
  end else begin 
    __isx_uninstallonly_AddDownloadTask(ProductIndex, url, dst);
  end;
end;

procedure ISX_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar);
{
  Add a Execute Task to a product
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddExecuteTask(ProductIndex, workingDirectory, command, arguments);
  end else begin 
    __isx_uninstallonly_AddExecuteTask(ProductIndex, workingDirectory, command, arguments);
  end;
end;

procedure ISX_AddUnZipTask(ProductIndex: Integer; path, dst: PAnsiChar; clear: Bool);
{
  Add an UnZip Task to a product
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddUnZipTask(ProductIndex, path, dst, clear);
  end else begin 
    __isx_uninstallonly_AddUnZipTask(ProductIndex, path, dst, clear);
  end;
end;

procedure ISX_AddDeleteTask(ProductIndex: Integer; path: PAnsiChar; exitIfFail: Boolean);
{
  Add a Delete Task to a product 
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddDeleteTask(ProductIndex, path, exitIfFail);
  end else begin 
    __isx_uninstallonly_AddDeleteTask(ProductIndex, path, exitIfFail);
  end;
end;


procedure ISX_AddFakeTask(ProductIndex: Integer; name: PAnsiChar);
{
  Add a Fake Task (it's only a timed progress bar) to a product 
  NOTE: Used for test 
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddFakeTask(ProductIndex, name);
  end else begin 
    __isx_uninstallonly_AddFakeTask(ProductIndex, name);
  end;
end;

function ISX_GetReadyMemo(Space, NewLine: PAnsiChar): PAnsiChar;
{
  Return the InnoSetup Memo string which is a digest of
  all operations which has been scheduled
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    Result := __isx_setuponly_GetReadyMemo(Space, NewLine);
  end else begin 
    Result := __isx_uninstallonly_GetReadyMemo(Space, NewLine);
  end;
end;

function ISX_Run(): PAnsiChar;
{
  Do sequential all tasks associated to all products
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then
    begin 
    Result := __isx_setuponly_Run(StrToInt(ExpandConstant('{wizardhwnd}')), true);
    end
  else
    begin 
    Result := __isx_uninstallonly_Run(0, false);
    end;
end;

procedure ISX_Wait(ms: Integer);
{
  Make current thread wait for a delay in millisecond
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then
    begin 
    __isx_setuponly_Wait(ms);
    end
  else
    begin 
    __isx_uninstallonly_Wait(ms);
    end;
end;

procedure ISX_Terminate();
{
  Terminate ISX to delete it properly
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then
    begin 
      UnloadDLL(ExpandConstant('{tmp}') + '\isx.dll');
   end
   else
    begin 
      UnloadDLL(ExpandConstant('{app}\._unins000.isx') + '\isx.dll');
   end;
end;