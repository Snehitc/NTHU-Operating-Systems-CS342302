# 👷‍♂️Under Construction 👷‍♂️
# 🏫NTHU: 🧑‍🏫Operating Systems (CS342302)

# 🖥️Software packages
Note that the versions mentioned are the ones which I used for solving the checkpoints, and these versions might be outdated now. You can look into the latest versions of software.
| Software (version) | Purpose | Constrain |
| :---:         |     ---     |   ---     | 
| [Cygwin](https://www.cygwin.com/) (3.5.4)  | For Unix-like Environment   |  Required (If using a Windows system) |
| [SDCC](https://sdcc.sourceforge.net/) (4.4.0)     | Compiler suite that targets the Intel MCS51 based microprocessors       |  Required | 
| [Notepad++](https://notepad-plus-plus.org/) (8.7.1)    | Write and edit .c files       |   Optional (Use any software which can write .c)  |
| [EdSim51DI](https://edsim51.com/) (2.1.36)   | Simulator for 8051       |  Required  |


# 📑Programming Project Checkpoints

| Status | Checkpoint | Task |
| :---:         |     :---:      |          --- |
| ✔️  | [PPC 1](https://github.com/Snehitc/NTHU-Operating-Systems-CS342302/tree/main/ppc1)     | Producer: Generate one character at a time from 'A' to 'Z' and start again from 'A'  <br/>Consumer: Write the character to the serial Tx port (which can be seen at the UART Rx port)    |
| ✔️     | PPC 2       | Same as PPC 1, with the difference that instead of using ThreadYield, we use Timer0Handler.      |
| ✔️     | PPC 3       | Similar to PPC 2, but here we use 3 buffers instead of 1 and Semaphore for controlling shared resources.   |
| ✔️     | PPC 4       | Similar to PPC 3, with the key difference here, we have 2 producers <br/>  <div>&nbsp;&nbsp;&nbsp;&nbsp;Producer 1: [As Previous] Generate characters from 'A' to 'Z' and start again from 'A'. <br/> &nbsp;&nbsp;&nbsp;&nbsp;Producer 2: Generate numbers from '0' to '9' and start again from '0'.</div>   |
| ✅     | PPC 5       | ✔️ Part 1: Peripheral devices: Button bank, Keypad, LCD  <br/> ✖️ Part 2: Dinosaur game $${\color{red}[Incomplete]}$$      |
