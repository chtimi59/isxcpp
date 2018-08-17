[Files]

Source: "..\bin\*.dll"; DestDir: "{app}\._unins000.isx"; Flags: ignoreversion replacesameversion recursesubdirs createallsubdirs;

[code]

// DLL SIGNATURES

procedure __isx_setuponly_Initialize(isInstall: bool; tmpPath, isxPath, lang: PAnsiChar);
external  'Initialize@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_Initialize(isInstall: bool; tmp, isx, lang: PAnsiChar);
external  'Initialize@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

procedure  __isx_setuponly_ClearProducts();
external  'ClearProducts@files:isx.dll stdcall setuponly';
procedure  __isx_uninstallonly_ClearProducts();
external  'ClearProducts@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_CreateProduct(ProductName: PAnsiChar): Integer;
external  'CreateProduct@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_CreateProduct(ProductName: PAnsiChar): Integer;
external  'CreateProduct@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

// ---

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
  isSetup: bool;
  isInitDone: bool;

function ISX_InitializeSetup(): Boolean;
{
  Initialize ISX
  Please perfoms this call on InnoSetup's 'InitializeSetup()'
}
begin
  Log('Initialize ISX in SETUP mode')
  if (isInitDone) then RaiseException('ISX Already initialized');
  isSetup := true;
  ExtractTemporaryFiles('{app}\._unins000.isx\*');
  __isx_setuponly_Initialize(isSetup, ExpandConstant('{tmp}'), ExpandConstant('{tmp}') + '\{app}\._unins000.isx\', ExpandConstant('{language}'));
  isInitDone := True;
  Result := True;
end;

function ISX_InitializeUninstall(): Boolean;
{
  Initialize ISX
  Please make it call on InnoSetup's 'InitializeUninstall()'
}
begin
  Log('Initialize ISX in UNINSTALL mode')
  if (isInitDone) then RaiseException('ISX Already initialized');
  isSetup := false;
  __isx_uninstallonly_Initialize(isSetup, ExpandConstant('{tmp}'), ExpandConstant('{app}\._unins000.isx'), ExpandConstant('{language}'));
  isInitDone := True;
  Result := True;
end;

procedure  ISX_ClearProducts();
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

// ---

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