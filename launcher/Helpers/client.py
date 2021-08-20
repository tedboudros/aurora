from datetime import datetime
import subprocess
import os
import sys
from pathlib import Path
import logging

# Constants
from Helpers.constants import CLIENT_DIR, MSYS2_MINGW64_EXECUTABLE, TIME_TO_WAIT_IF_IT_DIDNT_FIND_CMAKE_TASK
from Helpers.logger import make_logger

logger = make_logger('Launcher')

def run_cmake_command(args_list):
    if os.name == 'nt':
        cmake_args = [MSYS2_MINGW64_EXECUTABLE, 'cmake']
    else:
        cmake_args = ['cmake']

    process = subprocess.Popen(cmake_args + args_list, stdin=subprocess.PIPE, stderr=subprocess.PIPE,
                               stdout=subprocess.PIPE)

    logger.log(level=logging.INFO, msg=f"Running | cmake {' '.join(args_list)} | with PID: {process.pid}")

    if os.name == 'nt':
        import psutil

        is_running = True
        has_found_cmake = False
        initial_timestamp = None

        while is_running:

            running_cmake = []
            for p in psutil.process_iter():
                try:
                    if p.name() == 'cmake.exe':
                        running_cmake.append(p)
                        has_found_cmake = True
                except psutil.Error:
                    pass

            if not initial_timestamp:
                initial_timestamp = datetime.now()

            if len(running_cmake) == 0 and has_found_cmake:
                is_running = False

            time_elapsed_since_started_searching_for_cmake = datetime.now() - initial_timestamp

            if time_elapsed_since_started_searching_for_cmake.seconds > TIME_TO_WAIT_IF_IT_DIDNT_FIND_CMAKE_TASK and not has_found_cmake:
                is_running = False
    else:
        process.wait()

    return process


class AuroraClient:
    def __init__(self, env, port):
        self.client = None
        self.env = env
        self.port = port

    def refresh(self):
        self.compile()
        self.kill()
        self.run()

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

        logger.log(level=logging.INFO, msg=f"Starting compilation of the client | ENV: {self.env}")
        build_dir = f"{CLIENT_DIR}/build-{self.env}"

        build_path = Path(build_dir)
        if not build_path.exists():
            os.mkdir(build_dir)

        extra_build_args = []

        if os.name == 'nt':
            extra_build_args = ['-G', "MinGW Makefiles"]

        os.chdir(build_dir)

        run_cmake_command(extra_build_args + ['..', f"-DCMAKE_BUILD_TYPE={self.env}"])
        run_cmake_command(['--build', "."])
        run_cmake_command(['--install', "."])

        os.chdir(current_working_dir)

        logger.log(level=logging.INFO, msg=f"Finished compilation of the client!")

    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(CLIENT_DIR)

        executable = f"./bin/{self.env}/Aurora"
        client = subprocess.Popen([executable, str(self.port)], stdout=subprocess.PIPE, universal_newlines=True)

        print(os.getcwd(), CLIENT_DIR, executable, client)

        os.chdir(current_working_dir)

        self.client = client
        return client
