# NTHU: Operating Systems (CS342302)

# Programming Project Checkpoints:  
### PPC 1:
- [x] Task: 
  - Producer: Generate one character at a time from 'A' to 'Z' and start again from 'A'.
  - Consumer: Write the character to the serial Tx port (which can be seen at the UART Rx port)

### PPC 2:
- [x] Task: Same as PPC 1, with the difference that instead of using ThreadYield, we use Timer0Handler.

### PPC 3:
- [x] Task: Similar to PPC 2, but here we use 3 buffers instead of 1 and Semaphore for controlling shared resources.

### PPC 4:
- [x] Task: Similar to PPC 3, with the key difference here, we have 2 producers
  - Producer 1: [As Previous] Generate characters from 'A' to 'Z' and start again from 'A'.
  - Producer 2: Generate numbers from '0' to '9' and start again from '0'.

### PPC 5: 
- [ ] Task: 
  - [x] Part 1: Peripheral devices:
    Button bank, Keypad, LCD  
  - [ ] $${\color{red}[Incomplete]}$$ Part 2: Dinosaur game
