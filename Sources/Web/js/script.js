let timerId;
let debug = 0;

let profile = {
	lines: [],
	params: [],
	sections: []
}

function showToast(severity, name, data) {
	let toastContent = $('#toast-content');
	let toast = $('<div/>', {
		class : 'toast',
		'data-delay' : '10000',
		css : {
			width : '300px',
		},
	}).prependTo(toastContent);
	if (severity == 'alarm')
		toast.css('background-color', '#e7a8ad');
	else if (severity == 'normal')
		toast.css('background-color', '#b6d8be');
	else
		toast.css('background-color', 'transparent');
	toast.append(
		'<div class="toast-header">' +
	    '<strong class="mr-auto">'+ name + '</strong>' +
	    '<button type="button" class="ml-2 mb-1 close" data-dismiss="toast" aria-label="Close">' +
	      '<span aria-hidden="true">&times;</span>' +
	    '</button>' +
	  '</div>' +
	  '<div class="toast-body">' + data + '</div>'
	);
	toast.toast('show');
}

function getParamColorString(color) {
	if (color == 3) {
		return 'Normal';
	}	else if (color == 4) {
		return 'Warning';
	}	else if (color == 5) {
		return 'Minor';
	}	else if (color == 6) {
		return 'Major';
	}	else if (color == 7) {
		return 'Critical';
	}
		return '';
}

function getParamValueString(param) {
	if (param.enum != undefined)	{
		for (i = 0; i < param.enum.length; i++)		{
			if (param.value == param.enum[i].value)
				return param.enum[i].string;
		}
	}	else if (param.bitmask != undefined)	{
		let result;
		let value = parseInt(param.value, 10);
		if (value == 0)
			return param.bitmask[0].string;
		for (i = 1; i < param.bitmask.length; i++)		{
			if (parseInt(param.value, 10) & param.bitmask[i].mask)			{
				if (result == undefined)
					result = param.bitmask[i].string;
				else
					result += ',' +  param.bitmask[i].string;
			}
		}
		return result;
	}
	return param.value;
}

function updateProfileTable(section) {
	for (let id = 1; id <= 2; id++) {
		for (let i = 0; i < section.params.length; i++) {
			if (section.params[i].hidden)
				$('#profile_' + id + '_' + section.params[i].name).addClass('profile_param_value_hidden');
			else {
				$('#profile_' + id + '_' + section.params[i].name).removeClass('profile_param_value_hidden');
				let value = $('#profile_' + id + '_' + section.params[i].name + '_p');
				if (value.length)
					value.text(getParamValueString(section.params[i]));
				if (section.name == 'Dynamic') {
					let color = $('#profile_' + id + '_' + section.params[i].name + '_color');
					if (section.params[i].colors.current == 7) {
						color.css('background-color', '#e7a8ad');
					}	else if (section.params[i].colors.current == 6) {
						color.css('background-color', '#f0c4a1');
					} else if (section.params[i].colors.current == 5) {
						color.css('background-color', '#f7e49c');
					} else if (section.params[i].colors.current == 4) {
						color.css('background-color', '#a1c4d6');
					} else if (section.params[i].colors.current == 3) {
						color.css('background-color', '#b6d8be');
					} else {
						color.css('background-color', 'transparent');
					}
				}
			}
		}
	}
}

function parseProfileSectionValues(section, tableText) {
	let table;
	table = tableText.split(';');
	if (section.params.length != table.length)
		return;
	for (let i = 0; i < table.length; i++) {
		if (table[i] == '\x01') {
			section.params[i].hidden = 1;
		}	else {
			section.params[i].hidden = 0;
			if (section.name == 'Dynamic') {
				let values = table[i].split('@');
				section.params[i].value = values[0];
				if (values.length > 1)
					section.params[i].colors.current = values[1];
				else
					section.params[i].colors.current = 0;
			}	else {
				section.params[i].value = table[i];
			}
	  }
	}
}

