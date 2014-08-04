<?php

/**
 *  @file
 *  Mini Engine Bootstrap
 *  ("front controller")
 */

//      Engine
include 'engine/vendor/vendors.inc';
include 'engine/core/core.inc';

//      Settings
include 'engine/core/config/default_settings.inc';
include 'config/settings.inc';

//      Extensions
include 'engine/core/extend.inc';

//      Php settings
include 'engine/core/config/php_runtime_settings.inc';

//      "Any request" handler
include 'routing/_global.inc';

//      Router
//      Matches current HTTP request with corresponding "handlers",
//      & includes those context-specific files matched.
include 'engine/core/router.inc';

//      "Render" templates
include 'engine/core/render.inc';

