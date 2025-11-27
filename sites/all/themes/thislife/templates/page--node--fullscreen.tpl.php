<?php

/**
* @file
* Default theme implementation to display a single Drupal page.
*
* The doctype, html, head and body tags are not in this template. Instead they
* can be found in the html.tpl.php template in this directory.
*
* Available variables:
*
* General utility variables:
* - $base_path: The base URL path of the Drupal installation. At the very
*   least, this will always default to /.
* - $directory: The directory the template is located in, e.g. modules/system
*   or themes/bartik.
* - $is_front: TRUE if the current page is the front page.
* - $logged_in: TRUE if the user is registered and signed in.
* - $is_admin: TRUE if the user has permission to access administration pages.
*
* Site identity:
* - $front_page: The URL of the front page. Use this instead of $base_path,
*   when linking to the front page. This includes the language domain or
*   prefix.
* - $logo: The path to the logo image, as defined in theme configuration.
* - $site_name: The name of the site, empty when display has been disabled
*   in theme settings.
* - $site_slogan: The slogan of the site, empty when display has been disabled
*   in theme settings.
*
* Navigation:
* - $main_menu (array): An array containing the Main menu links for the
*   site, if they have been configured.
* - $secondary_menu (array): An array containing the Secondary menu links for
*   the site, if they have been configured.
* - $breadcrumb: The breadcrumb trail for the current page.
*
* Page content (in order of occurrence in the default page.tpl.php):
* - $title_prefix (array): An array containing additional output populated by
*   modules, intended to be displayed in front of the main title tag that
*   appears in the template.
* - $title: The page title, for use in the actual HTML content.
* - $title_suffix (array): An array containing additional output populated by
*   modules, intended to be displayed after the main title tag that appears in
*   the template.
* - $messages: HTML for status and error messages. Should be displayed
*   prominently.
* - $tabs (array): Tabs linking to any sub-pages beneath the current page
*   (e.g., the view and edit tabs when displaying a node).
* - $action_links (array): Actions local to the page, such as 'Add menu' on the
*   menu administration interface.
* - $feed_icons: A string of all feed icons for the current page.
* - $node: The node object, if there is an automatically-loaded node
*   associated with the page, and the node ID is the second argument
*   in the page's path (e.g. node/12345 and node/12345/revisions, but not
*   comment/reply/12345).
*
* Regions:
* - $page['help']: Dynamic help text, mostly for admin pages.
* - $page['highlighted']: Items for the highlighted content region.
* - $page['content']: The main content of the current page.
* - $page['sidebar_first']: Items for the first sidebar.
* - $page['sidebar_second']: Items for the second sidebar.
* - $page['header']: Items for the header region.
* - $page['footer']: Items for the footer region.
*
* @see template_preprocess()
* @see template_preprocess_page()
* @see template_process()
* @see html.tpl.php
*
* @ingroup themeable
*/
?>
<header id="site-header" role="banner"<?php print $title_attributes; ?>>
  <div class="container clearfix">
    <div class="site-name">
      <a href="<?php print $front_page ?>"><span class="icon icon-flag"></span><span class="icon icon-wordmark"></span><span class="element-invisible"><?php print $site_name ?></span></a>
    </div>
    <h1 class="element-invisible"><?php print $title ?></h1>
  </div>
</header>
<main id="main">
  <?php print render($page['content']); ?>
  <script src='https://api.tiles.mapbox.com/mapbox-gl-js/v0.44.1/mapbox-gl.js'></script>
<link href='https://api.tiles.mapbox.com/mapbox-gl-js/v0.44.1/mapbox-gl.css' rel='stylesheet' />
<script src="https://code.jquery.com/jquery-2.1.4.min.js" type="text/javascript"></script>

