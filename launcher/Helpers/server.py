import subprocess
import os

# Constants
from Helpers.constants import SERVER_DIR
from Helpers.logger import make_logger

logger = make_logger('Launcher')
SERVER_DIR = "server"


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

    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(SERVER_DIR)

        verbose_arr = ['-v'] if self.should_log else []

        server = subprocess.Popen(["python3", "main.py", str(self.port)] + verbose_arr)
        logger.info("Successfully launched the server")

        os.chdir(current_working_dir)

        self.server = server
        return server
