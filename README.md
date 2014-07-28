Mini-engine
===========

Old school minimalist Php "webapp".

Non-OOP for historical reasons (some of its parts come from old snippets, reused & transformed over the years).  
I still come back to it from time to time for quick & small tasks, so I thought I might as well clean it up & leave it here.

## Has been & could be used :
- for Tiny projects (this is not a CMS - no admin, login, etc.)
- as a Prototyping tool
- as a Front-end / Php code playground & "crashtest"
- for Tiny headless web services (REST)
- for Data migration "preprocessing" (though we now tend to always use Symfony2)
- for Batch operations
- as a Bash commands store & launchpad (over SSH using PhpSeclib) -> I decided to move this into another repo, for simplifying & reusing this "Mini-engine" more easily.

## How
Structure :
```
project-root-dir/
├── config/
│   ├── settings.inc
│   └── entities/
├── engine/
│   ├── core/
│   │   ├── router.inc
│   │   ├── render.inc
│   │   └── ...
│   ├── extensions/
│   ├── vendor/
│   ├── writeable/
│   └── bootstrap.inc
├── routing/
├── theme/
│   ├── _assets/
│   │   ├── css
│   │   │   └── main.css
│   │   ├── js
│   │   │   └── main.js
│   │   └── vendor/
│   ├── page.tpl.php
│   ├── index.tpl.php
│   └── ...
├── .htaccess
├── favicon.ico
├── index.php
└── ...
```

@todo document basic usage