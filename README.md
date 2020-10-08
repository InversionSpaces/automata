# Automata
Tools for working with finite state automata. Only NFA with one symbol transitions are supported.
## Supported operations
- NFA -> DFA
- DFA -> Full DFA
- Full DFA -> Inversed Full DFA
- Full DFA -> Minimal Full DFA

# Compiling
```bash
make
```

# Usage
```bash
./automata AUTOMATA_FILE
```

## Commands
- print
- determine
- fullify
- inverse
- minimize
- exit

## Agreements
Note that user must verify input for commands, otherwise their results would be incorrect.
For example, minimization of not Full DFA produces unpredictable output.

# Automata file structure
```
[Number of transitions]
[Transitions in form:
  FROM GUARD TO
  FROM and TO - numbers
  GUARD - symbol
]
[Number of end states]
[List of end states]
```
See `example` file.

# Running tests
```
make tests
./tests
```
