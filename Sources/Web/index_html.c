const char index_html[] = \
	"<html>\r\n"\
	"	<head>\r\n"\
	"		<meta charset=\"utf-8\">\r\n"\
	"		<title>IPG Horizon: ADM-10</title>\r\n"\
	"		<link rel=\"stylesheet\" href=\"css/style.css\">\r\n"\
	"		<link rel=\"stylesheet\" href=\"css/bootstrap.min.css\">\r\n"\
	"		<!--\r\n"\
	"		<script src=\"js/profile_adm200.js\"></script>\r\n"\
	"		<script src=\"js/colors_adm200.js\"></script>\r\n"\
	"		<script src=\"js/info_adm200.js\"></script>\r\n"\
	"		<script src=\"js/dynamic_adm200.js\"></script>\r\n"\
	"		<script src=\"js/config_adm200.js\"></script>\r\n"\
	"		<script src=\"js/thr_adm200.js\"></script>\r\n"\
	"		<script src=\"js/led_adm200.js\"></script>\r\n"\
	"		<script src=\"js/perf_adm200.js\"></script>\r\n"\
	"		<script src=\"js/port_adm200.js\"></script>\r\n"\
	"		<script src=\"js/log.js\"></script>\r\n"\
	"		-->\r\n"\
	"		<script src=\"js/jquery-3.4.1.slim.min.js\"></script>\r\n"\
	"		<script src=\"js/popper.min.js\"></script>\r\n"\
	"		<script src=\"js/bootstrap.min.js\"></script>\r\n"\
	"		<script src=\"js/script.js\"></script>\r\n"\
	"	</head>\r\n"\
	"		<nav class=\"navbar navbar-expand navbar-light bg-light fixed-top\">\r\n"\
	"		  <a class=\"navbar-brand\" href=\"#\">ADM-10</a>\r\n"\
	"		  <button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarNav\" aria-controls=\"navbarNav\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">\r\n"\
	"		    <span class=\"navbar-toggler-icon\"></span>\r\n"\
	"		  </button>\r\n"\
	"		  <div class=\"collapse navbar-collapse\" id=\"navbarNav\">\r\n"\
	"		    <ul class=\"navbar-nav\">\r\n"\
	"		      <li class=\"nav-item\">\r\n"\
	"		        <a class=\"nav-link active\" id=\"nav-profile\" href=\"#\">Profile</a>\r\n"\
	"		      </li>\r\n"\
	"					<li class=\"nav-item dropdown\">\r\n"\
	"		        <a class=\"nav-link dropdown-toggle\" href=\"#\" id=\"nav-system\" role=\"button\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\">\r\n"\
	"		          System\r\n"\
	"		        </a>\r\n"\
	"		        <div class=\"dropdown-menu\" aria-labelledby=\"nav-system\">\r\n"\
	"		          <a class=\"dropdown-item\" id=\"nav-system-os\" href=\"#\">OS</a>\r\n"\
	"		          <a class=\"dropdown-item\" id=\"nav-system-hardware\" href=\"#\">Hardware</a>\r\n"\
	"							<!--\r\n"\
	"							<a class=\"dropdown-item\" id=\"nav-system-update\" href=\"#\">Update</a>\r\n"\
	"							-->\r\n"\
	"		        </div>\r\n"\
	"		      </li>\r\n"\
	"		      <li class=\"nav-item\" id=\"nav-log\">\r\n"\
	"		        <a class=\"nav-link\" href=\"#\">Log</a>\r\n"\
	"		      </li>\r\n"\
	"		    </ul>\r\n"\
	"		  </div>\r\n"\
	"		</nav>\r\n"\
	"		<div class=\"container-fluid content\" id=\"profile-content\">\r\n"\
	"			<div class=\"row overflow-hidden align-content-center\" id=\"profile\" style=\"height:100%\">\r\n"\
	"				<div class=\"d-flex justify-content-center\" style=\"width:100%\">\r\n"\
	"					<div class=\"spinner-grow\" role=\"status\"></div>\r\n"\
	"				</div>\r\n"\
	"				<div class=\"d-flex justify-content-center\" style=\"width:100%\">\r\n"\
	"		    	<p id=\"system_text\">Loading profile...</p>\r\n"\
	"				</div>\r\n"\
	"			</div>\r\n"\
	"		</div>\r\n"\
	"		<div class=\"container-fluid content\" id=\"system-os-content\" style=\"display:none\">\r\n"\
	"			<div class=\"row\" id=\"system-os\" >\r\n"\
	"				<div class=\"col\">\r\n"\
	"					<div class=\"card\">\r\n"\
	"				    <div class=\"m-0 p-0 card-header\">\r\n"\
	"				      <span class=\"m-0 p-1\">CPU</span>\r\n"\
	"				    </div>\r\n"\
	"			      <div class=\"m-0 p-0 card-body\">\r\n"\
	"							<div class=\"m-0 p-0\" id=\"system-os-cpu-heap\" >\r\n"\
	"								<span id=\"system-os-cpu-heap-label\" class=\"m-0 p-1 font-weight-light m-0 p-2\">Heap:</span>\r\n"\
	"								<span id=\"system-os-cpu-heap-value\" class=\"m-0 p-1 font-weight-bold m-0 p-2\">0</span>\r\n"\
	"							</div>\r\n"\
	"						</div>\r\n"\
	"					</div>\r\n"\
	"				</div>\r\n"\
	"				<div class=\"col\">\r\n"\
	"					<div class=\"card\">\r\n"\
	"				    <div class=\"m-0 p-0 card-header\">\r\n"\
	"				      <span class=\"m-0 p-1\">Threads</span>\r\n"\
	"				    </div>\r\n"\
	"			      <div class=\"m-0 p-0 card-body\">\r\n"\
	"							<table class=\"m-0 p-1 table\" id=\"system-os-threads-table\">\r\n"\
	"								<thead>\r\n"\
	"									<tr>\r\n"\
	"										<th scope=\"col\">Name</th>\r\n"\
	"										<th scope=\"col\">State</th>\r\n"\
	"										<th scope=\"col\">Priority</th>\r\n"\
	"										<th scope=\"col\">Stack</th>\r\n"\
	"										<th scope=\"col\">Number</th>\r\n"\
	"									</tr>\r\n"\
	"								</thead>\r\n"\
	"								<tbody id=\"system-os-threads-tbody\">\r\n"\
	"								</tbody>\r\n"\
	"							</table>\r\n"\
	"						</div>\r\n"\
	"					</div>\r\n"\
	"				</div>\r\n"\
	"			</div>\r\n"\
	"		</div>\r\n"\
	"		<div class=\"container-fluid content\" id=\"system-hardware-content\" style=\"display:none\">\r\n"\
	"			<div class=\"row\" id=\"system-hardware\">\r\n"\
	"				<div class=\"col\">\r\n"\
	"					<div class=\"card\">\r\n"\
	"				    <div class=\"m-0 p-0 card-header\">\r\n"\
	"				      <span class=\"m-0 p-1\">Power</span>\r\n"\
	"				    </div>\r\n"\
	"			      <div class=\"m-0 p-0 card-body\">\r\n"\
	"							<table class=\"m-0 p-1 table\" id=\"system-hardware-power-table\">\r\n"\
	"								<thead>\r\n"\
	"									<tr>\r\n"\
	"										<th scope=\"col\">Name</th>\r\n"\
	"										<th scope=\"col\">Value</th>\r\n"\
	"									</tr>\r\n"\
	"								</thead>\r\n"\
	"								<tbody id=\"system-hardware-power-tbody\">\r\n"\
	"								</tbody>\r\n"\
	"							</table>\r\n"\
	"						</div>\r\n"\
	"					</div>\r\n"\
	"				</div>\r\n"\
	"				<div class=\"col\">\r\n"\
	"					<div class=\"card\">\r\n"\
	"				    <div class=\"m-0 p-0 card-header\">\r\n"\
	"				      <span class=\"m-0 p-1\">IC</span>\r\n"\
	"				    </div>\r\n"\
	"			      <div class=\"m-0 p-0 card-body\">\r\n"\
	"							<table class=\"m-0 p-1 table\" id=\"system-hardware-ic-table\">\r\n"\
	"								<thead>\r\n"\
	"									<tr>\r\n"\
	"										<th scope=\"col\">Name</th>\r\n"\
	"										<th scope=\"col\">ID</th>\r\n"\
	"										<th scope=\"col\">Status</th>\r\n"\
	"									</tr>\r\n"\
	"								</thead>\r\n"\
	"								<tbody id=\"system-hardware-ic-tbody\">\r\n"\
	"								</tbody>\r\n"\
	"							</table>\r\n"\
	"						</div>\r\n"\
	"					</div>\r\n"\
	"				</div>\r\n"\
	"			</div>\r\n"\
	"		</div>\r\n"\
	"		<div class=\"container-fluid content\" id=\"system-update-content\" style=\"display:none\">\r\n"\
	"			<div class=\"card m-1 p-1\">\r\n"\
	"				<div class=\"m-0 p-0 card-header\">\r\n"\
	"					<span class=\"m-0 p-1\">Software</span>\r\n"\
	"				</div>\r\n"\
	"				<div class=\"m-1 p-1 card-body\">\r\n"\
	"					<div class=\"input-group\">\r\n"\
	"					  <div class=\"input-group-prepend\">\r\n"\
	"					    <span class=\"input-group-text\" id=\"system-update-software-btn\">Upload</span>\r\n"\
	"					  </div>\r\n"\
	"					  <div class=\"custom-file\">\r\n"\
	"					    <input type=\"file\" class=\"custom-file-input\" id=\"system-update-software-input\">\r\n"\
	"					    <label class=\"custom-file-label\" for=\"system-update-software-input\">Browse</label>\r\n"\
	"					  </div>\r\n"\
	"					</div>\r\n"\
	"				</div>\r\n"\
	"			</div>\r\n"\
	"		</div>\r\n"\
	"		<div class=\"container-fluid content\" id=\"log-content\" style=\"display:none\">\r\n"\
	"			<div class=\"row\" id=\"log\" style=\"height:100%\">\r\n"\
	"				<table class=\"m-0 p-1 table\" id=\"log-table\">\r\n"\
	"					<thead>\r\n"\
	"						<tr>\r\n"\
	"							<th scope=\"col\">Time</th>\r\n"\
	"							<th scope=\"col\">Name</th>\r\n"\
	"							<th scope=\"col\">Id</th>\r\n"\
	"							<th scope=\"col\">Data</th>\r\n"\
	"						</tr>\r\n"\
	"					</thead>\r\n"\
	"					<tbody id=\"log-tbody\">\r\n"\
	"					</tbody>\r\n"\
	"				</table>\r\n"\
	"			</div>\r\n"\
	"		</div>\r\n"\
	"	</body>\r\n"\
	"</html>\r\n"\
	"\r\n"\
;

const unsigned int index_html_size = sizeof(index_html) - 1;

