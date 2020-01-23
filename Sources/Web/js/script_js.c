const char js_script_js[] = \
	"let timerId;\r\n"\
	"let debug = 0;\r\n"\
	"\r\n"\
	"let profile = {\r\n"\
	"	lines: [],\r\n"\
	"	params: [],\r\n"\
	"	sections: []\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getParamColorString(color) {\r\n"\
	"	if (color == 3) {\r\n"\
	"		return 'Normal';\r\n"\
	"	}	else if (color == 4) {\r\n"\
	"		return 'Warning';\r\n"\
	"	}	else if (color == 5) {\r\n"\
	"		return 'Minor';\r\n"\
	"	}	else if (color == 6) {\r\n"\
	"		return 'Major';\r\n"\
	"	}	else if (color == 7) {\r\n"\
	"		return 'Critical';\r\n"\
	"	}\r\n"\
	"		return '';\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getParamValueString(param) {\r\n"\
	"	if (param.enum != undefined)	{\r\n"\
	"		for (i = 0; i < param.enum.length; i++)		{\r\n"\
	"			if (param.value == param.enum[i].value)\r\n"\
	"				return param.enum[i].string;\r\n"\
	"		}\r\n"\
	"	}	else if (param.bitmask != undefined)	{\r\n"\
	"		let result;\r\n"\
	"		let value = parseInt(param.value, 10);\r\n"\
	"		if (value == 0)\r\n"\
	"			return param.bitmask[0].string;\r\n"\
	"		for (i = 1; i < param.bitmask.length; i++)		{\r\n"\
	"			if (parseInt(param.value, 10) & param.bitmask[i].mask)			{\r\n"\
	"				if (result == undefined)\r\n"\
	"					result = param.bitmask[i].string;\r\n"\
	"				else\r\n"\
	"					result += ',' +  param.bitmask[i].string;\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"		return result;\r\n"\
	"	}\r\n"\
	"	return param.value;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateProfileTable(section) {\r\n"\
	"	for (let id = 1; id <= 2; id++) {\r\n"\
	"		for (let i = 0; i < section.params.length; i++) {\r\n"\
	"			if (section.params[i].hidden)\r\n"\
	"				$('#profile_' + id + '_' + section.params[i].name).addClass('profile_param_value_hidden');\r\n"\
	"			else {\r\n"\
	"				$('#profile_' + id + '_' + section.params[i].name).removeClass('profile_param_value_hidden');\r\n"\
	"				let value = $('#profile_' + id + '_' + section.params[i].name + '_p');\r\n"\
	"				if (value.length)\r\n"\
	"					value.text(getParamValueString(section.params[i]));\r\n"\
	"				if (section.name == 'Dynamic') {\r\n"\
	"					let color = $('#profile_' + id + '_' + section.params[i].name + '_color');\r\n"\
	"					if (section.params[i].colors.current == 7) {\r\n"\
	"						color.css('background-color', '#e7a8ad');\r\n"\
	"					}	else if (section.params[i].colors.current == 6) {\r\n"\
	"						color.css('background-color', '#f0c4a1');\r\n"\
	"					} else if (section.params[i].colors.current == 5) {\r\n"\
	"						color.css('background-color', '#f7e49c');\r\n"\
	"					} else if (section.params[i].colors.current == 4) {\r\n"\
	"						color.css('background-color', '#a1c4d6');\r\n"\
	"					} else if (section.params[i].colors.current == 3) {\r\n"\
	"						color.css('background-color', '#b6d8be');\r\n"\
	"					} else if (section.params[i].colors.current == 3) {\r\n"\
	"						color.css('background-color', 'transparent');\r\n"\
	"					}\r\n"\
	"				}\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileSectionValues(section, tableText) {\r\n"\
	"	let table;\r\n"\
	"	table = tableText.split(';');\r\n"\
	"	if (section.params.length != table.length)\r\n"\
	"		return;\r\n"\
	"	for (let i = 0; i < table.length; i++) {\r\n"\
	"		if (table[i] == '\\x01') {\r\n"\
	"			section.params[i].hidden = 1;\r\n"\
	"		}	else {\r\n"\
	"			section.params[i].hidden = 0;\r\n"\
	"			if (section.name == 'Dynamic') {\r\n"\
	"				let values = table[i].split('@');\r\n"\
	"				section.params[i].value = values[0];\r\n"\
	"				if (values.length > 1)\r\n"\
	"					section.params[i].colors.current = values[1];\r\n"\
	"				else\r\n"\
	"					section.params[i].colors.current = 0;\r\n"\
	"			}	else {\r\n"\
	"				section.params[i].value = table[i];\r\n"\
	"			}\r\n"\
	"	  }\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getProfileSectionValues (sectionIdx, callback) {\r\n"\
	"	console.log('getProfileSectionValues: ' + profile.sections[sectionIdx].idx);\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/profile/sections/' + profile.sections[sectionIdx].idx + '/values', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() {\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getProfileSectionValues() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				parseProfileSectionValues(profile.sections[sectionIdx], xhr.responseText);\r\n"\
	"				updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"				if (callback != undefined)\r\n"\
	"					callback(sectionIdx);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		if (profile.sections[sectionIdx].name == 'Info') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profileInfoValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Dynamic') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profileDynamicValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Config') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profileConfigValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Threshould') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profileThrValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Led') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profileLedValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Counter') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profilePerfValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}	else if (profile.sections[sectionIdx].name == 'Port') {\r\n"\
	"			parseProfileSectionValues(profile.sections[sectionIdx], profilePerfValues);\r\n"\
	"			updateProfileTable(profile.sections[sectionIdx]);\r\n"\
	"		}\r\n"\
	"		if (callback != undefined)\r\n"\
	"			callback(sectionIdx);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getProfileSectionValueCallback(sectionIdx) {\r\n"\
	"	sectionIdx = (sectionIdx + 1) % profile.sections.length;\r\n"\
	"	timerId = setTimeout(() => getProfileSectionValues(sectionIdx, getProfileSectionValueCallback), 100);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getProfileSections() {\r\n"\
	"	console.log('getProfileSections');\r\n"\
	"	let sectionIdx = 0;\r\n"\
	"	clearTimeout(timerId);\r\n"\
	"	getProfileSectionValues(0, getProfileSectionValueCallback);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileParamLine(section, line) {\r\n"\
	"	let line_words = line.split(';');\r\n"\
	"	let param = {};\r\n"\
	"	param.section = section;\r\n"\
	"	param.groups = [];\r\n"\
	"	param.groups[0] = 'All';\r\n"\
	"	param.name = line_words[0];\r\n"\
	"	param.idx = line_words[1];\r\n"\
	"	param.type = line_words[2];\r\n"\
	"	param.access = line_words[3];\r\n"\
	"	param.units = line_words[4];\r\n"\
	"	param.descr = line_words[6];\r\n"\
	"	param.value = '0';\r\n"\
	"	param.hidden = 0;\r\n"\
	"	return param;\r\n"\
	"}\r\n"\
	"\r\n"\
	"const profileThrNames = ['HMax', 'CMax', 'WMax', 'WMin', 'CMin', 'HMin'];\r\n"\
	"\r\n"\
	"function parseProfileParamLine(section, line) {\r\n"\
	"	let line_words = line.split(';');\r\n"\
	"	let param = {};\r\n"\
	"	param.section = section;\r\n"\
	"	param.groups = [];\r\n"\
	"	param.groups[0] = 'All';\r\n"\
	"	param.name = line_words[0];\r\n"\
	"	param.idx = line_words[1];\r\n"\
	"	param.type = line_words[2];\r\n"\
	"	param.access = line_words[3];\r\n"\
	"	param.units = line_words[4];\r\n"\
	"	param.descr = line_words[6];\r\n"\
	"	param.value = '0';\r\n"\
	"	if (section == 'Dynamic') {\r\n"\
	"		param.thr = [];\r\n"\
	"		param.colors = {};\r\n"\
	"		param.colors.current = 0;\r\n"\
	"		param.colors.warning = 0;\r\n"\
	"		param.colors.critical = 0;\r\n"\
	"	}\r\n"\
	"	if (section == 'Config') {\r\n"\
	"		param.thr = [];\r\n"\
	"	}\r\n"\
	"	return param;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileEnumLine(params, line) {\r\n"\
	"	let line_words = line.split(';');\r\n"\
	"	for (let i = 0; i < params.length; i++)	{\r\n"\
	"		if (line_words[0] == params[i].name) {\r\n"\
	"			let line_words_enums = line_words[1].split('|');\r\n"\
	"			params[i].enum = [];\r\n"\
	"			for (let j = 0; j < line_words_enums.length; j++)	{\r\n"\
	"				let line_words_enums_values = line_words_enums[j].split('@');\r\n"\
	"				params[i].enum[j] = {};\r\n"\
	"				params[i].enum[j].string = line_words_enums_values[0];\r\n"\
	"				params[i].enum[j].value = line_words_enums_values[1];\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileBitmskLine(params, line) {\r\n"\
	"	let line_words = line.split(';');\r\n"\
	"	for (let i = 0; i < params.length; i++)	{\r\n"\
	"		if (line_words[0] == params[i].name) {\r\n"\
	"			let line_words_bitmsks = line_words[2].split('|');\r\n"\
	"			params[i].bitmask = [];\r\n"\
	"			params[i].bitmask[0] = {};\r\n"\
	"			params[i].bitmask[0].mask = 0\r\n"\
	"			params[i].bitmask[0].string = line_words[1];\r\n"\
	"			for (let j = 0; j < line_words_bitmsks.length; j++) {\r\n"\
	"				let line_words_bitmsks_values = line_words_bitmsks[j].split('@');\r\n"\
	"				params[i].bitmask[j + 1] = {};\r\n"\
	"				params[i].bitmask[j + 1].string = line_words_bitmsks_values[0];\r\n"\
	"				params[i].bitmask[j + 1].mask = 1 << parseInt(line_words_bitmsks_values[1], 10);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileGroupLine(params, line) {\r\n"\
	"	let group_info = line.split(';');\r\n"\
	"	let group_params = group_info[2].split('|');\r\n"\
	"	let group = {};\r\n"\
	"\r\n"\
	"	group.idx = group_info[0];\r\n"\
	"	group.name = group_info[1];\r\n"\
	"	group.params = [];\r\n"\
	"\r\n"\
	"	for (let i = 0; i < group_params.length; i++)	{\r\n"\
	"		for (let j = 0; j < params.length; j++) {\r\n"\
	"			if (params[j].name == group_params[i]) {\r\n"\
	"				group.params[i] = params[j];\r\n"\
	"				params[j].groups[params[j].groups.length] = group.name;\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"	return group;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function createProfileParamSection(name, idx) {\r\n"\
	"	let section = {};\r\n"\
	"	section.idx = idx;\r\n"\
	"	section.name = name;\r\n"\
	"	section.params = [];\r\n"\
	"	section.groups = [];\r\n"\
	"	return section;\r\n"\
	"}\r\n"\
	"\r\n"\
	"const profileParamSectionsMap  = [\r\n"\
	"	{	profileName: '[1]', profileGroup: '[Group1]', sectionName: 'Info', sectionIdx: 1 },\r\n"\
	"	{	profileName: '[2]', profileGroup: '[GROUPMON]', sectionName: 'Dynamic', sectionIdx: 2 },\r\n"\
	"	{	profileName: '[3]', profileGroup: '[Group3]', sectionName: 'Config', sectionIdx: 3 },\r\n"\
	"	{	profileName: '[4]', profileGroup: '[Group4]', sectionName: 'Threshould', sectionIdx: 4 },\r\n"\
	"	{	profileName: '[5]', profileGroup: '[Group5]', sectionName: 'Led', sectionIdx: 5 },\r\n"\
	"	{	profileName: '[8]', profileGroup: '[Group8]', sectionName: 'Counter', sectionIdx: 8 },\r\n"\
	"	{	profileName: '[103]', profileGroup: '[Group103]', sectionName: 'Port', sectionIdx: 103 },\r\n"\
	"];\r\n"\
	"\r\n"\
	"function parseProfileText(profileText) {\r\n"\
	"	let profileSection;\r\n"\
	"	let profileSectionType;\r\n"\
	"	let profileSectionName;\r\n"\
	"	let profileThrSection;\r\n"\
	"\r\n"\
	"	profile.lines = profileText.split('\\0');\r\n"\
	"	for (let i = 0; i < profile.lines.length - 1; i++)	{\r\n"\
	"		if (profile.lines[i][0] == '[')	{\r\n"\
	"			let sectionMapItem;\r\n"\
	"\r\n"\
	"			sectionMapItem = profileParamSectionsMap.find(item => item.profileName == profile.lines[i]);\r\n"\
	"			if (sectionMapItem != undefined) {\r\n"\
	"				profileSection = createProfileParamSection(sectionMapItem.sectionName, sectionMapItem.sectionIdx);\r\n"\
	"				profile.sections.push(profileSection);\r\n"\
	"				profileSectionName = sectionMapItem.sectionName;\r\n"\
	"				profileSectionType = 'Param';\r\n"\
	"				continue;\r\n"\
	"			}\r\n"\
	"\r\n"\
	"			sectionMapItem = profileParamSectionsMap.find(item => item.profileGroup == profile.lines[i]);\r\n"\
	"			if (sectionMapItem != undefined) {\r\n"\
	"				profileSection = profile.sections.find(section => section.name == sectionMapItem.sectionName);\r\n"\
	"				profileSectionName = sectionMapItem.sectionName;\r\n"\
	"				profileSectionType = 'Group';\r\n"\
	"				continue;\r\n"\
	"			}\r\n"\
	"\r\n"\
	"			if (profile.lines[i] == '[EnumParam]') {\r\n"\
	"				profileSectionName = profile.lines[i];\r\n"\
	"				profileSectionType = 'Enum';\r\n"\
	"				continue;\r\n"\
	"			}\r\n"\
	"\r\n"\
	"			if (profile.lines[i] == '[BitmaskParam]') {\r\n"\
	"				profileSectionName = profile.lines[i];\r\n"\
	"				profileSectionType = 'Bitmask';\r\n"\
	"				continue;\r\n"\
	"			}\r\n"\
	"\r\n"\
	"			profileEnumSectionName = profileEnumSectionName = profile.lines[i];\r\n"\
	"			profileSectionType = 'Unknown';\r\n"\
	"		} else if (profileSectionType == 'Param') {\r\n"\
	"			let param = parseProfileParamLine(profileSectionName, profile.lines[i]);\r\n"\
	"			profile.params.push(param);\r\n"\
	"			profileSection.params.push(param);\r\n"\
	"		} else if (profileSectionType == 'Enum') {\r\n"\
	"			parseProfileEnumLine(profile.params, profile.lines[i]);\r\n"\
	"		} else if (profileSectionType == 'Bitmask') {\r\n"\
	"			parseProfileBitmskLine(profile.params, profile.lines[i]);\r\n"\
	"		} else if (profileSectionType == 'Group') {\r\n"\
	"			profileSection.groups.push(parseProfileGroupLine(profile.params, profile.lines[i]));\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"\r\n"\
	"	profileThrSection = profile.sections.find(section => section.name == 'Threshould');\r\n"\
	"	profileSection = profile.sections.find(section => section.name == 'Dynamic');\r\n"\
	"	if ((profileSection != undefined) && (profileThrSection != undefined)) {\r\n"\
	"		for (let i = 0; i < profileSection.params.length; i++) {\r\n"\
	"			for (let j = 0; j < profileThrNames.length; j++) {\r\n"\
	"				let profileParamThr = profileThrSection.params.find(param => param.name == (profileSection.params[i].name + profileThrNames[j]))\r\n"\
	"				if (profileParamThr != undefined)\r\n"\
	"					profileSection.params[i].thr.push(profileParamThr);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"	profileSection = profile.sections.find(section => section.name == 'Config');\r\n"\
	"	if ((profileSection != undefined) && (profileThrSection != undefined)) {\r\n"\
	"		for (let i = 0; i < profileSection.params.length; i++) {\r\n"\
	"			for (let j = 0; j < profileThrNames.length; j++) {\r\n"\
	"				let profileParamThr = profileThrSection.params.find(param => param.name == (profileSection.params[i].name + profileThrNames[j]))\r\n"\
	"				if (profileParamThr != undefined)\r\n"\
	"					profileSection.params[i].thr.push(profileParamThr);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function parseProfileColors(__profileColors) {\r\n"\
	"	let profileSection = profile.sections.find(section => section.name == 'Dynamic');\r\n"\
	"	let profileColors = __profileColors.split(';');\r\n"\
	"\r\n"\
	"	if (profileColors.length != profileSection.params.length)\r\n"\
	"		return;\r\n"\
	"\r\n"\
	"	for (let i = 0; i < profileSection.params.length; i++) {\r\n"\
	"		let paramColors = profileColors[i].split('@');\r\n"\
	"		if (paramColors.length == 2) {\r\n"\
	"			profileSection.params[i].colors.warning = parseInt(paramColors[0]);\r\n"\
	"			profileSection.params[i].colors.critical = parseInt(paramColors[1]);\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamColor(id, parent, param) {\r\n"\
	"	let td = $('<td/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_color',\r\n"\
	"		css : {\r\n"\
	"			'background-color' : 'transparent',\r\n"\
	"		},\r\n"\
	"	}).appendTo(parent);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamName(id, parent, param) {\r\n"\
	"	let td = $('<td/>').appendTo(parent);\r\n"\
	"	let name = $('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_name',\r\n"\
	"		class : 'profile_' + id +'_param_name font-weight-bold m-0',\r\n"\
	"	}).appendTo(td);\r\n"\
	"\r\n"\
	"	if (param.units != '')\r\n"\
	"		name.text(param.name + ', ' + param.units);\r\n"\
	"	else\r\n"\
	"		name.text(param.name);\r\n"\
	"\r\n"\
	"	$('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_descr',\r\n"\
	"		class : 'profile_' + id + '_param_descr font-weight-light m-0',\r\n"\
	"		text : param.descr,\r\n"\
	"	}).appendTo(td);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamValue(id, parent, param) {\r\n"\
	"	let td = $('<td/>').appendTo(parent);\r\n"\
	"	let p = $('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_p',\r\n"\
	"		class : 'profile_' + id + '_param_p m-0',\r\n"\
	"		text : getParamValueString(param),\r\n"\
	"	}).appendTo(td);\r\n"\
	"\r\n"\
	"	if (parseInt(param.access, 10) & ((1 << 1) | (1 << 3) | (1 << 5) | (1 << 7)))	{\r\n"\
	"		if (param.enum != undefined) {\r\n"\
	"			let select = $('<select/>', {\r\n"\
	"				id : 'profile_' + id + '_' + param.name + '_select',\r\n"\
	"				value : param.value,\r\n"\
	"				class : 'form-control',\r\n"\
	"				on : {\r\n"\
	"					submit : () => { profileSaveButtonOnClick(id, param); },\r\n"\
	"				},\r\n"\
	"				css : {\r\n"\
	"					display : 'none'\r\n"\
	"				},\r\n"\
	"			}).appendTo(td);\r\n"\
	"			for (let i = 0; i < param.enum.length; i++)	{\r\n"\
	"				$('<option/>', {\r\n"\
	"					value : param.enum[i].value,\r\n"\
	"					text : param.enum[i].string,\r\n"\
	"				}).appendTo(select);\r\n"\
	"			}\r\n"\
	"		}	else {\r\n"\
	"			$('<input/>', {\r\n"\
	"				id : 'profile_' + id + '_' + param.name + '_input',\r\n"\
	"				type : 'text',\r\n"\
	"				value : getParamValueString(param),\r\n"\
	"				class : 'form-control',\r\n"\
	"				on : {\r\n"\
	"					submit : () => { profileSaveButtonOnClick(id, param); },\r\n"\
	"				},\r\n"\
	"				css : {\r\n"\
	"					display : 'none'\r\n"\
	"				},\r\n"\
	"			}).appendTo(td);\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileParamValueEditButtonOnClick(id, param) {\r\n"\
	"	let p = $('#profile_' + id + '_' + param.name + '_p');\r\n"\
	"	let edit = $('#profile_' + id + '_' + param.name + '_edit');\r\n"\
	"	let save = $('#profile_' + id + '_' + param.name + '_save');\r\n"\
	"	let input = $('#profile_' + id + '_' + param.name + '_input');\r\n"\
	"	let select = $('#profile_' + id + '_' + param.name + '_select');\r\n"\
	"\r\n"\
	"	p.hide();\r\n"\
	"	edit.hide();\r\n"\
	"	if (input.length)	{\r\n"\
	"		input.val(param.value);\r\n"\
	"		input.show();\r\n"\
	"	}\r\n"\
	"	if (select.length) {\r\n"\
	"		select.val(param.value);\r\n"\
	"		select.show();\r\n"\
	"	}\r\n"\
	"	save.show();\r\n"\
	"	return false;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateProfileParamValue(id, param, value) {\r\n"\
	"	param.value = value;\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_p').text(getParamValueString(id, param));\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_spinner').hide();\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_edit').show()\r\n"\
	"}\r\n"\
	"\r\n"\
	"function putProfileParamValue(id, param, value) {\r\n"\
	"	if (!debug) {\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		let section = profile.sections.find(item => item.name == param.section);\r\n"\
	"		xhr.open('PUT', 'api/v1/profile/sections/' + section.idx + '/params/' + param.idx + '/value', true);\r\n"\
	"		xhr.send(value);\r\n"\
	"		xhr.onreadystatechange = function() {\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] putProfileParamValue() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				updateProfileParamValue(id, param, value);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		setTimeout(() => updateProfileParamValue(id, param, value), 1000);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileParamValueSaveButtonOnClick(id, param) {\r\n"\
	"	let p = $('#profile_' + id + '_' + param.name + '_p');\r\n"\
	"	let edit = $('#profile_' + id + '_' + param.name + '_edit');\r\n"\
	"	let save = $('#profile_' + id + '_' + param.name + '_save');\r\n"\
	"	let input = $('#profile_' + id + '_' + param.name + '_input');\r\n"\
	"	let select = $('#profile_' + id + '_' + param.name + '_select');\r\n"\
	"	let spinner = $('#profile_' + id + '_' + param.name + '_spinner');\r\n"\
	"	let value;\r\n"\
	"\r\n"\
	"	if (input.length)	{\r\n"\
	"		input.hide();\r\n"\
	"		value = input.val();\r\n"\
	"	}\r\n"\
	"	if (select.length)	{\r\n"\
	"		select.hide();\r\n"\
	"		value = select.val();\r\n"\
	"	}\r\n"\
	"	save.hide();\r\n"\
	"	edit.hide();\r\n"\
	"	p.show();\r\n"\
	"	spinner.show();\r\n"\
	"	putProfileParamValue(id, param, value);\r\n"\
	"	return false;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileParamValueDetailsButtonOnClick(id, param) {\r\n"\
	"	let col;\r\n"\
	"\r\n"\
	"	col = $('#profile_' + id + '_' + param.name + '_ccolor');\r\n"\
	"	if (col.length != 0)\r\n"\
	"		col.toggleClass('profile_param_section_group_hidden');\r\n"\
	"\r\n"\
	"	col = $('#profile_' + id + '_' + param.name + '_wcolor');\r\n"\
	"	if (col.length != 0)\r\n"\
	"		col.toggleClass('profile_param_section_group_hidden');\r\n"\
	"\r\n"\
	"	for (i = 0; i < profileThrNames.length; i++) {\r\n"\
	"		let thr = $('#profile_' + id + '_' + param.name + profileThrNames[i]);\r\n"\
	"		if (thr.length != 0)\r\n"\
	"			thr.toggleClass('profile_param_section_group_hidden');\r\n"\
	"	}\r\n"\
	"	return false;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamValueEdit(id, parent, param) {\r\n"\
	"	let td = $('<td/>').appendTo(parent);\r\n"\
	"	if (parseInt(param.access, 10) & ((1 << 1) | (1 << 3) | (1 << 5) | (1 << 7)))	{\r\n"\
	"		$('<button/>', {\r\n"\
	"			id : 'profile_' + id + '_' + param.name + '_edit',\r\n"\
	"			text : 'Edit',\r\n"\
	"			class : 'btn btn-primary',\r\n"\
	"			on : {\r\n"\
	"				click : () => { profileParamValueEditButtonOnClick(id, param); },\r\n"\
	"			}\r\n"\
	"		}).appendTo(td);\r\n"\
	"		$('<button/>', {\r\n"\
	"			id : 'profile_' + id + '_' + param.name + '_save',\r\n"\
	"			text : 'Save',\r\n"\
	"			class : 'btn btn-primary',\r\n"\
	"			on : {\r\n"\
	"				click : () => { profileParamValueSaveButtonOnClick(id, param); },\r\n"\
	"			},\r\n"\
	"			css : {\r\n"\
	"				display : 'none',\r\n"\
	"			}\r\n"\
	"		}).appendTo(td);\r\n"\
	"		$('<div/>', {\r\n"\
	"			id : 'profile_' + id + '_' + param.name + '_spinner',\r\n"\
	"			class : 'spinner-border text-primary',\r\n"\
	"			css : {\r\n"\
	"				display : 'none',\r\n"\
	"			}\r\n"\
	"		}).appendTo(td);\r\n"\
	"	}\r\n"\
	"	if (param.section == 'Dynamic') {\r\n"\
	"		if ((param.thr.length != 0) || (param.colors.warning != 0) || (param.colors.critical != 0)) {\r\n"\
	"			$('<button/>', {\r\n"\
	"				id : 'profile_' + id + '_' + param.name + '_save',\r\n"\
	"				text : '...',\r\n"\
	"				class : 'btn btn-primary',\r\n"\
	"				on : {\r\n"\
	"					click : () => { profileParamValueDetailsButtonOnClick(id, param); },\r\n"\
	"				},\r\n"\
	"			}).appendTo(td);\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileParamColorEditButtonOnClick(id, param, color) {\r\n"\
	"	let p = $('#profile_' + id + '_' + param.name + '_' + color + '_p');\r\n"\
	"	let edit = $('#profile_' + id + '_' + param.name + '_' + color + '_edit');\r\n"\
	"	let save = $('#profile_' + id + '_' + param.name + '_' + color + '_save');\r\n"\
	"	let select = $('#profile_' + id + '_' + param.name + '_' + color + '_select');\r\n"\
	"\r\n"\
	"	p.hide();\r\n"\
	"	edit.hide();\r\n"\
	"	if (select.length)	{\r\n"\
	"		if (color == 'wcolor')\r\n"\
	"			select.val(param.colors.warning);\r\n"\
	"		else if (color == 'ccolor')\r\n"\
	"			select.val(param.colors.critical);\r\n"\
	"		select.show();\r\n"\
	"		select.focus();\r\n"\
	"	}\r\n"\
	"	save.show();\r\n"\
	"	return false;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateProfileParamColor(id, param, color, value) {\r\n"\
	"	if (color == 'wcolor')\r\n"\
	"		param.colors.warning = value;\r\n"\
	"	else if (color == 'ccolor')\r\n"\
	"		param.colors.critical = value;\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_' + color + '_p').text(getParamColorString(value));\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_' + color + '_spinner').hide();\r\n"\
	"	$('#profile_' + id + '_' + param.name + '_' + color + '_edit').show()\r\n"\
	"}\r\n"\
	"\r\n"\
	"function putProfileParamColor(id, param, color, value) {\r\n"\
	"	if (!debug) {\r\n"\
	"		let body;\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		let section = profile.sections.find(item => item.name == param.section);\r\n"\
	"		xhr.open('PUT', 'api/v1/profile/sections/' + section.idx + '/params/' + param.idx + '/colors', true);\r\n"\
	"		if (color == 'wcolor')\r\n"\
	"			body = value + ';' + param.colors.critical;\r\n"\
	"		else if (color == 'ccolor')\r\n"\
	"			body = param.colors.warning + ';' + value;\r\n"\
	"		xhr.send(body);\r\n"\
	"		xhr.onreadystatechange = function() {\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] putProfileParamColor() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				updateProfileParamColor(id, param, color, value);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		setTimeout(() => updateProfileParamColor(id, param, color, value), 1000);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileParamColorSaveButtonOnClick(id, param, color) {\r\n"\
	"	let p = $('#profile_' + id + '_' + param.name + '_' + color + '_p');\r\n"\
	"	let edit = $('#profile_' + id + '_' + param.name + '_' + color + '_edit');\r\n"\
	"	let save = $('#profile_' + id + '_' + param.name + '_' + color + '_save');\r\n"\
	"	let select = $('#profile_' + id + '_' + param.name + '_' + color + '_select');\r\n"\
	"	let spinner = $('#profile_' + id + '_' + param.name + '_' + color + '_spinner');\r\n"\
	"	let value;\r\n"\
	"\r\n"\
	"	if (select.length) {\r\n"\
	"		select.hide();\r\n"\
	"		value = select.val();\r\n"\
	"	}\r\n"\
	"	save.hide();\r\n"\
	"	edit.hide();\r\n"\
	"	p.show();\r\n"\
	"	spinner.show();\r\n"\
	"	putProfileParamColor(id, param, color, value);\r\n"\
	"	return false;\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamWDColor(id, parent, param, color) {\r\n"\
	"	let tr = $('<tr/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '',\r\n"\
	"		class : 'profile_' + id + '_param profile_' + id + '_color',\r\n"\
	"	}).appendTo(parent);\r\n"\
	"\r\n"\
	"	let td = $('<td/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_color',\r\n"\
	"		css : {\r\n"\
	"			'background-color' : 'transparent',\r\n"\
	"		},\r\n"\
	"	}).appendTo(tr);\r\n"\
	"\r\n"\
	"	td = $('<td/>').appendTo(tr);\r\n"\
	"	let p = $('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_name',\r\n"\
	"		class : 'profile_' + id +'_param_' + color + '_name font-weight-bold m-0',\r\n"\
	"	}).appendTo(td);\r\n"\
	"	if (color == 'wcolor') {\r\n"\
	"		p.text(param.name + 'WColor');\r\n"\
	"	}	else if (color == 'ccolor') {\r\n"\
	"		p.text(param.name + 'CColor');\r\n"\
	"	}\r\n"\
	"\r\n"\
	"	p = $('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_descr',\r\n"\
	"		class : 'profile_' + id + '_param_' + color + '_descr font-weight-light m-0',\r\n"\
	"	}).appendTo(td);\r\n"\
	"	if (color == 'wcolor') {\r\n"\
	"		p.text(param.name + ' warning color');\r\n"\
	"	}	else if (color == 'ccolor') {\r\n"\
	"		p.text(param.name + ' critical color');\r\n"\
	"	}\r\n"\
	"\r\n"\
	"	td = $('<td/>').appendTo(tr);\r\n"\
	"	p = $('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_p',\r\n"\
	"		class : 'profile_' + id + '_param_p m-0',\r\n"\
	"		text : getParamColorString(param.color),\r\n"\
	"	}).appendTo(td);\r\n"\
	"	if (color == 'wcolor') {\r\n"\
	"		p.text(getParamColorString(param.colors.warning));\r\n"\
	"	}	else if (color == 'ccolor') {\r\n"\
	"		p.text(getParamColorString(param.colors.critical));\r\n"\
	"	}\r\n"\
	"\r\n"\
	"	let select = $('<select/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_select',\r\n"\
	"		value : param.value,\r\n"\
	"		class : 'form-control',\r\n"\
	"		on : {\r\n"\
	"			submit : () => { profileParamColorSaveButtonOnClick(id, param, color); },\r\n"\
	"		},\r\n"\
	"		css : {\r\n"\
	"			display : 'none'\r\n"\
	"		},\r\n"\
	"	}).appendTo(td);\r\n"\
	"	for (let i = 3; i <= 7; i++) {\r\n"\
	"		$('<option/>', {\r\n"\
	"			value : i,\r\n"\
	"			text : getParamColorString(i),\r\n"\
	"		}).appendTo(select);\r\n"\
	"	}\r\n"\
	"\r\n"\
	"	td = $('<td/>').appendTo(tr);\r\n"\
	"	$('<button/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_edit',\r\n"\
	"		text : 'Edit',\r\n"\
	"		class : 'btn btn-primary',\r\n"\
	"		on : {\r\n"\
	"			click : () => { profileParamColorEditButtonOnClick(id, param, color); },\r\n"\
	"		}\r\n"\
	"	}).appendTo(td);\r\n"\
	"	$('<button/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_save',\r\n"\
	"		text : 'Save',\r\n"\
	"		class : 'btn btn-primary',\r\n"\
	"		on : {\r\n"\
	"			click : () => { profileParamColorSaveButtonOnClick(id, param, color); },\r\n"\
	"		},\r\n"\
	"		css : {\r\n"\
	"			display : 'none',\r\n"\
	"		}\r\n"\
	"	}).appendTo(td);\r\n"\
	"	$('<div/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name + '_' + color + '_spinner',\r\n"\
	"		class : 'spinner-border text-primary',\r\n"\
	"		css : {\r\n"\
	"			display : 'none',\r\n"\
	"		}\r\n"\
	"	}).appendTo(td);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamThr(id, parent, param) {\r\n"\
	"	for (let i = 0; i < param.thr.length; i++) {\r\n"\
	"		tr = $('<tr/>', {\r\n"\
	"			id : 'profile_' + id + '_' + param.thr[i].name,\r\n"\
	"			class : 'profile_' + id + '_param',\r\n"\
	"		}).appendTo(parent);\r\n"\
	"		for (let j = 0; j < param.thr[i].groups.length; j++)\r\n"\
	"			tr.addClass('profile_' + id + '_param_section_' + param.thr[i].section + '_group_' + param.thr[i].groups[j].trim().replace(/[^a-zA-Z0-9]/g, '_'));\r\n"\
	"		appendProfileParamColor(id, tr, param.thr[i]);\r\n"\
	"		appendProfileParamName(id, tr, param.thr[i]);\r\n"\
	"		appendProfileParamValue(id, tr, param.thr[i]);\r\n"\
	"		appendProfileParamValueEdit(id, tr, param.thr[i]);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParam(id, parent, param) {\r\n"\
	"	if (param.section == 'Threshould')\r\n"\
	"		return;\r\n"\
	"	let tr = $('<tr/>', {\r\n"\
	"		id : 'profile_' + id + '_' + param.name,\r\n"\
	"		class : 'profile_' + id + '_param',\r\n"\
	"	}).appendTo(parent);\r\n"\
	"	for (let i = 0; i < param.groups.length; i++)\r\n"\
	"		tr.addClass('profile_' + id + '_param_section_' + param.section + '_group_' + param.groups[i].trim().replace(/[^a-zA-Z0-9]/g, '_'));\r\n"\
	"	appendProfileParamColor(id, tr, param);\r\n"\
	"	appendProfileParamName(id, tr, param);\r\n"\
	"	appendProfileParamValue(id, tr, param);\r\n"\
	"	appendProfileParamValueEdit(id, tr, param);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileParamsTable (id, parent) {\r\n"\
	"	let profileTableDiv = $('<div/>', {\r\n"\
	"		id : 'profile_' + id + '_table_div',\r\n"\
	"		class : 'row p-2',\r\n"\
	"	}).appendTo(parent);\r\n"\
	"\r\n"\
	"	profileTableDiv.append (\r\n"\
	"		'<table class=\\'table\\' id=\\'profile_' + id + '\\'_table\\'>' +\r\n"\
	"			'<thead id=\\'profile_' + id + '_table_thead\\'>' +\r\n"\
	"				'<tr>' +\r\n"\
	"					'<th scope=\\'col\\' style=\\'text-align:left; width:30px\\'></th>' +\r\n"\
	"				  '<th scope=\\'col\\' style=\\'text-align:left\\'><p class=\\'m-0\\'>Name</p></th>' +\r\n"\
	"				  '<th scope=\\'col\\' style=\\'text-align:left\\'><p class=\\'m-0\\'>Value</p></th>' +\r\n"\
	"				  '<th scope=\\'col\\' style=\\'text-align:left; width:90px\\'></th>' +\r\n"\
	"				'</tr>' +\r\n"\
	"			'</thead>' +\r\n"\
	"			'<tbody id=\\'profile_' + id + '_table_tbody\\'>' +\r\n"\
	"			'</tbody>' +\r\n"\
	"		'</table>'\r\n"\
	"	);\r\n"\
	"	let tbody = $('#profile_' + id + '_table_tbody');\r\n"\
	"	for (let i = 0; i < profile.params.length; i++) {\r\n"\
	"		appendProfileParam(id, tbody, profile.params[i]);\r\n"\
	"		if (profile.params[i].thr != undefined)\r\n"\
	"			appendProfileParamThr(id, tbody, profile.params[i]);\r\n"\
	"		if ((profile.params[i].section == 'Dynamic') && (profile.params[i].colors.warning != 0))\r\n"\
	"			appendProfileParamWDColor(id, tbody, profile.params[i], 'wcolor');\r\n"\
	"		if ((profile.params[i].section == 'Dynamic') && (profile.params[i].colors.critical != 0))\r\n"\
	"			appendProfileParamWDColor(id, tbody, profile.params[i], 'ccolor');\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileMenuSectionSelectOnChange(id) {\r\n"\
	"	$('.profile_' + id + '_menu_section_group_select').hide();\r\n"\
	"	let section = $('#profile_' + id + '_menu_section_select option:selected').text();\r\n"\
	"	let sectionGroupsSelect = $('#profile_' + id + '_menu_section_' + section + '_group_select');\r\n"\
	"	sectionGroupsSelect.value = 0;\r\n"\
	"	sectionGroupsSelect.show();\r\n"\
	"	profileMenuSectionGroupSelectOnChange(id, section);\r\n"\
	"}\r\n"\
	"\r\n"\
	"function profileMenuSectionGroupSelectOnChange(id, section) {\r\n"\
	"	$('.profile_' + id + '_param').addClass('profile_param_section_group_hidden');\r\n"\
	"	let group = $('#profile_' + id + '_menu_section_' + section + '_group_select option:selected').text().trim().replace(/[^a-zA-Z0-9]/g, '_');\r\n"\
	"	$('.profile_' + id + '_param_section_' + section + '_group_'+ group).removeClass('profile_param_section_group_hidden');\r\n"\
	"}\r\n"\
	"\r\n"\
	"function appendProfileMenu (id, parent) {\r\n"\
	"	let profileMenuDiv = $('<div/>', {\r\n"\
	"		id : 'profile_' + id + '_menu_div',\r\n"\
	"		class: 'row align-items-center flex-row p-2',\r\n"\
	"	}).appendTo(parent);\r\n"\
	"\r\n"\
	"	$('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_menu_section_p',\r\n"\
	"		text : 'Section:',\r\n"\
	"		class: 'm-0 p-2',\r\n"\
	"		css : {\r\n"\
	"			width : 'auto',\r\n"\
	"			display : 'inline-block',\r\n"\
	"		}\r\n"\
	"	}).appendTo(profileMenuDiv);\r\n"\
	"	let sectionSelect = $('<select/>', {\r\n"\
	"		id : 'profile_' + id + '_menu_section_select',\r\n"\
	"		class : 'form-control',\r\n"\
	"		on : {\r\n"\
	"			change : () => { profileMenuSectionSelectOnChange(id); },\r\n"\
	"		},\r\n"\
	"		css : {\r\n"\
	"			width : 'auto',\r\n"\
	"			display : 'inline-block',\r\n"\
	"		}\r\n"\
	"	}).appendTo(profileMenuDiv);\r\n"\
	"	$('<p/>', {\r\n"\
	"		id : 'profile_' + id + '_menu_group_p',\r\n"\
	"		text : 'Group:',\r\n"\
	"		class: 'm-0 p-2',\r\n"\
	"		css : {\r\n"\
	"			width : 'auto',\r\n"\
	"			display : 'inline-block',\r\n"\
	"		}\r\n"\
	"	}).appendTo(profileMenuDiv);\r\n"\
	"	for (let i = 0; i < profile.sections.length; i++)	{\r\n"\
	"		let sectionSelectOption = $('<option/>', {\r\n"\
	"			value : profile.sections[i].idx,\r\n"\
	"			text : profile.sections[i].name,\r\n"\
	"		});\r\n"\
	"		sectionSelectOption.appendTo(sectionSelect);\r\n"\
	"\r\n"\
	"		let sectionGroupSelect = $('<select/>', {\r\n"\
	"			id : 'profile_' + id + '_menu_section_' + profile.sections[i].name + '_group_select',\r\n"\
	"			class : 'profile_' + id + '_menu_section_group_select form-control',\r\n"\
	"			on : {\r\n"\
	"				change : () => { profileMenuSectionGroupSelectOnChange(id, profile.sections[i].name); },\r\n"\
	"			},\r\n"\
	"			css : {\r\n"\
	"				width : 'auto',\r\n"\
	"				display : 'inline-block',\r\n"\
	"			}\r\n"\
	"		});\r\n"\
	"		sectionGroupSelect.appendTo(profileMenuDiv);\r\n"\
	"\r\n"\
	"		let sectionGroupSelectOption = $('<option/>', {\r\n"\
	"			value : 0,\r\n"\
	"			text : 'All',\r\n"\
	"		});\r\n"\
	"		sectionGroupSelectOption.appendTo(sectionGroupSelect);\r\n"\
	"\r\n"\
	"		for (let j = 0; j < profile.sections[i].groups.length; j++)\r\n"\
	"		{\r\n"\
	"			let sectionGroupSelectOption = $('<option/>', {\r\n"\
	"				value : profile.sections[i].groups[j].idx,\r\n"\
	"				text : profile.sections[i].groups[j].name,\r\n"\
	"			});\r\n"\
	"			sectionGroupSelectOption.appendTo(sectionGroupSelect);\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getProfileColors(callback) {\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/profile/sections/2/colors', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() { // (3)\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getProfile() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				parseProfileColors(xhr.responseText);\r\n"\
	"				if (callback != undefined)\r\n"\
	"					callback();\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		parseProfileColors(profileColors);\r\n"\
	"		if (callback != undefined)\r\n"\
	"			callback();\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getProfile(callback) {\r\n"\
	"	console.log('getProfile');\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/profile', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() { // (3)\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getProfile() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				parseProfileText(xhr.responseText);\r\n"\
	"				if (callback != undefined)\r\n"\
	"					callback();\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else	{\r\n"\
	"		parseProfileText(profileText.join('\\0'));\r\n"\
	"		if (callback != undefined)\r\n"\
	"			callback();\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateSystemOS(systemOS) {\r\n"\
	"	if (systemOS.heap != undefined ) {\r\n"\
	"		$('#system-os-cpu-heap-value').text(systemOS.heap);\r\n"\
	"	}\r\n"\
	"	if (systemOS.threads != undefined) {\r\n"\
	"    let threads_tbody = $('#system-os-threads-tbody');\r\n"\
	"		for (let i = 0; i < systemOS.threads.length; i++ ) {\r\n"\
	"			let thread_name = systemOS.threads[i].name.trim().replace(/[^a-zA-Z0-9]/g, '-');\r\n"\
	"			let thread_tr = $('#system-os-thread-' + thread_name + '-tr');\r\n"\
	"			if (thread_tr.length) {\r\n"\
	"				$('#system-os-thread-' + thread_name + '-name-td').text(systemOS.threads[i].name);\r\n"\
	"				$('#system-os-thread-' + thread_name + '-state-td').text(systemOS.threads[i].state);\r\n"\
	"				$('#system-os-thread-' + thread_name + '-priority-td').text(systemOS.threads[i].priority);\r\n"\
	"				$('#system-os-thread-' + thread_name + '-stack-td').text(systemOS.threads[i].stack);\r\n"\
	"				$('#system-os-thread-' + thread_name + '-number-td').text(systemOS.threads[i].number);\r\n"\
	"			}	else {\r\n"\
	"				thread_tr = $('<tr/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-tr',\r\n"\
	"				}).appendTo(threads_tbody);\r\n"\
	"				$('<td/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-name-td',\r\n"\
	"					text : systemOS.threads[i].name,\r\n"\
	"				}).appendTo(thread_tr);\r\n"\
	"				$('<td/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-state-td',\r\n"\
	"					text : systemOS.threads[i].state,\r\n"\
	"				}).appendTo(thread_tr);\r\n"\
	"				$('<td/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-priority-td',\r\n"\
	"					text : systemOS.threads[i].priority,\r\n"\
	"				}).appendTo(thread_tr);\r\n"\
	"				$('<td/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-stack-td',\r\n"\
	"					text : systemOS.threads[i].stack,\r\n"\
	"				}).appendTo(thread_tr);\r\n"\
	"				$('<td/>', {\r\n"\
	"					id : 'system-os-thread-' + thread_name + '-number-td',\r\n"\
	"					text : systemOS.threads[i].number,\r\n"\
	"				}).appendTo(thread_tr);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getSystemOS() {\r\n"\
	"	console.log('getSystemOS');\r\n"\
	"	clearTimeout(timerId);\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/system/os', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() { // (3)\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getSystemOS() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				let systemOS = JSON.parse(xhr.responseText);\r\n"\
	"				updateSystemOS(systemOS);\r\n"\
	"				timerId = setTimeout(() => getSystemOS(), 1000);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		timerId = setTimeout(() => getSystemOS(), 1000);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateSystemHardware(systemHardware) {\r\n"\
	"	if (systemHardware.power != undefined )	{\r\n"\
	"		let power_tbody = $('#system-hardware-power-tbody');\r\n"\
	"		power_tbody.empty();\r\n"\
	"		for (let i = 0; i < systemHardware.power.length; i++) {\r\n"\
	"			power_tbody.append(\r\n"\
	"				'<tr>' +\r\n"\
	"					'<td>'+ systemHardware.power[i].config + '</td>' +\r\n"\
	"					'<td>'+ systemHardware.power[i].monitor  + '</td>' +\r\n"\
	"				'</tr>'\r\n"\
	"			);\r\n"\
	"		}\r\n"\
	"	}\r\n"\
	"	if (systemHardware.ic != undefined) {\r\n"\
	"    let ic_tbody = $('#system-hardware-ic-tbody');\r\n"\
	"    ic_tbody.empty();\r\n"\
	"    for (let property in systemHardware.ic) {\r\n"\
	"    	if ($.isArray(systemHardware.ic[property])) {\r\n"\
	"    		for (let i = 0; i < systemHardware.ic[property].length; i++) {\r\n"\
	"    			ic_tbody.append(\r\n"\
	"				'<tr>' +\r\n"\
	"					'<td>'+ property + '</td>' +\r\n"\
	"					'<td>'+ systemHardware.ic[property][i].id + '</td>' +\r\n"\
	"					'<td>'+ systemHardware.ic[property][i].status  + '</td>' +\r\n"\
	"				'</tr>'\r\n"\
	"	        );\r\n"\
	"    		}\r\n"\
	"    	}	else {\r\n"\
	"    		ic_tbody.append(\r\n"\
	"					'<tr>' +\r\n"\
	"						'<td>'+ property + '</td>' +\r\n"\
	"						'<td>'+ systemHardware.ic[property].id + '</td>' +\r\n"\
	"						'<td>'+ systemHardware.ic[property].status  + '</td>' +\r\n"\
	"					'</tr>'\r\n"\
	"				);\r\n"\
	"    	}\r\n"\
	"    }\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getSystemHardware() {\r\n"\
	"	console.log('getSystemHardware');\r\n"\
	"	clearTimeout(timerId);\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/system/hardware', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() {\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getSystemHardware() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				let systemHardware = JSON.parse(xhr.responseText);\r\n"\
	"				updateSystemHardware(systemHardware);\r\n"\
	"				timerId = setTimeout(() => getSystemHardware(), 1000);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else	{\r\n"\
	"		let systemHardware = JSON.parse(system_hardware_json);\r\n"\
	"		updateSystemHardware(systemHardware);\r\n"\
	"		timerId = setTimeout(() => getSystemHardware(), 1000);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function updateLog(log) {\r\n"\
	"	let log_tbody = $('#log-tbody');\r\n"\
	"	log_tbody.empty();\r\n"\
	"	for (let i = 0; i < log.records.length; i++) {\r\n"\
	"		log_tbody.append(\r\n"\
	"			'<tr>' +\r\n"\
	"				'<td>'+ log.records[i].time + '</td>' +\r\n"\
	"				'<td>'+ log.records[i].name + '</td>' +\r\n"\
	"				'<td>'+ log.records[i].id + '</td>' +\r\n"\
	"				'<td>'+ log.records[i].data + '</td>' +\r\n"\
	"			'</tr>'\r\n"\
	"		);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function getLog() {\r\n"\
	"	console.log('getLog');\r\n"\
	"	clearTimeout(timerId);\r\n"\
	"	if (!debug)	{\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('GET', 'api/v1/log', true);\r\n"\
	"		xhr.send();\r\n"\
	"		xhr.onreadystatechange = function() {\r\n"\
	"			if (xhr.readyState != 4)\r\n"\
	"				return;\r\n"\
	"			if (xhr.status != 200) {\r\n"\
	"				console.log('[Error] getLog() Status: ' + xhr.status);\r\n"\
	"			} else {\r\n"\
	"				let log = JSON.parse(xhr.responseText);\r\n"\
	"				updateLog(log);\r\n"\
	"				//timerId = setTimeout(() => getLog(), 1000);\r\n"\
	"			}\r\n"\
	"		}\r\n"\
	"	}	else {\r\n"\
	"		let log = JSON.parse(log_json);\r\n"\
	"		updateLog(log);\r\n"\
	"	}\r\n"\
	"}\r\n"\
	"\r\n"\
	"function registerNavbarCallbacks() {\r\n"\
	"	$('.nav-item .nav-link').on('click', function(){\r\n"\
	"		$('.nav-item').find('.active').removeClass('active');\r\n"\
	"		$(this).addClass('active');\r\n"\
	"	});\r\n"\
	"	$('#nav-profile').on('click', function(){\r\n"\
	"		$('.content').hide();\r\n"\
	"		$('#profile-content').show();\r\n"\
	"		clearTimeout(timerId);\r\n"\
	"		getProfileSections();\r\n"\
	"	});\r\n"\
	"	$('#nav-system-update').on('click', function(){\r\n"\
	"		$('.content').hide();\r\n"\
	"		$('#system-update-content').show();\r\n"\
	"		clearTimeout(timerId);\r\n"\
	"	});\r\n"\
	"	$('#nav-system-os').on('click', function(){\r\n"\
	"		$('.content').hide();\r\n"\
	"		$('#system-os-content').show();\r\n"\
	"		clearTimeout(timerId);\r\n"\
	"		getSystemOS();\r\n"\
	"	});\r\n"\
	"	$('#nav-system-hardware').on('click', function(){\r\n"\
	"		$('.content').hide();\r\n"\
	"		$('#system-hardware-content').show();\r\n"\
	"		clearTimeout(timerId);\r\n"\
	"		getSystemHardware();\r\n"\
	"	});\r\n"\
	"	$('#nav-log').on('click', function(){\r\n"\
	"		$('.content').hide();\r\n"\
	"		$('#log-content').show();\r\n"\
	"		clearTimeout(timerId);\r\n"\
	"		getLog();\r\n"\
	"	});\r\n"\
	"}\r\n"\
	"\r\n"\
	"function registerUploadCallbacks() {\r\n"\
	"	$(\"#system-update-software-btn\").on('click', function() {\r\n"\
	"		let file_data = $('#system-update-software-input').prop('files')[0];\r\n"\
	"		//let form_data = new FormData();\r\n"\
	"		//form_data.append('file', file_data);\r\n"\
	"		let xhr = new XMLHttpRequest();\r\n"\
	"		xhr.open('POST', '/api/v1/system/update/software', true);\r\n"\
	"		xhr.upload.onprogress = function(event) {\r\n"\
	"		  console.log( 'Загружено на сервер ' + event.loaded + ' байт из ' + event.total );\r\n"\
	"		}\r\n"\
	"		xhr.upload.onload = function() {\r\n"\
	"		  console.log( 'Данные полностью загружены на сервер!' );\r\n"\
	"		}\r\n"\
	"		xhr.upload.onerror = function() {\r\n"\
	"		  console.log( 'Произошла ошибка при загрузке данных на сервер!' );\r\n"\
	"		}\r\n"\
	"		xhr.send(file_data);\r\n"\
	"	});\r\n"\
	"}\r\n"\
	"\r\n"\
	"function DOMContentLoadedEvent () {\r\n"\
	"	registerNavbarCallbacks();\r\n"\
	"	registerUploadCallbacks();\r\n"\
	"	getProfile( () => {\r\n"\
	"		getProfileColors( () => {\r\n"\
	"			let profileDiv = $('#profile');\r\n"\
	"			profileDiv.empty();\r\n"\
	"			let profile1Div = $('<div/>', {\r\n"\
	"				id : 'profile_1',\r\n"\
	"				class : 'col overflow-auto',\r\n"\
	"				css : {\r\n"\
	"					height : '100%',\r\n"\
	"				},\r\n"\
	"			}).appendTo(profileDiv);\r\n"\
	"			let profile2Div = $('<div/>', {\r\n"\
	"				id : 'profile_2',\r\n"\
	"				class : 'col overflow-auto d-sm-none d-md-block',\r\n"\
	"				css : {\r\n"\
	"					height : '100%',\r\n"\
	"				},\r\n"\
	"			}).appendTo(profileDiv);\r\n"\
	"			appendProfileMenu(1, profile1Div);\r\n"\
	"			appendProfileParamsTable(1, profile1Div);\r\n"\
	"			profileMenuSectionSelectOnChange(1, profile1Div);\r\n"\
	"			appendProfileMenu(2, profile2Div);\r\n"\
	"			appendProfileParamsTable(2, profile2Div);\r\n"\
	"			profileMenuSectionSelectOnChange(2, profile2Div);\r\n"\
	"			getProfileSections();\r\n"\
	"		});\r\n"\
	"	});\r\n"\
	"}\r\n"\
	"\r\n"\
	"document.addEventListener('DOMContentLoaded', DOMContentLoadedEvent);\r\n"\
	"\r\n"\
;

const unsigned int js_script_js_size = sizeof(js_script_js) - 1;

