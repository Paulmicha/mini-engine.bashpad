<?php

/**
 *  @file
 *  Mini Engine Bootstrap
 */

//      Engine
include 'engine/vendor/vendors.inc';
include 'engine/core/core.inc';

//      Config
include 'engine/core/config/default_settings.inc';
include 'config/settings.inc';

//      Extensions & runtime settings
include 'engine/core/extensions.inc';
include 'engine/core/config/php_runtime_settings.inc';

//      "Any request" handler
include 'routing/_global.inc';

//      Router
//      Matches current HTTP request with corresponding "handlers",
//      & includes those context-specific files matched.
include 'engine/core/router.inc';

//      "Render" templates
include 'engine/core/render.inc';

