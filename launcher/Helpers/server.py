import subprocess
import os
import logging

# Constants
from Helpers.constants import SERVER_DIR
SERVER_DIR = "server"

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')
client_logger = logging.getLogger('Server')


class AuroraServer:
    def __init__(self, env, options):
        self.server = None
        self.env = env
        self.options = options
        self.run()

    def refresh(self):
        self.kill()
        self.run()

    def kill(self):
        if self.server:
            self.server.kill()
            self.server.wait()

    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(SERVER_DIR)
        print(os.getcwd())
        server = subprocess.Popen(["python3", "main.py"])

        os.chdir(current_working_dir)

        self.server = server
        return server