<style>
.mapboxgl-map {
	font-family: "maiola", Georgia, "Times New Roman", Times, serif;
}
.mapboxgl-popup-close-button {
	color:#ffffff;
}
.mapboxgl-popup-content {
	background: rgba(19,59,76,0.86);
	color: #ffffff;
	padding-top:10px;
  max-width: 276px;
  line-height: 1.35;
  font-size: 12pt;
}
.mapboxgl-popup-content h2 {
	padding-bottom:2px;
}
.mapboxgl-popup-anchor-bottom .mapboxgl-popup-tip {
	border-top-color:rgba(19,59,76,0.86);
    margin-bottom: 10px;
}

.mapboxgl-popup-anchor-top .mapboxgl-popup-tip {
	border-bottom-color:rgba(19,59,76,0.86);
    margin-top: 10px;
}

.mapboxgl-popup-anchor-right .mapboxgl-popup-tip {
	border-left-color:rgba(19,59,76,0.86);
}

.mapboxgl-popup-anchor-left .mapboxgl-popup-tip {
	border-right-color:rgba(19,59,76,0.86);
}

h2 {
	margin:0px;
	padding:0px;
}

a {
  color:#ffffff;
}

p {
  margin: 0;
  padding: 5px 0px;
}
#features {
  position: absolute;
	padding:20px 20px 10px 20px;
  z-index:1000;
  font-family: "maiola", Georgia, "Times New Roman", Times, serif;
  background: rgba(19,59,76,0.86);
  color:#ffffff;
}
#features ol {
	margin:0px;
	padding:10px 16px 20px 16px;
}
#features ol li {
	line-height:1.45em;
	opacity:0.6;
	cursor: pointer;
}
#features ol li.active, #features ol li:hover {
	opacity:1;
}
nav {
  width: 82px;
  margin: 0 auto;
}
nav > a {
	display:block;
  float:left;
}
nav > a > span {
	display:block;
	margin-top:5px;
  width: 0;
  height: 0;
	opacity: 0.6;
}
nav > a:hover > span {
	opacity:1;
}
#mapHome {
	opacity:1;
	cursor:pointer;
}
#mapHome:hover {
	opacity:1 !important;
}
#prev span {
  border-top: 10px solid transparent;
  border-bottom: 10px solid transparent;
  border-right:10px solid #ffffff;
	margin-right: 15px;
}
#next span {
  border-top: 10px solid transparent;
  border-bottom: 10px solid transparent;
  border-left:10px solid #ffffff;
	margin-left: 15px;
}
@media (min-width: 768px) {
	#features {
		width:250px;
	  bottom: 35px;
	  left:35px;
	}
	#mobile_slide {
		display:none;
	}
}
@media (max-width: 767px) {
	#features {
		width: 96%;
		bottom: 0px;
    padding-left: 2%;
    padding-right: 2%;
		left:0%;
	}
	#features ol {
		display:none;
	}
	.mapboxgl-popup {
		display:none !important;
	}
	#mobile_slide, #intro_text {
		padding: 0px 25px;
        padding-bottom: 10px;
	}
	#mobile_slide h2 {
		padding-bottom: 5px;
	}
	#intro_text.hide {
		display:none;
	}
}


#main {
  font-size: 16px;
  line-height: 1.32;
}
#main h2 {
  font-size: 24px;
  line-height: 27px;
  margin: 0;
  padding: 0;
}
#main .mapboxgl-popup-content {
  font-size: 16px;
  line-height: 1.4;
}
#main .mapboxgl-popup-content h2 {
  padding-bottom: 8px;
}
#features {
  width: 100%;
}
@media (min-width: 768px) {
	#features {
		width: 290px;
	}
  .mapboxgl-popup-content {
    padding: 20px;
    max-width: 290px;
  }
}
#features a {
  color: #ffffff;
  text-decoration: underline;
}
body { margin:0; padding:0; font-family: "maiola", Georgia, "Times New Roman", Times, serif; }
#map { position:fixed; top:61px; bottom:0; width:100%; }
</style>


