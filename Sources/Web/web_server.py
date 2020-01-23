from http.server import HTTPServer, BaseHTTPRequestHandler

class HttpProcessor(BaseHTTPRequestHandler):
	def do_GET(self):
		self.send_response(200)
		self.send_header('content-type','text/html')
		self.end_headers()
		self.wfile.write("hello !")


serv = HTTPServer(("localhost",80),HttpProcessor)
serv.serve_forever()
