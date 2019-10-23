
f = open("UCD90120A 2.3.4.0 Address 126 SMBus PMBus Config Script.txt", "r")
data = f.read()
f.close()

data = data.replace("\t", " ")
start = 0
end = 0
Values = ""
ValueLen = 0
ValuesCnt = 0
Configs = "const UCD90120A_CFG_CMD_T BSP_UCD90120A_CFG_Table[] = {\n"
while start < len(data):
    end = start + data[start:].find("\n")
    line = data[start:end+1]
    if data[start:start + 2] == "//" and data[end:end + 1] == "\n":
        Configs = Configs + "\t" + data[start:end + 1]
    else:
        words = data[start:end+1].split()
        if words[0] == "SendByte":
            ValuesCnt = ValuesCnt + 1
            Configs = Configs + "\t{\n"
            Configs = Configs + "\t\tPMBUS_CMD_TYPE_SEND_BYTE,\n"
            Configs = Configs + "\t\t" + words[2] + ",\n"
            Configs = Configs + "\t\tNULL,\n"
            Configs = Configs + "\t\t0,\n"
            Configs = Configs + "\t},\n"
        elif words[0] == "WriteByte" or words[0] == "WriteWord" or words[0] == "BlockWrite":
            ValuesCnt = ValuesCnt + 1
            Configs = Configs + "\t{\n"
            if words[0] == "WriteByte":
                Configs = Configs + "\t\tPMBUS_CMD_TYPE_WRITE_BYTE,\n"
            elif words[0] == "WriteWord":
                Configs = Configs + "\t\tPMBUS_CMD_TYPE_WRITE_WORD,\n"
            elif words[0] == "BlockWrite":
                Configs = Configs + "\t\tPMBUS_CMD_TYPE_WRITE_BLOCK,\n"
            Configs = Configs + "\t\t" + words[2] + ",\n"
            Configs = Configs + "\t\t" + "(uint8_t*)Values" + str(ValuesCnt) + ",\n"
            Configs = Configs + "\t\t" + "sizeof(Values" + str(ValuesCnt) + ") / sizeof(Values" + str(ValuesCnt) + "[0]),\n"
            Configs = Configs + "\t},\n"
            words.pop(0)
            words.pop(0)
            words.pop(0)
            ValueLen = 0
            Values = Values + "const uint8_t Values" + str(ValuesCnt) + "[] = {"
            while ValueLen < len(words):
                Values = Values + words[ValueLen] + ", "
                ValueLen = ValueLen + 1
            Values = Values[:-2] + "};\n"
    start = end + 1
Configs = Configs + "};\n"
f = open("bsp_pwr_cfg.c", "w+")
f.write("#include \"ucd90120a_cfg.h\"\n")
f.write("\n")
f.write(Values)
f.write("\n")
f.write(Configs)
f.write("\n")
f.write("const uint32_t BSP_UCD90120A_CFG_TableSize = sizeof(BSP_UCD90120A_CFG_Table) / sizeof(BSP_UCD90120A_CFG_Table[0]);\n")
f.write("\n")
f.close()



