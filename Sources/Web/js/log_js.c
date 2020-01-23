const char js_log_js[] = \
	"var  log_json = `\r\n"\
	"{\r\n"\
	"	\"max-records-cnt\" : \"256\",\r\n"\
	"	\"records-cnt\" : \"65\",\r\n"\
	"	\"records\" : [\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0400\",\r\n"\
	"			\"data\" : \"PCA9555 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0410\",\r\n"\
	"			\"data\" : \"DIM3R3400 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0410\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 11\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0410\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 11 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0410\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 12\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.0410\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 12 0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.1040\",\r\n"\
	"			\"data\" : \"UCD90120A Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 9\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"0.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 9 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0390\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 10\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0390\",\r\n"\
	"			\"data\" : \"DIM3R3400 PowerGood: 0x1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0390\",\r\n"\
	"			\"data\" : \"DIM3R3400 Rail A: 43.22\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0390\",\r\n"\
	"			\"data\" : \"DIM3R3400 Rail A: 0.00\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A PowerGood: 0x0fff\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 0: 4.97\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 1: 1.10\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 2: 1.20\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 3: 1.79\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 4: 2.49\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 5: 3.32\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 6: 0.90\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 7: 0.90\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"UCD90120A Rail 8: 11.96\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 7\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0550\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 8\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0650\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 7 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0650\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 8 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0960\",\r\n"\
	"			\"data\" : \"SI5351 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0960\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 5\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.0960\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 5 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.5770\",\r\n"\
	"			\"data\" : \"SI5342 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.5770\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 3\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"1.5770\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 3 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.0580\",\r\n"\
	"			\"data\" : \"SI5342 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.0580\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 4\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.0580\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x06 0x47\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.0580\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 4 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5390\",\r\n"\
	"			\"data\" : \"SI5342 Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5460\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 13\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5460\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x07 0xc0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5460\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 13 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5470\",\r\n"\
	"			\"data\" : \"FPGA W: 0x000c 0x83cf\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5470\",\r\n"\
	"			\"data\" : \"FPGA Init: Ok\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5700\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 6\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5700\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x06 0x07\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.5700\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 6 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.8710\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 6 0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"2.8710\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x02 0xbf\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x06 0x05\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x06 0x04\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Init: 2\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x06 0x00\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 1 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 0 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 2 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 0 0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.1040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x02 0xbe\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.2040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 0 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.2040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x02 0xbf\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.3040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 2 0\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.3040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x02 0xbb\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.4040\",\r\n"\
	"			\"data\" : \"PCA9555 Pin Set: 2 1\"\r\n"\
	"		},\r\n"\
	"		{\r\n"\
	"			\"time\" : \"3.4040\",\r\n"\
	"			\"data\" : \"PCA9555 Write: 0x02 0xbf\"\r\n"\
	"		}\r\n"\
	"	]\r\n"\
	"}\r\n"\
	"`;\r\n"\
	"\r\n"\
;

const unsigned int js_log_js_size = sizeof(js_log_js) - 1;

