
CREATE TABLE IF NOT EXISTS `cache` (
    `id_cache` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `namespace` VARCHAR(128) COLLATE utf8_unicode_ci NOT NULL,
    `datestamp` BIGINT unsigned,
    `datestamp_expire` BIGINT unsigned,
    `data` LONGBLOB NOT NULL,
    PRIMARY KEY (`id_cache`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `has_dblog` (
    `id_has_dblog` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id` BIGINT unsigned NOT NULL,
    `id_dblog` BIGINT unsigned NOT NULL,
    `db_table` VARCHAR(128) COLLATE utf8_unicode_ci NOT NULL,
    PRIMARY KEY (`id_has_dblog`),
    KEY `id` (`id`),
    KEY `id_dblog` (`id_dblog`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `dblog` (
    `id_dblog` BIGINT unsigned NOT NULL AUTO_INCREMENT COMMENT 'Primary Key: Unique dblog event ID.',
    `type` VARCHAR(64) NOT NULL DEFAULT '' COMMENT 'Type of log message, for example `user` or `page not found.`',
    `message` LONGTEXT COLLATE utf8_unicode_ci NOT NULL COMMENT 'Text of log message.',
    `severity` TINYINT(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The severity level of the event ranges from 0 (Emergency) to 7 (Debug)',
    `data` BLOB NOT NULL COMMENT 'Serialized data',
    `location` TEXT COLLATE utf8_unicode_ci NOT NULL COMMENT 'URL of the origin of the event.',
    `referer` TEXT COLLATE utf8_unicode_ci COMMENT 'URL of referring page.',
    `hostname` VARCHAR(128) NOT NULL DEFAULT '' COMMENT 'Hostname of the user who triggered the event.',
    `timestamp` INT(11) NOT NULL DEFAULT '0' COMMENT 'Unix timestamp of when event occurred.',
    `datestamp` BIGINT unsigned,
    PRIMARY KEY (`id_dblog`),
    KEY `type` (`type`),
    KEY `severity` (`severity`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `thread` (
    `id_thread` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_batch` BIGINT unsigned NOT NULL,
    `state` VARCHAR(32) COLLATE utf8_unicode_ci NOT NULL,
    `preset` TINYINT unsigned,
    `datestamp` BIGINT unsigned,
    `data` BLOB,
    PRIMARY KEY (`id_thread`),
    KEY `id_batch` (`id_batch`),
    KEY `preset` (`preset`),
    KEY `state` (`state`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `batch` (
    `id_batch` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_thread` BIGINT unsigned NOT NULL,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `description` TEXT COLLATE utf8_unicode_ci,
    `datestamp` BIGINT unsigned,
    `data` BLOB,
    PRIMARY KEY (`id_batch`),
    KEY `id_thread` (`id_thread`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `batch_step` (
    `id_batch_step` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_batch` BIGINT unsigned NOT NULL,
    `id_command` BIGINT unsigned NOT NULL,
    `id_environment` BIGINT unsigned NOT NULL,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `step` TINYINT unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`id_batch_step`),
    KEY `id_batch` (`id_batch`),
    KEY `id_command` (`id_command`),
    KEY `id_environment` (`id_environment`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `command` (
    `id_command` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_thread` BIGINT unsigned NOT NULL,
    `guid` VARCHAR(128) COLLATE utf8_unicode_ci,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `description` TEXT COLLATE utf8_unicode_ci,
    `command` TEXT COLLATE utf8_unicode_ci,
    `data` BLOB,
    `datestamp` BIGINT unsigned,
    PRIMARY KEY (`id_command`),
    KEY `id_thread` (`id_thread`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `folder` (
    `id_folder` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_environment` BIGINT unsigned NOT NULL,
    `guid` VARCHAR(128) COLLATE utf8_unicode_ci,
    `path` TEXT COLLATE utf8_unicode_ci,
    `owner_group` VARCHAR(64) COLLATE utf8_unicode_ci,
    `permissions` VARCHAR(32) COLLATE utf8_unicode_ci,
    PRIMARY KEY (`id_folder`),
    KEY `id_environment` (`id_environment`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `environment` (
    `id_environment` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id_server` BIGINT unsigned NOT NULL,
    `guid` VARCHAR(128) COLLATE utf8_unicode_ci,
    `namespace` VARCHAR(255) COLLATE utf8_unicode_ci,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `url` TEXT COLLATE utf8_unicode_ci,
    `description` TEXT COLLATE utf8_unicode_ci,
    `data` BLOB,
    PRIMARY KEY (`id_environment`),
    KEY `id_server` (`id_server`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `server` (
    `id_server` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `guid` VARCHAR(128) COLLATE utf8_unicode_ci,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `hostname` VARCHAR(255) COLLATE utf8_unicode_ci,
    `description` TEXT COLLATE utf8_unicode_ci,
    `data` BLOB,
    PRIMARY KEY (`id_server`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `has_tag` (
    `id_has_tag` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `id` BIGINT unsigned NOT NULL,
    `id_tag` BIGINT unsigned NOT NULL,
    `db_table` VARCHAR(128) COLLATE utf8_unicode_ci NOT NULL,
    `weight` TINYINT unsigned,
    PRIMARY KEY (`id_has_tag`),
    KEY `id` (`id`),
    KEY `id_tag` (`id_tag`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE IF NOT EXISTS `tag` (
    `id_tag` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `description` TEXT COLLATE utf8_unicode_ci,
    `data` BLOB,
    PRIMARY KEY (`id_tag`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