function getProfileSectionValues (sectionIdx, callback) {
	console.log('getProfileSectionValues: ' + profile.sections[sectionIdx].idx);
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/profile/sections/' + profile.sections[sectionIdx].idx + '/values', true);
		xhr.send();
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getProfileSectionValues() Status: ' + xhr.status);
				showToast('alarm', 'Profile', 'Get section ' + profile.sections[sectionIdx].idx + ' values: failed');
			} else {
				parseProfileSectionValues(profile.sections[sectionIdx], xhr.responseText);
				updateProfileTable(profile.sections[sectionIdx]);
				if (callback != undefined)
					callback(sectionIdx);
			}
		}
	}	else {
		if (profile.sections[sectionIdx].name == 'Info') {
			parseProfileSectionValues(profile.sections[sectionIdx], profileInfoValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Dynamic') {
			parseProfileSectionValues(profile.sections[sectionIdx], profileDynamicValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Config') {
			parseProfileSectionValues(profile.sections[sectionIdx], profileConfigValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Threshould') {
			parseProfileSectionValues(profile.sections[sectionIdx], profileThrValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Led') {
			parseProfileSectionValues(profile.sections[sectionIdx], profileLedValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Counter') {
			parseProfileSectionValues(profile.sections[sectionIdx], profilePerfValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}	else if (profile.sections[sectionIdx].name == 'Port') {
			parseProfileSectionValues(profile.sections[sectionIdx], profilePerfValues);
			updateProfileTable(profile.sections[sectionIdx]);
		}
		if (callback != undefined)
			callback(sectionIdx);
	}
}

function getProfileSectionValueCallback(sectionIdx) {
	sectionIdx = (sectionIdx + 1) % profile.sections.length;
	timerId = setTimeout(() => getProfileSectionValues(sectionIdx, getProfileSectionValueCallback), 100);
}

function getProfileSections() {
	console.log('getProfileSections');
	let sectionIdx = 0;
	clearTimeout(timerId);
	getProfileSectionValues(0, getProfileSectionValueCallback);
}

function parseProfileParamLine(section, line) {
	let line_words = line.split(';');
	let param = {};
	param.section = section;
	param.groups = [];
	param.groups[0] = 'All';
	param.name = line_words[0];
	param.idx = line_words[1];
	param.type = line_words[2];
	param.access = line_words[3];
	param.units = line_words[4];
	param.descr = line_words[6];
	param.value = '0';
	param.hidden = 0;
	return param;
}

const profileThrNames = ['HMax', 'CMax', 'WMax', 'WMin', 'CMin', 'HMin'];

function parseProfileParamLine(section, line) {
	let line_words = line.split(';');
	let param = {};
	param.section = section;
	param.groups = [];
	param.groups[0] = 'All';
	param.name = line_words[0];
	param.idx = line_words[1];
	param.type = line_words[2];
	param.access = line_words[3];
	param.units = line_words[4];
	param.descr = line_words[6];
	param.value = '0';
	if (section == 'Dynamic') {
		param.thr = [];
		param.colors = {};
		param.colors.current = 0;
		param.colors.warning = 0;
		param.colors.critical = 0;
	}
	if (section == 'Config') {
		param.thr = [];
	}
	return param;
}

function parseProfileEnumLine(params, line) {
	let line_words = line.split(';');
	for (let i = 0; i < params.length; i++)	{
		if (line_words[0] == params[i].name) {
			let line_words_enums = line_words[1].split('|');
			params[i].enum = [];
			for (let j = 0; j < line_words_enums.length; j++)	{
				let line_words_enums_values = line_words_enums[j].split('@');
				params[i].enum[j] = {};
				params[i].enum[j].string = line_words_enums_values[0];
				params[i].enum[j].value = line_words_enums_values[1];
			}
		}
	}
}

function parseProfileBitmskLine(params, line) {
	let line_words = line.split(';');
	for (let i = 0; i < params.length; i++)	{
		if (line_words[0] == params[i].name) {
			let line_words_bitmsks = line_words[2].split('|');
			params[i].bitmask = [];
			params[i].bitmask[0] = {};
			params[i].bitmask[0].mask = 0
			params[i].bitmask[0].string = line_words[1];
			for (let j = 0; j < line_words_bitmsks.length; j++) {
				let line_words_bitmsks_values = line_words_bitmsks[j].split('@');
				params[i].bitmask[j + 1] = {};
				params[i].bitmask[j + 1].string = line_words_bitmsks_values[0];
				params[i].bitmask[j + 1].mask = 1 << parseInt(line_words_bitmsks_values[1], 10);
			}
		}
	}
}

function parseProfileGroupLine(params, line) {
	let group_info = line.split(';');
	let group_params = group_info[2].split('|');
	let group = {};

	group.idx = group_info[0];
	group.name = group_info[1];
	group.params = [];

	for (let i = 0; i < group_params.length; i++)	{
		for (let j = 0; j < params.length; j++) {
			if (params[j].name == group_params[i]) {
				group.params[i] = params[j];
				params[j].groups[params[j].groups.length] = group.name;
			}
		}
	}
	return group;
}

function createProfileParamSection(name, idx) {
	let section = {};
	section.idx = idx;
	section.name = name;
	section.params = [];
	section.groups = [];
	return section;
}

const profileParamSectionsMap  = [
	{	profileName: '[1]', profileGroup: '[Group1]', sectionName: 'Info', sectionIdx: 1 },
	{	profileName: '[2]', profileGroup: '[GROUPMON]', sectionName: 'Dynamic', sectionIdx: 2 },
	{	profileName: '[3]', profileGroup: '[Group3]', sectionName: 'Config', sectionIdx: 3 },
	{	profileName: '[4]', profileGroup: '[Group4]', sectionName: 'Threshould', sectionIdx: 4 },
	{	profileName: '[5]', profileGroup: '[Group5]', sectionName: 'Led', sectionIdx: 5 },
	{	profileName: '[8]', profileGroup: '[Group8]', sectionName: 'Counter', sectionIdx: 8 },
	{	profileName: '[103]', profileGroup: '[Group103]', sectionName: 'Port', sectionIdx: 103 },
];

function parseProfileText(profileText) {
	let profileSection;
	let profileSectionType;
	let profileSectionName;
	let profileThrSection;

	profile.lines = profileText.split('\r\n');
	for (let i = 0; i < profile.lines.length - 1; i++)	{
		if (profile.lines[i][0] == '[')	{
			let sectionMapItem;

			sectionMapItem = profileParamSectionsMap.find(item => item.profileName == profile.lines[i]);
			if (sectionMapItem != undefined) {
				profileSection = createProfileParamSection(sectionMapItem.sectionName, sectionMapItem.sectionIdx);
				profile.sections.push(profileSection);
				profileSectionName = sectionMapItem.sectionName;
				profileSectionType = 'Param';
				continue;
			}

			sectionMapItem = profileParamSectionsMap.find(item => item.profileGroup == profile.lines[i]);
			if (sectionMapItem != undefined) {
				profileSection = profile.sections.find(section => section.name == sectionMapItem.sectionName);
				profileSectionName = sectionMapItem.sectionName;
				profileSectionType = 'Group';
				continue;
			}

			if (profile.lines[i] == '[EnumParam]') {
				profileSectionName = profile.lines[i];
				profileSectionType = 'Enum';
				continue;
			}

			if (profile.lines[i] == '[BitmaskParam]') {
				profileSectionName = profile.lines[i];
				profileSectionType = 'Bitmask';
				continue;
			}

			profileEnumSectionName = profileEnumSectionName = profile.lines[i];
			profileSectionType = 'Unknown';
		} else if (profileSectionType == 'Param') {
			let param = parseProfileParamLine(profileSectionName, profile.lines[i]);
			profile.params.push(param);
			profileSection.params.push(param);
		} else if (profileSectionType == 'Enum') {
			parseProfileEnumLine(profile.params, profile.lines[i]);
		} else if (profileSectionType == 'Bitmask') {
			parseProfileBitmskLine(profile.params, profile.lines[i]);
		} else if (profileSectionType == 'Group') {
			profileSection.groups.push(parseProfileGroupLine(profile.params, profile.lines[i]));
		}
	}

	profileThrSection = profile.sections.find(section => section.name == 'Threshould');
	profileSection = profile.sections.find(section => section.name == 'Dynamic');
	if ((profileSection != undefined) && (profileThrSection != undefined)) {
		for (let i = 0; i < profileSection.params.length; i++) {
			for (let j = 0; j < profileThrNames.length; j++) {
				let profileParamThr = profileThrSection.params.find(param => param.name == (profileSection.params[i].name + profileThrNames[j]))
				if (profileParamThr != undefined)
					profileSection.params[i].thr.push(profileParamThr);
			}
		}
	}
	profileSection = profile.sections.find(section => section.name == 'Config');
	if ((profileSection != undefined) && (profileThrSection != undefined)) {
		for (let i = 0; i < profileSection.params.length; i++) {
			for (let j = 0; j < profileThrNames.length; j++) {
				let profileParamThr = profileThrSection.params.find(param => param.name == (profileSection.params[i].name + profileThrNames[j]))
				if (profileParamThr != undefined)
					profileSection.params[i].thr.push(profileParamThr);
			}
		}
	}
}

function parseProfileColors(__profileColors) {
	let profileSection = profile.sections.find(section => section.name == 'Dynamic');
	let profileColors = __profileColors.split(';');

	if (profileColors.length != profileSection.params.length)
		return;

	for (let i = 0; i < profileSection.params.length; i++) {
		let paramColors = profileColors[i].split('@');
		if (paramColors.length == 2) {
			profileSection.params[i].colors.warning = parseInt(paramColors[0]);
			profileSection.params[i].colors.critical = parseInt(paramColors[1]);
		}
	}
}

function appendProfileParamColor(id, parent, param) {
	let td = $('<td/>', {
		id : 'profile_' + id + '_' + param.name + '_color',
		css : {
			'background-color' : 'transparent',
		},
	}).appendTo(parent);
}

function appendProfileParamName(id, parent, param) {
	let td = $('<td/>').appendTo(parent);
	let name = $('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_name',
		class : 'profile_' + id +'_param_name font-weight-bold m-0',
	}).appendTo(td);

	if (param.units != '')
		name.text(param.name + ', ' + param.units);
	else
		name.text(param.name);

	$('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_descr',
		class : 'profile_' + id + '_param_descr font-weight-light m-0',
		text : param.descr,
	}).appendTo(td);
}

function appendProfileParamValue(id, parent, param) {
	let td = $('<td/>').appendTo(parent);
	let p = $('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_p',
		class : 'profile_' + id + '_param_p m-0',
		text : getParamValueString(param),
	}).appendTo(td);

	if (parseInt(param.access, 10) & ((1 << 1) | (1 << 3) | (1 << 5) | (1 << 7)))	{
		if (param.enum != undefined) {
			let select = $('<select/>', {
				id : 'profile_' + id + '_' + param.name + '_select',
				value : param.value,
				class : 'form-control',
				on : {
					submit : () => { profileSaveButtonOnClick(id, param); },
				},
				css : {
					display : 'none'
				},
			}).appendTo(td);
			for (let i = 0; i < param.enum.length; i++)	{
				$('<option/>', {
					value : param.enum[i].value,
					text : param.enum[i].string,
				}).appendTo(select);
			}
		}	else {
			$('<input/>', {
				id : 'profile_' + id + '_' + param.name + '_input',
				type : 'text',
				value : getParamValueString(param),
				class : 'form-control',
				on : {
					submit : () => { profileSaveButtonOnClick(id, param); },
				},
				css : {
					display : 'none'
				},
			}).appendTo(td);
		}
	}
}

function profileParamValueEditButtonOnClick(id, param) {
	let p = $('#profile_' + id + '_' + param.name + '_p');
	let edit = $('#profile_' + id + '_' + param.name + '_edit');
	let save = $('#profile_' + id + '_' + param.name + '_save');
	let input = $('#profile_' + id + '_' + param.name + '_input');
	let select = $('#profile_' + id + '_' + param.name + '_select');

	p.hide();
	edit.hide();
	if (input.length)	{
		input.val(param.value);
		input.show();
	}
	if (select.length) {
		select.val(param.value);
		select.show();
	}
	save.show();
	return false;
}

function updateProfileParamValue(id, param, value) {
	param.value = value;
	$('#profile_' + id + '_' + param.name + '_p').text(getParamValueString(param));
	$('#profile_' + id + '_' + param.name + '_spinner').hide();
	$('#profile_' + id + '_' + param.name + '_edit').show()
}

function putProfileParamValue(id, param, value) {
	if (!debug) {
		let xhr = new XMLHttpRequest();
		let section = profile.sections.find(item => item.name == param.section);
		xhr.open('PUT', 'api/v1/profile/sections/' + section.idx + '/params/' + param.idx + '/value', true);
		xhr.send(value);
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] putProfileParamValue() Status: ' + xhr.status);
				updateProfileParamValue(id, param, param.value);
				let valueSave = param.value;
				param.value = value;
				showToast('alarm', 'Profile', 'Set '+ param.name + '=' + getParamValueString(param) + ': failed');
				param.value = valueSave;
			} else {
				updateProfileParamValue(id, param, value);
				showToast('normal', 'Profile', 'Set '+ param.name + '=' + getParamValueString(param) + ': ok');
			}
		}
	}	else {
		setTimeout(() => updateProfileParamValue(id, param, value), 1000);
	}
}

