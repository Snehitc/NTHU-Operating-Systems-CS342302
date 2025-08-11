# 👷‍♂️Under Construction👷‍♂️

# Task
Similar to PPC 2, but here we use 3 buffers instead of 1 and a Semaphore for controlling shared resources.<br/>

> ### PPC 2
> Same as PPC 1, with the difference that instead of using ThreadYield, we use Timer0Handler.<br/>
> ### PPC 1
> - Producer: Generate one character at a time from 'A' to 'Z' and start again from 'A'
> - Consumer: Write the character to the serial Tx port (which can be seen at the UART Rx port)

# Guidelines
Check the files mentioned below; the required [TODO] section in the mentioned .c files is solved, and find the report given in .pdf.
| Files | Details |
| --- | --- |
| [*Project Checkpoint 2 - CS 342302 Fall 2024.pdf*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/Project%20Checkpoint%203%20-%20CS%20342302%20Fall%202024.pdf) | Guidelines given which describe the overall task |
| [*ppc2.pdf*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/ppc3.pdf) | $${\color{green}Solved}$$: Solution report |
| [*preemptive.h*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/preemptive.h) | Based on *preemptive.h* from PPC 2 with addition of **Semaphore** |
| [*Makefile*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/Makefile) | Same as *Makefile* from PPC 2 (Reason: filenames in PPC 2 and 3 were same) |
| [*preemptive.c*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/preemptive.c) | Same as $${\color{green}Solved}$$:*preemptive.c* from PPC 2 |
| [*testpreempt.c:*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc3/files/testpreempt.c) | $${\color{green}Solved}$$: Key Changes from PPC 2<br/> - BUFFER_SIZE<br/> - Semaphore mutex, Semaphore full, Semaphore empty; |

# Advice
Read and understand the report [beginners friendly]; it has all the necessary details required to complete this task and a nice description about how the code is executing.