<div id='features'>
  <div id="mobile_slide">
  </div>
  <div id="intro_text">
    <h2>Walls</h2>
    <p>Once a wall is built, it becomes a fact on the landscape that can totally change the logic of the world around it. In the episode “The Walls,” we have stories about people at walls all over the globe. Fly around to visit the walls in each story.  <a href="https://www.thisamericanlife.org/641/the-walls" target="_blank">Listen to 641: "The Walls."</a></p>
  </div>
  <ol>
  <li>Northern Ireland</li>
  <li>Turkey and Syria</li>
  <li>Western Sahara</li>
  <li>United States and Mexico</li>
  <li>Pakistan and India</li>
  <li>North Korea and South Korea</li>
  <li>Morocco and Spain</li>
  <li>Norway and Russia</li>
  </ol>

  <nav>
   <a href="#" id="prev"><span></span></a>
   <a><img src="data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB3aWR0aD0iMzAiIGhlaWdodD0iMzAiIHZpZXdCb3g9IjAgMCAzMCAzMCI+PGRlZnM+PGNsaXBQYXRoIGlkPSJjbGlwLXBhdGgiPjxyZWN0IHg9Ii0xNTQiIHk9Ii0xMTI4LjUiIHdpZHRoPSIxNzI5IiBoZWlnaHQ9IjEwMzUiIGZpbGw9Im5vbmUiLz48L2NsaXBQYXRoPjwvZGVmcz48dGl0bGU+QXJ0Ym9hcmQgMzwvdGl0bGU+PGcgaWQ9IlBBTkVfY29weSIgZGF0YS1uYW1lPSJQQU5FIGNvcHkiPjxnIGNsaXAtcGF0aD0idXJsKCNjbGlwLXBhdGgpIj48aW1hZ2Ugd2lkdGg9IjE3ODIiIGhlaWdodD0iMTE3MiIgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoLTE2OCAtMTE0OC41KSBzY2FsZSgwLjk4NTQpIiB4bGluazpocmVmPSIuLi8uLi8uLi9Wb2x1bWVzL1BST0pFQ1RTL1RoaXNfQW1lcmljYW5fTGlmZS8xOTk3X0VwNjQxX1RoZV9XYWxscy9EZXNpZ24vcGxhY2VkX2ltYWdlcy9DdWV0YS5wbmciLz48L2c+PC9nPjxnIGlkPSJQQU5FX2NvcHlfMiIgZGF0YS1uYW1lPSJQQU5FIGNvcHkgMiI+PHBhdGggZD0iTTE1LC41MDZhMTQuNTIsMTQuNTIsMCwwLDAtMy4xMy4zNDgxYy45Njc5LjY3MiwzLjEzLDEuMjQ3NiwzLjEzLDEuOTQxMSwwLC43NjMxLTEuNjY5MiwxLjUyNjItMS42NjkyLDIuMjg5MnMuOTA2Mi43NjMxLDEuNjY5Mi43NjMxLjc2MzEtLjc2MzEsMS41MjYyLTEuNTI2MiwwLTEuNTI2MiwwLTIuMjg5MlMxOC4xLDIuOTg2MSwxOC4xLDIuOTg2MXMxLjQ3ODUsMS4zMzU0LDIuMjQxNiwxLjMzNTRjLjQ5MjUsMC0xLjE1NzUsMS41MTYyLTEuNTI2MiwyLjYyMzEtLjIwMjYuNjA4MSwxLjY2OTIuNDc2OS45MDYyLDEuOTA3Ny0uNTA3OS45NTIyLS45MDYyLjA0NzctMS42NjkyLjgxMDhzLS4zMzM4LDEuMjg3Ny0uMzMzOCwyLjA1MDguMzMzOCwxLjAwMTUsMS44NiwxLjc2NDZTMTguMSwxNS4zODYyLDE4LjEsMTQuNjIzMWEyLjI4NTYsMi4yODU2LDAsMCwwLTEuODEyMy0xLjg2Yy0uNzYzMSwwLTEuNjIxNi4yMzg1LTIuMzg0Ni4yMzg1cy0xLjg2Ljc2MzEtMS44NiwxLjUyNjIsMi4xOTM5LjQ3NjksMi4xOTM5LjQ3NjlsMS41MjYyLS43NjMxQTMuNzY3OCwzLjc2NzgsMCwwLDEsMTUsMTYuNTMwOGMtLjc2MzEuNzYzMS43NjMxLDAsMS41MjYyLjc2MzFhMS40MzA4LDEuNDMwOCwwLDAsMCwyLjI4OTIsMCwyLjgwMzMsMi44MDMzLDAsMCwxLDIuMjg5Mi0uNzYzMWMuNzYzMSwwLDEuNTI2MiwyLjI4OTIsMi4yODkyLDMuMDUyM3MuNzYzMSwyLjI4OTItLjc2MzEsMy44MTU0LTIuMzIzOC45MzM1LTIuNjcwOCwxLjk1NTRjLS44NDI4LDIuNDgxNi0yLjYwMzcsMy4zNTMzLTQuMTExOSw0LjEwNjVBMTQuNDg5MywxNC40ODkzLDAsMCwwLDE1LC41MDZaIiBmaWxsPSIjZmZmIi8+PHBhdGggZD0iTTE1LDI4Ljc0bC43NjMxLTEuNTI2MmE3LjEzNTIsNy4xMzUyLDAsMCwwLC43MTU0LTIuMTkzOWMwLS43NjMxLS43MTU0LTEuNjIxNi0uNzE1NC0yLjM4NDZzLTEuNTI2Mi0uNzYzMS0xLjUyNjItMi4yODkyVjE4LjA1NjlsLTEuNTI2Mi0xLjUyNjJhNy42ODY0LDcuNjg2NCwwLDAsMC0yLjI4OTItLjc2MzFjLS43NjMxLDAtLjc2MzEtLjc2MzEtLjc2MzEtMi4yODkycy0uNzYzMS0xLjUyNjItLjc2MzEtMS41MjYyYTIuNjY0MiwyLjY2NDIsMCwwLDAtLjc2MzEsMS41MjYyYzAsLjc2MzEsMCwxLjUyNjItLjc2MzEsMS41MjYyczAtLjc2MzEsMC0uNzYzMS42Njc3LTIuNDMyMy0uMDk1NC0zLjE5NTQtLjA5NTQtMS4zMzU0LjY2NzctMy42MjQ2TDcuMTMwNyw0LjY1NTMsNS45NzQsMy42N2ExNC40ODUyLDE0LjQ4NTIsMCwwLDAsOC44NDM0LDI1LjgyNEExLjY0LDEuNjQsMCwwLDEsMTUsMjguNzRaIiBmaWxsPSIjZmZmIi8+PC9nPjwvc3ZnPg==" id="mapHome"></a>
   <a href="#" id="next"><span></span></a>
  </nav>
