/*global Drupal, jQuery, Mustache, window, console, AndroidInterface, jplayer_swfPath, ga */

window.addEventListener("load", function() {
	window.cookieconsent.initialise({
		"palette": {
			"popup": {
				"background": "#ffffff",
				"text": "#191919"
			},
			"button": {
				"background": "#ffffff",
				"text": "#191919"
			}
		},
		"theme": "edgeless",
		"content": {
			"message": "We use cookies and other tracking technologies to enhance your browsing experience. If you continue to use our site, you agree to the use of such cookies. For more info, see our <a href=\"https://www.thisamericanlife.org/page/privacy-policy\">privacy policy</a>.",
			"dismiss": '',
			"link": false,
			"href": "https://www.thisamericanlife.org/page/privacy-policy"
		}
	});
});


(function($) {

	$.fn.matchHeight._maintainScroll = true;

	$(document).ready(function() {
		$('body').addClass('loaded');
	});

	Drupal.behaviors.thislife = {
		attach: function() {


		}
	};

	Drupal.behaviors.thislifePlayer = {
		attach: function() {


			var $body = $('body'),
				$header = $('#site-header'),
				$burger = $('#burger');


			var
				$main = $('#main'),
				$sidebar = $('#sidebar'),
				$top = $('#top'),
				$jp = $("#jplayer"),
				$player = $("#player"),
				$progress = $player.find('.jp-progress'),
				$info = $player.find('#player-info'),
				$share_modal = $('#share-modal'),
				$player_share_modal = $('#player-share-modal'),
				last_position = 0,
				playing = false,
				ready = false,
				current_episode = false,
				current_act = 0,
				last_path = window.location.pathname + window.location.search,
				$window = $(window),
				scrollTop = 0,
				windowHeight = 0,
				scrolled = false,
				viewSize = function() {
					var width = $window.width(),
						newview = (width >= 1024 ? 'desktop' : (width >= 768 ? 'tablet' : 'mobile'));
					return newview;
				},
				view = viewSize(),
				updatePage = function() {
					scrollTop = $window.scrollTop();
					animateElements();
					window.requestAnimationFrame(updatePage);
				},
				animateElements = function() {
					if (!scrolled && scrollTop > 0) {
						scrolled = true;
						$body.addClass('scrolled');
					} else if (scrolled && scrollTop <= 0) {
						scrolled = false;
						$body.removeClass('scrolled');
					}
				},
				playAct = function(act) {
					var data = current_episode,
						current_timestamp = 0;
					$('a.play-act').removeClass('playing');

					$info.find('.title').html('').hide();
					if (typeof act !== 'undefined') {
						current_act = act;
					} else {
						current_act = 0;
					}
					if (typeof current_episode.acts !== 'undefined') {
						$.each(current_episode.acts, function(i, act) {
							if (act.number === current_act) {
								current_timestamp = act.timestamp;
								$info.find('.title').html(act.name).css('display', 'inline-block');
							}
						});
					}
					$jp.jPlayer('play', current_timestamp);
					$('a.play-' + current_episode.episode + '-' + current_act).addClass('playing');
				},
				playEpisode = function(act) {
					var data = current_episode,
						current_timestamp = 0;
					$('a.playing').removeClass('playing');
					$info.find('.episode').html(data.title);
					$info.find('.image').html($('<img/>').attr('src', data.thumbnail));
					$info.find('.title').html('').hide();
					$progress.find('.jump').remove();
					if (typeof act !== 'undefined') {
						current_act = act;
					} else {
						current_act = 0;
					}
					if (typeof current_episode.acts !== 'undefined') {
						$.each(current_episode.acts, function(i, act) {
							if (act.number === current_act) {
								current_timestamp = act.timestamp;
								$info.find('.title').html(act.name).css('display', 'inline-block');
							}
						});
					}
					$jp.jPlayer("setMedia", {
						mp3: data.audio
					}).jPlayer('play', current_timestamp);
					$('a.play-' + current_episode.episode + '-' + current_act).addClass('playing');
				},
				resizePage = function() {
					scrollTop = $window.scrollTop();
					windowHeight = $window.height();
					view = viewSize();
				},
				initLinks = function() {

					$('.contextual-links-wrapper').find('a').addClass('ignore');

					// External Links
					$('a.shareout').once('links').addClass('external').on('click', function(e) {
						var event = new CustomEvent('talGoto', {
							detail: {
								type: 'external',
								url: $(this).prop('href'),
								ctrlKey: e.ctrlKey,
								metaKey: e.metaKey
							}
						});

						window.dispatchEvent(event);
						if ($jp.length) {
							e.preventDefault();
						}
					});

					$share_modal.find('a.close').once('links').addClass('ignore').on('click', function(e) {
						e.preventDefault();
						$body.removeClass('open-share');
					});

					$('a.share').once('links').addClass('ignore').on('click', function(e) {
						e.preventDefault();
						$body.addClass('open-share');
					});

					$player_share_modal.find('a.close').once('links').addClass('ignore').on('click', function(e) {
						e.preventDefault();
						$body.removeClass('open-player-share');
					});

					$('a.player-share').once('links').addClass('ignore').on('click', function(e) {
						e.preventDefault();
						$body.addClass('open-player-share');
					});

					// External Links
					$('a[href]:not([href*="' + window.location.hostname + '"]):not([href^="#"]):not([href^="/"]):not([href^="javascript:"]):not([href^="mailto:"]):not(.cut)').once('links').addClass('external').on('click', function(e) {
						var event = new CustomEvent('talGoto', {
							detail: {
								type: 'external',
								url: $(this).prop('href'),
								ctrlKey: e.ctrlKey,
								metaKey: e.metaKey
							}
						});

						window.dispatchEvent(event);
						if ($jp.length) {
							e.preventDefault();
						}
					});

					// External Links
					$('a.external').once('links').on('click', function(e) {
						var event = new CustomEvent('talGoto', {
							detail: {
								type: 'external',
								url: $(this).prop('href'),
								ctrlKey: e.ctrlKey,
								metaKey: e.metaKey
							}
						});

						window.dispatchEvent(event);
						if ($jp.length) {
							e.preventDefault();
						}
					});

					//  Links
					$('a[href*="' + window.location.hostname + '"],[href^="/"]').not('.goto, .play, .ignore').once('links').addClass('internal').on('click', function(e) {
						var event = new CustomEvent('talGoto', {
							detail: {
								type: 'internal',
								url: $(this).prop('href'),
								ctrlKey: e.ctrlKey,
								metaKey: e.metaKey
							}
						});
						window.dispatchEvent(event);
						if ($jp.length) {
							e.preventDefault();
						}
					});

					$('.node-episode').once('links').each(function() {
						var detail = {
							type: $(this).data('type'),
							id: $(this).data('id'),
							episode: $(this).data('episode'),
						};
						$(this).on('click.special', 'a.goto-episode', function(e) {
							detail.url = $(this).prop('href');
							detail.ctrlKey = e.ctrlKey;
							detail.metaKey = e.metaKey;
							var event = new CustomEvent('talGoto', {
								detail: detail
							});
							window.dispatchEvent(event);
							if ($jp.length) {
								e.preventDefault();
							}
						});
					});


					var $actions = $('ul.actions');
					$actions.find('a.download').off('click').on('click.download', function(e) {
						e.preventDefault();
					});

					// Shortcut
					$('a.cut').attr('target', '_blank');
					$actions.on('click.shortcut', 'a.cut', function(e) {
						var event = new CustomEvent('talShortcut', {
							detail: {
								type: $(this).data('type'),
								id: $(this).data('id'),
								url: $(this).prop('href')
							}
						});
						window.dispatchEvent(event);
					});

					// Play
					$('a.play').off('click').on('click.play', function(e) {
						e.preventDefault();
						if ($(this).hasClass('playing')) {
							$jp.jPlayer('pause');
						} else {

							if (!$body.hasClass('player') && $body.hasClass('node-type-homepage')) {
								var event = new CustomEvent('talGoto', {
									detail: {
										type: 'internal',
										url: $(this).prop('href')
									}
								});
								window.dispatchEvent(event);
							}

							var event = new CustomEvent('talPlay', {
								detail: {
									type: $(this).data('type'),
									id: $(this).data('id'),
									episode: $(this).data('episode'),
									act: $(this).data('act')
								}
							});
							window.dispatchEvent(event);
						}
					});
				},
				initPage = function() {
					initLinks();
					Drupal.behaviors.contextualLinks.attach();
					Drupal.behaviors.thislifeHeight.attach();
					Drupal.behaviors.thislifeAbout.attach();
					Drupal.behaviors.thislifeExtras.attach();
					if (typeof current_episode !== 'undefined') {
						if (playing) {
							$('a.play-' + current_episode.episode + '-' + current_act).addClass('playing');
							$('a.play-' + current_episode.episode).addClass('playing');
						} else {

							$('a.play-act').removeClass('playing');
							$('a.play-' + current_episode.episode).removeClass('playing');
						}
					}
				},
				gotoPath = function(url, data, replace) {
					var split = url.split('#'),
						path = split[0],
						fragment = split[1],
						scrollTo = 0;

					if (path[0] === '/') {
						if (last_path === path) {
							$top.removeClass('disabled');
							if (fragment && $('#' + fragment).length) {
								var offset = $('#' + fragment).offset().top;
								$('html, body').animate({
									scrollTop: offset
								}, 0);
							} else {
								$('html, body').animate({
									scrollTop: 0
								}, 0);
							}
						} else {
							if (typeof replace === 'undefined' && last_path.substring(0, 8) === '/archive') {
								// If archive, save current state for when we need to go back
								$saved_archive.html($main.html()).data({
									path: last_path,
									scroll: scrollTop
								});
							}
							var goto_path = Drupal.settings.basePath + 'goto' + path;
							if (path === '/') {
								goto_path += '?type=json';
							}
							$.ajax({
								url: goto_path,
								cache: true,
								data: data,
								dataType: 'json',
								error: function() {
									window.open(path, '_self');
								},
								success: function(data) {
									last_path = path;
									if (window.history.pushState) {
										if (replace) {
											window.history.replaceState({
												path: path
											}, '', path);
										} else {
											window.history.pushState({
												path: path
											}, '', path);
										}
									}
									$top.empty();
									$sidebar.empty();
									if (typeof replace !== 'undefined' && replace && url.substring(0, 8) === '/archive' && $saved_archive.html() && $saved_archive.data('path') === url) {
										// We're going back to the archive. Try to load from saved state.
										if ($saved_archive.data('scroll')) {
											scrollTo = $saved_archive.data('scroll');
										}
										$main.html($saved_archive.html());
										$main.find('.links-processed').removeClass('links-processed');
										$saved_archive.empty().removeData('scroll path');
									} else {
										$main.html(data.main);
									}
									if (typeof data.sidebar !== 'undefined') {
										$sidebar.html(data.sidebar);
									}
									if (typeof data.top !== 'undefined') {
										$top.html(data.top);
									}
									if (typeof data.color !== 'undefined') {
										$header.css('background-color', data.color);
									} else {
										$header.css('background-color', '');
									}
									$nav.find('li.active-trail').removeClass('active-trail');
									$nav.find('a.active').removeClass('active');
									open_menu = false;
									$body.removeClass('open-menu player-open');

									if (typeof data.facebook !== 'undefined') {
										$share_modal.find('a.facebook').attr('href', data.facebook);
									}
									if (typeof data.twitter !== 'undefined') {
										$share_modal.find('a.twitter').attr('href', data.twitter);
									}
									if (typeof data.mail !== 'undefined') {
										$share_modal.find('a.mail').attr('href', data.mail);
									}
									if (fragment && fragment === 'keyword' && $('#edit-keyword').length) {
										$('#edit-keyword').focus();
									} else if (fragment && $('#' + fragment).length) {
										var offset = $('#' + fragment).offset().top;
										$('html, body').animate({
											scrollTop: offset
										}, 0);
									} else {
										$('html, body').animate({
											scrollTop: scrollTo
										}, 0);
									}

									var classes = $('body').attr("class").split(' ');
									$.each(classes, function(i, c) {
										if (c.indexOf("page-") === 0) {
											$('body').removeClass(c);
										}
										if (c.indexOf("node-") === 0) {
											$('body').removeClass(c);
										}
									});
									if (typeof data.node_type !== 'undefined') {
										$('body').addClass('node-type-' + data.node_type);
									}
									if (typeof data.section !== 'undefined') {
										$('body').addClass('page-' + data.section);
										if (!isNaN(data.section)) {
											$('body').addClass('page-episode-number-' + data.section);
										}
										var section = data.section;
										if (section === 'listen') {
											section = 'how-to-listen';
										}
										$nav.find('li.' + section).addClass('active-trail');
									}

									$('title').text(data.title);
									initPage();

								}
							}); /* end ajax */

						}

					}
				};

			initLinks();
			$(window).on('load', function() {
				resizePage();
			}).on("throttledresize", function() {
				resizePage();
			});
			resizePage();
			window.requestAnimationFrame(updatePage);

			if ($jp.length) {
				var fingerprint,
					heartbeat = function() {
						if (typeof fingerprint !== 'undefined') {
							var data = {
								fingerprint: fingerprint,
								episode: current_episode.episode,
								position: last_position,
								source: 'thisamericanlife'
							};
							$.ajax({
								url: "https://hb.serialpodcast.org/",
								data: data
							});
						}
					};
				new Fingerprint2().get(function(result) {
					fingerprint = result;
				});


				$jp.jPlayer({
					ready: function() {
						ready = true;
					},
					timeupdate: function(event) {
						var position = parseInt(event.jPlayer.status.currentTime),
							new_total = Math.floor(event.jPlayer.status.duration),
							new_act = 0;
						if (typeof current_episode.acts !== 'undefined') {
							$.each(current_episode.acts, function(i, act) {
								if (position >= act.timestamp) {
									new_act = act.number;
								}
							});
						}
						if (new_act !== current_act) {
							$('a.play-act').removeClass('playing');
							if (typeof current_episode.acts !== 'undefined') {
								$.each(current_episode.acts, function(i, act) {
									if (act.number === new_act) {
										current_act = act.number;
										$info.find('.title').html(act.name).css('display', 'inline-block');
										if (typeof act.byline !== 'undefined') {
											$info.find('.contributor').html(act.byline);
										} else {
											$info.find('.contributor').empty();
										}
										if (typeof act.summary !== 'undefined') {
											$info.find('.body').html(act.summary);
										} else {
											$info.find('.body').empty();
										}
										initLinks();
									}
								});
							}
							$('a.play-' + current_episode.episode + '-' + current_act).addClass('playing');
						}
						if (new_total > 60 && position % 1 === 0) {
							if (position > (last_position + 14)) {
								last_position = position;
								heartbeat();
							}
						}

						var status = event.jPlayer.status;
						$('.jp-remaining').text($.jPlayer.convertTime(status.duration - status.currentTime));
					},
					play: function() {
						playing = true;
						$('body').addClass('player playing');
						$('a.play-' + current_episode.episode + '-' + current_act).addClass('playing');
						$('a.play-' + current_episode.episode).addClass('playing');
					},
					pause: function() {
						$('body').removeClass('playing');
						playing = false;
						$('a.play-act').removeClass('playing');
						$('a.play-' + current_episode.episode).removeClass('playing');
					},
					loadeddata: function(event) {
						var duration = event.jPlayer.status.duration;
						if (typeof current_episode.acts !== 'undefined') {
							$.each(current_episode.acts, function(i, act) {
								if (act.number > 0) {
									var $jump = $('<a/>').addClass('jump').css({
										left: (act.timestamp / duration * 100) + '%'
									}).on('click', function() {
										$jp.jPlayer('play', act.timestamp);
									});
									$progress.append($jump);
								}
							});
						}
					},
					ended: function() {
						playing = false;
						$('body').removeClass('playing');
						last_position = 0;
					},
					swfPath: jplayer_swfPath,
					wmode: "window"
				});

				$player.find('a.cut').on('click', function(e) {
					e.preventDefault();
					if (typeof current_episode !== false && typeof current_episode.episode !== 'undefined') {
						var url = 'https://shortcut.thisamericanlife.org/#/clipping/' + current_episode.episode;
						if ($jp.data("jPlayer").status.currentTime) {
							var timestamp = Math.floor($jp.data("jPlayer").status.currentTime);
							url += '/' + timestamp;
						}
						$jp.jPlayer('pause');
						window.open(url, '_blank');
					}
				});

				$(window).on('talGoto', function(e) {
					var detail = e.originalEvent.detail;
					if ((typeof detail.ctrlKey !== 'undefined' && detail.ctrlKey) || (typeof detail.metaKey !== 'undefined' && detail.metaKey)) {
						window.open(detail.url, '_blank');
					} else if ($body.hasClass('page-broadcast')) {
						window.open(detail.url, '_blank');
					} else if ($body.hasClass('page-status-404-not-found')) {
						window.open(detail.url, '_self');
					} else if ($body.hasClass('page-heartbeat')) {
						window.open(detail.url, '_self');
					} else if (detail.type === 'external') {
						window.open(detail.url, '_blank');
					} else {
						window.open(detail.url, 'welcome');
						/*
						var path = detail.url;
						if (path[0] !== '/') {
							path = path.replace('http://' + window.location.hostname, '');
							path = path.replace('https://' + window.location.hostname, '');
						}
						if (path.match(/^\/(user|admin|system|heartbeat)/i)) {
							// Admin path, open regularly
							window.open(detail.url, '_self');
						} else if (path[0] === '/') {
							gotoPath(path);
						}
						*/
					}
				});

				$(window).bind('popstate', function() {
					var path = window.location.pathname + window.location.search;
					gotoPath(path, {}, true);
				});

				$(window).on('talPlay', function(e) {
					var detail = e.originalEvent.detail;
					if (typeof detail.type !== 'undefined') {
						$('body').addClass('player');
						if (detail.type === 'episode') {
							if (typeof detail.episode !== 'undefined') {
								if (typeof current_episode === false || current_episode.episode !== detail.episode) {
									if ($('#playlist-data').length) {
										var data = JSON.parse($('#playlist-data').html());
										current_episode = data;
										if (typeof data.facebook !== 'undefined') {
											$player_share_modal.find('a.facebook').attr('href', data.facebook);
										}
										if (typeof data.twitter !== 'undefined') {
											$player_share_modal.find('a.twitter').attr('href', data.twitter);
										}
										if (typeof data.mail !== 'undefined') {
											$player_share_modal.find('a.mail').attr('href', data.mail);
										}
										playEpisode();
									} else {
										var $playlist_data = $('#playlist-data-' + detail.episode);
										if ($playlist_data.length) {
											var data = JSON.parse($playlist_data.html());
											current_episode = data;
											if (typeof data.facebook !== 'undefined') {
												$player_share_modal.find('a.facebook').attr('href', data.facebook);
											}
											if (typeof data.twitter !== 'undefined') {
												$player_share_modal.find('a.twitter').attr('href', data.twitter);
											}
											if (typeof data.mail !== 'undefined') {
												$player_share_modal.find('a.mail').attr('href', data.mail);
											}
											playEpisode();
										}
									}
								} else {
									$jp.jPlayer('play');
								}
							}
						} else if (detail.type === 'act') {
							if (typeof detail.episode !== 'undefined') {
								if (typeof current_episode === false || current_episode.episode !== detail.episode) {
									if ($('#playlist-data').length) {
										var data = JSON.parse($('#playlist-data').html());
										current_episode = data;
										if (typeof data.facebook !== 'undefined') {
											$player_share_modal.find('a.facebook').attr('href', data.facebook);
										}
										if (typeof data.twitter !== 'undefined') {
											$player_share_modal.find('a.twitter').attr('href', data.twitter);
										}
										if (typeof data.mail !== 'undefined') {
											$player_share_modal.find('a.mail').attr('href', data.mail);
										}
										playEpisode(detail.act);
									}
								} else if (detail.act !== current_act) {
									playAct(detail.act);
								} else {
									$jp.jPlayer('play');
								}
							}
						}
					}
				});

				$player.on('click', 'a, .jp-progress', function(e) {
					e.stopPropagation();
				});

				$player.on('click', 'a.close', function() {
					$body.removeClass('player-open');
				});

				$player.find('a.jp-rewind').click(function(e) {
					e.preventDefault();
					if (ready) {
						var new_time = Math.floor($jp.data("jPlayer").status.currentTime) - 10;
						if (new_time < 0) {
							new_time = 0;
						}
						$jp.jPlayer('play', new_time);
					}
				});

				$player.find('a.jp-forward').click(function(e) {
					e.preventDefault();
					if (ready) {
						var new_time = Math.floor($jp.data("jPlayer").status.currentTime) + 30;
						if (new_time < $jp.data("jPlayer").status.duration) {
							$jp.jPlayer('play', new_time);
						}
					}
				});

				$player.find('a.jp-previous').click(function(e) {
					e.preventDefault();
					if (ready && current_episode) {
						if (typeof current_episode.acts !== 'undefined') {
							var time = Math.floor($jp.data("jPlayer").status.currentTime),
								acts = current_episode.acts.slice().reverse();
							$.each(acts, function(i, act) {
								if (act.number <= current_act && act.timestamp < time) {
									$jp.jPlayer('play', act.timestamp);
									return false;
								}
							});
						}
					}
				});

				$player.find('a.jp-next').click(function(e) {
					e.preventDefault();
					if (ready && current_episode) {
						if (typeof current_episode.acts !== 'undefined') {
							var time = Math.floor($jp.data("jPlayer").status.currentTime);
							$.each(current_episode.acts, function(i, act) {
								if (act.number > current_act && act.timestamp > time) {
									$jp.jPlayer('play', act.timestamp);
									return false;
								}
							});
						}
					}
				});

				$player.on('click', function() {
					if (view === 'mobile') {
						if ($body.hasClass('player-open')) {

						} else {
							$body.addClass('player-open');
						}
					}
				});

			}

			$main.on('click', 'a.pager', function(e) {
				e.preventDefault();

				var $a = $(this),
					path = $(this).attr('href');
				$.ajax({
					url: path,
					cache: true,
					dataType: 'json',
					success: function(data) {
						var $pager = $a.parents('ul.pager').parent('.item-list');
						$pager.replaceWith(data.html);
						if (window.history.pushState) {
							window.history.replaceState({
								path: path,
							}, '', data.path);
						}
						Drupal.behaviors.thislifeHeight.attach();
						initLinks();
						if (typeof ga !== 'undefined') {
							ga('send', 'pageview', {
								'page': path,
								'title': data.title
							});
						}

					}
				});

			});

		}
	};



	Drupal.behaviors.thislifeHeight = {
		attach: function() {
			$('#main').find('.node.view-teaser').matchHeight();
		}
	};


	/* This is the end */

})(jQuery);
