"""
Variable expansion functionality.
Equivalent to expand/ modules in the original C implementation.
"""

import re
import os
from typing import List, Optional

from .parser import Command, Redirection
from .env import EnvironmentManager


class VariableExpander:
    """Handles expansion of variables and special characters in commands."""
    
    def __init__(self, env_manager: EnvironmentManager):
        """Initialize expander with environment manager."""
        self.env_manager = env_manager
        
    def expand_commands(self, commands: List[Command], last_status: int) -> List[Command]:
        """
        Expand variables in all commands.
        Equivalent to expand_commands() in the original C implementation.
        """
        expanded_commands = []
        
        for command in commands:
            expanded_command = self.expand_command(command, last_status)
            if expanded_command:
                expanded_commands.append(expanded_command)
                
        return expanded_commands
        
    def expand_command(self, command: Command, last_status: int) -> Optional[Command]:
        """Expand variables in a single command."""
        expanded_args = []
        
        # Expand arguments
        for arg in command.arguments:
            expanded_arg = self.expand_token(arg, last_status)
            if expanded_arg is not None:
                # Handle case where expansion results in multiple words
                expanded_words = self._split_expanded_token(expanded_arg)
                expanded_args.extend(expanded_words)
                
        # Expand redirection targets
        expanded_redirections = []
        for redir in command.redirections:
            expanded_target = self.expand_token(redir.target, last_status)
            if expanded_target is not None:
                # Redirection targets should not be split into multiple words
                expanded_redirections.append(Redirection(redir.type, expanded_target))
                
        if not expanded_args:
            return None
            
        return Command(
            arguments=expanded_args,
            redirections=expanded_redirections
        )
        
    def expand_token(self, token: str, last_status: int) -> Optional[str]:
        """
        Expand variables in a single token.
        Handles $VAR, ${VAR}, $?, and quotes.
        """
        if not token:
            return token
            
        result = ""
        i = 0
        in_single_quote = False
        in_double_quote = False
        
        while i < len(token):
            char = token[i]
            
            if char == "'" and not in_double_quote:
                in_single_quote = not in_single_quote
                # Don't include the quote in the result
                
            elif char == '"' and not in_single_quote:
                in_double_quote = not in_double_quote
                # Don't include the quote in the result
                
            elif char == '$' and not in_single_quote:
                # Variable expansion
                var_expansion, chars_consumed = self._expand_variable(token[i:], last_status)
                result += var_expansion
                i += chars_consumed - 1  # -1 because we'll increment at the end
                
            else:
                result += char
                
            i += 1
            
        return result
        
    def _expand_variable(self, var_str: str, last_status: int) -> tuple[str, int]:
        """
        Expand a variable starting with $.
        Returns (expanded_value, characters_consumed).
        """
        if len(var_str) < 2:
            return "$", 1
            
        if var_str[1] == '?':
            # Special case: $? expands to last exit status
            return str(last_status), 2
            
        elif var_str[1] == '{':
            # ${VAR} format
            end_brace = var_str.find('}', 2)
            if end_brace == -1:
                # No closing brace, treat as literal
                return "$", 1
            var_name = var_str[2:end_brace]
            var_value = self.env_manager.get(var_name) or ""
            return var_value, end_brace + 1
            
        elif var_str[1].isalpha() or var_str[1] == '_':
            # $VAR format
            var_name = ""
            i = 1
            while i < len(var_str) and (var_str[i].isalnum() or var_str[i] == '_'):
                var_name += var_str[i]
                i += 1
            var_value = self.env_manager.get(var_name) or ""
            return var_value, i
            
        else:
            # Not a valid variable, treat as literal
            return "$", 1
            
    def _split_expanded_token(self, token: str) -> List[str]:
        """
        Split expanded token on whitespace (for unquoted expansions).
        This simulates word splitting in shell expansion.
        """
        # For simplicity, just split on whitespace
        # In a more complete implementation, we'd need to track
        # which parts were quoted to avoid splitting those
        words = token.split()
        return words if words else [token]
        
    def expand_heredoc_line(self, line: str, last_status: int) -> str:
        """
        Expand variables in a heredoc line.
        Variables are expanded in heredocs unless delimiter is quoted.
        """
        return self.expand_token(line, last_status)