</div>

<div id='map'></div>

<script>

var map = new mapboxgl.Map({
    container: 'map',
    style: 'mapbox://styles/whitneydangerfield/cjeu8fy333pxu2slm0g9cejai?optimize=true',
    center: [0,35],
    zoom: 2,
    pitch: 0,
    bearing: 0,
    minZoom: 1,
    maxZoom: 16.9
});

var walls = {
    '0': {
        id: 0,
        title: 'Northern Ireland',
        text: 'In Northern Ireland, it’s been 20 years since peace was declared between Protestants and Catholics, but there are still walls, some over 50 feet high, that separate neighborhoods. A few gates allow traffic through.',
        //duration: 4000,
        bearing: -32,
        center: [-5.94764,54.601427],
        flightLng: -5.94786,
        flightLat: 54.60166,
        zoom: 15.56,
        pitch: 52,
        speed: 0.8
    },
    '1': {
        id: 1,
        title: 'Turkey and Syria',
        text: 'The wall between Turkey and Syria will be more than 500 miles long when it’s finished. It’s about 10 feet high and made out of concrete blocks.',
        //duration: 4000,
        center: [37.085339, 36.6370716],
        flightLng: 37.08597,
        flightLat: 36.63930,
        bearing: 13.666,
        zoom: 13.338,
        pitch: 60,
        speed: 0.8
    },
    '2': {
        id: 2,
        title: 'Western Sahara',
        text: 'This barrier is about 1,700 miles along, across the desert, and is made of several walls of rock and sand, plus ditches, barbed wire, more than 100,000 soldiers, and several million landmines. A massive refugee settlement is nearby in Algeria.',
        bearing: 60,
        center: [-8.75954, 27.42639366],
        flightLng: -8.75721,
        flightLat: 27.42757,
        zoom: 13.8,
        pitch: 60,
        speed: 0.8
    },
    '3': {
        id: 3,
        title: 'United States and Mexico',
        text: 'Nogales is the site of the first official border fence between the U.S. and Mexico. Recently, a specific population started settling in the Mexican side of the town: Deported parents who are trying to be as close as they can to their kids in the U.S.',
        bearing: -45,
        center: [-110.94429, 31.334454],
        flightLng: -110.94758,
        flightLat: 31.33696,
        zoom: 13.26,
        pitch: 60,
        speed: 0.8
    },
    '4': {
        id: 4,
        title: 'Pakistan and India',
        text: 'There is only one road on this long border where you can cross between these two countries. It’s in the town of Wagah where every day at sundown, soldiers perform an elaborately choreographed ceremony that includes sky-high kicks. One year India lowered the height of its kicks as a sign of good will. Pakistan did not follow suit.',
        bearing: 51,
        center: [74.5737, 31.6051],
        flightLng: 74.575753,
        flightLat: 31.6063158,
        zoom: 13.6,
        pitch: 50,
        speed: 0.8
    },
    '5': {
        id: 5,
        title: 'North Korea and South Korea',
        text: 'The Demilitarized Zone, shown here, runs 150 miles between North and South Korea. North Korea says there’s a concrete wall inside that is 62 feet thick, several stories high, and runs the entire length of the zone. Curiously, according to North Korea, you can only see this wall from the north. They say it’s invisible from the south.',
        bearing: -48.50,
        center: [126.679, 37.9094],
        flightLng: 126.67484,
        flightLat: 37.912407,
        zoom: 13.5,
        pitch: 50,
        speed: 0.8
    },
    '6': {
        id: 6,
        title: 'Morocco and Spain',
        text: 'The Spanish city of Melilla is disconnected from the rest of Spain on the northern coast of Africa. The town is less than five square miles, but there are four fences around it, with razor wire on top, cameras, and guards. Sometimes hundreds of people rush all at once to try to get over.',
        bearing: 33.77919,
        center: [-2.959524, 35.27727],
        flightLng: -2.959524,
        flightLat: 35.28727,
        zoom: 13.28,
        pitch: 52.5,
        speed: 0.8
    },
    '7': {
        id: 7,
        title: 'Norway and Russia',
        text: 'This wall, built as a barrier to immigrants, is perplexingly only 650 feet long. Norway built it right next to a 120-mile fence that Russia already had at the same border.',
        bearing: 44.1791,
        center: [30.206522279, 69.65802348655097],
        flightLng: 30.206522279,
        flightLat: 69.65802348655097,
        zoom: 14.3,
        pitch: 52.5,
        speed: 0.8
    }
};