function profileParamValueSaveButtonOnClick(id, param) {
	let p = $('#profile_' + id + '_' + param.name + '_p');
	let edit = $('#profile_' + id + '_' + param.name + '_edit');
	let save = $('#profile_' + id + '_' + param.name + '_save');
	let input = $('#profile_' + id + '_' + param.name + '_input');
	let select = $('#profile_' + id + '_' + param.name + '_select');
	let spinner = $('#profile_' + id + '_' + param.name + '_spinner');
	let value;

	if (input.length)	{
		input.hide();
		value = input.val();
	}
	if (select.length)	{
		select.hide();
		value = select.val();
	}
	save.hide();
	edit.hide();
	p.show();
	spinner.show();
	putProfileParamValue(id, param, value);
	return false;
}

function profileParamValueDetailsButtonOnClick(id, param) {
	let col;

	col = $('#profile_' + id + '_' + param.name + '_ccolor');
	if (col.length != 0)
		col.toggleClass('profile_param_section_group_hidden');

	col = $('#profile_' + id + '_' + param.name + '_wcolor');
	if (col.length != 0)
		col.toggleClass('profile_param_section_group_hidden');

	for (i = 0; i < profileThrNames.length; i++) {
		let thr = $('#profile_' + id + '_' + param.name + profileThrNames[i]);
		if (thr.length != 0)
			thr.toggleClass('profile_param_section_group_hidden');
	}
	return false;
}

