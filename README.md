# minishell

## Commands
```
make
./minishell
```

Or,
```
make
./minishell cmd
```
to run a command directly through minishell

### envp
* $HOME is initialized as the $PWD (= the path) of the projet's root directory

### Command prompt
* The command prompt is composed of:<br />
[$LOGNAME]@[$NAME / $SESSION_MANAGER]:[PATH]$
* $NAME or $SESSION_MANAGER is used, depending on the system
* Unsetting LOGNAME or NAME/SESSION_MANAGER does not change the concerned part
* If the current directory's path is equal to $HOME, the path is replaced by a tilde ('~')
* If the current directory's position is lower than $HOME, the tilde does not appear
* If the current directory is the root directory of all the system files, only "\$" is shown

## Implemented commands

### cd
* The cd command updates $PWD and the path of the command prompt when the directory is changed
* "cd .." works, as chdir() understands it as an argument
* cd returns an error if it comes with more than a single argument
* "cd" alone gets to $HOME

### echo
* Every argument will be printed unless it begins with "-n", which will activate the -n option.
* The character '\' is not echoed, as in bash.

### env

### exit

### export

### pwd

### unset
* Unsetting $PATH and requesting a command throws and error except when running the following implemented commands: cd, echo, exit, export, unset. This behavior is similar to bash.

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
On the last line, replace what follows "obj:" with the path corresponding to your own system.<br />
The path appears inside the valgrind reports.<br />
<br />
Then, run the following command:
```
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes  --suppressions=ignore_readline_leaks ./minishell
```
Here, the part following "--suppressions=" corresponds to the name of the file mentionned above.