var hoverPopup = new mapboxgl.Popup({closeButton: false, closeOnClick: true});
var popup = new mapboxgl.Popup({closeOnClick: true});
var currentId = 0;

var flying = false;

map.on('load', function() {

    if ($(window).width() > 960) {
        map.setLayoutProperty('mapbox-satellite', 'visibility', 'none');
    }


    map.addSource("Walls", {
        type: "vector",
        url: "mapbox://whitneydangerfield.2e9a16d3"
        //url: "mapbox://whitneydangerfield.b055bb49" //Backup Source
    });

    map.addLayer({
        "id": "Walls_Lines",
        "type": "line",
        "source": "Walls",
        "source-layer": "Wall_Locations_polylines",
        "minzoom": 1,
        "maxzoom": 12,
        "paint": {
            "line-color": '#eb0000',
            "line-width": 2.25
        }
    });

    map.addLayer({
        "id": "Walls_Pts",
        "type": "symbol",
        "source": "Walls",
        "source-layer": "Wall_Locations_points",
        "minzoom": 1,
        "maxzoom": 10,
        "layout": {
            "icon-image": 'Red_dot_30',
            "icon-size": .65
        },
        "paint": {
            'icon-opacity':{
                "type": "exponential",
                "stops": [
                    [9, 1],
                    [10, 0]
                ]
            }
        }
    });

    map.addLayer({
        "id": "Added_Boundary_Segments",
        "type": "line",
        "source": "Walls",
        "source-layer": "Backup_Boundary_Lines",
        "minzoom": 12,
        "maxzoom": 17,
        "paint": {
            "line-color": '#000000',
            "line-width": 3,
            "line-opacity":.25
        }
    });

    map.addLayer({
        "id": "Walls_Polys",
        "type": "fill-extrusion",
        "source": "Walls",
        "source-layer": "Wall_Locations_polygons",
        "minzoom": 8,
        "paint": {
            'fill-extrusion-color': '#eb0000',
            'fill-extrusion-height':{
                "type": "exponential",
                "stops": [
                    [11, 100],
                    [12, 100],
                    [13, 100],
                    [14, 75],
                    [15, 25],
                    [16, 10],
                    [17, 3]
                ]
            },
            'fill-extrusion-base': 0,
            // Make extrusions fully opaque at max zoom.
            'fill-extrusion-opacity':{
                "type": "exponential",
                "stops": [
                    [11, .6],
                    [12, .5],
                    [13, .5],
                    [14, .4],
                    [15, .35],
                    [16, .3],
                    [17, 0]
                ]
            }
        }
    });

    map.addLayer({
        "id": "Added_Labels",
        "type": "symbol",
        "source": "Walls",
        "source-layer": "Added_Label_points",
        "minzoom": 10,
        "maxzoom": 17,
        "layout": {
            "text-field": '{Name}',
            "text-font": ["League Spartan Bold", "Arial Unicode MS Bold"],
            "text-letter-spacing": 1.5,
            "text-line-height": 2,
            "text-size": 20,
            "text-transform": 'uppercase'
        },
        "paint": {
            "text-color": 'white',
            "text-opacity": .5
        }
    });

    map.on('zoom', function(){
        if (map.getZoom() < 10){
            popup.remove();
            if ($(window).width() > 960) {
                map.setLayoutProperty('mapbox-satellite', 'visibility', 'none');
            }
            //map.setLayoutProperty('mapbox-satellite', 'visibility', 'none');
        }
        popup.remove()
    });

    map.on('flystart', function(){
        if (popup.isOpen() === true){
            popup.remove();
        }
        popup.remove()
        flying = true;
        console.log(flying);
    });
    map.on('flyend', function(){
        flying = false;
        console.log('done flying');
    });

    //Hover popup and pointer to cursor
    map.on("mousemove", 'Walls_Pts', function(e) {
        map.getCanvas().style.cursor = 'pointer';
        buildHoverPopup(e, 'Walls_Pts');
    });
    map.on("mouseleave", 'Walls_Pts', function() {
        map.getCanvas().style.cursor = '';
        hoverPopup.remove();
    });

    map.on("click", 'Walls_Pts', function(e) {
        var clickedFeature = map.queryRenderedFeatures(e.point, 'Walls_Pts');
        var wallId = clickedFeature[0].properties.Id;
        if (clickedFeature.length){
        flyToWall(wallId);
        map.fire('flystart')
        }
    });

});