function appendProfileParamValueEdit(id, parent, param) {
	let td = $('<td/>').appendTo(parent);
	if (parseInt(param.access, 10) & ((1 << 1) | (1 << 3) | (1 << 5) | (1 << 7)))	{
		$('<button/>', {
			id : 'profile_' + id + '_' + param.name + '_edit',
			text : 'Edit',
			class : 'btn btn-primary',
			on : {
				click : () => { profileParamValueEditButtonOnClick(id, param); },
			}
		}).appendTo(td);
		$('<button/>', {
			id : 'profile_' + id + '_' + param.name + '_save',
			text : 'Save',
			class : 'btn btn-primary',
			on : {
				click : () => { profileParamValueSaveButtonOnClick(id, param); },
			},
			css : {
				display : 'none',
			}
		}).appendTo(td);
		$('<div/>', {
			id : 'profile_' + id + '_' + param.name + '_spinner',
			class : 'spinner-border text-primary',
			css : {
				display : 'none',
			}
		}).appendTo(td);
	}
	if (param.section == 'Dynamic') {
		if ((param.thr.length != 0) || (param.colors.warning != 0) || (param.colors.critical != 0)) {
			$('<button/>', {
				id : 'profile_' + id + '_' + param.name + '_save',
				text : '...',
				class : 'btn btn-primary',
				on : {
					click : () => { profileParamValueDetailsButtonOnClick(id, param); },
				},
			}).appendTo(td);
		}
	}
}

function profileParamColorEditButtonOnClick(id, param, color) {
	let p = $('#profile_' + id + '_' + param.name + '_' + color + '_p');
	let edit = $('#profile_' + id + '_' + param.name + '_' + color + '_edit');
	let save = $('#profile_' + id + '_' + param.name + '_' + color + '_save');
	let select = $('#profile_' + id + '_' + param.name + '_' + color + '_select');

	p.hide();
	edit.hide();
	if (select.length)	{
		if (color == 'wcolor')
			select.val(param.colors.warning);
		else if (color == 'ccolor')
			select.val(param.colors.critical);
		select.show();
		select.focus();
	}
	save.show();
	return false;
}

function updateProfileParamColor(id, param, color, value) {
	if (color == 'wcolor')
		param.colors.warning = value;
	else if (color == 'ccolor')
		param.colors.critical = value;
	$('#profile_' + id + '_' + param.name + '_' + color + '_p').text(getParamColorString(value));
	$('#profile_' + id + '_' + param.name + '_' + color + '_spinner').hide();
	$('#profile_' + id + '_' + param.name + '_' + color + '_edit').show()
}

function putProfileParamColor(id, param, color, value) {
	if (!debug) {
		let body;
		let xhr = new XMLHttpRequest();
		let section = profile.sections.find(item => item.name == param.section);
		xhr.open('PUT', 'api/v1/profile/sections/' + section.idx + '/params/' + param.idx + '/colors', true);
		if (color == 'wcolor')
			body = value + ';' + param.colors.critical;
		else if (color == 'ccolor')
			body = param.colors.warning + ';' + value;
		xhr.send(body);
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;

			if (xhr.status != 200) {
				console.log('[Error] putProfileParamColor() Status: ' + xhr.status);
				if (color == 'wcolor') {
					updateProfileParamColor(id, param, color, param.colors.warning);
					let colorSave = param.colors.warning;
					param.colors.warning = color;
					showToast('alarm', 'Profile', 'Set ' + param.name + 'WColor=' + getParamColorString(param.colors.warning) + ': failed');
					param.colors.warning = colorSave;
				}
				else if (color == 'ccolor')	{
					let colorSave = param.colors.critical;
					param.colors.warning = color;
					updateProfileParamColor(id, param, color, param.colors.critical);
					showToast('alarm', 'Profile', 'Set ' + param.name + 'CColor=' + getParamColorString(param.colors.critical) + ': failed');
					param.colors.critical = colorSave;
				}
			} else {
				updateProfileParamColor(id, param, color, value);
				if (color == 'wcolor') {
					showToast('normal', 'Profile', 'Set ' + param.name + 'WColor=' + getParamColorString(param.colors.warning) + ': ok');
				}
				else if (color == 'ccolor')	{
					showToast('normal', 'Profile', 'Set ' + param.name + 'CColor=' + getParamColorString(param.colors.critical) + ': ok');
				}
			}
		}
	}	else {
		setTimeout(() => updateProfileParamColor(id, param, color, value), 1000);
	}
}

function profileParamColorSaveButtonOnClick(id, param, color) {
	let p = $('#profile_' + id + '_' + param.name + '_' + color + '_p');
	let edit = $('#profile_' + id + '_' + param.name + '_' + color + '_edit');
	let save = $('#profile_' + id + '_' + param.name + '_' + color + '_save');
	let select = $('#profile_' + id + '_' + param.name + '_' + color + '_select');
	let spinner = $('#profile_' + id + '_' + param.name + '_' + color + '_spinner');
	let value;

	if (select.length) {
		select.hide();
		value = select.val();
	}
	save.hide();
	edit.hide();
	p.show();
	spinner.show();
	putProfileParamColor(id, param, color, value);
	return false;
}

