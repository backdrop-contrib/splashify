jQuery(document).ready(function($) {
  var jsmode = Backdrop.settings.splashify.js_mode;

  // Prevents a flicker before the splash page shows up.
  if (jsmode == 'redirect') {
    hidepage();
  }

  var now = new Date();
  var nowtimeSeconds = now.getTime() / 1000;
  var referrer = document.referrer + '';
  var hostname = window.location.hostname + '';
  var splash = $.jStorage.get("splash", 0);
  var splashalways = Backdrop.settings.splashify.js_splash_always;
  var what_urls = Backdrop.settings.splashify.js_mode_settings.urls;
  var referrer_check = Backdrop.settings.splashify.js_disable_referrer_check;
  var js_mode_settings = Backdrop.settings.splashify.js_mode_settings;

  // This updates the referer string by taking out the url parameter from the
  // url...which is included from google search results (as an example).
  if(referrer.indexOf('?') != -1) {
    referrer = referrer.substr(0,referrer.indexOf('?'));
  }

  // Stop the splash page from show up if on the splash page. Also prevent
  // the splash from showing up from internal links (dependent on the
  // referrer check settings).
  if ((referrer.search(hostname) != -1 && !referrer_check) || jQuery.inArray(window.location.pathname, what_urls) > -1) {
    showpage();
    return;
  }

  // Determine if we should display the splash page.
  var displaysplash = false;
  if (!splash || splash < nowtimeSeconds || splashalways=='1') {
    displaysplash = true;
  }

  // Display the splash page?
  if(displaysplash){
    var expireAfter = Backdrop.settings.splashify.js_expire_after;
    var last_url = $.jStorage.get('splashlasturl', '');
    var url = '';

    // Set when the splash variable should expire next.
    $.jStorage.set("splash", nowtimeSeconds + expireAfter);

    // Determine the url we are working with, which is based on the mode.
    if(Backdrop.settings.splashify.js_mode_settings.system_splash != ''){
      // Display the system splash page.
      url = Backdrop.settings.splashify.js_mode_settings.system_splash;
    } else if(Backdrop.settings.splashify.js_mode_settings.mode == 'sequence'){
      // Display the splash pages in sequence.
      var new_url_index = 0;
      var last_url_index = jQuery.inArray(last_url, what_urls);
      if(last_url_index > -1 && last_url_index+1 < Backdrop.settings.splashify.js_mode_settings.total_urls){
        new_url_index = last_url_index + 1;
      }
      url = what_urls[new_url_index];
    } else if(Backdrop.settings.splashify.js_mode_settings.mode == 'random'){
      // Display a random splash page.
      var new_url_index = Math.floor(Math.random() * Backdrop.settings.splashify.js_mode_settings.total_urls);
      url = what_urls[new_url_index];
    }

    $.jStorage.set('splashlasturl', url);

    /**
     * This function displays the splash.
     */
    function open_splash() {
      // Display the splash page.
      if(jsmode == 'redirect') {
        // Redirect.
        window.location.replace(url);
      } else if(jsmode == 'colorbox') {
        // Open a ColorBox.
        var colorbox_options = {
          transition:'elastic',
          iframe:true,
          href:url,
          width:Backdrop.settings.splashify.js_mode_settings.size_width,
          height:Backdrop.settings.splashify.js_mode_settings.size_height
        };

        if (url.substring(0, 16) == "_splashify_ajax/") {
          // Load the ajax node page via AJAX.
          colorbox_options.iframe = false;
        }

        $.colorbox(colorbox_options);
      } else if(jsmode == 'window') {
        // Open a popup window.
        window.open(url, 'splash', Backdrop.settings.splashify.js_mode_settings.size);
      }
    }

    /**
     * This function closes the splash.
     */
    function close_splash() {
      $.colorbox.close();
    }

    if (js_mode_settings.how_delay_enable == 1) {
      setTimeout(open_splash, js_mode_settings.how_delay);
    } else {
      open_splash();
    }

    if (js_mode_settings.how_autoclose_enable == 1 && jsmode == 'colorbox') {
      setTimeout(close_splash, js_mode_settings.how_autoclose);
    }
  } else if(jsmode == 'redirect') {
      showpage();
  }
});

function showpage() {
  jQuery('html').show();
}

function hidepage() {
  jQuery('html').hide();
}
