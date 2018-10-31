#include "isx/isx.iss"

[Setup]
AppName=testApp
AppVersion=1.0.0
DefaultDirName={pf}\testApp

[code]

procedure example1();
var
  httpStatus: Integer;
  data: AnsiString;
begin

  data := ISX_HttpGet('http://localhost:8081/json', httpStatus);
  Log(IntToStr(httpStatus));
  if (httpStatus <> 200) then exit;

  ISX_HttpPost('http://localhost:8081/api', 'application/json', data, httpStatus);
  Log(IntToStr(httpStatus));
  if (httpStatus <> 200) then exit;

end;

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
  example1();
  Result := False;
end;
