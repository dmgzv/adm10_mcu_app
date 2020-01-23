import glob

def convert_bin_file_to_c (f_name):
    f = open(f_name, "rb")
    data = bytearray(f.read())
    f.close
    lines = data.split("\n")
    name = f_name.replace(".", "_").replace("-", "_")
    f = open(name + ".c", "w+")
    f.write("const char " + name.replace("/", "_").replace("\\", "_") + "[] = {\\\n")
    idx = 0
    for b in data:
        if idx == 0:
            f.write('\t');
        f.write('0x{:02x}, '.format(b))
        idx = idx + 1
        if idx == 16:
            f.write('\\\n');
            idx = 0
    f.write("};\n\n")
    f.write("const unsigned int " + name.replace("/", "_").replace("\\", "_") + "_size = sizeof(" + name.replace("/", "_").replace("\\", "_") + ");\n\n")
    f.close()

def convert_ascii_file_to_c (f_name):
    f = open(f_name, "r")
    data = f.read()
    f.close
    lines = data.split("\n")
    name = f_name.replace(".", "_").replace("-", "_")
    f = open(name + ".c", "w+")
    f.write("const char " + name.replace("/", "_").replace("\\", "_") + "[] = \\\n")    
    for line in lines:
        f.write("\t\"" + line.replace("\\", "\\\\").replace("\"", "\\\"") + "\\r\\n\"\\\n")
    f.write(";\n\n")
    f.write("const unsigned int " + name.replace("/", "_").replace("\\", "_") + "_size = sizeof(" + name.replace("/", "_").replace("\\", "_") + ") - 1;\n\n")
    f.close()

convert_ascii_file_to_c("index.html")

for f_name in glob.glob("js/*.js"):
    convert_ascii_file_to_c(f_name)

for f_name in glob.glob("css/*.css"):
    convert_ascii_file_to_c(f_name)

for f_name in glob.glob("fonts/*.ttf"):
    convert_bin_file_to_c(f_name)

for f_name in glob.glob("fonts/*.woff"):
    convert_bin_file_to_c(f_name)

for f_name in glob.glob("fonts/*.woff2"):
    convert_bin_file_to_c(f_name)

