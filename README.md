# mine-sweeper-mp
Another mine sweeper :)\
Learn project.\
Based on ncurses.\
Have fun.

## How to use?
* git clone https://github.com/palmar125/mine-sweeper-mp.git
* cd mine-sweeper-mp
* make 
* ./mine-sweeper-mp

## Problems
If you see sth like this:
```
mine-sweeper-mp.c:1:10: fatal error: ncurses.h: No such file or directory
    1 | #include <ncurses.h>
      |          ^~~~~~~~~~~
compilation terminated.
```
you need to install ncurses\
on ubunut/debian and similar first check: 
```
dpkg -l |grep ncurses
```
if you see sth like:
```
libncurses6:amd64               6.4+20240113-1ubuntu2                   amd64      shared libraries for terminal handling
libncursesw6:amd64              6.4+20240113-1ubuntu2                   amd64        shared libraries for terminal handling (wide character support)
ncurses-base                    6.4+20240113-1ubuntu2                   all          basic terminal type definitions
ncurses-bin                     6.4+20240113-1ubuntu2                   amd64        terminal-related programs and man pages
ncurses-term                    6.4+20240113-1ubuntu2                   all          additional terminal type definitions
```
you need to install development package:
```
sudo apt update
sudo apt install libncurses-dev
```
if you see nothing, you need to install whole ncurses package:
```
sudo apt install libncurses6 libncursesw6 libncurses-dev
```
after instalation, you should see installed whole ncurses package:
```
dpkg -l |grep ncurses
libncurses-dev:amd64            6.4+20240113-1ubuntu2                   amd64        developer's libraries for ncurses
libncurses6:amd64               6.4+20240113-1ubuntu2                   amd64        shared libraries for terminal handling
libncursesw6:amd64              6.4+20240113-1ubuntu2                   amd64        shared libraries for terminal handling (wide character support)
ncurses-base                    6.4+20240113-1ubuntu2                   all          basic terminal type definitions
ncurses-bin                     6.4+20240113-1ubuntu2                   amd64        terminal-related programs and man pages
ncurses-term                    6.4+20240113-1ubuntu2                   all          additional terminal type definitions
```
and now, make will work.

## Finall word
All suggestions are welcome.

## Where to learn ncurses?
* https://github.com/gittup/ncurses/tree/gittup/test
* https://c-for-dummies.com/ncurses/
* https://invisible-island.net/ncurses/ncurses-intro.html
* https://github.com/mbmcmullen27/ncurses
* https://www.linuxjournal.com/content/getting-started-ncurses
* https://invisible-island.net/ncurses/howto/NCURSES-Programming-HOWTO.html
* https://www.sbarjatiya.com/notes_wiki/index.php/Using_ncurses_library_with_C
* https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
* http://jbwyatt.com/ncurses.html
