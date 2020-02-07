class WebFile:
	def __init__(self, fname, ftype):
		self.name = fname
		self.type = ftype
		
	def read_data(self):
		f = open(self.name, "rb")
		self.data = bytearray(f.read())
		f.close

addr = 256
web_file_list = ''
web_files = []
web_files.append(WebFile('index.html', 'html'))
web_files.append(WebFile('css/bootstrap.min.css', 'css'))
web_files.append(WebFile('css/style.css', 'css'))
web_files.append(WebFile('js/jquery-3.4.1.slim.min.js', 'js'))
web_files.append(WebFile('js/popper.min.js', 'js'))
web_files.append(WebFile('js/bootstrap.min.js', 'js'))
web_files.append(WebFile('js/script.js', 'js'))

f = open("web.bin", "wb+")

for web_file in web_files:
	web_file.read_data()
	addr += len(web_file.data)
	f.write(bytearray('name: ' + web_file.name + '\r\n', encoding='utf8'))
	f.write(bytearray('type: ' + web_file.type + '\r\n', encoding='utf8'))
	f.write(bytearray('size: ' + str(len(web_file.data)) + '\r\n', encoding='utf8'))
	f.write(bytearray('\r\n', encoding='utf8'))
	f.write(web_file.data)

f.close()
