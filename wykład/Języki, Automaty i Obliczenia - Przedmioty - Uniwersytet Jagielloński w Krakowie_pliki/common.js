/* COMMON */

/* SERVLET */

var servlets = new Object();
var _formSubmissionAllowed = true;
var _warnOnNavigate = false;
var _navigationNotAllowed = false;
var _warnOnNavigateConfirmationPL = "Zmieniłeś dane na stronie, lecz nie zapisałeś zmian. Jeśli będziesz kontynuował, to zmiany te NIE zostaną zapisane.\n\nKliknij OK, aby kontynuować bez zapisywania zmian.\nKliknij Anuluj, aby powrócić do strony i móc zapisać zmiany.";
var _warnOnNavigateConfirmationEN = "You've changed data on this page, but did not save these changes. If you continue current action, these changes will NOT be saved.\n\nClick OK to continue without saving changes.\nClick Cancel to return to previous page (where you'll probably want to save changes).";


var Common = {
	/* ACTION */
	htmlActionLink : function(action, description, params, className)
	{
		var link = $("<a>", {
			href: Common.actionLink(action, params),
			text: description,
			"class": className
		});

		return link.attr('outerHTML');
	},
	actionLink : function(action, params)
	{
		return "kontroler.php?_action=actionx:"+Common.encodeActionString(action, params);
	},
	encodeActionString : function(action, params)
	{
		var parts = new Array();
		for (key in params)
		{
			value = params[key];
			if ((value != null) && (key != null))
				parts.push(key+":"+Common.encodeActionParam(value));
		}
		return action+"("+parts.join(";")+")";
	},
	encodeActionParam : function(value)
	{
		function dechex(d)
		{
			function hex(n) {
				return n.toString(16);
			}
			if (d <= 0x7f) {
				return hex(d & 0x7f);
			}
			var ret = "";
			var pref = 0x80;
			var n = 6;
			var b6;
			while (d >> n) {
				pref = pref>>1 | 0x80;
				b6 = d & 0xFF>>2;
				d = d>>6;
				ret = hex(0x80 | b6) + ret;
				n--;
			}
			return hex(pref | d) + ret;
		}
		
		value += ''; // tostr
		var result = '';
		var i;
		var c;
		for (i=0; i < value.length; i++)
		{
			c = value.charAt(i);
			if(((c>='0')&&(c<='9'))
				|| ((c>='a')&&(c<='z'))
				|| ((c>='A')&&(c<='Z'))
				|| (c=='-') || (c=='_'))
				{
				result += c;
			} else {
				var code = dechex(value.charCodeAt(i));
				result += "@" + (code.length/2) + code;
			}
		}
		return result;
	},
	/* SERVLET */
	confirmWarnings : function()
	{
		if (!_warnOnNavigate)
			return true;
		if (_navigationNotAllowed)
			return alert(lang=='pl'?_warnOnNavigateConfirmationPL:_warnOnNavigateConfirmationEN);
		else
			return confirm(lang=='pl'?_warnOnNavigateConfirmationPL:_warnOnNavigateConfirmationEN);
	},
	servletRegister : function(name, action, params)
	{
		var servlet = new Object();
		servlet.name = name;
		servlet.action = action;
		servlet.params = params;
		servlet.denyMsg = null;
		servlets[name] = servlet;
	},
	servletUpdateParam : function(name, key, value)
	{
		servlets[name].params[key] = value;
	},
	servletDenyPostback : function(name, msg)
	{
		servlets[name].denyMsg = msg;
	},
	servletAllowPostback : function(name)
	{
		servlets[name].denyMsg = null;
	},
	servletPostback : function(name)
	{
		var servlet = servlets[name];
		if (servlet.denyMsg != null) {
			alert(servlet.denyMsg);
			return;
		}
		var link = Common.actionLink(servlet.action, servlet.params);
		document.location = link;
	},
	/* NAVBAR */
	navOptClick : function(name)
	{
		var checkbox = document.getElementById('navoptcheckbox'+name);
		var tr = document.getElementById('navopttr'+name);
		if (checkbox.checked) {
			tr.style.display='';
		} else {
			tr.style.display='none';
		}
	},
	navBarOrder : function (form,itemsNo,link)
	{
		var order = "";
		var i;
		for(i=1;i<=itemsNo;i++) {
			eval("x=form.select"+i);
			if (!x) break;
			eval("order+=form.select"+i+".value");
		}
		order = order.replace(/_/g,"");
		link = link.replace(/order_to_be_instered_here/, order);
		document.location = link;
	},
	/* POPUP (tip) */
	findPosX : function(obj)
	{
		var curleft = 0;
		if (obj.offsetParent)
		{
			while (obj.offsetParent)
			{
				curleft += obj.offsetLeft;
				if (obj.style.borderLeftWidth && obj.style.borderLeftWidth.search('px') > -1) {
					curleft += parseInt(obj.style.borderLeftWidth.split('px')[0]);
				}
				obj = obj.offsetParent;
			}
		}
		else if (obj.x)
			curleft += obj.x;
		return curleft;
	},
	findPosY : function(obj)
	{
		var curtop = 0;
		if (obj.offsetParent)
		{
			while (obj.offsetParent)
			{
				curtop += obj.offsetTop;
				if (obj.style.borderTopWidth && obj.style.borderTopWidth.search('px') > -1) {
					curtop += parseInt(obj.style.borderTopWidth.split('px')[0]);
				}
				obj = obj.offsetParent;
			}
		}
		else if (obj.y)
			curtop += obj.y;
		return curtop;
	},
	popupOn : function(gdzie,opis,width,onLeft,y)
	{
		var y = typeof(y) != 'undefined' ? y : 18;
		var popup = document.getElementById("wrpopup");
		popup.closing = false;
		popup.style.width = width+"px";
		popup.style.top = Common.findPosY(gdzie)+y+"px";
		var x = Common.findPosX(gdzie);
		var clientWidth = document.body.clientWidth;
		if ((x > (clientWidth-(width+30))) || (onLeft))
			x = x - width;
		popup.style.left = x+"px";
		popup.innerHTML = opis;
		popup.style.display = "block";
	},
	popupOff : function()
	{
		var popup = document.getElementById("wrpopup");
		popup.closing = true;
		setTimeout('var popup = document.getElementById("wrpopup"); if (popup.closing) { popup.style.display="none"; }', 50);
	},
	getCookie : function(name)
	{
		var cookieValue = null;
		if (document.cookie && document.cookie != '') {
			var cookies = document.cookie.split(';');
			for (var i = 0; i < cookies.length; i++) {
				var cookie = jQuery.trim(cookies[i]);
				// Does this cookie string begin with the name we want?
				if (cookie.substring(0, name.length + 1) == (name + '=')) {
					cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
					break;
				}
			}
		}
		return cookieValue;
	}
};

