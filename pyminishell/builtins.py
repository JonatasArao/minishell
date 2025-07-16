"""
Built-in commands implementation.
Equivalent to builtin/ modules in the original C implementation.
"""

import os
import sys
from typing import List, Optional

from .parser import Command
from .env import EnvironmentManager


class BuiltinCommands:
    """Implements shell built-in commands."""
    
    def __init__(self, env_manager: EnvironmentManager):
        """Initialize with environment manager."""
        self.env_manager = env_manager
        
    def is_builtin(self, command: Command) -> bool:
        """Check if command is a built-in."""
        if not command.arguments:
            return False
        builtin_names = ['echo', 'cd', 'pwd', 'export', 'unset', 'env', 'exit']
        return command.arguments[0] in builtin_names
        
    def execute_builtin(self, command: Command) -> int:
        """Execute a built-in command and return exit status."""
        if not command.arguments:
            return 1
            
        cmd_name = command.arguments[0]
        
        if cmd_name == 'echo':
            return self._echo(command.arguments[1:])
        elif cmd_name == 'cd':
            return self._cd(command.arguments[1:])
        elif cmd_name == 'pwd':
            return self._pwd()
        elif cmd_name == 'export':
            return self._export(command.arguments[1:])
        elif cmd_name == 'unset':
            return self._unset(command.arguments[1:])
        elif cmd_name == 'env':
            return self._env()
        elif cmd_name == 'exit':
            return self._exit(command.arguments[1:])
        else:
            return 1
            
    def _echo(self, args: List[str]) -> int:
        """
        Implement echo command.
        Equivalent to builtin/echo.c
        """
        newline = True
        start_index = 0
        
        # Check for -n flag
        if args and args[0] == '-n':
            newline = False
            start_index = 1
            
        # Print arguments separated by spaces
        output = ' '.join(args[start_index:])
        if newline:
            print(output)
        else:
            print(output, end='')
            
        return 0
        
    def _cd(self, args: List[str]) -> int:
        """
        Implement cd command.
        Equivalent to builtin/cd.c
        """
        if len(args) == 0:
            # No argument - go to HOME
            home = self.env_manager.get('HOME')
            if not home:
                print("minishell: cd: HOME not set", file=sys.stderr)
                return 1
            target_dir = home
        elif len(args) == 1:
            target_dir = args[0]
            if target_dir == '-':
                # cd - means go to OLDPWD
                oldpwd = self.env_manager.get('OLDPWD')
                if not oldpwd:
                    print("minishell: cd: OLDPWD not set", file=sys.stderr)
                    return 1
                target_dir = oldpwd
                print(target_dir)  # Print directory when using cd -
        else:
            print("minishell: cd: too many arguments", file=sys.stderr)
            return 1
            
        try:
            # Save current directory as OLDPWD
            current_dir = os.getcwd()
            
            # Change directory
            os.chdir(target_dir)
            
            # Update PWD and OLDPWD
            new_dir = os.getcwd()
            self.env_manager.set('OLDPWD', current_dir)
            self.env_manager.set('PWD', new_dir)
            
            return 0
        except OSError as e:
            print(f"minishell: cd: {target_dir}: {e.strerror}", file=sys.stderr)
            return 1
            
    def _pwd(self) -> int:
        """
        Implement pwd command.
        Equivalent to builtin/pwd.c
        """
        try:
            print(os.getcwd())
            return 0
        except OSError:
            print("minishell: pwd: error getting current directory", file=sys.stderr)
            return 1
            
    def _export(self, args: List[str]) -> int:
        """
        Implement export command.
        Equivalent to builtin/export.c
        """
        if not args:
            # Print all environment variables in export format
            env_vars = self.env_manager.get_all()
            for key, value in sorted(env_vars.items()):
                print(f"declare -x {key}=\"{value}\"")
            return 0
            
        exit_status = 0
        for arg in args:
            if '=' in arg:
                # Variable assignment
                key, value = arg.split('=', 1)
                if self.env_manager.is_valid_identifier(key):
                    self.env_manager.set(key, value)
                else:
                    print(f"minishell: export: `{key}': not a valid identifier", file=sys.stderr)
                    exit_status = 1
            else:
                # Just mark as exported (no value assignment)
                if self.env_manager.is_valid_identifier(arg):
                    # In our simple implementation, we don't track export status separately
                    pass
                else:
                    print(f"minishell: export: `{arg}': not a valid identifier", file=sys.stderr)
                    exit_status = 1
                    
        return exit_status
        
    def _unset(self, args: List[str]) -> int:
        """
        Implement unset command.
        Equivalent to builtin/unset.c
        """
        if not args:
            return 0
            
        exit_status = 0
        for arg in args:
            if self.env_manager.is_valid_identifier(arg):
                self.env_manager.unset(arg)
            else:
                print(f"minishell: unset: `{arg}': not a valid identifier", file=sys.stderr)
                exit_status = 1
                
        return exit_status
        
    def _env(self) -> int:
        """
        Implement env command.
        Equivalent to builtin/env.c
        """
        env_vars = self.env_manager.get_all()
        for key, value in sorted(env_vars.items()):
            print(f"{key}={value}")
        return 0
        
    def _exit(self, args: List[str]) -> int:
        """
        Implement exit command.
        Equivalent to builtin/exit.c
        """
        print("exit")
        
        if not args:
            sys.exit(0)
            
        if len(args) > 1:
            print("minishell: exit: too many arguments", file=sys.stderr)
            return 1
            
        try:
            exit_code = int(args[0])
            sys.exit(exit_code % 256)  # Ensure exit code is in valid range
        except ValueError:
            print(f"minishell: exit: {args[0]}: numeric argument required", file=sys.stderr)
            sys.exit(2)