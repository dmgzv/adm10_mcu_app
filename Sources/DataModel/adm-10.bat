@echo on
C:\python27\python c:\python27\scripts\pyang -p .\yang\ECS ^
	--plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm_10 ^
	yang\ADM-10\ipg-common@2019-08-10.yang ^
	yang\ADM-10\ipg-sfp@2019-11-27.yang ^
	yang\ADM-10\ipg-adm-10@2019-11-28.yang ^
	yang\ADM-10\ipg-hidden@2019-08-28.yang
