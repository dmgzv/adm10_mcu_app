@echo off
C:\python27\python c:\python27\scripts\pyang --plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm10 ^
	YangFiles\ipg-common@2019-08-10.yang ^
	YangFiles\ipg-sfp@2019-11-27.yang ^
	YangFiles\ipg-adm-10@2019-11-28.yang ^
	YangFiles\ipg-hidden@2019-08-28.yang
