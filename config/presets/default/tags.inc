<?php

/**
 *  @file
 *  Default tags
 *  
 *  @todo 2013/12/18 23:22:46 - new folder structure for @evol 2013/12/18 23:11:56 (import / export feature)
 */

//      Set default tags
$default_tags = array(
    
    'Type of Server' => array(
        'Debian' => array(
            'Debian 6 "Squeeze"',
            'Debian 7 "Wheezy"',
        ),
        'Ubuntu' => array(
            'Ubuntu Server 12 LTS',
        ),
        'CentOS' => array(
            'CentOS 6',
        ),
    ),
    
    'Type of environment' => array(
        'Dev',
        'Staging',
        'Migration',
        'Production',
    ),
    
    //      This one is a bit generic, originally for use in 'batch' and 'presets' (thread)
    'Topic' => array(
        'Test',
        'Cron',
        'Maintenance',
        'Installation',
        'Initialization',
        'Security',
        'Migration',
        'Database operations',
        'File',
        'Utilities',
        'System',
    ),
    
    "Type of folder" => array(
        'Project Root',
        'Writeable',
        'Protected',
        'DB Dumps',
    ),
    
    'Version control system' => array(
        'SVN' => array(
            "SVN < 1.7",
            "SVN >= 1.7",
        ),
        'Git',
        'Mercurial',
    ),
    
    'Database' => array(
        'MySQL',
        'PostgreSQL',
        'PostGIS',
        'MariaDB',
        'MongoDB',
        'CouchDB',
        'Redis',
        'Cassandra',
        'Neo4J',
    ),
    
    'Server-side programming language' => array(
        'Php' => array(
            'Php 4',
            'Php 5' => array(
                'Php 5.1',
                'Php 5.2',
                'Php 5.3',
                'Php 5.4',
                'Php 5.5',
            ),
            'Php 6',
        ),
        'Shell' => array(
            'Bash',
        ),
        'Java',
        'Ruby',
        'Python',
        'Perl',
        'Haskell',
    ),
    
    'Framework' => array(
        'Symfony' => array(
            'Symfony 1',
            'Symfony 2.3',
        ),
        'KnpRadBundle',
        'Zend',
        'Django',
        'Rails',
        'Laravel',
        'Slim',
        'Sinatra',
    ),
    
    'CMS / CMF' => array(
        'Drupal',
        'Wordpress',
        'Spip',
        'Joomla',
        'Bolt',
        'Plone',
    ),
    
    'Third-party' => array(
        'Apache Solr' => array(
            'Apache Solr 3.5',
            'Apache Solr 3.6',
            'Apache Solr 4.4',
        ),
        'Apache Stanbol',
        'Tomcat',
        'Memcache',
        'Varnish',
        'Munin',
        'Nagios',
        'LDAP',
        'Kerberos',
        'Webmin',
    ),
);

//      @todo 2013/07/29 16:07:17 - error checking
save_hierarchical_tags( $default_tags );

//      Feedback
$content .= alert( "Default tags have been created.", 'success' );