$("#mapHome").on('click',function (){
   $("#features li").removeClass('active');
	 $("#intro_text").removeClass('hide');
	 $("#mobile_slide").html("");
	 $(this).css('opacity',1);
	 map.flyTo({
        center: [0,35],
        zoom: 2,
        pitch: 0,
        bearing: 0
    });
});

$("#features li").on('click',function (){
    var id = $(this).index();
		$("#features li").removeClass('active');
		$("#mapHome").css('opacity',0.6);
		$(this).addClass('active');
    flyToWall(id);
    map.fire('flystart')
});

$("#next").on('click',function(){
	var next = 0;
	if ($("#features li.active").length) {
		next = $("#features li.active").index()+1;
		if (next == $("#features li").length) next = 0;
	}
	$("#features li").removeClass('active');
	$("#mapHome").css('opacity',0.6);
	$("#features li").eq(next).addClass('active');
  flyToWall(next);
  map.fire('flystart')
});

$("#prev").on('click',function(){
	var prev = $("#features li").length-1;
	if ($("#features li.active").length) {
		prev = $("#features li.active").index()-1;
	}
	if (prev < 0) prev = $("#features li").length-1;
	$("#features li").removeClass('active');
	$("#mapHome").css('opacity',0.6);
	$("#features li").eq(prev).addClass('active');
  flyToWall(prev);
  map.fire('flystart')
});

