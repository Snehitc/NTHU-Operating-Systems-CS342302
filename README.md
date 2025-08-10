# Under Construction
# NTHU: Operating Systems (CS342302)

# Programming Project Checkpoints:  

| Status | Checkpoint | Task |
| :---:         |     :---:      |          :--- |
| ✔️  | PPC 1     | Producer: Generate one character at a time from 'A' to 'Z' and start again from 'A'  <br/>Consumer: Write the character to the serial Tx port (which can be seen at the UART Rx port)    |
| ✔️     | PPC 2       | Same as PPC 1, with the difference that instead of using ThreadYield, we use Timer0Handler.      |
| ✔️     | PPC 3       | Similar to PPC 2, but here we use 3 buffers instead of 1 and Semaphore for controlling shared resources.   |
| ✔️     | PPC 4       | Similar to PPC 3, with the key difference here, we have 2 producers <br/>  <div>&nbsp;&nbsp;&nbsp;&nbsp;Producer 1: [As Previous] Generate characters from 'A' to 'Z' and start again from 'A'. <br/> &nbsp;&nbsp;&nbsp;&nbsp;Producer 2: Generate numbers from '0' to '9' and start again from '0'.</div>   |
| ✅     | PPC 5       | Part 1: Peripheral devices: Button bank, Keypad, LCD  <br/> ✖️: Part 2: Dinosaur game $${\color{red}[Incomplete]}$$      |
