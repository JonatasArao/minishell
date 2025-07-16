"""
Environment variable management.
Equivalent to expand/envp_manager.c and expand/env_vars_list.c in the original C implementation.
"""

import os
from typing import Dict, List, Optional


class EnvironmentManager:
    """Manages environment variables for the shell."""
    
    def __init__(self):
        """Initialize with current environment variables."""
        self.env_vars = dict(os.environ)
        
    def get(self, key: str) -> Optional[str]:
        """Get environment variable value."""
        return self.env_vars.get(key)
        
    def set(self, key: str, value: str) -> bool:
        """Set environment variable."""
        try:
            self.env_vars[key] = value
            os.environ[key] = value
            return True
        except Exception:
            return False
            
    def unset(self, key: str) -> bool:
        """Remove environment variable."""
        try:
            if key in self.env_vars:
                del self.env_vars[key]
            if key in os.environ:
                del os.environ[key]
            return True
        except Exception:
            return False
            
    def get_all(self) -> Dict[str, str]:
        """Get all environment variables."""
        return self.env_vars.copy()
        
    def get_envp(self) -> List[str]:
        """Get environment in envp format (key=value strings)."""
        return [f"{key}={value}" for key, value in self.env_vars.items()]
        
    def export_variable(self, assignment: str) -> bool:
        """
        Export a variable from assignment string (key=value format).
        Returns True on success, False on failure.
        """
        if '=' in assignment:
            key, value = assignment.split('=', 1)
            return self.set(key, value)
        else:
            # Just mark as exported (no value change)
            return True
            
    def is_valid_identifier(self, name: str) -> bool:
        """Check if a name is a valid shell identifier."""
        if not name:
            return False
        if not (name[0].isalpha() or name[0] == '_'):
            return False
        return all(c.isalnum() or c == '_' for c in name[1:])