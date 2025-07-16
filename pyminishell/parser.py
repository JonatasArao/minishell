"""
Command parsing functionality.
Equivalent to parse/extract_commands.c in the original C implementation.
"""

from typing import List, Dict, Optional, Any
from dataclasses import dataclass


@dataclass
class Redirection:
    """Represents a redirection operation."""
    type: str  # '<', '>', '>>', '<<'
    target: str


@dataclass
class Command:
    """Represents a shell command with arguments and redirections."""
    arguments: List[str]
    redirections: List[Redirection]
    
    def __post_init__(self):
        """Initialize file descriptors and process ID."""
        self.input_fd = None
        self.output_fd = None
        self.heredoc_fd = None
        self.pid = None
        self.pipe_read_fd = None
        self.pipe_write_fd = None


class CommandParser:
    """Handles parsing of tokens into command structures."""
    
    def __init__(self):
        """Initialize parser."""
        pass
        
    def extract_commands(self, tokens: List[str]) -> List[Command]:
        """
        Parse tokens into a list of Command objects.
        Handles pipes to separate commands and redirections.
        """
        if not tokens:
            return []
            
        commands = []
        current_args = []
        current_redirections = []
        i = 0
        
        while i < len(tokens):
            token = tokens[i]
            
            if token == '|':
                # End current command and start new one
                if current_args:  # Only add if there are arguments
                    commands.append(Command(
                        arguments=current_args.copy(),
                        redirections=current_redirections.copy()
                    ))
                current_args = []
                current_redirections = []
                
            elif token in ['<', '>', '>>', '<<']:
                # Handle redirection
                if i + 1 < len(tokens):
                    target = tokens[i + 1]
                    current_redirections.append(Redirection(type=token, target=target))
                    i += 1  # Skip the target token
                else:
                    # Invalid syntax - redirection without target
                    return []
                    
            else:
                # Regular argument
                current_args.append(token)
                
            i += 1
            
        # Add the last command if it has arguments
        if current_args:
            commands.append(Command(
                arguments=current_args,
                redirections=current_redirections
            ))
            
        return commands
        
    def is_builtin_command(self, command: Command) -> bool:
        """Check if command is a built-in command."""
        if not command.arguments:
            return False
            
        builtin_names = ['echo', 'cd', 'pwd', 'export', 'unset', 'env', 'exit']
        return command.arguments[0] in builtin_names