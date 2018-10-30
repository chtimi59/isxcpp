[Files]

; provide isx for uninstall
Source: "isx\bin\*"; DestDir: "{app}\._unins000.isx"; Flags: ignoreversion replacesameversion recursesubdirs createallsubdirs;

; don't provide isx for uninstall
; Source: "isx\bin\*"; DestDir: "{app}\._unins000.isx"; Flags: dontcopy 

[code]

// DLL SIGNATURES

function  __isx_setuponly_GetStringFromPtr(ptr: Cardinal): PAnsiChar;
external  'PassThrought@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_GetStringFromPtr(ptr: Cardinal): PAnsiChar;
external  'PassThrought@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_GetPtrFromString(str: PAnsiChar): Cardinal;
external  'PassThrought@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_GetPtrFromString(str: PAnsiChar): Cardinal;
external  'PassThrought@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

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

procedure __isx_setuponly_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar; cancelable: Boolean);
external  'AddExecuteTask@files:isx.dll stdcall setuponly';
procedure __isx_uninstallonly_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar; cancelable: Boolean);
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

function  __isx_setuponly_HttpGet(url: PAnsiChar; var pCode: Integer): PAnsiChar;
external  'HttpGet@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_HttpGet(url: PAnsiChar; var pCode: Integer): PAnsiChar;
external  'HttpGet@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_HttpPost(url: PAnsiChar; contentType: PAnsiChar; body: PAnsiChar; var pCode: Integer): PAnsiChar;
external  'HttpPost@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_HttpPost(url: PAnsiChar; contentType: PAnsiChar; body: PAnsiChar; var pCode: Integer): PAnsiChar;
external  'HttpPost@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonParse(data: PAnsiChar; var hdl: Integer): Integer;
external  'JsonParse@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonParse(data: PAnsiChar; var hdl: Integer): Integer;
external  'JsonParse@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonStringify(hdl: Integer; var ptr: Cardinal): Integer;
external  'JsonStringify@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonStringify(hdl: Integer; var ptr: Cardinal): Integer;
external  'JsonStringify@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonInt(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Integer;
external  'JsonInt@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonInt(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Integer;
external  'JsonInt@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonIntFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Integer;
external  'JsonIntFromIdx@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonIntFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Integer;
external  'JsonIntFromIdx@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonFloat(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Single): Integer;
external  'JsonFloat@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonFloat(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Single): Integer;
external  'JsonFloat@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonFloatFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Single): Integer;
external  'JsonFloatFromIdx@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonFloatFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Single): Integer;
external  'JsonFloatFromIdx@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonString(hdl: Integer; path: PAnsiChar; isSet: Integer; var ptr: Cardinal): Integer;
external  'JsonString@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonString(hdl: Integer; path: PAnsiChar; isSet: Integer; var ptr: Cardinal): Integer;
external  'JsonString@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonStringFromIdx(hdl: Integer; const index: Integer; isSet: Integer; var ptr: Cardinal): Integer;
external  'JsonStringFromIdx@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonStringFromIdx(hdl: Integer; const index: Integer; isSet: Integer; var ptr: Cardinal): Integer;
external  'JsonStringFromIdx@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonObj(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Integer;
external  'JsonObj@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonObj(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Integer;
external  'JsonObj@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonObjFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Integer;
external  'JsonObjFromIdx@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonObjFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Integer;
external  'JsonObjFromIdx@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonSize(hdl: Integer): Integer;
external  'JsonSize@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonSize(hdl: Integer): Integer;
external  'JsonSize@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_JsonSize(hdl: Integer): Integer;
external  'JsonSize@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_JsonSize(hdl: Integer): Integer;
external  'JsonSize@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_VerCompare(v1, v2: PAnsiChar): Integer;
external  'VerCompare@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_VerCompare(v1, v2: PAnsiChar): Integer;
external  'VerCompare@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

function  __isx_setuponly_VerSatisfy(semver, version: PAnsiChar): Integer;
external  'VerSatisfy@files:isx.dll stdcall setuponly';
function  __isx_uninstallonly_VerSatisfy(semver, version: PAnsiChar): Integer;
external  'VerSatisfy@{app}\._unins000.isx\isx.dll stdcall uninstallonly';

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

