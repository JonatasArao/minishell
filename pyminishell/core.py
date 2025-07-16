"""
Core minishell functionality.
Equivalent to core.c in the original C implementation.
"""

import os
import sys
from typing import List, Dict, Optional, Tuple

from .tokenizer import Tokenizer
from .parser import CommandParser
from .expander import VariableExpander
from .executor import CommandExecutor
from .builtins import BuiltinCommands
from .env import EnvironmentManager


class MinishellCore:
    """Main shell class that coordinates all shell operations."""
    
    def __init__(self):
        """Initialize the shell with environment and components."""
        self.env_manager = EnvironmentManager()
        self.tokenizer = Tokenizer()
        self.parser = CommandParser()
        self.expander = VariableExpander(self.env_manager)
        self.builtins = BuiltinCommands(self.env_manager)
        self.executor = CommandExecutor(self.env_manager, self.builtins)
        self.last_status = 0
        
    def execute_input(self, user_input: str) -> None:
        """
        Process and execute user input.
        Equivalent to process_input() function in main.c
        """
        # Skip empty input or whitespace-only
        if not user_input or user_input.isspace():
            return
            
        try:
            # Tokenize input
            tokens = self.tokenizer.extract_tokens(user_input)
            if not tokens:
                return
                
            # Validate tokens
            if not self.tokenizer.is_token_list_valid(tokens):
                print("minishell: syntax error", file=sys.stderr)
                self.last_status = 2
                return
                
            # Parse into commands
            commands = self.parser.extract_commands(tokens)
            if not commands:
                return
                
            # Expand variables in commands
            expanded_commands = self.expander.expand_commands(commands, self.last_status)
            if not expanded_commands:
                return
                
            # Execute commands
            self.last_status = self.executor.execute_commands(expanded_commands)
            
        except Exception as e:
            print(f"minishell: {e}", file=sys.stderr)
            self.last_status = 1