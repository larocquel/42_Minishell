<div align="center">
  <img src="https://github.com/larocquel/42_Minishell/raw/f7b0ecb4e4201d5fd2637876294ca07e1b543cad/img/minishell_banner.png" width="100%" />
</div>

> "Creating a shell is the closest you can get to talking directly to the Kernel."

## ⚒ Builders

* <a href="https://github.com/larocquel"><img align="left" src="https://avatars.githubusercontent.com/larocquel?size=100" width="50"></a>
**[Leonardo La Rocque](https://github.com/larocquel)**
<br><br/>

* <a href="https://github.com/davmendo"><img align="left" src="https://avatars.githubusercontent.com/davmendo?size=100" width="50"></a>
**[David Mendonça](https://github.com/davmendo)**
<br><br/>

## 🖳 About

**Minishell** is essentially a mini-recreation of the Unix command-line interpreter (Bash). This project is one of the most complex and rewarding milestones in the 42 curriculum. It requires a deep understanding of processes, file descriptors, signals, and memory management in C.

Our goal was to create a robust shell that handles:

- Parsing and expanding inputs.
- Managing pipes (`|`) and redirections (`<`, `>`, `<<`, `>>`).
- Executing binaries and builtins.
- Handling signals behaving exactly like Bash.

---

## 📽 Preview
<div align="center">
  <img src="https://github.com/larocquel/42_Minishell/blob/4a4ee57c95f877e1749bc15b33e949bed37880f6/img/minishell_demo.gif" alt="Demo" width="700" />
</div>

---

## ☠ Skeleton

Here is an overview of our architecture. We aimed for modularity to keep the codebase clean and maintainable.

```bash
minishell/
├── libft/              # Our custom C library (extended)
├── include/            # Header files (.h)
│   └── minishell.h     # Main header with structs and prototypes
├── src/
│   ├── 00_main/        # Entry point, loop & cleanup
│   ├── 01_parsing/     # Lexer, Tokenizer & Parser logic
│   ├── 02_exec/        # Executor, Pipes & Forking logic
│   ├── 03_builtins/    # cd, echo, env, exit, export, pwd, unset
│   ├── 04_redirect/    # Heredoc & FD duplications
│   ├── 05_env/         # Environment variable management (Linked List)
│   └── 06_signals/     # Sigaction handlers (Interactive vs Child)
├── Makefile            # Compilation rules
└── README.md           # This beauty

```

---

## ⬇ Installation & Usage

### Prerequisites

- GCC or Clang
- Make
- Readline library (usually pre-installed on Unix systems)

### Running the Shell

1. **Clone the repository:**
```bash
git clone https://github.com/larocquel/42_Minishell.git
```


2. **Compile:**
```bash
make
```


3. **Launch:**
```bash
./minishell
```


4. **Clean up:**
```bash
make fclean
```

5. **Extra rules**
```bash
make clean
make re
```
6. **Leak check**
```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
```


---

## ✌︎︎ Fun Facts 

Did you know?

* **The Name:** "Bash" stands for **"Bourne Again SHell"**. It is a pun on the name of Stephen Bourne, the creator of the original Unix shell (`sh`).
* **The Shebang:** The characters `#!` at the beginning of scripts are called "shebang" (hash-bang). It tells the kernel which interpreter to use.
* **Ken Thompson:** The first Unix shell was written by Ken Thompson (creator of B and co-creator of C) in 1971. It didn't even have pipes back then!
* **Zombie Processes:** In Unix, a child process that has finished execution but hasn't been waited for by its parent becomes a "zombie". Our Minishell makes sure to bury its zombies correctly using `waitpid`.

---

## ✔ Test Matrix

Below is an overview of suggested test cases.


| Category | Command / Action | Result | Exit Status ($?) |
| --- | --- | --- | --- |
| **Redirection** | `ls > a > b > c` | Output in `c`. `a` & `b` empty. | 0 |
| **Builtin (cd)** | `unset HOME` then `cd` | Error "HOME not set". | 1 |
| **Builtin (cd)** | `unset OLDPWD` then `cd -` | Error "OLDPWD not set". | 1 |
| **Builtin (export)** | `export 1TEST=val` | Error "not a valid identifier". | 1 |
| **Builtin (export)** | `export =test` | Error "not a valid identifier". | 1 |
| **Execution** | `/bin/ls ../../../bin/ls` | Path resolution works. | 0 |
| **Execution** | `chmod 000 dir; cd dir` | "Permission denied". | 1 |
| **Execution** | `/tmp` | "Is a directory". | 126 |
| **Parsing** | `echo      -n      hi` | Prints `hi` (newline skipped). | 0 |
| **Expansão** | `export A=A; echo $A$A` | Prints `AA`. | 0 |

---

## ♡ Acknowledgements

This project was a beast, and we couldn't have tamed it without the help of the 42 Porto community.
A special shoutout to:

* <a href="https://github.com/larocqueg"><img align="left" src="https://avatars.githubusercontent.com/larocqueg?size=100" width="50"></a>
**[Gabriel La Rocque](https://github.com/larocqueg)**
<br>For the game-changing suggestion to implement Environment Variables using *Linked Lists* instead of a complex *Binary Tree* structure. Simplicity is key!
<br clear="left"/>

* <a href="https://github.com/therappha"><img align="left" src="https://avatars.githubusercontent.com/therappha?size=100" width="50"></a>
**[Rafael Ferreira](https://github.com/therappha)**
<br>For the critical insight on memory management—specifically, ensuring child processes perform a full cleanup before exiting to prevent inherited leaks.
<br clear="left"/>

* <a href="https://github.com/Daniela-Padilha"><img align="left" src="https://avatars.githubusercontent.com/Daniela-Padilha?size=100" width="50"></a>
**[Daniela Padilha](https://github.com/Daniela-Padilha)**
<br>For providing support of great importance; her advice and guidance were of immense value throughout this journey.
<br clear="left"/>

* <a href="https://github.com/izzytoot"><img align="left" src="https://avatars.githubusercontent.com/izzytoot?size=100" width="50"></a>
**[Isabel Tootil](https://github.com/izzytoot)**
<br>For sharing her incredibly rigorous and well-organized test documentation, which pushed our Minishell to the limit and ensured its robustness.
<br clear="left"/>

