# minishell

## Description

This project is about creating a simple shell in C.

## Commands
```
make
./minishell
```

Or,
```
make
./minishell [command]
```
to run a command directly through minishell

### envp
* $HOME is initialized as the $PWD (= the path) of the projet's root directory

### Command prompt
<img src="screenshots/cmd_prompt.png" width=60% />
The command prompt is composed of:
```
[$LOGNAME]@[$NAME / $SESSION_MANAGER]:[PATH]$
```
* $NAME or $SESSION_MANAGER is used, depending on the system<br />
* Unsetting LOGNAME or NAME/SESSION_MANAGER does not change the concerned part<br />
* If the current directory's path is equal to $HOME, the path is replaced by a tilde ('~')<br />
* If the current directory's position is lower than $HOME, the tilde does not appear<br />
* If the current directory is the root directory of all the system files, only "\$" is shown

## Implemented commands

### cd
* The cd command updates $PWD and the path of the command prompt when the directory is changed
* "cd .." works, as chdir() understands ".." as an argument
* cd returns an error if it comes with more than a single argument
* "cd" alone gets to $HOME

### exit
* Any argument is ignored

### export
* Every argument is exported to the minishell's envp, unless it is invalid.
* An argument is invalid when:<br />
1. The first character of the variable's name is a digit.<br />
2. At least one of the characters in the variable's name is not alphanumeric, except for the underscore character ('\_').
3. The argument begins with the assigning equal character ('=').
4. There is at least one space character before the equal character.

### unset
* Unsetting $PATH and requesting a command throws and error except when running the following implemented commands: cd, echo, exit, export, unset. This behavior is similar in bash.

### pwd
* When $PWD is unset, the pwd command will print the current absolute path, using getcwd().

### echo
* Every argument will be printed, separated by a space, unless it begins with "-n", which will activate the -n option.
* The character '\\' is not echoed, as in bash.

### env
* Any argument is ignored
* It prints all the environment variables that are active within minishell.

## Leaks

To check all kinds of leaks while ignoring the onces dued to readline(), create a file with the following content:
```
{
	ignore_readline_leaks
	Memcheck:Leak
	...
	obj:/lib/x86_64-linux-gnu/libreadline.so.7.0
}
```
You can name this file anything you want.< br />
On the last line, replace what follows "obj:" with the path corresponding to your own system.<br />
The path appears inside the valgrind reports.<br />
<br />
Then, run the following command:
```
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes  --suppressions=ignore_readline_leaks ./minishell
```
Here, the part following "--suppressions=" corresponds to the name of the file mentionned above.
