# ISX Init
Bootup Innosetup extension
```pascal
function ISX_InitializeSetup(isQuiet: Boolean): Boolean;
{
  Initialize ISX
  Please perfoms this call on InnoSetup's 'InitializeSetup()'
}
```
```pascal
function ISX_InitializeUninstall(isQuiet: Boolean): Boolean;
{
  Initialize ISX
  Please make it call on InnoSetup's 'InitializeUninstall()'
}
```
```pascal
procedure ISX_Terminate();
{
  Terminate ISX to delete it properly
}
```

# REST API
Online webservices support
```pascal
function ISX_HttpGet(url: PAnsiChar; var pCode: Integer): PAnsiChar;
{
  Returns BODY from GET
}
```
```pascal
function ISX_HttpPost(url: PAnsiChar; contentType: PAnsiChar; body: PAnsiChar; var pCode: Integer): PAnsiChar;
{
  Returns BODY from POST
}
```

# JSON
Json Schema support. [What is json?]([https://json-schema.org)

```pascal
function ISX_JsonParse(data: PAnsiChar; var hdl: Integer): Boolean;
{
  Create a JSON Handler from string
}
```
```pascal
function ISX_JsonStringify(hdl: Integer; out val: String): Boolean;
{
  Stringify a JSON handler
}
```
```pascal
function ISX_JsonInt(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get int from JSON handler
}
```
```pascal
function ISX_JsonIntFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get int at [index] from JSON handler of a an array
}
```
```pascal
function ISX_JsonFloat(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Single): Boolean;
{
  Set or Get float from JSON handler
}
```
```pascal
function ISX_JsonFloatFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Single): Boolean;
{
  Set or Get float at [index] from JSON handler of a an array
}
```
```pascal
function ISX_JsonString(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: string): Boolean;
{
  Set or Get string from JSON handler
}
```
```pascal
function ISX_JsonStringFromIdx(hdl: Integer; const index: Integer; isSet: Integer; out val: string): Boolean;
{
  Set or Get string at [index] from JSON handler of a an array
}
```
```pascal
function ISX_JsonObj(hdl: Integer; path: PAnsiChar; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get Object from JSON handler
  
  Note: Array are treated as regular json object where keys are indexes
}
```
```pascal
function ISX_JsonObjFromIdx(hdl: Integer; index: Integer; isSet: Integer; var val: Integer): Boolean;
{
  Set or Get Object at [index] from JSON handler of a an array

  Note: Array are treated as regular json object where keys are indexes
}
```
```pascal
function ISX_JsonSize(hdl: Integer): Integer;
{
  Get Json size
}
```

## SEMVER
Semantic Versioning support. [What is semver ?](https://semver.org/)
```pascal
function ISX_VerCompare(v1, v2: PAnsiChar): Integer;
{
  Compare 2 version
  return -3  if v1 is an invalid version
  return -2  if v2 is an invalid version
  return -1  if v1 is lower than v2
  return  0  if v1 is equal to v2
  return  >0 if v1 is higher then v2
}
```
```pascal
function ISX_VerSatisfy(semver, version: PAnsiChar): Boolean;
{
  Semver Checks
  return 1 if version statisfy semver, 0 otherwise
}
```
```pascal
function ISX_VerFormat(version: PAnsiChar; nbDigits: Integer): PAnsiChar;
{
   Format Version String
}
```

# Miscellaneous
Miscellaneous helpers
```pascal
procedure ISX_Wait(ms: Integer);
{
  Make current thread wait for a delay in millisecond
}
```

# Dependencies
- Dependencies are defined by a set of so nammed **"Products"**.
- each "Products" contains a set of **"Tasks"**.

Hence, for instance, add a dependency on "Visual C++ x64 2015 Redistributable" may be defined like so:
```pascal
// Add 'Visual C++ x64 2015 Redistributable'
i := ISX_CreateProduct('Visual C++ x64 2015 Redistributable');
// - Which first need to download a binary
ISX_AddDownloadTask(i, 'http://download.microsoft.com/.../vc_redist.x64.exe', ExpandConstant('{tmp}/vc_redist.x64.exe'));
// - And then execute it
ISX_AddExecuteTask(i, ExpandConstant('{tmp}'), 'vc_redist.x64.exe', '/norestart');

// ...

// Run sequential all tasks associated to all products
ISX_Run();
```
## 1. Dependencies management
```pascal
function ISX_Run(): PAnsiChar;
{
  Do sequential all tasks associated to all products
}
```
```pascal
function ISX_RunEx(callback: TTaskCallback): PAnsiChar;
{
  Do sequential all tasks associated to all products (with callback)
}
```
```pascal
function ISX_GetReadyMemo(Space, NewLine: PAnsiChar): PAnsiChar;
{
  Return the InnoSetup Memo string which is a digest of
  all operations which has been scheduled
}
```
## 2. Products
```pascal
procedure ISX_ClearProducts();
{
  Clear Product list
}
```
```pascal
function ISX_CreateProduct(ProductName: PAnsiChar): Integer;
{
  Add a product in ISX List
}
```
## 3. Tasks
```pascal
procedure ISX_AddDownloadTask(ProductIndex: Integer; url, dst: PAnsiChar);
{
  Add a Download Task to a product
}
```
```pascal
procedure ISX_AddExecuteTask(ProductIndex: Integer; workingDirectory, command, arguments: PAnsiChar; cancelable: Boolean);
{
  Add a Execute Task to a product
}
```
```pascal
procedure ISX_AddUnZipTask(ProductIndex: Integer; path, dst: PAnsiChar; clear: Bool);
{
  Add an UnZip Task to a product
}
```
```pascal
procedure ISX_AddDeleteTask(ProductIndex: Integer; path: PAnsiChar; exitIfFail: Boolean);
{
  Add a Delete Task to a product 
}
```
```pascal
procedure ISX_AddEmptyTask(ProductIndex: Integer);
{
  Add an Empty Task to a product 
}
```
```pascal
procedure ISX_AddFakeTask(ProductIndex: Integer; name: PAnsiChar);
{
  Add a Fake Task (it's only a timed progress bar) to a product 
  NOTE: Used for test 
}
```
