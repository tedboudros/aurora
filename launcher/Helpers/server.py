import subprocess
import os
import logging
import sys

# Constants
from Helpers.constants import SERVER_DIR
from Helpers.logger import make_logger

logger = make_logger('Launcher')

class AuroraServer:
    def __init__(self, env, port, should_log):
        self.server = None
        self.env = env
        self.port = port
        self.should_log = should_log

    def refresh(self):
        self.kill()
        self.run()

    def kill(self):
        if self.server:
            self.server.kill()
            self.server.wait()

    def poll(self):
        if self.server:
            return self.server.poll()
        return None

    def readline(self):
        if self.server:
            return self.server.stdout.readline()
        return ''


    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(SERVER_DIR)

        verbose_arr = ['-v'] if self.should_log else []

        try:
            server = subprocess.Popen(["python3", "main.py", str(self.port)] + verbose_arr, stdout=subprocess.PIPE, universal_newlines=True)
            logger.log(level=logging.INFO, msg=f"Launching the server")
        except:
            logger.log(level=logging.ERROR, msg=f"Cannot launch the server")


        os.chdir(current_working_dir)

        self.server = server
        return server