function appendProfileParamWDColor(id, parent, param, color) {
	let tr = $('<tr/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '',
		class : 'profile_' + id + '_param profile_' + id + '_color',
	}).appendTo(parent);

	let td = $('<td/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_color',
		css : {
			'background-color' : 'transparent',
		},
	}).appendTo(tr);

	td = $('<td/>').appendTo(tr);
	let p = $('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_name',
		class : 'profile_' + id +'_param_' + color + '_name font-weight-bold m-0',
	}).appendTo(td);
	if (color == 'wcolor') {
		p.text(param.name + 'WColor');
	}	else if (color == 'ccolor') {
		p.text(param.name + 'CColor');
	}

	p = $('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_descr',
		class : 'profile_' + id + '_param_' + color + '_descr font-weight-light m-0',
	}).appendTo(td);
	if (color == 'wcolor') {
		p.text(param.name + ' warning color');
	}	else if (color == 'ccolor') {
		p.text(param.name + ' critical color');
	}

	td = $('<td/>').appendTo(tr);
	p = $('<p/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_p',
		class : 'profile_' + id + '_param_p m-0',
		text : getParamColorString(param.color),
	}).appendTo(td);
	if (color == 'wcolor') {
		p.text(getParamColorString(param.colors.warning));
	}	else if (color == 'ccolor') {
		p.text(getParamColorString(param.colors.critical));
	}

	let select = $('<select/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_select',
		value : param.value,
		class : 'form-control',
		on : {
			submit : () => { profileParamColorSaveButtonOnClick(id, param, color); },
		},
		css : {
			display : 'none'
		},
	}).appendTo(td);
	for (let i = 3; i <= 7; i++) {
		$('<option/>', {
			value : i,
			text : getParamColorString(i),
		}).appendTo(select);
	}

	td = $('<td/>').appendTo(tr);
	$('<button/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_edit',
		text : 'Edit',
		class : 'btn btn-primary',
		on : {
			click : () => { profileParamColorEditButtonOnClick(id, param, color); },
		}
	}).appendTo(td);
	$('<button/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_save',
		text : 'Save',
		class : 'btn btn-primary',
		on : {
			click : () => { profileParamColorSaveButtonOnClick(id, param, color); },
		},
		css : {
			display : 'none',
		}
	}).appendTo(td);
	$('<div/>', {
		id : 'profile_' + id + '_' + param.name + '_' + color + '_spinner',
		class : 'spinner-border text-primary',
		css : {
			display : 'none',
		}
	}).appendTo(td);
}

function appendProfileParamThr(id, parent, param) {
	for (let i = 0; i < param.thr.length; i++) {
		tr = $('<tr/>', {
			id : 'profile_' + id + '_' + param.thr[i].name,
			class : 'profile_' + id + '_param',
		}).appendTo(parent);
		for (let j = 0; j < param.thr[i].groups.length; j++)
			tr.addClass('profile_' + id + '_param_section_' + param.thr[i].section + '_group_' + param.thr[i].groups[j].trim().replace(/[^a-zA-Z0-9]/g, '_'));
		appendProfileParamColor(id, tr, param.thr[i]);
		appendProfileParamName(id, tr, param.thr[i]);
		appendProfileParamValue(id, tr, param.thr[i]);
		appendProfileParamValueEdit(id, tr, param.thr[i]);
	}
}

function appendProfileParam(id, parent, param) {
	if (param.section == 'Threshould')
		return;
	let tr = $('<tr/>', {
		id : 'profile_' + id + '_' + param.name,
		class : 'profile_' + id + '_param',
	}).appendTo(parent);
	for (let i = 0; i < param.groups.length; i++)
		tr.addClass('profile_' + id + '_param_section_' + param.section + '_group_' + param.groups[i].trim().replace(/[^a-zA-Z0-9]/g, '_'));
	appendProfileParamColor(id, tr, param);
	appendProfileParamName(id, tr, param);
	appendProfileParamValue(id, tr, param);
	appendProfileParamValueEdit(id, tr, param);
}

function appendProfileParamsTable (id, parent) {
	let profileTableDiv = $('<div/>', {
		id : 'profile_' + id + '_table_div',
		class : 'row p-2',
	}).appendTo(parent);

	profileTableDiv.append (
		'<table class=\'table\' id=\'profile_' + id + '\'_table\'>' +
			'<thead id=\'profile_' + id + '_table_thead\'>' +
				'<tr>' +
					'<th scope=\'col\' style=\'text-align:left; width:30px\'></th>' +
				  '<th scope=\'col\' style=\'text-align:left\'><p class=\'m-0\'>Name</p></th>' +
				  '<th scope=\'col\' style=\'text-align:left\'><p class=\'m-0\'>Value</p></th>' +
				  '<th scope=\'col\' style=\'text-align:left; width:90px\'></th>' +
				'</tr>' +
			'</thead>' +
			'<tbody id=\'profile_' + id + '_table_tbody\'>' +
			'</tbody>' +
		'</table>'
	);
	let tbody = $('#profile_' + id + '_table_tbody');
	for (let i = 0; i < profile.params.length; i++) {
		appendProfileParam(id, tbody, profile.params[i]);
		if (profile.params[i].thr != undefined)
			appendProfileParamThr(id, tbody, profile.params[i]);
		if ((profile.params[i].section == 'Dynamic') && (profile.params[i].colors.warning != 0))
			appendProfileParamWDColor(id, tbody, profile.params[i], 'wcolor');
		if ((profile.params[i].section == 'Dynamic') && (profile.params[i].colors.critical != 0))
			appendProfileParamWDColor(id, tbody, profile.params[i], 'ccolor');
	}
}

function profileMenuSectionSelectOnChange(id) {
	$('.profile_' + id + '_menu_section_group_select').hide();
	let section = $('#profile_' + id + '_menu_section_select option:selected').text();
	let sectionGroupsSelect = $('#profile_' + id + '_menu_section_' + section + '_group_select');
	sectionGroupsSelect.value = 0;
	sectionGroupsSelect.show();
	profileMenuSectionGroupSelectOnChange(id, section);
}

function profileMenuSectionGroupSelectOnChange(id, section) {
	$('.profile_' + id + '_param').addClass('profile_param_section_group_hidden');
	let group = $('#profile_' + id + '_menu_section_' + section + '_group_select option:selected').text().trim().replace(/[^a-zA-Z0-9]/g, '_');
	$('.profile_' + id + '_param_section_' + section + '_group_'+ group).removeClass('profile_param_section_group_hidden');
}

