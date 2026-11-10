#!/usr/bin/env python3
"""
Smart Room IoT Controller - Local Hardware Gateway & Dashboard Server
Author: Gopal M V
"""

import os
import sys

# Forward execution to root server.py
ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, ROOT_DIR)

if __name__ == "__main__":
    from server import start_server
    start_server()
