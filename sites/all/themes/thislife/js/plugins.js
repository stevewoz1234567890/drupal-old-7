// Avoid `console` errors in browsers that lack a console.
(function() {
	var method;
	var noop = function() {};
	var methods = [
    'assert', 'clear', 'count', 'debug', 'dir', 'dirxml', 'error',
    'exception', 'group', 'groupCollapsed', 'groupEnd', 'info', 'log',
    'markTimeline', 'profile', 'profileEnd', 'table', 'time', 'timeEnd',
    'timeline', 'timelineEnd', 'timeStamp', 'trace', 'warn'
  ];
	var length = methods.length;
	var console = (window.console = window.console || {});

	while (length--) {
		method = methods[length];

		// Only stub undefined methods.
		if (!console[method]) {
			console[method] = noop;
		}
	}
}());

(function() {
	if (typeof window.CustomEvent === "function") {
		return false;
	}

	function CustomEvent(event, params) {
		params = params || {
			bubbles: false,
			cancelable: false,
			detail: undefined
		};
		var evt = document.createEvent('CustomEvent');
		evt.initCustomEvent(event, params.bubbles, params.cancelable, params.detail);
		return evt;
	}

	CustomEvent.prototype = window.Event.prototype;

	window.CustomEvent = CustomEvent;
})();

/*
 * throttledresize: special jQuery event that happens at a reduced rate compared to "resize"
 *
 * latest version and complete README available on Github:
 * https://github.com/louisremi/jquery-smartresize
 *
 * Copyright 2012 @louis_remi
 * Licensed under the MIT license.
 *
 * This saved you an hour of work?
 * Send me music http://www.amazon.co.uk/wishlist/HNTU0468LQON
 */
(function($) {

	var $event = $.event,
		$special,
		dummy = {
			_: 0
		},
		frame = 0,
		wasResized, animRunning;

	$special = $event.special.throttledresize = {
		setup: function() {
			$(this).on("resize", $special.handler);
		},
		teardown: function() {
			$(this).off("resize", $special.handler);
		},
		handler: function(event, execAsap) {
			// Save the context
			var context = this,
				args = arguments;

			wasResized = true;

			if (!animRunning) {
				setInterval(function() {
					frame++;

					if (frame > $special.threshold && wasResized || execAsap) {
						// set correct event type
						event.type = "throttledresize";
						$event.dispatch.apply(context, args);
						wasResized = false;
						frame = 0;
					}
					if (frame > 9) {
						$(dummy).stop();
						animRunning = false;
						frame = 0;
					}
				}, 30);
				animRunning = true;
			}
		},
		threshold: 0
	};

})(jQuery);

/*!
 * JavaScript Cookie v2.2.0
 * https://github.com/js-cookie/js-cookie
 *
 * Copyright 2006, 2015 Klaus Hartl & Fagner Brack
 * Released under the MIT license
 */