function appendProfileMenu (id, parent) {
	let profileMenuDiv = $('<div/>', {
		id : 'profile_' + id + '_menu_div',
		class: 'row align-items-center flex-row p-2',
	}).appendTo(parent);

	$('<p/>', {
		id : 'profile_' + id + '_menu_section_p',
		text : 'Section:',
		class: 'm-0 p-2',
		css : {
			width : 'auto',
			display : 'inline-block',
		}
	}).appendTo(profileMenuDiv);
	let sectionSelect = $('<select/>', {
		id : 'profile_' + id + '_menu_section_select',
		class : 'form-control',
		on : {
			change : () => { profileMenuSectionSelectOnChange(id); },
		},
		css : {
			width : 'auto',
			display : 'inline-block',
		}
	}).appendTo(profileMenuDiv);
	$('<p/>', {
		id : 'profile_' + id + '_menu_group_p',
		text : 'Group:',
		class: 'm-0 p-2',
		css : {
			width : 'auto',
			display : 'inline-block',
		}
	}).appendTo(profileMenuDiv);
	for (let i = 0; i < profile.sections.length; i++)	{
		let sectionSelectOption = $('<option/>', {
			value : profile.sections[i].idx,
			text : profile.sections[i].name,
		});
		sectionSelectOption.appendTo(sectionSelect);

		let sectionGroupSelect = $('<select/>', {
			id : 'profile_' + id + '_menu_section_' + profile.sections[i].name + '_group_select',
			class : 'profile_' + id + '_menu_section_group_select form-control',
			on : {
				change : () => { profileMenuSectionGroupSelectOnChange(id, profile.sections[i].name); },
			},
			css : {
				width : 'auto',
				display : 'inline-block',
			}
		});
		sectionGroupSelect.appendTo(profileMenuDiv);

		let sectionGroupSelectOption = $('<option/>', {
			value : 0,
			text : 'All',
		});
		sectionGroupSelectOption.appendTo(sectionGroupSelect);

		for (let j = 0; j < profile.sections[i].groups.length; j++)
		{
			let sectionGroupSelectOption = $('<option/>', {
				value : profile.sections[i].groups[j].idx,
				text : profile.sections[i].groups[j].name,
			});
			sectionGroupSelectOption.appendTo(sectionGroupSelect);
		}
	}
}

function getProfileColors(callback) {
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/profile/sections/2/colors', true);
		xhr.send();
		xhr.onreadystatechange = function() { // (3)
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getProfileColors() Status: ' + xhr.status);
				showToast('alarm', 'Profile', 'Get section 2 colors: failed');
			} else {
				parseProfileColors(xhr.responseText);
				if (callback != undefined)
					callback();
			}
		}
	}	else {
		parseProfileColors(profileColors);
		if (callback != undefined)
			callback();
	}
}

function getProfile(callback) {
	console.log('getProfile');
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/profile', true);
		xhr.send();
		xhr.onreadystatechange = function() { // (3)
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getProfile() Status: ' + xhr.status);
				showToast('alarm', 'Profile', 'Get profile: failed');
			} else {
				parseProfileText(xhr.responseText);
				if (callback != undefined)
					callback();
			}
		}
	}	else	{
		parseProfileText(profileText.join('\r\n'));
		if (callback != undefined)
			callback();
	}
}

function updateSystemOS(systemOS) {
	if (systemOS.heap != undefined ) {
		$('#system-os-cpu-heap-value').text(systemOS.heap);
	}
	if (systemOS.threads != undefined) {
    let threads_tbody = $('#system-os-threads-tbody');
		for (let i = 0; i < systemOS.threads.length; i++ ) {
			let thread_name = systemOS.threads[i].name.trim().replace(/[^a-zA-Z0-9]/g, '-');
			let thread_tr = $('#system-os-thread-' + thread_name + '-tr');
			if (thread_tr.length) {
				$('#system-os-thread-' + thread_name + '-name-td').text(systemOS.threads[i].name);
				$('#system-os-thread-' + thread_name + '-state-td').text(systemOS.threads[i].state);
				$('#system-os-thread-' + thread_name + '-priority-td').text(systemOS.threads[i].priority);
				$('#system-os-thread-' + thread_name + '-stack-td').text(systemOS.threads[i].stack);
				$('#system-os-thread-' + thread_name + '-number-td').text(systemOS.threads[i].number);
			}	else {
				thread_tr = $('<tr/>', {
					id : 'system-os-thread-' + thread_name + '-tr',
				}).appendTo(threads_tbody);
				$('<td/>', {
					id : 'system-os-thread-' + thread_name + '-name-td',
					text : systemOS.threads[i].name,
				}).appendTo(thread_tr);
				$('<td/>', {
					id : 'system-os-thread-' + thread_name + '-state-td',
					text : systemOS.threads[i].state,
				}).appendTo(thread_tr);
				$('<td/>', {
					id : 'system-os-thread-' + thread_name + '-priority-td',
					text : systemOS.threads[i].priority,
				}).appendTo(thread_tr);
				$('<td/>', {
					id : 'system-os-thread-' + thread_name + '-stack-td',
					text : systemOS.threads[i].stack,
				}).appendTo(thread_tr);
				$('<td/>', {
					id : 'system-os-thread-' + thread_name + '-number-td',
					text : systemOS.threads[i].number,
				}).appendTo(thread_tr);
			}
		}
	}
}

function getSystemOS() {
	console.log('getSystemOS');
	clearTimeout(timerId);
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/system/os', true);
		xhr.send();
		xhr.onreadystatechange = function() { // (3)
			if (xhr.readyState != 4)
				return;

			if (xhr.status != 200) {
				console.log('[Error] getSystemOS() Status: ' + xhr.status);
				showToast('alarm', 'System', 'Get OS: failed');
			} else {
				let systemOS = JSON.parse(xhr.responseText);
				updateSystemOS(systemOS);
				timerId = setTimeout(() => getSystemOS(), 1000);
			}
		}
	}	else {
		timerId = setTimeout(() => getSystemOS(), 1000);
	}
}

