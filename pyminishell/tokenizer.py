"""
Tokenization functionality.
Equivalent to parse/extract_tokens.c and parse/token_validation.c in the original C implementation.
"""

import re
from typing import List, Optional


class Tokenizer:
    """Handles tokenization of shell input."""
    
    def __init__(self):
        """Initialize tokenizer."""
        self.tokens = []
        
    def extract_tokens(self, input_str: str) -> List[str]:
        """
        Extract tokens from input string.
        Handles quotes, pipes, redirections, and word splitting.
        """
        tokens = []
        current_token = ""
        in_single_quote = False
        in_double_quote = False
        i = 0
        
        while i < len(input_str):
            char = input_str[i]
            
            if char == "'" and not in_double_quote:
                in_single_quote = not in_single_quote
                current_token += char
            elif char == '"' and not in_single_quote:
                in_double_quote = not in_double_quote
                current_token += char
            elif char in ' \t\n' and not in_single_quote and not in_double_quote:
                if current_token:
                    tokens.append(current_token)
                    current_token = ""
                # Skip whitespace
                while i < len(input_str) and input_str[i] in ' \t\n':
                    i += 1
                i -= 1  # Compensate for the increment at the end of loop
            elif char == '|' and not in_single_quote and not in_double_quote:
                if current_token:
                    tokens.append(current_token)
                    current_token = ""
                tokens.append('|')
            elif char in '<>' and not in_single_quote and not in_double_quote:
                if current_token:
                    tokens.append(current_token)
                    current_token = ""
                # Handle << and >>
                if i + 1 < len(input_str) and input_str[i + 1] == char:
                    tokens.append(char + char)
                    i += 1
                else:
                    tokens.append(char)
            else:
                current_token += char
                
            i += 1
            
        if current_token:
            tokens.append(current_token)
            
        return tokens
        
    def is_token_list_valid(self, tokens: List[str]) -> bool:
        """
        Validate token list for proper syntax.
        Checks for proper quote pairing, pipe placement, etc.
        """
        if not tokens:
            return True
            
        # Check for unclosed quotes
        for token in tokens:
            if not self._is_valid_quotes(token):
                return False
                
        # Check for proper pipe and redirection syntax
        for i, token in enumerate(tokens):
            if token == '|':
                # Pipe cannot be first or last token
                if i == 0 or i == len(tokens) - 1:
                    return False
                # Pipe cannot follow another pipe or redirection
                if i > 0 and tokens[i-1] in ['|', '<', '>', '<<', '>>']:
                    return False
                # Pipe cannot precede redirection
                if i < len(tokens) - 1 and tokens[i+1] in ['<', '>', '<<', '>>']:
                    return False
                    
            elif token in ['<', '>', '<<', '>>']:
                # Redirection must have a target
                if i == len(tokens) - 1:
                    return False
                # Redirection target cannot be another operator
                if tokens[i+1] in ['|', '<', '>', '<<', '>>']:
                    return False
                    
        return True
        
    def _is_valid_quotes(self, token: str) -> bool:
        """Check if quotes are properly paired in a token."""
        single_count = 0
        double_count = 0
        in_single = False
        in_double = False
        
        for char in token:
            if char == "'" and not in_double:
                single_count += 1
                in_single = not in_single
            elif char == '"' and not in_single:
                double_count += 1
                in_double = not in_double
                
        return single_count % 2 == 0 and double_count % 2 == 0
        
    def is_redirection(self, token: str) -> bool:
        """Check if token is a redirection operator."""
        return token in ['<', '>', '<<', '>>']
        
    def is_pipe(self, token: str) -> bool:
        """Check if token is a pipe operator."""
        return token == '|'
        
    def is_heredoc(self, token: str) -> bool:
        """Check if token is a heredoc operator."""
        return token == '<<'
        
    def is_input_redirection(self, token: str) -> bool:
        """Check if token is an input redirection."""
        return token in ['<', '<<']
        
    def is_output_redirection(self, token: str) -> bool:
        """Check if token is an output redirection."""
        return token in ['>', '>>']