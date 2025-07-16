"""
Signal handling for minishell.
Equivalent to signal/signal_handlers.c in the original C implementation.
"""

import signal
import sys


def setup_signal_handlers(shell):
    """Setup signal handlers for the shell."""
    
    def sigint_handler(signum, frame):
        """Handle SIGINT (Ctrl+C) during shell prompt."""
        print()  # Print newline
        # Don't exit, just return to prompt
        
    def sigquit_handler(signum, frame):
        """Handle SIGQUIT (Ctrl+\\) - ignore it."""
        pass
        
    # Set up signal handlers
    signal.signal(signal.SIGINT, sigint_handler)
    signal.signal(signal.SIGQUIT, sigquit_handler)
    
    # For child processes, we'll handle signals differently
    # This will be implemented in the executor module