function updateSystemHardware(systemHardware) {
	if (systemHardware.power != undefined )	{
		let power_tbody = $('#system-hardware-power-tbody');
		power_tbody.empty();
		for (let i = 0; i < systemHardware.power.length; i++) {
			power_tbody.append(
				'<tr>' +
					'<td>'+ systemHardware.power[i].config + '</td>' +
					'<td>'+ systemHardware.power[i].monitor  + '</td>' +
				'</tr>'
			);
		}
	}
	if (systemHardware.devices != undefined) {
		let devices_tbody = $('#system-hardware-devices-tbody');
		devices_tbody.empty();
		for (let i = 0; i < systemHardware.devices.length; i++) {
			devices_tbody.append(
				'<tr>' +
					'<td>'+ systemHardware.devices[i].name + '</td>' +
					'<td>'+ systemHardware.devices[i].id + '</td>' +
					'<td>'+ systemHardware.devices[i].status  + '</td>' +
					'<td>'+ systemHardware.devices[i].init_cnt + '</td>' +
					'<td>'+ systemHardware.devices[i].init_err  + '</td>' +
					'<td>'+ systemHardware.devices[i].update_cnt + '</td>' +
					'<td>'+ systemHardware.devices[i].update_err  + '</td>' +
				'</tr>'
			);
		}
	}
}

function getSystemHardware() {
	console.log('getSystemHardware');
	clearTimeout(timerId);
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/system/hardware', true);
		xhr.send();
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getSystemHardware() Status: ' + xhr.status);
				showToast('alarm', 'System', 'Get hardware: failed');
			} else {
				let systemHardware = JSON.parse(xhr.responseText);
				updateSystemHardware(systemHardware);
				timerId = setTimeout(() => getSystemHardware(), 1000);
			}
		}
	}	else	{
		let systemHardware = JSON.parse(system_hardware_json);
		updateSystemHardware(systemHardware);
		timerId = setTimeout(() => getSystemHardware(), 1000);
	}
}

function updateSystemInterfaces(systemInterfaces) {
	if (systemInterfaces.i2c != undefined )	{
		let i2c_tbody = $('#system-interfaces-i2c-tbody');
		i2c_tbody.empty();
		for (let i = 0; i < systemInterfaces.i2c.length; i++) {
			i2c_tbody.append(
				'<tr>' +
					'<td>'+ systemInterfaces.i2c[i].name + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].overrun + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].berr + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].arlo + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].af + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].dma + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].timeout + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].seq_rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].tx_timeout + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].seq_tx_timeout + '</td>' +
					'<td>'+ systemInterfaces.i2c[i].queue_overflow + '</td>' +
				'</tr>'
			);
		}
	}
	if (systemInterfaces.spi != undefined )	{
		let spi_tbody = $('#system-interfaces-spi-tbody');
		spi_tbody.empty();
		for (let i = 0; i < systemInterfaces.spi.length; i++) {
			spi_tbody.append(
				'<tr>' +
					'<td>'+ systemInterfaces.spi[i].name + '</td>' +
					'<td>'+ systemInterfaces.spi[i].overrun + '</td>' +
					'<td>'+ systemInterfaces.spi[i].mode_fault + '</td>' +
					'<td>'+ systemInterfaces.spi[i].crc + '</td>' +
					'<td>'+ systemInterfaces.spi[i].frame + '</td>' +
					'<td>'+ systemInterfaces.spi[i].dma + '</td>' +
					'<td>'+ systemInterfaces.spi[i].timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].seq_rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].tx_timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].seq_tx_timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].tx_rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.spi[i].queue_overflow + '</td>' +
				'</tr>'
			);
		}
	}
	if (systemInterfaces.uart != undefined )	{
		let uart_tbody = $('#system-interfaces-uart-tbody');
		uart_tbody.empty();
		for (let i = 0; i < systemInterfaces.uart.length; i++) {
			uart_tbody.append(
				'<tr>' +
					'<td>'+ systemInterfaces.uart[i].name + '</td>' +
					'<td>'+ systemInterfaces.uart[i].overrun + '</td>' +
					'<td>'+ systemInterfaces.uart[i].parity + '</td>' +
					'<td>'+ systemInterfaces.uart[i].noise + '</td>' +
					'<td>'+ systemInterfaces.uart[i].frame + '</td>' +
					'<td>'+ systemInterfaces.uart[i].dma + '</td>' +
					'<td>'+ systemInterfaces.uart[i].timeout + '</td>' +
					'<td>'+ systemInterfaces.uart[i].rx_timeout + '</td>' +
					'<td>'+ systemInterfaces.uart[i].rx_cmd_timeout + '</td>' +
					'<td>'+ systemInterfaces.uart[i].tx_timeout + '</td>' +
					'<td>'+ systemInterfaces.uart[i].rx_queue_overflow + '</td>' +
					'<td>'+ systemInterfaces.uart[i].tx_queue_overflow + '</td>' +
				'</tr>'
			);
		}
	}
}

function getSystemInterfaces() {
	console.log('getSystemInterfaces');
	clearTimeout(timerId);
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/system/interfaces', true);
		xhr.send();
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getSystemInterfaces() Status: ' + xhr.status);
				showToast('alarm', 'System', 'Get interfaces: failed');
			} else {
				let systemInterfaces = JSON.parse(xhr.responseText);
				updateSystemInterfaces(systemInterfaces);
				timerId = setTimeout(() => getSystemInterfaces(), 1000);
			}
		}
	}	else	{
		let systemInterfaces = JSON.parse(system_hardware_json);
		updateSystemInterfaces(systemInterfaces);
		timerId = setTimeout(() => getSystemInterfaces(), 1000);
	}
}

function updateLog(log) {
	let log_tbody = $('#log-tbody');
	log_tbody.empty();
	for (let i = 0; i < log.records.length; i++) {
		let background_color = 'transparent';
		if (log.records[i].severity == '0') {
			log.records[i].severity = 'INFO';
		}
		else if (log.records[i].severity == '1') {
			log.records[i].severity = 'WARN';
			background_color = '#f7e49c';
		}
		else if (log.records[i].severity == '2') {
			log.records[i].severity = 'ERROR';
			background_color = '#e7a8ad';
		}
		log_tbody.append(
			'<tr style="background-color:' + background_color + '">' +
				'<td>'+ log.records[i].time + '</td>' +
				'<td>'+ log.records[i].severity + '</td>' +
				'<td>'+ log.records[i].category + '</td>' +
				'<td>'+ log.records[i].object + '</td>' +
				'<td>'+ log.records[i].object_id + '</td>' +
				'<td>'+ log.records[i].description + '</td>' +
			'</tr>'
		);
	}
}

