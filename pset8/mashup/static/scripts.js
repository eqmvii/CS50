// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 39.9168, lng: -75.3877}, // Media, PA
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);
    
    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    // place is a JSONified row from the database of U.S. places
    
    // Instantiate marker: google.maps.marker
    //console.log("this function has been called")
    // console.log(place.latitude + " " + place.longitude + typeof place.latitude + typeof place.longitude);
    var myLatLang ={lat: place.latitude, lng: place.longitude};
    // console.log(myLatLang);
    
    var marker = new google.maps.Marker({
        position: myLatLang,
        label: "SPÖKA NEWS: " + place.place_name + ", " + place.admin_name1,
        map: map,
        icon: "http://i.imgur.com/6TZeioD.png"
    });
    
    //$.getJSON(Flask.url_for("articles"),
    
    articleURL = Flask.url_for("articles") + "?geo=" + place.postal_code;
    console.log(articleURL);
    
    
    $.getJSON(articleURL)
    .done(function(data, textStatus, jqXHR) {
        
        // Save data for use in our map 
        marker.addListener('click', function() {
        console.log(data);
        var chunk = "<h3>News from this area:</h3><ul>"; // the HTML needed to inject into the info window
        // Use a maximum of 5 news links
        var linkCap = 0;
        if (data.length > 5) {
            linkCap = 5;
        }
        else {
            linkCap = data.length;
        }
        
        // Add news link HTML into the chunk to be sent to the showInfo function
        for (var i = 0; i < linkCap; i++)
            {
                var longTitle = data[i].title
                if (longTitle.length > 100) {
                    longTitle = longTitle.substring(0,97) + "...";
                }
                chunk += ('<li><a href="' + data[i].link + '">' + longTitle + "</a></li>");
            }
        chunk += "</ul>";
        //chunk = "Hi there!";
        //console.log("Sending: " + chunk + " to showInfo");
        showInfo(marker, chunk);
        map.setCenter(marker.getPosition());
        });
        
    })
    .fail(function(jqXHR, textStatus, errorThrown) {
        // log error to browser's console
        console.log(errorThrown.toString());
    });


    // Listen for clicks on marker: google.maps.event.addListener

  
    // Get articles for palce: $.getJSON
    
    // Build list of links to articles: Markers is a global variable. Find a way to represent the whole list as an array, object, etc. Be able to remove.
    
    // Remember marker: 
    markers.push(marker);
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" +
                "{{place_name}}, {{admin_name1}}, {{postal_code}}" +
                "</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box on page load
    $("#q").focus();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    // iterate through markers and remove them
    for (i = 0; i < markers.length; i++)
    {
        //console.log("Removing marker...")
        markers[i].setMap(null);
    }
    markers = [];
    

}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {

       // remove old markers from map
       removeMarkers();

       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
           addMarker(data[i]);
       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
};
