1. What happens to the child's memory when `exec()` is called?
exec() replaces the childs entire process image (code, heap, stack, and mapped segments) with the new program. Basically the old memory are discarded and the process starts running the new executable. The PID stays the same but almost everything about the address space is rebuilt (excluding open file desc that weren't maked close on exec). I was digging around and was curious on how mechanisms like ASLR and dynamic linking (the loader + shared libs) change what the new memory layout looks like after exec().

2. Why do we need both `fork()` and `exec()`? Why not one system call?
We need for() to make a separate child process so the "shell" (Parent) can keep running and manage multiple commands while exec() is used to transform the child into the requested program. Separating them also lets the parent / child do setup work in between (redirecting stdin/stdout, setting env var, or changing directories) before the command runs. 

3. What happens if you call `exec()` in the parent process instead of the child?
If the Parent calls exec() the Parent process gets replaced by the command meaning executor/shell loop disappears and you likely wont get another primpt after the command finishes. Basically, you "lose" the program that was supposed to manage future commands because it turned into ls, pwd, etc.

4. Why must the child call `exit()` instead of return after a failed `exec()`?
If exec() fails and the child returns, it can fall back into code meant for the Parent's control flow (like continuing prompt loop), making duplicate shells or weird double execution behaviors. Calling exit() cleanly terminates only the child process and returns a clear failure status to the parent via wait/waitpid. I am curious on the practicality between exit() and _exit() in a child after fork() ESPECIALLY in regards to "buffered I/O" and why some guidelines recommend _exit() over exit().