function getLog() {
	console.log('getLog');
	clearTimeout(timerId);
	if (!debug)	{
		let xhr = new XMLHttpRequest();
		xhr.open('GET', 'api/v1/log', true);
		xhr.send();
		xhr.onreadystatechange = function() {
			if (xhr.readyState != 4)
				return;
			if (xhr.status != 200) {
				console.log('[Error] getLog() Status: ' + xhr.status);
				showToast('alarm', 'Log', 'Get log: failed');
			} else {
				let log = JSON.parse(xhr.responseText);
				updateLog(log);
				//timerId = setTimeout(() => getLog(), 1000);
			}
		}
	}	else {
		let log = JSON.parse(log_json);
		updateLog(log);
	}
}

function registerNavbarCallbacks() {
	$('.nav-item .nav-link').on('click', function(){
		$('.nav-item').find('.active').removeClass('active');
		$(this).addClass('active');
	});
	$('#nav-profile').on('click', function(){
		$('.content').hide();
		$('#profile-content').show();
		clearTimeout(timerId);
		getProfileSections();
	});
	$('#nav-system-update').on('click', function(){
		$('.content').hide();
		$('#system-update-content').show();
		clearTimeout(timerId);
	});
	$('#nav-system-os').on('click', function(){
		$('.content').hide();
		$('#system-os-content').show();
		clearTimeout(timerId);
		getSystemOS();
	});
	$('#nav-system-hardware').on('click', function(){
		$('.content').hide();
		$('#system-hardware-content').show();
		clearTimeout(timerId);
		getSystemHardware();
	});
	$('#nav-system-interfaces').on('click', function(){
		$('.content').hide();
		$('#system-interfaces-content').show();
		clearTimeout(timerId);
		getSystemInterfaces();
	});
	$('#nav-log').on('click', function(){
		$('.content').hide();
		$('#log-content').show();
		clearTimeout(timerId);
		getLog();
	});
}

function registerUpdateMCUSoftwareCallbacks() {
	$('#system-update-mcu-software-input').on('change',function(){
		let fileName = $(this).prop('files')[0].name;
		$(this).next('.custom-file-label').html(fileName);
	})
	
	$("#system-update-mcu-software-btn").on('click', function() {
		let file_data = $('#system-update-mcu-software-input').prop('files')[0];
		//let form_data = new FormData();
		//form_data.append('file', file_data);
		let xhr = new XMLHttpRequest();
		xhr.open('POST', '/api/v1/system/update/mcu/software', true);
		xhr.upload.onprogress = function(event) {
		  console.log( 'Отправлено ' + event.loaded + ' байт из ' + event.total );
		}
		xhr.onload = function() {
		  showToast('normal', 'System', 'MCU software update: ok');
		  console.log( 'Данные полностью загружены' );
		}
		xhr.onerror = function() {
		  showToast('alarm', 'System', 'MCU software update: failed');
		  console.log( 'Произошла ошибка при загрузке данных' );
		}
		xhr.send(file_data);
	});
}

function registerUpdateMCUWebCallbacks() {
	$('#system-update-mcu-web-input').on('change',function(){
		let fileName = $(this).prop('files')[0].name;
		$(this).next('.custom-file-label').html(fileName);
	})
	
	$("#system-update-mcu-web-btn").on('click', function() {
		let file_data = $('#system-update-mcu-web-input').prop('files')[0];
		//let form_data = new FormData();
		//form_data.append('file', file_data);
		let xhr = new XMLHttpRequest();
		xhr.open('POST', '/api/v1/system/update/mcu/web', true);
		xhr.upload.onprogress = function(event) {
		  console.log( 'Отправлено ' + event.loaded + ' байт из ' + event.total );
		}
		xhr.onload = function() {
		  showToast('normal', 'System', 'MCU web update: ok');
		  console.log( 'Данные полностью загружены' );
		}
		xhr.onerror = function() {
		  showToast('alarm', 'System', 'MCU web update: failed');
		  console.log( 'Произошла ошибка при загрузке данных' );
		}
		xhr.send(file_data);
	});
}

function registerUpdateMCULangpackCallbacks() {
	$('#system-update-mcu-langpack-input').on('change',function(){
		let fileName = $(this).prop('files')[0].name;
		$(this).next('.custom-file-label').html(fileName);
	})
	
	
	$("#system-update-mcu-langpack-btn").on('click', function() {
		let file_data = $('#system-update-mcu-langpack-input').prop('files')[0];
		//let form_data = new FormData();
		//form_data.append('file', file_data);
		let xhr = new XMLHttpRequest();
		xhr.open('PUT', '/api/v1/profile/langpack', true);
		xhr.upload.onprogress = function(event) {
		  console.log( 'Отправлено ' + event.loaded + ' байт из ' + event.total );
		}
		xhr.onload = function() {
		  showToast('normal', 'System', 'Profile langpack update: ok');
		  console.log( 'Данные полностью загружены' );
		}
		xhr.onerror = function() {
		  showToast('alarm', 'System', 'Profile langpack update: failed');
		  console.log( 'Произошла ошибка при загрузке данных' );
		}
		xhr.send(file_data);
	});
}

function DOMContentLoadedEvent () {
	registerNavbarCallbacks();
	registerUpdateMCUSoftwareCallbacks();
	registerUpdateMCUWebCallbacks();
	registerUpdateMCULangpackCallbacks();
	getProfile( () => {
		getProfileColors( () => {
			let profileDiv = $('#profile');
			profileDiv.empty();
			let profile1Div = $('<div/>', {
				id : 'profile_1',
				class : 'col overflow-auto',
				css : {
					height : '100%',
				},
			}).appendTo(profileDiv);
			let profile2Div = $('<div/>', {
				id : 'profile_2',
				class : 'col overflow-auto d-sm-none d-md-block',
				css : {
					height : '100%',
				},
			}).appendTo(profileDiv);
			appendProfileMenu(1, profile1Div);
			appendProfileParamsTable(1, profile1Div);
			profileMenuSectionSelectOnChange(1, profile1Div);
			appendProfileMenu(2, profile2Div);
			appendProfileParamsTable(2, profile2Div);
			profileMenuSectionSelectOnChange(2, profile2Div);
			getProfileSections();
		});
	});
}

document.addEventListener('DOMContentLoaded', DOMContentLoadedEvent);
