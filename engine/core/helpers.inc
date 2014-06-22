<?php

/**
 *  @file
 *  Mini-engine
 *  Hub include file : helpers
 */

include 'helpers_str.inc';
include 'helpers_misc.inc';
include 'helpers_array.inc';
include 'helpers_theming.inc';

if ( $enable_date || $enable_db ) {
    include 'helpers_date.inc';
}

if ( $enable_debug ) {
    include 'helpers_debug.inc';
}

if ( $enable_file || $enable_form_file_selector ) {
    include 'helpers_file.inc';
}

if ( $enable_log ) {
    include 'helpers_logs.inc';
}

if ( $enable_notification ) {
    include 'helpers_notifications.inc';
}

if ( $enable_form || $enable_form_theming ) {
    include 'helpers_form_theming.inc';
}

if ( $enable_form || $enable_form_multigroup ) {
    include 'helpers_form_multigroup.inc';
}

if ( $enable_form || $enable_form_file_selector ) {
    include 'helpers_form_file_selector.inc';
}

if ( $enable_datatable || ( $enable_cache && $enable_system_routes )) {
    include 'helpers_datatable.inc';
}


