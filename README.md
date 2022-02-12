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
* It is composed of:<br />
[$LOGNAME]@[$NAME / $SESSION_MANAGER]:[PATH]$
* $NAME or $SESSION_MANAGER is used, depending on the system
* Unsetting LOGNAME or NAME/SESSION_MANAGER does not change the concerned part
* If the current directory is the one that figures inside the $HOME value, the path is replaced by a tilde : '~'
* If the current directory's position is lower than $HOME, the tilde does not appear
* If the current directory is the root directory of all the system files, only "\$" is shown

## Implemented commands

### cd
* It updates $PWD and the path of the command prompt when the directory is changed
* "cd .." works, as chdir() understands it as an argument
* cd returns an error if it comes with more than a single argument
* "cd" alone gets to $HOME

### unset
* Unsetting $PATH and requesting a command throws and error except when running the following implemented commands: cd, echo, exit, export, unset. This behavior is similar to bash.
