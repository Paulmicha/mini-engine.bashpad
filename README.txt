
Mini-engine
(Here be dragons)

This is a legacy minimalist RAD Php application,
which was designed exclusively to execute small batch processes.

It still has its uses : namely to store & execute Bash commands (with tokens),
on a configurable list of servers and environments.

I've used it over the years for repetitive tasks surrounding various PHP projects.
It was never meant to be Object-Oriented, and it can be seen as a "good" source of antipatterns - if there is such a thing,
but it could be an interesting exercise to rebuild it in "modern" PHP, using things like Symfony 2 or Laravel for example.

So basically, this is a "museum" of various Php snippets from a few years back put together.

WARNING : this is not meant to be publicly accessible over the web, it's supposed to be run on LAN,
because it can connect to other servers through SSH in order to execute Bash commands.
That's why there's an access restriction by default in "index.php" based on IP address,
which only allows IP address starting with "192.168".

---

Third-party Bundled projects Credits :
- HTML5 Boilerplate (MIT)
- Twitter Bootstrap 2.3.2 Copyright 2012 Twitter, Inc. (Apache 2)
- Bootswatch 'Cosmo' Copyright Thomas Park (Apache 2)
- Krumo 22 Copyright Kaloyan K. Tsvetkov <kaloyan@kaloyan.info> (LGPL)
- PhpSecLib 0.3.1 Copyright MMVIII Jim Wigginton (MIT)
- Chosen v0.12.1 Copyright 2011-2013 by Harvest (MIT)
- Font Awesome 3.2.1 Copyright Dave Gandy (MIT + SIL OFL 1.1)
- HighlightJS 7.3 Copyright (c) 2006, Ivan Sagalaev + github.com style (c) Vasily Polovnyov
- jquery.dataTables 1.9.4 Copyright 2008-2012 Allan Jardine, all rights reserved. (GPL v2)

---

@license GPL v2
@author Paulmicha


