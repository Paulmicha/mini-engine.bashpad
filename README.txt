
Mini-engine
(quasi-finished)

This project is a minimalist Bash commands store and launch pad.

It's currently built on top of a custom Php app from scratch, which was originally designed for automating repetitive tasks (such as data preparation for migrations). It should be rebuilt in modern PHP - for ex. using Symfony 2 components, and its current state can be seen as my personal museum of Php snippets put together (hence Here be dragons).

WARNING : this application will contain extremely sensitive & unobfuscated data, and it was never meant to be publicly accessible over the web (because it can connect to other servers through SSH in order to execute Bash commands). It is probably unsecure, because the underlying old code base wasn't built for it : we only use it in LAN for speeding up repetitive tasks on our development environments, and I would strongly advise not to host it anywhere accessible online.

---

Third-party Bundled projects Credits :
- HTML5 Boilerplate (MIT)
- Twitter Bootstrap 2.3.2 Copyright 2012 Twitter, Inc. (Apache 2)
- Bootswatch 'Cosmo' Copyright Thomas Park (Apache 2)
- Krumo 22 Copyright Kaloyan K. Tsvetkov <kaloyan@kaloyan.info> (LGPL)
- PhpSecLib 0.3.5 Copyright MMVIII Jim Wigginton (MIT) -- Note : with modifications for include paths
- Chosen v0.12.1 Copyright 2011-2013 by Harvest (MIT)
- Font Awesome 3.2.1 Copyright Dave Gandy (MIT + SIL OFL 1.1)
- HighlightJS 7.3 Copyright (c) 2006, Ivan Sagalaev + github.com style (c) Vasily Polovnyov
- jquery.dataTables 1.9.4 Copyright 2008-2012 Allan Jardine, all rights reserved. (GPL v2)

---

@license GPL v2
@author Paulmicha

