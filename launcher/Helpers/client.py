from datetime import datetime
import subprocess
import os
import sys
from pathlib import Path
import logging

# Constants
from Helpers.constants import CLIENT_DIR, get_cmake_win_cmd
from Helpers.logger import make_logger

launcher_logger = make_logger('Launcher')
cmake_logger = make_logger('CMake   ')

def run_cmake_command(arguments):
    posix_command = f"cmake {arguments}"
    win_command = get_cmake_win_cmd(arguments)

    process = subprocess.Popen(win_command if os.name == 'nt' else posix_command, stdin=subprocess.PIPE, stderr=subprocess.PIPE,
                               stdout=subprocess.PIPE, universal_newlines=True)

    launcher_logger.log(level=logging.INFO, msg=f"Running: | {posix_command} | with PID: {process.pid}")

    while True:
        error_output = process.stderr.readline()

        if process.poll() is None and error_output == '':
            break
        if error_output:
            cmake_logger.error(error_output.strip())

    process.wait()

    return process


class AuroraClient:
    def __init__(self, env, port):
        self.client = None
        self.env = env
        self.port = port
        self.executable = f"bin/{self.env}/Aurora{'.exe' if os.name == 'nt' else ''}"

    def refresh(self):
        has_compiled = self.compile()
        self.kill()
        if has_compiled: self.run()

    def kill(self):
        if self.client:
            self.client.kill()
            self.client.wait()
            self.client = None

    def poll(self):
        if self.client:
            return self.client.poll()
        return None

    def readline(self):
        if self.client:
            return self.client.stdout.readline()
        return ''

    def compile(self):
        current_working_dir = os.path.abspath(os.getcwd())

        launcher_logger.log(level=logging.INFO, msg=f"Starting compilation of the client | ENV: {self.env}")
        build_dir = f"{CLIENT_DIR}/build-{self.env}"

        build_path = Path(build_dir)
        if not build_path.exists():
            os.mkdir(build_dir)

        # Remove the old Aurora executable. This is done to later check if the new one exists so that we know something in the compilation didn't go wrong
        try:
            os.remove(f"{CLIENT_DIR}/{self.executable}")
        except OSError:
            pass

        extra_build_args = ""
        if os.name == 'nt':
            extra_build_args = '-G "MinGW Makefiles" '

        os.chdir(build_dir)

        run_cmake_command(extra_build_args + f'.. -DCMAKE_BUILD_TYPE={self.env}')
        run_cmake_command('--build .')
        run_cmake_command('--install .')

        os.chdir(current_working_dir)

        executable_exists = os.path.isfile(f"{CLIENT_DIR}/{self.executable}")

        if(executable_exists):
            launcher_logger.log(level=logging.INFO, msg=f"Successfully compiled the client")
        else:
            launcher_logger.log(level=logging.ERROR, msg=f"Cannot compile the client - Probably a cmake error")
            return False

        return True
        

    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(CLIENT_DIR)
        
        try:
            client = subprocess.Popen([f"./{self.executable}", str(self.port)], stdout=subprocess.PIPE, universal_newlines=True)
            launcher_logger.log(level=logging.INFO, msg=f"Launching the client")
        except:
            launcher_logger.log(level=logging.ERROR, msg=f"Cannot launch the client")
        

        os.chdir(current_working_dir)

        self.client = client
        return client
