@echo on
C:\python27\python c:\python27\scripts\pyang -p .\yang\ECS ^
	--plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm_10_clbr ^
	yang\ADM-10\ipg-common@2019-08-10.yang ^
	yang\ADM-10\ipg-adm-10-clbr@2020-02-10.yang