/* INNE */

function checkLimit(field, countField, maxlimit)
{
	if (field.value.length > maxlimit)
		field.value = field.value.substring(0, maxlimit);

	if (countField != null)
		countField.value = maxlimit - field.value.length;
}

function utfbyteslength(s)
{
	var encoded = encodeURI(s);
	if (encoded.indexOf("%") != -1)
	{
		var count = encoded.split("%").length - 1;
		var tmp = encoded.length - (count * 3)
		return count + tmp
	}
	else
		return encoded.length;
}


function checkLimit2(fieldId, maxlimit, type)
{
	var field = document.getElementById(fieldId);
	var countField = document.getElementById("_pozostalo_" + fieldId);
	if (type == 'bytes')
		var length = utfbyteslength(field.value);
	else if (type == 'chars')
		var length = field.value.length + field.value.split("\n").length - 1;
	if (countField != null)
	{
		var current_counter = maxlimit - length;
		var kolor = '#bb77bb';
		var opis = '';
		if (length > maxlimit)
		{
			field.style.backgroundColor = '#ffeb99';
			field.style.borderColor = '#dd0000';
			field.style.borderWidth = '2px';
			field.style.borderStyle = 'dotted';
			kolor = '#dd0000';
			if (lang == 'pl')
				opis = " -- <b style='text-decoration: blink'>przekroczono limit znaków!</b>";
			else
				opis = " -- <b style='text-decoration: blink'>character limit exceeded!</b>";
		}
		else if (length > 0.8*maxlimit)
		{
			field.style.backgroundColor = '';
			field.style.borderColor = '';
			field.style.borderWidth = '';
			field.style.borderStyle = '';
			if (lang == 'pl')
				opis = " -- <b style='text-decoration: blink'>uwaga</b> - zbliżasz się do limitu znaków!";
			else
				opis = " -- <b style='text-decoration: blink'>warning</b> - aproaching character limit!";
		}
		else
		{
			field.style.backgroundColor = '';
			field.style.borderColor = '';
			field.style.borderWidth = '';
			field.style.borderStyle = '';
		}
		if (lang == 'pl')
			countField.innerHTML =
			"Limit " + '<span style="color:#bb77bb;font-weight:bold;">' + maxlimit +
			'</span>, ' + 'wprowadzono' + '<span  style="color:'+kolor+'; font-weight:bold;"> ' +
			length + '</span> ' + 'znaków ' + opis;
		else
			countField.innerHTML =
			"Limit " + '<span style="color:#bb77bb;font-weight:bold;">' + maxlimit +
			'</span>, ' + 'entered' + '<span  style="color:'+kolor+'; font-weight:bold;"> ' +
			length + '</span> ' + 'characters ' + opis;
		countField.style.display = "inline";
	}
}

