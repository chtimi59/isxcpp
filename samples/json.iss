#include "isx/isx.iss"

[Setup]
AppName=testApp
AppVersion=1.0.0
DefaultDirName={pf}\testApp

[code]

procedure example1();
var
  hJson: Integer;
  a: Integer;
  pi: Single;
  greeting: AnsiString;
  str: AnsiString;
begin

  str := '{  "a":5,  "foo":{ "greeting": "hello" },  "pi":3.141516  }';

  if (not ISX_JsonParse(str, hJson)) then exit;
  if (not ISX_JsonInt(hJson, 'a', 0, a)) then exit;
  if (not ISX_JsonFloat(hJson, 'pi', 0, pi)) then exit;
  if (not ISX_JsonString(hJson, 'foo.greeting', 0, greeting)) then exit;
  
  a := 2*a;
  pi := pi/2;
  greeting := greeting + ' world !';
 
  if (not ISX_JsonInt(hJson, 'a', 1, a)) then exit;
  if (not ISX_JsonFloat(hJson, 'pi', 1, pi)) then exit;
  if (not ISX_JsonString(hJson, 'foo.greeting', 1, greeting)) then exit;

  if (not ISX_JsonStringify(hJson, str)) then exit;
  Log(str); // {"a":10,"foo":{"greeting":"hello world !"},"pi":1.5707579851150513}

end;


procedure example2();
var
  hArray: Integer;
  i,a: Integer;
  str: AnsiString;
  count: Integer; 
begin

  ISX_JsonParse('[]', hArray);
  for i := 0 to 10 do
  begin
    a := i*10;
    ISX_JsonIntFromIdx(hArray, i, 1, a);
   end;

  ISX_JsonStringify(hArray, str);
  Log(str); // [0,10,20,30,40,50,60,70,80,90,100]

  count := ISX_JsonSize(hArray)
  ISX_JsonIntFromIdx(hArray, count/2, 0, a);
  Log(IntToStr(a)) // 50
  
end;

procedure example3();
var
  hJson: Integer;
  j1, j2, j3: Integer;
  str: AnsiString;
begin

  ISX_JsonParse('{}',hJson);  
  ISX_JsonParse('{"a":1, "b":2}', j1);
  ISX_JsonObj(hJson, 'A', 1, j1);
  ISX_JsonParse('{"c":1, "d":2}', j2);
  ISX_JsonObj(hJson, 'B', 1, j2);

  ISX_JsonStringify(hJson, str);
  Log(str); // {"A":{"a":1,"b":2},"B":{"c":1,"d":2}}

  ISX_JsonObj(hJson, 'B', 0, j3);
  ISX_JsonStringify(j3, str);
  Log(str); // {"c":1,"d":2}

end;

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
  example1();  
  example2();  
  example3();  
  Result := False;
end;
