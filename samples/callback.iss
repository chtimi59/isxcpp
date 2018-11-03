[Setup]
AppName=testApp
AppVersion=1.0.0
DefaultDirName={pf}\testApp

[code]

#include "isx/isx.iss"

type
    TCallback = function(a,b,c,d,e,f:Integer): Integer;

function Callback(a,b,c,d,e,f:Integer): Integer;
begin
    Log('Callback(' + IntToStr(a) +','+ IntToStr(b) +','+ IntToStr(c) +','+ IntToStr(d)+','+ IntToStr(e)+','+ IntToStr(f)+')');
    Result := 1234;
end;

function WrapCallback(method: TCallback; paramcount: Integer): Cardinal;
external 'WrapCallback@files:isx.dll stdcall';

function TestCallback(cb: Cardinal): Integer;
external 'TestCallback@files:isx.dll stdcall';


function InitializeSetup(): Boolean;
var ret, h: Cardinal ;
begin

  h := WrapCallback(@Callback,6);

  ret := TestCallback(h);
  Log(IntToStr(ret));
  Result := False;
end;


