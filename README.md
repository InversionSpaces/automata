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
See `test` file.
