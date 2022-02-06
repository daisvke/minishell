# minishell

### envp
* $HOME is initialized as the $PWD (= the path) of the projet's root directory

### command prompt
* It is composed of:<br />
[$LOGNAME]@[$NAME]:[PATH]$
* If the current directory is the one that figures inside the $HOME value, the path is replaced by a tilde : '~'
* If the current directory's position is lower than $HOME, the tilde does not appear
* If the current directory is the root directory of all the system files, only "\$" is shown

## Implemented commands

### cd
* It updates $PWD and the path of the command prompt when the directory is changed
* "cd .." works, as chdir() understands it as an argument
* cd returns an error if it comes with more than a single argument
* "cd" alone gets to $HOME
