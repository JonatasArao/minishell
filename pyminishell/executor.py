"""
Command execution functionality.
Equivalent to execute/ modules in the original C implementation.
"""

import os
import sys
import subprocess
import signal
import tempfile
from typing import List, Optional, Tuple, IO
from pathlib import Path

from .parser import Command, Redirection
from .env import EnvironmentManager
from .builtins import BuiltinCommands
from .expander import VariableExpander


class CommandExecutor:
    """Handles execution of commands and pipelines."""
    
    def __init__(self, env_manager: EnvironmentManager, builtins: BuiltinCommands):
        """Initialize executor with environment and built-ins."""
        self.env_manager = env_manager
        self.builtins = builtins
        self.expander = VariableExpander(env_manager)
        
    def execute_commands(self, commands: List[Command]) -> int:
        """
        Execute a list of commands (pipeline).
        Equivalent to execute_commands() in the original C implementation.
        """
        if not commands:
            return 0
            
        if len(commands) == 1:
            # Single command (no pipeline)
            return self._execute_single_command(commands[0])
        else:
            # Pipeline
            return self._execute_pipeline(commands)
            
    def _execute_single_command(self, command: Command) -> int:
        """Execute a single command with redirections."""
        # Setup heredocs first
        if not self._setup_heredocs(command):
            return 1
            
        # Apply redirections
        saved_fds = self._backup_std_fds()
        try:
            if not self._apply_redirections(command):
                return 1
                
            # Execute the command
            if self.builtins.is_builtin(command):
                return self.builtins.execute_builtin(command)
            else:
                return self._execute_external_command(command)
        finally:
            self._restore_std_fds(saved_fds)
            
    def _execute_pipeline(self, commands: List[Command]) -> int:
        """Execute a pipeline of commands."""
        processes = []
        pipe_fds = []
        
        try:
            # Create pipes between commands
            for i in range(len(commands) - 1):
                r_fd, w_fd = os.pipe()
                pipe_fds.append((r_fd, w_fd))
                
            # Execute each command in the pipeline
            for i, command in enumerate(commands):
                # Setup heredocs
                if not self._setup_heredocs(command):
                    return 1
                    
                # Setup pipe file descriptors
                stdin_fd = None
                stdout_fd = None
                
                if i > 0:  # Not the first command
                    stdin_fd = pipe_fds[i - 1][0]
                if i < len(commands) - 1:  # Not the last command
                    stdout_fd = pipe_fds[i][1]
                    
                # Execute command
                if self.builtins.is_builtin(command):
                    # Built-ins in pipelines need special handling
                    process = self._execute_builtin_in_pipeline(
                        command, stdin_fd, stdout_fd)
                else:
                    process = self._execute_external_in_pipeline(
                        command, stdin_fd, stdout_fd)
                        
                if process:
                    processes.append(process)
                    
            # Close all pipe file descriptors in parent
            for r_fd, w_fd in pipe_fds:
                os.close(r_fd)
                os.close(w_fd)
                
            # Wait for all processes and get last exit status
            last_status = 0
            for process in processes:
                if hasattr(process, 'wait'):
                    last_status = process.wait()
                    
            return last_status
            
        except Exception as e:
            print(f"minishell: pipeline error: {e}", file=sys.stderr)
            return 1
        finally:
            # Cleanup any remaining processes
            for process in processes:
                if hasattr(process, 'poll') and process.poll() is None:
                    process.terminate()
                    
    def _execute_external_command(self, command: Command) -> int:
        """Execute an external command."""
        if not command.arguments:
            return 1
            
        cmd_path = self._find_command_path(command.arguments[0])
        if not cmd_path:
            print(f"minishell: {command.arguments[0]}: command not found", file=sys.stderr)
            return 127
            
        try:
            result = subprocess.run(
                [cmd_path] + command.arguments[1:],
                env=self.env_manager.get_all(),
                capture_output=False
            )
            return result.returncode
        except Exception as e:
            print(f"minishell: {command.arguments[0]}: {e}", file=sys.stderr)
            return 1
            
    def _execute_external_in_pipeline(self, command: Command, stdin_fd: Optional[int], 
                                    stdout_fd: Optional[int]) -> Optional[subprocess.Popen]:
        """Execute external command in pipeline."""
        if not command.arguments:
            return None
            
        cmd_path = self._find_command_path(command.arguments[0])
        if not cmd_path:
            print(f"minishell: {command.arguments[0]}: command not found", file=sys.stderr)
            return None
            
        try:
            # Setup stdin/stdout for pipeline
            stdin_source = stdin_fd if stdin_fd is not None else None
            stdout_dest = stdout_fd if stdout_fd is not None else None
            
            process = subprocess.Popen(
                [cmd_path] + command.arguments[1:],
                stdin=stdin_source,
                stdout=stdout_dest,
                env=self.env_manager.get_all()
            )
            return process
        except Exception as e:
            print(f"minishell: {command.arguments[0]}: {e}", file=sys.stderr)
            return None
            
    def _execute_builtin_in_pipeline(self, command: Command, stdin_fd: Optional[int], 
                                   stdout_fd: Optional[int]) -> Optional[subprocess.Popen]:
        """Execute built-in command in pipeline (using subprocess for consistency)."""
        # For simplicity, we'll execute built-ins in a subprocess for pipelines
        # This is not optimal but maintains consistency with the pipeline model
        
        # Create a script that executes the built-in
        script = f"""
import sys
import os
sys.path.insert(0, '{os.path.dirname(__file__)}')
from pyminishell.builtins import BuiltinCommands
from pyminishell.env import EnvironmentManager
from pyminishell.parser import Command

env_manager = EnvironmentManager()
builtins = BuiltinCommands(env_manager)
command = Command(arguments={command.arguments!r}, redirections=[])
exit_code = builtins.execute_builtin(command)
sys.exit(exit_code)
"""
        
        try:
            process = subprocess.Popen(
                [sys.executable, '-c', script],
                stdin=stdin_fd,
                stdout=stdout_fd,
                env=self.env_manager.get_all()
            )
            return process
        except Exception as e:
            print(f"minishell: {command.arguments[0]}: {e}", file=sys.stderr)
            return None
            
    def _find_command_path(self, command: str) -> Optional[str]:
        """
        Find the full path to a command.
        Equivalent to get_command_path() in the original C implementation.
        """
        if '/' in command:
            # Absolute or relative path
            if os.path.isfile(command) and os.access(command, os.X_OK):
                return command
            return None
            
        # Search in PATH
        path_env = self.env_manager.get('PATH')
        if not path_env:
            return None
            
        for path_dir in path_env.split(':'):
            if not path_dir:
                continue
            full_path = os.path.join(path_dir, command)
            if os.path.isfile(full_path) and os.access(full_path, os.X_OK):
                return full_path
                
        return None
        
    def _setup_heredocs(self, command: Command) -> bool:
        """Setup heredoc redirections for a command."""
        for redir in command.redirections:
            if redir.type == '<<':
                # Create temporary file for heredoc
                try:
                    temp_fd, temp_path = tempfile.mkstemp()
                    delimiter = redir.target
                    
                    print(f"heredoc> ", end="", flush=True)
                    while True:
                        try:
                            line = input()
                            if line == delimiter:
                                break
                            # Expand variables in heredoc line
                            expanded_line = self.expander.expand_heredoc_line(line, 0)
                            os.write(temp_fd, (expanded_line + '\n').encode())
                            print(f"heredoc> ", end="", flush=True)
                        except EOFError:
                            break
                            
                    os.close(temp_fd)
                    
                    # Replace heredoc redirection with input redirection from temp file
                    redir.type = '<'
                    redir.target = temp_path
                    command.heredoc_fd = temp_path
                    
                except Exception as e:
                    print(f"minishell: heredoc error: {e}", file=sys.stderr)
                    return False
                    
        return True
        
    def _apply_redirections(self, command: Command) -> bool:
        """Apply input/output redirections for a command."""
        for redir in command.redirections:
            try:
                if redir.type == '<':
                    # Input redirection
                    fd = os.open(redir.target, os.O_RDONLY)
                    os.dup2(fd, 0)  # Redirect stdin
                    os.close(fd)
                elif redir.type == '>':
                    # Output redirection (truncate)
                    fd = os.open(redir.target, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o644)
                    os.dup2(fd, 1)  # Redirect stdout
                    os.close(fd)
                elif redir.type == '>>':
                    # Output redirection (append)
                    fd = os.open(redir.target, os.O_WRONLY | os.O_CREAT | os.O_APPEND, 0o644)
                    os.dup2(fd, 1)  # Redirect stdout
                    os.close(fd)
            except OSError as e:
                print(f"minishell: {redir.target}: {e.strerror}", file=sys.stderr)
                return False
                
        return True
        
    def _backup_std_fds(self) -> Tuple[int, int]:
        """Backup stdin and stdout file descriptors."""
        stdin_backup = os.dup(0)
        stdout_backup = os.dup(1)
        return (stdin_backup, stdout_backup)
        
    def _restore_std_fds(self, saved_fds: Tuple[int, int]) -> None:
        """Restore stdin and stdout file descriptors."""
        stdin_backup, stdout_backup = saved_fds
        os.dup2(stdin_backup, 0)
        os.dup2(stdout_backup, 1)
        os.close(stdin_backup)
        os.close(stdout_backup)