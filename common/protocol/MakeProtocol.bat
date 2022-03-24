@echo off
%~dp0protocol_maker.exe %~dp0StructDef.xml %~dp0
%~dp0protocol_maker.exe %~dp0ClientWS.xml %~dp0
%~dp0protocol_maker.exe %~dp0ClientGS.xml %~dp0
%~dp0protocol_maker.exe %~dp0ClientCS.xml %~dp0
%~dp0protocol_maker.exe %~dp0Global.xml %~dp0
%~dp0protocol_maker.exe %~dp0WSDB.xml %~dp0
%~dp0protocol_maker.exe %~dp0WSGS.xml %~dp0
%~dp0protocol_maker.exe %~dp0GSCS.xml %~dp0
%~dp0protocol_maker.exe %~dp0FightData.xml %~dp0
@pause