function init_watermark_decorator(inputId, watermark)
{
	var input = document.getElementById(inputId);
	input.watermarkText = watermark;
	if (input.addEventListener) {
		input.addEventListener('focus',removeWatermarkEvent,false);
		input.addEventListener('blur',addWatermarkEvent,false);
	} else {
		input.attachEvent('onfocus',removeWatermarkEvent);
		input.attachEvent('onblur',addWatermarkEvent);
	}
	addWatermark(input);
}

function addWatermarkEvent(e)
{
	var targ;
	if (!e) var e = window.event;
	if (e.target) targ = e.target;
	else if (e.srcElement) targ = e.srcElement;
	if (targ.nodeType == 3) // defeat Safari bug
		targ = targ.parentNode;
	setTimeout("addWatermark(document.getElementById('"+targ.id+"'))", 100);
}

function removeWatermarkEvent(e)
{
	var targ;
	if (!e) var e = window.event;
	if (e.target) targ = e.target;
	else if (e.srcElement) targ = e.srcElement;
	if (targ.nodeType == 3) // defeat Safari bug
		targ = targ.parentNode;
	removeWatermark(targ);
}

function addWatermark(input)
{
	if (input.value == "") {
		input.watermark = true;
		input.style.color = "#aaa";
		if (!input.style.height)
			input.style.height = "14px";
		input.style.verticalAlign = "middle";
		input.style.fontSize = "10px";
		input.value = input.watermarkText;
	}
}

function removeWatermark(input)
{
	if (input.watermark) {
		input.watermark = false;
		input.style.color = "";
		input.style.fontSize = "";
		input.value = "";
	}
}

function actionForm_submissionAllowed(formName)
{
	if (!_formSubmissionAllowed)
		return false;
	var lst = document.forms[formName].elements;
	for (var i=0; i<lst.length; i++)
	{
		if ((lst[i].type == 'text') && (lst[i].watermark) && (lst[i].watermark == true) && (lst[i].value == lst[i].watermarkText))
			lst[i].value = '';
			
		/* Jesli jest pole textarea i ma zdefiniowany limit i ten limit jest przekroczony to blokujemy formularz,
		 * a kursor przeskakuje do pola z przekroczonym limitem. */

		if ((lst[i].type == 'textarea') && (lst[i].style.borderStyle == 'dotted'))
		{
			alert((lang == 'pl')
				?"Przekroczono limit znaków jednego z pól tekstowych!\nPopraw zawartość pola i spróbuj ponownie."
				:"Character limit of one of the text fields has been exceeded!\nFix this field's value and try again.");
			lst[i].focus();
			return false;
		}
	}

	return true;
}

function debug(text)
{
	document.getElementById('jsdebug').innerHTML += text+' ';
}

$$ = $;  // tymczasowe, zob. issue#5797

$(function() {
	$('#layout-c11 .homelink')
		.click(function() {
			document.location = sys_www_path;
		});
	$('#layout-c12-t .m a').hover(
		function() {
			$(this)
				.stop(true, false)
				.animate({'background-position-y': '55%'}, 300);
		},
		function() {
			$(this)
				.stop(true, false)
				.animate({'background-position-y': '20%'}, 1000);
		}
	);
	$('#layout-c12-t .a a.lang').hover(
		function() {
			$(this)
				.stop(true, false)
				.animate({'opacity': '1'}, 200);
		},
		function() {
			$(this)
				.stop(true, false)
				.animate({'opacity': '.7'}, 500);
		}
	);
	$('#layout-c21 div.item.level1').prepend("<div class='bigarrow'></div>");
	$('#layout-c21 div.item a').hover(
		function() {
			$(this).closest('div.item')
				.stop(true, false)
				.addClass("hover")
				.animate({'background-position-x': '1px'}, 50);
		},
		function() {
			$(this).closest('div.item')
				.stop(true, false)
				.removeClass("hover")
				.animate({'background-position-x': '-2px'}, 100);
		}
	);
	$('#layout-c21 div.item.level1 a').hover(
		function() {
			$(this).closest('div.item').find('.bigarrow')
				.stop(true, false)
				.show()
				.animate({'opacity': 1}, 50);
		},
		function() {
			$(this).closest('div.item').find('.bigarrow')
				.stop(true, false)
				.animate({'opacity': 0}, 200, function() { $(this).hide(); });
		}
	);
	var deprecated = false;
	if ($.browser.msie) {
		major = parseInt($.browser.version)
		deprecated = (major <= 7);
	}
	else if ($.browser.mozilla) {
		deprecated = ($.browser.version).match("^1.0");
	}
	if (deprecated) {
		var elem = $(
			"<div>Używasz przestarzałej przeglądarki. By mieć pewność, że " +
			"strony będą wyświetlane poprawnie, zaktualizuj przeglądarkę.</div>"
		);
		elem.css({
			'color': '#c00',
			'font-weight': 'bold',
			'font-size': '16px',
			'margin-bottom': '20px'
		});
		$('#layout-c22').prepend(elem);
	}
});

