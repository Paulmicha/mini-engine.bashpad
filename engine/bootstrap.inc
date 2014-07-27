<?php

/**
 *  @file
 *  Mini Engine Bootstrap
 */

//      Config
include 'engine/core/config/default_settings.inc';
include 'config/settings.inc';
include 'engine/core/config/php_runtime_settings.inc';

//      Engine
include 'engine/vendor/vendors.inc';
include 'engine/core/core.inc';

//      Global context handler (always included)
include 'routing/_global.inc';

//      Request handling
include 'engine/core/http_request_engine.inc';

//      Rendering templates
include 'engine/core/theme_rendering.inc';