function buildHoverPopup(location, layer) {
  var hoverFeature = map.queryRenderedFeatures(location.point, layer);
  var hoverPopupText = "";
  if (popup.isOpen() === true){
      hoverPopup.remove();
  } else{
      hoverPopup.remove();
      hoverPopupText = hoverFeature[0].properties.Place;
      hoverPopup.setLngLat(location.lngLat)
          .setHTML(hoverPopupText)
          .addTo(map);
  }
}

function flyToWall(id) {
    if ($(window).width() > 960) {
        map.setLayoutProperty('mapbox-satellite', 'visibility', 'none');
    }
    //map.setLayoutProperty('mapbox-satellite', 'visibility', 'none');
    popup.remove();
    currentId = id;
    var flightLng = walls[currentId].flightLng;
    var flightLat = walls[currentId].flightLat;
    var flightZoom = walls[currentId].zoom;
    var flightBearing = walls[currentId].bearing;
    var flightSpeed = walls[currentId].speed;
    var flightPitch = walls[currentId].pitch;

    //get notice of start/stop of flight path

    if ($(window).width() <= 960) {
        map.flyTo(walls[id]);
    } else{
        map.flyTo({center: [flightLng,flightLat], zoom: flightZoom, bearing: flightBearing, speed: flightSpeed, pitch: flightPitch});
    }
		var our_html = '<h2>'+walls[currentId].title+'</h2>'+walls[currentId].text;
		$("#mobile_slide").html(our_html);
		setTimeout(function() {
		  moveMapBoxCredits();
		}, 100);
		$("#intro_text").addClass('hide');
    console.log("flying to "+walls[id].title);
    //at end of flight path, trigger popup or trigger text fading on
}

map.on('moveend', function(){
    console.log("Current id is " + currentId);
		var our_html = '<h2>'+walls[currentId].title+'</h2>'+walls[currentId].text;

    if(flying) {
        map.fire('flyend');
        map.setLayoutProperty('mapbox-satellite', 'visibility', 'visible');
        console.log("Popup created at "+walls[currentId].title)
        popup.setLngLat(walls[currentId].center)
             .setHTML(our_html)
            .addTo(map);
    }
});

function moveMapBoxCredits() {
  if($("#features").css('left') === "0px") {
	  $(".mapboxgl-ctrl-bottom-left, .mapboxgl-ctrl-bottom-right").animate({'bottom':$("#features").outerHeight()});
  } else {
	  $(".mapboxgl-ctrl-bottom-left, .mapboxgl-ctrl-bottom-right").css('bottom',0);
  }
}
$(window).on("load resize", function(){
	moveMapBoxCredits();
});

</script>

</main>
<!--#content-->
