#include 'isx/isx.iss'

[Setup]
AppName=testApp
AppVersion=1.0.0
DefaultDirName={pf}\testApp

[code]

procedure example1();
var
  r: Integer;
  b: Boolean;
begin

  {VerFormat}

  Log(ISX_VerFormat('10.2.3', 2)); // '10.02.03'
  Log(ISX_VerFormat('10.2.3', 1)); // '' -- digits missing

  {VerCompare}

  r := ISX_VerCompare('2.3.0', '1.5.10');
  Log(IntToStr(r)); // 1
  r := ISX_VerCompare('2.3.0', '2.3.0');
  Log(IntToStr(r)); // 0  
  r := ISX_VerCompare('1.5.10', '2.3.0');
  Log(IntToStr(r)); // -1

  r := ISX_VerCompare('error', '1.2.3');
  Log(IntToStr(r)); // -3
  r := ISX_VerCompare('1.2.3', 'error');
  Log(IntToStr(r)); // -2

  {VerSatisfy}

  b := ISX_VerSatisfy('~1.3.10', '1.3.12');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('^1.3.10', '1.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('>=1.3.10', '5.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('>=1.3.10', '1.2.10');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE
  b := ISX_VerSatisfy('1.2', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('1.2.x', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('1.2.3', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('1.0.0', '1');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('1.0.0', '1.x.x');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE
  b := ISX_VerSatisfy('1', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE

  b := ISX_VerSatisfy('error', '1.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE
  b := ISX_VerSatisfy('1.5.2', 'error');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE
  b := ISX_VerSatisfy('ab1.3.10', '1.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE
  b := ISX_VerSatisfy('ab', '1.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // FALSE

  b := ISX_VerSatisfy('>=', '1.5.2');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE;
  b := ISX_VerSatisfy('x.x.x', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
  b := ISX_VerSatisfy('', '1.2.3');
  if (b) then Log('TRUE') else Log('FALSE'); // TRUE
end;

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
  example1();
  Result := False;
end;
