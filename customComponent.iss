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

function InitializeSetup(): Boolean;
begin
  Result := ISX_InitializeSetup(false);
end;

var
  CustomSelectTasksPage: TWizardPage;
  ComponentsList: TNewCheckListBox;

procedure InitializeWizard();
var
  {FindRec: TFindRec;}
  SelectComponentsLabel: TNewStaticText;
begin
  CustomSelectTasksPage :=
    CreateCustomPage(
      wpSelectComponents, SetupMessage(msgWizardSelectComponents),
      SetupMessage(msgSelectComponentsDesc));

  SelectComponentsLabel := TNewStaticText.Create(WizardForm);
  SelectComponentsLabel.Parent := CustomSelectTasksPage.Surface;
  SelectComponentsLabel.Top := 0;
  SelectComponentsLabel.Left := 0;
  SelectComponentsLabel.Width := CustomSelectTasksPage.Surface.Width;
  SelectComponentsLabel.AutoSize := False;
  SelectComponentsLabel.ShowAccelChar := False;
  SelectComponentsLabel.WordWrap := True;
  SelectComponentsLabel.Caption := SetupMessage(msgSelectComponentsLabel2);
  WizardForm.AdjustLabelHeight(SelectComponentsLabel);

  ComponentsList := TNewCheckListBox.Create(WizardForm);
  ComponentsList.Parent := CustomSelectTasksPage.Surface;
  ComponentsList.Top :=
    SelectComponentsLabel.Top + SelectComponentsLabel.Height + ScaleY(8);
  ComponentsList.Left := 0;
  ComponentsList.Width := CustomSelectTasksPage.Surface.Width;
  ComponentsList.Height := CustomSelectTasksPage.Surface.Height - ComponentsList.Top;

  ComponentsList.AddCheckBox('hello 1', '', 0, False, True, False, False, nil);
  ComponentsList.AddCheckBox('hello 2', '', 0, False, True, False, False, nil);
  ComponentsList.AddCheckBox('hello 3', '', 0, False, True, False, False, nil);

  {if FindFirst(ExpandConstant(AddBackslash(SourcePath) + '*.dat'), FindRec) then
  begin
    try
      repeat
        ComponentsList.AddCheckBox(FindRec.Name, '', 0, False, True, False, False, nil);
      until not FindNext(FindRec);
    finally
      FindClose(FindRec);
    end;
  end;}
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  I: Integer;
  name: string;
begin
  if CurStep = ssInstall then
  begin
    for I := 0 to ComponentsList.Items.Count - 1 do
    begin
      if ComponentsList.Checked[I] then
      begin
        name := ComponentsList.Items[I];
        Log(Format('"%s".', [name]));
      end;
    end;
  end;
end;