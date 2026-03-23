*This project has been created as part of the 42 curriculum by mefische and ntomas-g.*

# Minishell

## Description

Minishell is a project where we built our own simple Unix shell from scratch. The idea was to better understand how shells like bash actually work behind the scenes.

With this project, we learned how to execute commands, manage processes, and handle user input in a real command-line environment. We also implemented features like pipes, redirections, quotes, and heredocs, which made the parsing part both challenging and interesting.

## Instructions

### Compilation

To compile the project, run:

```
make
```

### Execution

After compilation, start the shell with:

```
./minishell
```

### Features

* Execution of simple and complex commands
* Environment variable management
* Pipes (`|`)
* Redirections:

  * Output overwrite (`>`)
  * Output append (`>>`)
  * Input (`<`)
* Heredoc (`<<`)
* Quote handling:

  * Single quotes (`'`)
  * Double quotes (`"`)
* Signal handling (e.g. Ctrl+C, Ctrl+D)
* Built-in commands:

  * `echo`
  * `cd`
  * `pwd`
  * `export`
  * `unset`
  * `env`
  * `exit`

## Resources

* Unix manual pages (`man bash`, `man execve`, `man fork`, `man pipe`)
* 42 Minishell subject and documentation
* Online tutorials about shell design and parsing
* AI tools were used for:

  * Understanding parsing strategies (quotes, heredocs)
  * Debugging and improving code structure

## Additional Information

### Usage Example

```
$ ./minishell
minishell$ echo "Hello World" | cat > output.txt
minishell$ cat << EOF
> This is a heredoc
> EOF
```

### Technical Choices

* Written in C
* Uses system calls such as `fork`, `execve`, `pipe`, and `dup2`
* Custom lexer and parser for handling quotes and tokenization
* Heredoc implementation with proper signal handling
* Linked list structures for environment variables

---

Feel free to explore, test, and try to crash this shell!
