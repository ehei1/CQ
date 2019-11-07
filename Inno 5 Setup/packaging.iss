; -- Example2.iss --
; Same as Example1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=CQ Beta
AppVerName=CQ Beta
AppPublisher=ehei Games
AppCopyright=Copyright (C) 2006 ehei Games
AppPublisherURL=http://blog.paran.com/ehei
AppVersion=1.0
DefaultDirName={pf}\CQ Beta
;DisableProgramGroupPage=yes
; ^ since no icons will be created in "{group}", we don't need the wizard
;   to ask for a group name.
DefaultGroupName=CQ Beta
UninstallDisplayIcon={app}\CQ.exe
Compression=lzma
SolidCompression=yes
ShowLanguageDialog=yes

[LangOptions]
LanguageCodePage=0

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
Name: kr; MessagesFile: "compiler:Languages\Korean-5-5.1.0.isl"

[Messages]
en.BeveledLabel=English
kr.BeveledLabel=Korean

[Dirs]
Name: "{app}\media\gui"
Name: "{app}\media\materials\programs"

[Files]
Source: "vcredist_x86.exe"; DestDir: "{app}"; AfterInstall: CheckInstallVC8DLL();
Source: "media\fonts\*"; DestDir: "{app}\media\fonts"
Source: "media\materials\scripts\*"; DestDir: "{app}\media\materials\scripts"
Source: "media\materials\textures\*"; DestDir: "{app}\media\materials\textures"
Source: "media\models\*"; DestDir: "{app}\media\models"
Source: "media\overlays\*"; DestDir: "{app}\media\overlays"
Source: "media\packs\*"; DestDir: "{app}\media\packs"
Source: "media\particle\*"; DestDir: "{app}\media\particle"
Source: "media\sounds\*"; DestDir: "{app}\media\sounds"
Source: "stage\*"; DestDir: "{app}\stage"
Source: "*"; Excludes: "vcredist_x86.exe, ogre.cfg, packaging.iss, CQ.pdb, Ogre.log, CEGUI.log, CQ.cache"; DestDir: "{app}"
Source: "Readme.txt"; DestDir: "{app}"; Languages: en; Flags: isreadme
Source: "Readme_kr.txt"; DestDir: "{app}"; Languages: kr; Flags: isreadme

[Icons]
Name: "{userdesktop}\CQ Beta"; Filename: "{app}\CQ.exe"
Name: "{group}\CQ Beta"; Filename: "{app}\CQ.exe"; Languages: en;
Name: "{group}\CQ Uninstall"; Filename: "{app}\unins000.exe"; Languages: en;
Name: "{group}\CQ Website"; Filename: "{app}\CQ.url"; Languages: en;
Name: "{group}\CQ 베타"; Filename: "{app}\CQ.exe"; Languages: kr;
Name: "{group}\CQ 웹사이트"; Filename: "{app}\CQ_kr.url"; Languages: kr;
Name: "{group}\CQ 제거"; Filename: "{app}\unins000.exe"; Languages: kr;

[Registry]
Root: HKLM; Subkey: "Software\ehei Games"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletevalue

[UninstallDelete]
Type: files; Name: "{app}\ogre.log"
Type: files; Name: "{app}\CQ.cache"
Type: files; Name: "{app}\CQ.suo"

[Run]
;Filename: "{app}\vcredist_x86.exe";
Filename: "{app}\CQ.EXE"; Description: "Launch application"; Languages: en; Flags: postinstall nowait skipifsilent unchecked
Filename: "{app}\CQ.EXE"; Description: "게임 시작"; Languages: kr; Flags: postinstall nowait skipifsilent unchecked

[Code]
function InitializeSetup(): Boolean;
var
  Version: TWindowsVersion;
begin
  GetWindowsVersionEx( Version );

  // check version of Windows and Service Pack
  if 2600 > Version.Build then
    begin
    MsgBox( 'CQ needs Windows XP & Service Pack 2  ', mbInformation, MB_OK );
    Result := False;
    end
  else
    if( 2 > Version.ServicePackMajor ) then
      begin
      MsgBox( 'CQ needs Windows Service Pack 2 ', mbInformation, MB_OK );
      Result := False;
      end
    else
      begin
      Result := True;
      end
end;

procedure CheckInstallVC8DLL();
var
  ResultCode: Integer;
begin
  if not FileOrDirExists( GetWinDir + '\WinSxS\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_0de06acd' ) then
    begin
    exec( ExpandConstant( '{app}\vcredist_x86.exe' ), '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode );
    end;
end;
