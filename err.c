<< eof << eof

/************************************************************
************************************************************/

$ <<d
a
d
$ mls
here
mls: command not found
$ ls
here
a  inc  Makefile  minishell  obj  README.md  src
$ ls
here
a  inc  Makefile  minishell  obj  README.md  src
$
=================================================================
==7372==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x4c6233 in malloc (/home/alien/documents/42/cursus_2021/min_try_hd/minishell+0x4c6233)
    #1 0x7fad3e31a4e8 in xmalloc (/lib/x86_64-linux-gnu/libreadline.so.7+0x3a4e8)

SUMMARY: AddressSanitizer: 32 byte(s) leaked in 1 allocation(s).

/************************************************************
************************************************************/

 ls | <<d cat | cat -e > b
inc  leaks  Makefile  minishell  obj  README.md  src
s
d
cat: -: input file is output file

/************************************************************
************************************************************/

*
05/02:
before cmd prompt:

LOGNAME = A
NAME = B

A@B

* protect readline with strlen (no longer than intmax ?)
* free char *prompt ?
* cd => go to HOME
* cd - => go to OLDPWD
* expansion (echo, cd etc) with ~ => PWD ?
* void *env to use ms/ppx
* unset/export => if unset, pwd
 for both, modify cmd prompt when reput prompt
* if pwd before ~ => /$ only

/************************************************************
************************************************************/

split & activate => do it with && and ||

/************************************************************
************************************************************/

ms_macro => macro ?
        => ms_macro + ppx_macro

/************************************************************
************************************************************/

SIGINT => exit status code ?

/************************************************************
************************************************************/

