# **minishell**

## **Description**

This project involves creating a simple shell in **C**.

---

## **Commands**

### **Running the Program**
To compile and run the program, use:
```bash
make
./minishell
```

Alternatively, you can run a command directly through **minishell** with:
```bash
make
./minishell [command]
```

---

### **envp**  
- `$HOME` is initialized as the `$PWD` (path) of the project's root directory.

---

### **Command Prompt Format**  
The command prompt is composed of:<br />
<img src="screenshots/cmd_prompt.png" width="60%" />

```plaintext
[$LOGNAME]@[$NAME / $SESSION_MANAGER]:[PATH]$
```

- `$NAME` or `$SESSION_MANAGER` is used, depending on the system.  
- Unsetting `$LOGNAME` or `$NAME/$SESSION_MANAGER` does not change this part.  
- If `$HOME` is unset, the **absolute current path** is used.  
- If the current directory is the same as `$HOME`, the path is replaced by a tilde (`~`).  
- If the current directory is the root directory, [PATH] only shows `/$`.  

---

## **Implemented Commands**

<img src="screenshots/minishell.png" />

### **cd**  
- The `cd` command updates `$PWD` and changes the command prompt's path when the directory is altered.  
- `cd ..` works as expected (using `chdir()`), navigating to the parent directory.  
- `cd -` is implemented. It replaces the '`-`' argument by the $OLDPWD value.
- If `cd` is called with more than one argument, it will return an error.  
- Running `cd` alone brings the user to `$HOME`.

### **exit**  
- Any argument passed to the `exit` command is ignored.

### **export**  
- Every valid argument is exported to **minishell's envp**.  
- Arguments are **invalid** if:
  1. The variable's name begins with a digit.  
  2. The variable's name contains non-alphanumeric characters (except underscores `_`).
  3. The argument begins with the equal sign `=`.
  4. There are spaces before the equal sign.

### **unset**  
- Unsetting `$PATH` and then trying to run a command (except for built-in commands: `cd`, `echo`, `exit`, `export`, `unset`) will throw an error. This mimics bash behavior.  
- Arguments are **unset** from **minishell's envp** unless:
  1. The variable is not found.  
  2. The variable is invalid (see **export** above).

### **pwd**  
- If `$PWD` is unset, the `pwd` command will print the **current absolute path**, using `getcwd()`.

### **echo**  
- All arguments are printed with spaces in between unless the `-n` option is used (to omit the trailing newline).  
- The backslash character (`\`) is not echoed, similar to bash behavior.

### **env**  
- This command ignores any arguments and prints all active environment variables within **minishell**.

#### Advantages of Using Linked Lists

Using a linked list to manage environment variables in a C shell offers several key benefits:

1. **Dynamic Size**  
   Easily add or remove variables without wasting memory.

2. **Efficient Modifications**  
   Insertions and deletions occur in constant time (O(1)).

3. **Easy Traversal**  
   Supports sequential access without the need for fixed indexing.

4. **Better Memory Management**  
   Allows for precise memory allocation and reduces the risk of memory leaks.

5. **Simpler Implementation**  
   Operations are easier due to the inherent structure of linked lists.

---

## Environment Variable Expansion

We have added comprehensive support for environment variable expansion, enabling dynamic substitution of variables within commands and scripts.

### Features

1. **Environment Variable Expansion**  
   - Any variable prefixed with a `$` and defined in the environment is expanded to its corresponding value. For example:
     ```bash
     echo $HOME
     ```
     will display the value of the `HOME` environment variable.

   - Variables enclosed in quotes, such as `"$HOME"`, are also expanded correctly. The quotes are preserved, but the variable is substituted with its value. For example:
     ```bash
     export MY_VAR=hello
     echo "$MY_VAR"
     ```
     Output:
     ```
     hello
     ```

2. **Special Variable Expansion**  
   - The `$?` variable is expanded to represent the exit status of the last executed command. For example:
     ```bash
     ls /nonexistent
     echo $?
     ```
     would output `2`, as the `ls` command failed to find the specified path.

3. **Undefined Variables**  
   - If a variable is not defined in the environment or is invalid, it is replaced with an empty string (`""`) during expansion. For example:
     ```bash
     echo $UNSET_VAR
     ```
     results in:
     ```
     (no output, replaced by an empty string followed by a newline character)
     ```

## **Leaks**

To check for memory leaks (excluding those due to `readline()`), create a suppression file with the following content:

```plaintext
{
  ignore_readline_leaks
  Memcheck:Leak
  ...
  obj:/lib/x86_64-linux-gnu/libreadline.so.7.0
}
```

You can name this file anything you'd like.  
Make sure to replace the last line with the correct path for your system.<br />
You can find this in the **valgrind** reports. For instance:

```
// With this output:

==356791==    by 0x4890CD4: readline (in /usr/lib/x86_64-linux-gnu/libreadline.so.8.1)
```
You will replace `obj:/lib/x86_64-linux-gnu/libreadline.so.7.0` by `obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.1`

Then, run:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes  --suppressions=ignore_readline_leaks ./minishell
```

This will suppress the specific leaks caused by `readline()` while checking for other memory issues.
