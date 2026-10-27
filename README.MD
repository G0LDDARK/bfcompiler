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
```plaintext
./bf_interpreter -h
Usage:
  -h:		print this page
  <number>:	size of the stack (default: 30000)
```

```plaintext
./bf_interpreter
BrainFuck Interpreter V1.0 Stack size: 30000
(Type 'exit' or 'quit' to leave, CTRL+D to exit, CTRL+C cancels input)
> ++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+. // Enter any bf commmand
Hello, World! // Output of the command (empty if no . in the program)
> exit // or quit
```