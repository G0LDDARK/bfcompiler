<div align="center">
    <img src="screenshots/ADD_README.png"/>
</div>

---

# BF Interpreter

## Build
```plaintext
make
```

## Usage
Help menu
```plaintext
./bf_interpreter -h
Usage:
  -h:		print this page
  <number>:	size of the stack (default: 30000)
```

BF commands
```plaintext
./bf_interpreter
BrainFuck Interpreter V1.0 Stack size: 30000
(Type 'exit' or 'quit' to leave, CTRL+D to exit, CTRL+C cancels input)
> ++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+. // Enter any bf commmand
Hello, World! // Output of the command (empty if no . in the program)
> exit // or quit
```

The visualize command to see the current state of the stack and the pointer
```plaintext
> visualize
-------------------------------------------------
| Index |     0 |     1 |     2 |     3 |     4 |
-------------------------------------------------
| Value |     0 |     0 |     0 |     0 |     0 |
| ASCII |   .   |   .   |   .   |   .   |   .   |
-------------------------------------------------
            ^
```