;
(function(factory) {
	var registeredInModuleLoader;
	if (typeof define === 'function' && define.amd) {
		define(factory);
		registeredInModuleLoader = true;
	}
	if (typeof exports === 'object') {
		module.exports = factory();
		registeredInModuleLoader = true;
	}
	if (!registeredInModuleLoader) {
		var OldCookies = window.Cookies;
		var api = window.Cookies = factory();
		api.noConflict = function() {
			window.Cookies = OldCookies;
			return api;
		};
	}
}(function() {
	function extend() {
		var i = 0;
		var result = {};
		for (; i < arguments.length; i++) {
			var attributes = arguments[i];
			for (var key in attributes) {
				result[key] = attributes[key];
			}
		}
		return result;
	}

	function decode(s) {
		return s.replace(/(%[0-9A-Z]{2})+/g, decodeURIComponent);
	}

	function init(converter) {
		function api() {}

		function set(key, value, attributes) {
			if (typeof document === 'undefined') {
				return;
			}

			attributes = extend({
				path: '/'
			}, api.defaults, attributes);

			if (typeof attributes.expires === 'number') {
				attributes.expires = new Date(new Date() * 1 + attributes.expires * 864e+5);
			}

			// We're using "expires" because "max-age" is not supported by IE
			attributes.expires = attributes.expires ? attributes.expires.toUTCString() : '';

			try {
				var result = JSON.stringify(value);
				if (/^[\{\[]/.test(result)) {
					value = result;
				}
			} catch (e) {}

			value = converter.write ?
				converter.write(value, key) :
				encodeURIComponent(String(value))
				.replace(/%(23|24|26|2B|3A|3C|3E|3D|2F|3F|40|5B|5D|5E|60|7B|7D|7C)/g, decodeURIComponent);

			key = encodeURIComponent(String(key))
				.replace(/%(23|24|26|2B|5E|60|7C)/g, decodeURIComponent)
				.replace(/[\(\)]/g, escape);

			var stringifiedAttributes = '';
			for (var attributeName in attributes) {
				if (!attributes[attributeName]) {
					continue;
				}
				stringifiedAttributes += '; ' + attributeName;
				if (attributes[attributeName] === true) {
					continue;
				}

				// Considers RFC 6265 section 5.2:
				// ...
				// 3.  If the remaining unparsed-attributes contains a %x3B (";")
				//     character:
				// Consume the characters of the unparsed-attributes up to,
				// not including, the first %x3B (";") character.
				// ...
				stringifiedAttributes += '=' + attributes[attributeName].split(';')[0];
			}

			return (document.cookie = key + '=' + value + stringifiedAttributes);
		}

		function get(key, json) {
			if (typeof document === 'undefined') {
				return;
			}

			var jar = {};
			// To prevent the for loop in the first place assign an empty array
			// in case there are no cookies at all.
			var cookies = document.cookie ? document.cookie.split('; ') : [];
			var i = 0;

			for (; i < cookies.length; i++) {
				var parts = cookies[i].split('=');
				var cookie = parts.slice(1).join('=');

				if (!json && cookie.charAt(0) === '"') {
					cookie = cookie.slice(1, -1);
				}

				try {
					var name = decode(parts[0]);
					cookie = (converter.read || converter)(cookie, name) ||
						decode(cookie);

					if (json) {
						try {
							cookie = JSON.parse(cookie);
						} catch (e) {}
					}

					jar[name] = cookie;

					if (key === name) {
						break;
					}
				} catch (e) {}
			}

			return key ? jar[key] : jar;
		}

		api.set = set;
		api.get = function(key) {
			return get(key, false /* read as raw */ );
		};
		api.getJSON = function(key) {
			return get(key, true /* read as json */ );
		};
		api.remove = function(key, attributes) {
			set(key, '', extend(attributes, {
				expires: -1
			}));
		};

		api.defaults = {};

		api.withConverter = init;

		return api;
	}

	return init(function() {});
}));

/*! modernizr 3.5.0 (Custom Build) | MIT *
 * https://modernizr.com/download/?-backgroundsize-borderradius-cssanimations-csscolumns-csstransforms-csstransforms3d-csstransitions-fontface-nthchild-objectfit-opacity-pointerevents-rgba-svg-touchevents-addtest-domprefixes-hasevent-mq-prefixed-prefixes-setclasses-shiv-testallprops-testprop-teststyles !*/
! function(e, t, n) {
	function r(e, t) {
		return typeof e === t
	}

	function o() {
		var e, t, n, o, i, a, s;
		for (var u in C)
			if (C.hasOwnProperty(u)) {
				if (e = [], t = C[u], t.name && (e.push(t.name.toLowerCase()), t.options && t.options.aliases && t.options.aliases.length))
					for (n = 0; n < t.options.aliases.length; n++) e.push(t.options.aliases[n].toLowerCase());
				for (o = r(t.fn, "function") ? t.fn() : t.fn, i = 0; i < e.length; i++) a = e[i], s = a.split("."), 1 === s.length ? Modernizr[s[0]] = o : (!Modernizr[s[0]] || Modernizr[s[0]] instanceof Boolean || (Modernizr[s[0]] = new Boolean(Modernizr[s[0]])), Modernizr[s[0]][s[1]] = o), S.push((o ? "" : "no-") + s.join("-"))
			}
	}

	function i(e) {
		var t = E.className,
			n = Modernizr._config.classPrefix || "";
		if (T && (t = t.baseVal), Modernizr._config.enableJSClass) {
			var r = new RegExp("(^|\\s)" + n + "no-js(\\s|$)");
			t = t.replace(r, "$1" + n + "js$2")
		}
		Modernizr._config.enableClasses && (t += " " + n + e.join(" " + n), T ? E.className.baseVal = t : E.className = t)
	}

	function a(e, t) {
		if ("object" == typeof e)
			for (var n in e) z(e, n) && a(n, e[n]);
		else {
			e = e.toLowerCase();
			var r = e.split("."),
				o = Modernizr[r[0]];
			if (2 == r.length && (o = o[r[1]]), "undefined" != typeof o) return Modernizr;
			t = "function" == typeof t ? t() : t, 1 == r.length ? Modernizr[r[0]] = t : (!Modernizr[r[0]] || Modernizr[r[0]] instanceof Boolean || (Modernizr[r[0]] = new Boolean(Modernizr[r[0]])), Modernizr[r[0]][r[1]] = t), i([(t && 0 != t ? "" : "no-") + r.join("-")]), Modernizr._trigger(e, t)
		}
		return Modernizr
	}

	function s() {
		return "function" != typeof t.createElement ? t.createElement(arguments[0]) : T ? t.createElementNS.call(t, "http://www.w3.org/2000/svg", arguments[0]) : t.createElement.apply(t, arguments)
	}

	function u(e) {
		return e.replace(/([a-z])-([a-z])/g, function(e, t, n) {
			return t + n.toUpperCase()
		}).replace(/^-/, "")
	}

	function l(e, t) {
		return !!~("" + e).indexOf(t)
	}

	function c() {
		var e = t.body;
		return e || (e = s(T ? "svg" : "body"), e.fake = !0), e
	}

	function f(e, n, r, o) {
		var i, a, u, l, f = "modernizr",
			d = s("div"),
			p = c();
		if (parseInt(r, 10))
			for (; r--;) u = s("div"), u.id = o ? o[r] : f + (r + 1), d.appendChild(u);
		return i = s("style"), i.type = "text/css", i.id = "s" + f, (p.fake ? p : d).appendChild(i), p.appendChild(d), i.styleSheet ? i.styleSheet.cssText = e : i.appendChild(t.createTextNode(e)), d.id = f, p.fake && (p.style.background = "", p.style.overflow = "hidden", l = E.style.overflow, E.style.overflow = "hidden", E.appendChild(p)), a = n(d, e), p.fake ? (p.parentNode.removeChild(p), E.style.overflow = l, E.offsetHeight) : d.parentNode.removeChild(d), !!a
	}

	function d(e) {
		return e.replace(/([A-Z])/g, function(e, t) {
			return "-" + t.toLowerCase()
		}).replace(/^ms-/, "-ms-")
	}

	function p(t, n, r) {
		var o;
		if ("getComputedStyle" in e) {
			o = getComputedStyle.call(e, t, n);
			var i = e.console;
			if (null !== o) r && (o = o.getPropertyValue(r));
			else if (i) {
				var a = i.error ? "error" : "log";
				i[a].call(i, "getComputedStyle returning null, its possible modernizr test results are inaccurate")
			}
		} else o = !n && t.currentStyle && t.currentStyle[r];
		return o
	}

	function m(t, r) {
		var o = t.length;
		if ("CSS" in e && "supports" in e.CSS) {
			for (; o--;)
				if (e.CSS.supports(d(t[o]), r)) return !0;
			return !1
		}
		if ("CSSSupportsRule" in e) {
			for (var i = []; o--;) i.push("(" + d(t[o]) + ":" + r + ")");
			return i = i.join(" or "), f("@supports (" + i + ") { #modernizr { position: absolute; } }", function(e) {
				return "absolute" == p(e, null, "position")
			})
		}
		return n
	}

	function h(e, t, o, i) {
		function a() {
			f && (delete O.style, delete O.modElem)
		}
		if (i = r(i, "undefined") ? !1 : i, !r(o, "undefined")) {
			var c = m(e, o);
			if (!r(c, "undefined")) return c
		}
		for (var f, d, p, h, v, g = ["modernizr", "tspan", "samp"]; !O.style && g.length;) f = !0, O.modElem = s(g.shift()), O.style = O.modElem.style;
		for (p = e.length, d = 0; p > d; d++)
			if (h = e[d], v = O.style[h], l(h, "-") && (h = u(h)), O.style[h] !== n) {
				if (i || r(o, "undefined")) return a(), "pfx" == t ? h : !0;
				try {
					O.style[h] = o
				} catch (y) {}
				if (O.style[h] != v) return a(), "pfx" == t ? h : !0
			}
		return a(), !1
	}

	function v(e, t) {
		return function() {
			return e.apply(t, arguments)
		}
	}

	function g(e, t, n) {
		var o;
		for (var i in e)
			if (e[i] in t) return n === !1 ? e[i] : (o = t[e[i]], r(o, "function") ? v(o, n || t) : o);
		return !1
	}

	function y(e, t, n, o, i) {
		var a = e.charAt(0).toUpperCase() + e.slice(1),
			s = (e + " " + B.join(a + " ") + a).split(" ");
		return r(t, "string") || r(t, "undefined") ? h(s, t, o, i) : (s = (e + " " + k.join(a + " ") + a).split(" "), g(s, t, n))
	}

	function b(e, t, r) {
		return y(e, n, n, t, r)
	}
	var S = [],
		C = [],
		w = {
			_version: "3.5.0",
			_config: {
				classPrefix: "",
				enableClasses: !0,
				enableJSClass: !0,
				usePrefixes: !0
			},
			_q: [],
			on: function(e, t) {
				var n = this;
				setTimeout(function() {
					t(n[e])
				}, 0)
			},
			addTest: function(e, t, n) {
				C.push({
					name: e,
					fn: t,
					options: n
				})
			},
			addAsyncTest: function(e) {
				C.push({
					name: null,
					fn: e
				})
			}
		},
		Modernizr = function() {};
	Modernizr.prototype = w, Modernizr = new Modernizr, Modernizr.addTest("svg", !!t.createElementNS && !!t.createElementNS("http://www.w3.org/2000/svg", "svg").createSVGRect);
	var x = w._config.usePrefixes ? " -webkit- -moz- -o- -ms- ".split(" ") : ["", ""];
	w._prefixes = x;
	var E = t.documentElement,
		T = "svg" === E.nodeName.toLowerCase();
	T || ! function(e, t) {
		function n(e, t) {
			var n = e.createElement("p"),
				r = e.getElementsByTagName("head")[0] || e.documentElement;
			return n.innerHTML = "x<style>" + t + "</style>", r.insertBefore(n.lastChild, r.firstChild)
		}

		function r() {
			var e = b.elements;
			return "string" == typeof e ? e.split(" ") : e
		}

		function o(e, t) {
			var n = b.elements;
			"string" != typeof n && (n = n.join(" ")), "string" != typeof e && (e = e.join(" ")), b.elements = n + " " + e, l(t)
		}

		function i(e) {
			var t = y[e[v]];
			return t || (t = {}, g++, e[v] = g, y[g] = t), t
		}

		function a(e, n, r) {
			if (n || (n = t), f) return n.createElement(e);
			r || (r = i(n));
			var o;
			return o = r.cache[e] ? r.cache[e].cloneNode() : h.test(e) ? (r.cache[e] = r.createElem(e)).cloneNode() : r.createElem(e), !o.canHaveChildren || m.test(e) || o.tagUrn ? o : r.frag.appendChild(o)
		}

		function s(e, n) {
			if (e || (e = t), f) return e.createDocumentFragment();
			n = n || i(e);
			for (var o = n.frag.cloneNode(), a = 0, s = r(), u = s.length; u > a; a++) o.createElement(s[a]);
			return o
		}

		function u(e, t) {
			t.cache || (t.cache = {}, t.createElem = e.createElement, t.createFrag = e.createDocumentFragment, t.frag = t.createFrag()), e.createElement = function(n) {
				return b.shivMethods ? a(n, e, t) : t.createElem(n)
			}, e.createDocumentFragment = Function("h,f", "return function(){var n=f.cloneNode(),c=n.createElement;h.shivMethods&&(" + r().join().replace(/[\w\-:]+/g, function(e) {
				return t.createElem(e), t.frag.createElement(e), 'c("' + e + '")'
			}) + ");return n}")(b, t.frag)
		}

		function l(e) {
			e || (e = t);
			var r = i(e);
			return !b.shivCSS || c || r.hasCSS || (r.hasCSS = !!n(e, "article,aside,dialog,figcaption,figure,footer,header,hgroup,main,nav,section{display:block}mark{background:#FF0;color:#000}template{display:none}")), f || u(e, r), e
		}
		var c, f, d = "3.7.3",
			p = e.html5 || {},
			m = /^<|^(?:button|map|select|textarea|object|iframe|option|optgroup)$/i,
			h = /^(?:a|b|code|div|fieldset|h1|h2|h3|h4|h5|h6|i|label|li|ol|p|q|span|strong|style|table|tbody|td|th|tr|ul)$/i,
			v = "_html5shiv",
			g = 0,
			y = {};
		! function() {
			try {
				var e = t.createElement("a");
				e.innerHTML = "<xyz></xyz>", c = "hidden" in e, f = 1 == e.childNodes.length || function() {
					t.createElement("a");
					var e = t.createDocumentFragment();
					return "undefined" == typeof e.cloneNode || "undefined" == typeof e.createDocumentFragment || "undefined" == typeof e.createElement
				}()
			} catch (n) {
				c = !0, f = !0
			}
		}();
		var b = {
			elements: p.elements || "abbr article aside audio bdi canvas data datalist details dialog figcaption figure footer header hgroup main mark meter nav output picture progress section summary template time video",
			version: d,
			shivCSS: p.shivCSS !== !1,
			supportsUnknownElements: f,
			shivMethods: p.shivMethods !== !1,
			type: "default",
			shivDocument: l,
			createElement: a,
			createDocumentFragment: s,
			addElements: o
		};
		e.html5 = b, l(t), "object" == typeof module && module.exports && (module.exports = b)
	}("undefined" != typeof e ? e : this, t);
	var _ = "Moz O ms Webkit",
		k = w._config.usePrefixes ? _.toLowerCase().split(" ") : [];
	w._domPrefixes = k;
	var z;
	! function() {
		var e = {}.hasOwnProperty;
		z = r(e, "undefined") || r(e.call, "undefined") ? function(e, t) {
			return t in e && r(e.constructor.prototype[t], "undefined")
		} : function(t, n) {
			return e.call(t, n)
		}
	}(), w._l = {}, w.on = function(e, t) {
		this._l[e] || (this._l[e] = []), this._l[e].push(t), Modernizr.hasOwnProperty(e) && setTimeout(function() {
			Modernizr._trigger(e, Modernizr[e])
		}, 0)
	}, w._trigger = function(e, t) {
		if (this._l[e]) {
			var n = this._l[e];
			setTimeout(function() {
				var e, r;
				for (e = 0; e < n.length; e++)(r = n[e])(t)
			}, 0), delete this._l[e]
		}
	}, Modernizr._q.push(function() {
		w.addTest = a
	});
	var j = function() {
		function e(e, t) {
			var o;
			return e ? (t && "string" != typeof t || (t = s(t || "div")), e = "on" + e, o = e in t, !o && r && (t.setAttribute || (t = s("div")), t.setAttribute(e, ""), o = "function" == typeof t[e], t[e] !== n && (t[e] = n), t.removeAttribute(e)), o) : !1
		}
		var r = !("onblur" in t.documentElement);
		return e
	}();
	w.hasEvent = j, Modernizr.addTest("pointerevents", function() {
		var e = !1,
			t = k.length;
		for (e = Modernizr.hasEvent("pointerdown"); t-- && !e;) j(k[t] + "pointerdown") && (e = !0);
		return e
	}), Modernizr.addTest("opacity", function() {
		var e = s("a").style;
		return e.cssText = x.join("opacity:.55;"), /^0.55$/.test(e.opacity)
	}), Modernizr.addTest("rgba", function() {
		var e = s("a").style;
		return e.cssText = "background-color:rgba(150,255,150,.5)", ("" + e.backgroundColor).indexOf("rgba") > -1
	});
	var N = "CSS" in e && "supports" in e.CSS,
		P = "supportsCSS" in e;
	Modernizr.addTest("supports", N || P);
	var R = w.testStyles = f;
	Modernizr.addTest("touchevents", function() {
		var n;
		if ("ontouchstart" in e || e.DocumentTouch && t instanceof DocumentTouch) n = !0;
		else {
			var r = ["@media (", x.join("touch-enabled),("), "heartz", ")", "{#modernizr{top:9px;position:absolute}}"].join("");
			R(r, function(e) {
				n = 9 === e.offsetTop
			})
		}
		return n
	});
	var F = function() {
		var e = navigator.userAgent,
			t = e.match(/w(eb)?osbrowser/gi),
			n = e.match(/windows phone/gi) && e.match(/iemobile\/([0-9])+/gi) && parseFloat(RegExp.$1) >= 9;
		return t || n
	}();
	F ? Modernizr.addTest("fontface", !1) : R('@font-face {font-family:"font";src:url("https://")}', function(e, n) {
		var r = t.getElementById("smodernizr"),
			o = r.sheet || r.styleSheet,
			i = o ? o.cssRules && o.cssRules[0] ? o.cssRules[0].cssText : o.cssText || "" : "",
			a = /src/i.test(i) && 0 === i.indexOf(n.split(" ")[0]);
		Modernizr.addTest("fontface", a)
	}), R("#modernizr div {width:1px} #modernizr div:nth-child(2n) {width:2px;}", function(e) {
		for (var t = e.getElementsByTagName("div"), n = !0, r = 0; 5 > r; r++) n = n && t[r].offsetWidth === r % 2 + 1;
		Modernizr.addTest("nthchild", n)
	}, 5);
	var A = function() {
		var t = e.matchMedia || e.msMatchMedia;
		return t ? function(e) {
			var n = t(e);
			return n && n.matches || !1
		} : function(t) {
			var n = !1;
			return f("@media " + t + " { #modernizr { position: absolute; } }", function(t) {
				n = "absolute" == (e.getComputedStyle ? e.getComputedStyle(t, null) : t.currentStyle).position
			}), n
		}
	}();
	w.mq = A;
	var B = w._config.usePrefixes ? _.split(" ") : [];
	w._cssomPrefixes = B;
	var L = function(t) {
		var r, o = x.length,
			i = e.CSSRule;
		if ("undefined" == typeof i) return n;
		if (!t) return !1;
		if (t = t.replace(/^@/, ""), r = t.replace(/-/g, "_").toUpperCase() + "_RULE", r in i) return "@" + t;
		for (var a = 0; o > a; a++) {
			var s = x[a],
				u = s.toUpperCase() + "_" + r;
			if (u in i) return "@-" + s.toLowerCase() + "-" + t
		}
		return !1
	};
	w.atRule = L;
	var M = {
		elem: s("modernizr")
	};
	Modernizr._q.push(function() {
		delete M.elem
	});
	var O = {
		style: M.elem.style
	};
	Modernizr._q.unshift(function() {
		delete O.style
	});
	w.testProp = function(e, t, r) {
		return h([e], n, t, r)
	};
	w.testAllProps = y;
	var D = w.prefixed = function(e, t, n) {
		return 0 === e.indexOf("@") ? L(e) : (-1 != e.indexOf("-") && (e = u(e)), t ? y(e, t, n) : y(e, "pfx"))
	};
	Modernizr.addTest("objectfit", !!D("objectFit"), {
			aliases: ["object-fit"]
		}), w.testAllProps = b, Modernizr.addTest("cssanimations", b("animationName", "a", !0)), Modernizr.addTest("backgroundsize", b("backgroundSize", "100%", !0)), Modernizr.addTest("borderradius", b("borderRadius", "0px", !0)),
		function() {
			Modernizr.addTest("csscolumns", function() {
				var e = !1,
					t = b("columnCount");
				try {
					e = !!t, e && (e = new Boolean(e))
				} catch (n) {}
				return e
			});
			for (var e, t, n = ["Width", "Span", "Fill", "Gap", "Rule", "RuleColor", "RuleStyle", "RuleWidth", "BreakBefore", "BreakAfter", "BreakInside"], r = 0; r < n.length; r++) e = n[r].toLowerCase(), t = b("column" + n[r]), ("breakbefore" === e || "breakafter" === e || "breakinside" == e) && (t = t || b(n[r])), Modernizr.addTest("csscolumns." + e, t)
		}(), Modernizr.addTest("csstransforms", function() {
			return -1 === navigator.userAgent.indexOf("Android 2.") && b("transform", "scale(1)", !0)
		}), Modernizr.addTest("csstransforms3d", function() {
			var e = !!b("perspective", "1px", !0),
				t = Modernizr._config.usePrefixes;
			if (e && (!t || "webkitPerspective" in E.style)) {
				var n, r = "#modernizr{width:0;height:0}";
				Modernizr.supports ? n = "@supports (perspective: 1px)" : (n = "@media (transform-3d)", t && (n += ",(-webkit-transform-3d)")), n += "{#modernizr{width:7px;height:18px;margin:0;padding:0;border:0}}", R(r + n, function(t) {
					e = 7 === t.offsetWidth && 18 === t.offsetHeight
				})
			}
			return e
		}), Modernizr.addTest("csstransitions", b("transition", "all", !0)), o(), i(S), delete w.addTest, delete w.addAsyncTest;
	for (var q = 0; q < Modernizr._q.length; q++) Modernizr._q[q]();
	e.Modernizr = Modernizr
}(window, document);