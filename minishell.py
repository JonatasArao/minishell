#!/usr/bin/env python3
"""
Minishell - A minimal shell implementation in Python
Migrated from C implementation by JonatasArao
"""

import os
import sys
import signal
import readline
from typing import List, Dict, Optional, Tuple

from pyminishell.core import MinishellCore
from pyminishell.signals import setup_signal_handlers


def main():
    """Main entry point for the minishell."""
    if len(sys.argv) > 1:
        # For compatibility with original C version that ignores argv
        pass
    
    # Initialize shell
    shell = MinishellCore()
    setup_signal_handlers(shell)
    
    # Main shell loop
    try:
        while True:
            try:
                # Read input
                user_input = input("Minishell $ ")
                
                # Process the input
                if user_input.strip():
                    readline.add_history(user_input)
                    shell.execute_input(user_input)
                    
            except EOFError:
                # Ctrl+D pressed
                break
            except KeyboardInterrupt:
                # Ctrl+C pressed
                print()  # New line after ^C
                shell.last_status = 130
                continue
                
    except Exception as e:
        print(f"Fatal error: {e}", file=sys.stderr)
        sys.exit(1)
    
    # Exit message
    print("exit")
    sys.exit(shell.last_status)


if __name__ == "__main__":
    main()