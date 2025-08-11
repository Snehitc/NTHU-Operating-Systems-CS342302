# 👷‍♂️Under Construction👷‍♂️

# Task
Based on PPC 3 with key changes, here we have two producers instead of one.<br/>

> - Producer 1: Generate one character at a time from 'A' to 'Z' and start again from 'A'
> - Producer 2: Generate one number at a time from '0' to '9' and start again from '0'

# Guidelines
Check the files mentioned below; the mentioned task is solved in .c files, and find the report given in .pdf.
| Files | Details |
| --- | --- |
| [*Project Checkpoint 4 - CS 342302 Fall 2024.pdf*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Project%20Checkpoint%204%20-%20CS%20342302%20Fall%202024.pdf) | Guidelines given which describe the overall task |
| [*ppc4.pdf*](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/ppc4.pdf) | $${\color{green}Solved}$$: Solution report |

| Files | Version | Details |
| --- | --- | --- |
| *preemptive.h* | [Approach_1](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_1-ThreadYield/preemptive.h) <br/> [Approach_2](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_2-ISR/preemptive.h) | Same as *preemptive.h* from PPC 3 (both Approach 1 and 2 are same)|
| *Makefile* | [Approach_1](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_1-ThreadYield/Makefile) <br/> [Approach_2](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_2-ISR/Makefile) | Same *Makefile* for both Approach 1 and 2 |
| *preemptive.c* | [Approach_1](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_1-ThreadYield/preemptive.c) <br/> [Approach_2](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_2-ISR/preemptive.c) | Same as $${\color{green}Solved}$$:*preemptive.c* from PPC 3 |
| *test3threads.c* | [Approach_1](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_1-ThreadYield/test3threads.c) <br/> [Approach_2](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/blob/main/ppc4/files/Approach_2-ISR/test3threads.c) | $${\color{green}Solved}$$: Key Changes in Approach 1 vs 2 is spawning <br/> - Approach_1: Threadyield()<br/> - Approach_2: ISR |

# Advice
Read and understand the report [beginners friendly]; it has all the necessary details required to complete this task and a nice description about how the code is executing.
