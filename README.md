# Minishell - Python Migration

This repository contains a minishell implementation that has been migrated from C to Python.

## Overview

The original C implementation has been converted to a Python equivalent while maintaining the same functionality and command-line interface.

## Features

- **Interactive shell prompt**: Displays "Minishell $ " and accepts user input
- **Command execution**: Executes external programs and built-in commands
- **Built-in commands**: 
  - `echo` - Print text to stdout (supports -n flag)
  - `cd` - Change directory (supports -, HOME directory)
  - `pwd` - Print working directory
  - `env` - Print environment variables
  - `export` - Set/export environment variables
  - `unset` - Remove environment variables
  - `exit` - Exit the shell
- **Variable expansion**: Supports $VAR, ${VAR}, and $? (exit status)
- **Quotes handling**: Proper single and double quote processing
- **Pipes**: Command pipelines (|)
- **Redirections**: Input (<), output (>, >>), and heredoc (<<)
- **Signal handling**: Proper Ctrl+C and Ctrl+\ handling
- **Environment management**: Full environment variable support

## Files Structure

### Python Implementation
- `minishell.py` - Main entry point
- `pyminishell/` - Python package containing all modules:
  - `core.py` - Main shell coordinator
  - `tokenizer.py` - Input tokenization and syntax validation
  - `parser.py` - Command and pipeline parsing
  - `expander.py` - Variable expansion and quote processing
  - `executor.py` - Command execution and pipeline handling
  - `builtins.py` - Built-in command implementations
  - `env.py` - Environment variable management
  - `signals.py` - Signal handling setup

### Testing and Setup
- `test_minishell.py` - Basic functionality tests
- `requirements.txt` - Python dependencies (none required - uses stdlib only)

### Original C Implementation
- All original C files remain in `src/`, `inc/`, and `libft/` directories

## Usage

### Running the Python Shell
```bash
python3 minishell.py
```

### Running Tests
```bash
python3 test_minishell.py
```

### Example Session
```bash
$ python3 minishell.py
Minishell $ echo "Hello, World!"
Hello, World!
Minishell $ pwd
/home/user/minishell
Minishell $ echo $HOME
/home/user
Minishell $ ls | wc -l
42
Minishell $ exit
exit
```

## Implementation Notes

### Migration Strategy
1. **Modular Design**: The C implementation's modular structure was preserved in Python
2. **Standard Library**: Uses only Python standard library (no external dependencies)
3. **Compatibility**: Maintains the same command-line interface and behavior
4. **Error Handling**: Replicates C version's error messages and exit codes

### Key Python Modules Used
- `subprocess` - For external command execution
- `os` - For file operations and environment management
- `signal` - For signal handling
- `readline` - For command-line editing and history
- `tempfile` - For heredoc temporary files

### Differences from C Version
- **Memory Management**: Python's garbage collection eliminates manual memory management
- **String Handling**: Python's built-in string operations replace custom C string functions
- **Error Handling**: Python exceptions are used instead of C error codes
- **Portability**: Python version is more portable across platforms

## Requirements

- Python 3.6 or higher
- Unix-like system (Linux, macOS)
- Standard Python library modules

## Development

The migration maintains feature parity with the original C implementation while leveraging Python's strengths for better maintainability and portability.

### Testing
Basic functionality is verified through `test_minishell.py`. The test suite covers:
- Built-in commands
- Variable expansion
- Basic command execution
- Error handling

### Future Enhancements
Potential improvements that could be added:
- Enhanced tab completion
- Command history persistence
- More sophisticated pipe error handling
- Extended globbing support
- Job control features