procedure ISX_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar; cancelable: Boolean);
{
  Add a Execute Task to a product
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    __isx_setuponly_AddExecuteTask(ProductIndex, workingDirectory, command, arguments, cancelable);
  end else begin 
    __isx_uninstallonly_AddExecuteTask(ProductIndex, workingDirectory, command, arguments, cancelable);
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

function ISX_HttpGet(url: PAnsiChar; var pCode: Integer): PAnsiChar;
{
  Returns BODY from GET
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := __isx_setuponly_HttpGet(url, pCode);
  end else begin 
    result := __isx_uninstallonly_HttpGet(url, pCode);
  end;
end;

function ISX_HttpPost(url: PAnsiChar; contentType: PAnsiChar; body: PAnsiChar; var pCode: Integer): PAnsiChar;
{
  Returns BODY from POST
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := __isx_setuponly_HttpPost(url, contentType, body, pCode);
  end else begin 
    result := __isx_uninstallonly_HttpPost(url, contentType, body, pCode);
  end;
end;

function ISX_JsonParse(data: PAnsiChar; var hdl: Integer): Boolean;
{
  Create a JSON Handler from string
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonParse(data, hdl);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonParse(data, hdl);
  end;
end;

function ISX_JsonStringify(hdl: Integer; out val: String): Boolean;
{
  Stringify a JSON handler
}
var
  ptr: Cardinal;
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin
    result := 1 = __isx_setuponly_JsonStringify(hdl, ptr);
    if (result) then val := __isx_setuponly_GetStringFromPtr(ptr);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonStringify(hdl, ptr);
    if (result) then val := __isx_uninstallonly_GetStringFromPtr(ptr);
  end;
end;

function ISX_JsonInt(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get int from JSON handler
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonInt(hdl, path, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonInt(hdl, path, isSet, val);
  end;
end;

function ISX_JsonIntFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get int from JSON handler
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonIntFromIdx(hdl, index, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonIntFromIdx(hdl, index, isSet, val);
  end;
end;

function ISX_JsonFloat(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Single): Boolean;
{
  Set or Get float from JSON handler
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonFloat(hdl, path, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonFloat(hdl, path, isSet, val);
  end;
end;

function ISX_JsonFloatFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Single): Boolean;
{
  Set or Get float from JSON handler
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonFloatFromIdx(hdl, index, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonFloatFromIdx(hdl, index, isSet, val);
  end;
end;

function ISX_JsonString(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: string): Boolean;
{
  Set or Get string from JSON handler
}
var
  ptr: Cardinal;
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin
    ptr := __isx_setuponly_GetPtrFromString(val);
    result := 1 = __isx_setuponly_JsonString(hdl, path, isSet, ptr);
    if (result) then val := __isx_setuponly_GetStringFromPtr(ptr);
  end else begin
    ptr :=  __isx_uninstallonly_GetPtrFromString(val);
    result := 1 = __isx_uninstallonly_JsonString(hdl, path, isSet, ptr);
    if (result) then val := __isx_uninstallonly_GetStringFromPtr(ptr);
  end;
end;

function ISX_JsonStringFromIdx(hdl: Integer; const index: Integer; isSet: Integer; out val: string): Boolean;
{
  Set or Get string from JSON handler
}
var
  ptr: Cardinal;
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin
    ptr := __isx_setuponly_GetPtrFromString(val);
    result := 1 = __isx_setuponly_JsonStringFromIdx(hdl, index, isSet, ptr);
    if (result) then val := __isx_setuponly_GetStringFromPtr(ptr);
  end else begin
    ptr :=  __isx_uninstallonly_GetPtrFromString(val);
    result := 1 = __isx_uninstallonly_JsonStringFromIdx(hdl, index, isSet, ptr);
    if (result) then val := __isx_uninstallonly_GetStringFromPtr(ptr);
  end;
end;

function ISX_JsonObj(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get Object from JSON handler
  Note: Array are treated as Object where keys are indexes
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonObj(hdl, path, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonObj(hdl, path, isSet, val);
  end;
end;

function ISX_JsonObjFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get Object from JSON handler
  Note: Array are treated as Object where keys are indexes
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := 1 = __isx_setuponly_JsonObjFromIdx(hdl, index, isSet, val);
  end else begin 
    result := 1 = __isx_uninstallonly_JsonObjFromIdx(hdl, index, isSet, val);
  end;
end;

function ISX_JsonSize(hdl: Integer): Integer;
{
  Get Json size
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := __isx_setuponly_JsonSize(hdl);
  end else begin 
    result := __isx_uninstallonly_JsonSize(hdl);
  end;
end;

function ISX_VerCompare(v1, v2: PAnsiChar): Integer;
{
  Compare 2 version
  return -3  if v1 is an invalid version
  return -2  if v2 is an invalid version
  return -1  if v1 is lower than v2
  return  0  if v1 is equal to v2
  return  >0 if v1 is higher then v2
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := __isx_setuponly_VerCompare(v1, v2);
  end else begin 
    result := __isx_uninstallonly_VerCompare(v1, v2);
  end;
end;

function ISX_VerSatisfy(semver, version: PAnsiChar): Boolean;
{
  Semver Checks
  return 1 if version statisfy semver, 0 otherwise
}
begin
  if (not isInitDone) then RaiseException('ISX not initialized');
  if (isSetup) then begin 
    result := (__isx_setuponly_VerSatisfy(semver, version) = 1);
  end else begin 
    result := (__isx_uninstallonly_VerSatisfy(semver, version) = 1);
  end;
end;
