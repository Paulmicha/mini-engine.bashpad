<?php

/**
 *  @file
 *  Mini-engine
 *  Hub include file - core APIs
 */

include 'api_routing.inc';
include 'api_pager.inc';

if ( $enable_db ||
    $enable_dblog ||
    $enable_cache ||
    $enable_tag ||
    $enable_token ||
    $enable_datatable ||
    $enable_entity ||
    $enable_ssh ) {
    include 'api_db.inc';
}

if ( $enable_dblog ) {
    include 'api_dblog.inc';
}

if ( $enable_cache ) {
    include 'api_cache.inc';
}

if ( $enable_entity )
{
    include 'api_entity.inc';
    include 'api_entity_storage.inc';
}

if ( $enable_ssh ) {
    include 'api_ssh.inc';
}

if ( $enable_tag ) {
    include 'api_tags.inc';
}

if ( $enable_token ) {
    include 'api_tokens.inc';
}

if ( $enable_form ) {
    include 'api_form.inc';
}

