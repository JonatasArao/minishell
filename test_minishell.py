#!/usr/bin/env python3
"""
Test script for Python minishell functionality.
"""

import subprocess
import tempfile
import os


def test_command(cmd_input, expected_output=None):
    """Test a command by sending it to minishell."""
    print(f"Testing: {cmd_input}")
    
    try:
        result = subprocess.run(
            ['python3', 'minishell.py'],
            input=f"{cmd_input}\nexit\n",
            capture_output=True,
            text=True,
            timeout=5
        )
        
        output = result.stdout
        print(f"Output: {output.strip()}")
        
        if expected_output and expected_output not in output:
            print(f"FAIL: Expected '{expected_output}' not found in output")
            return False
        else:
            print("PASS")
            return True
            
    except Exception as e:
        print(f"ERROR: {e}")
        return False
        
    print()


def main():
    """Run basic tests for the Python minishell."""
    print("Testing Python Minishell")
    print("=" * 40)
    
    tests_passed = 0
    total_tests = 0
    
    # Test basic echo
    total_tests += 1
    if test_command("echo hello world", "hello world"):
        tests_passed += 1
        
    # Test pwd
    total_tests += 1
    if test_command("pwd", "/home/runner/work/minishell/minishell"):
        tests_passed += 1
        
    # Test env (just check it runs)
    total_tests += 1
    if test_command("env", "PATH="):
        tests_passed += 1
        
    # Test variable expansion
    total_tests += 1
    if test_command("echo $HOME", "/home/runner"):
        tests_passed += 1
        
    # Test built-in exit status
    total_tests += 1
    if test_command("echo $?", "0"):
        tests_passed += 1
        
    print(f"\nResults: {tests_passed}/{total_tests} tests passed")
    
    if tests_passed == total_tests:
        print("All tests passed!")
        return 0
    else:
        print("Some tests failed.")
        return 1


if __name__ == "__main__":
    exit(main())