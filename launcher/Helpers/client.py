from datetime import datetime
import subprocess
import os
from pathlib import Path
import logging

# Constants
from Helpers.constants import CLIENT_DIR, MSYS2_MINGW64_EXECUTABLE, TIME_TO_WAIT_IF_IT_DIDNT_FIND_CMAKE_TASK

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')
client_logger = logging.getLogger('Client')


def run_cmake_command(args_list):
    if os.name == 'nt':
        cmake_args = [MSYS2_MINGW64_EXECUTABLE, 'cmake']
    else:
        cmake_args = ['cmake']

    process = subprocess.Popen(cmake_args + args_list, stdin=subprocess.PIPE, stderr=subprocess.PIPE,
                               stdout=subprocess.PIPE)

    client_logger.info(f"Running | cmake {' '.join(args_list)} | with PID: {process.pid}")

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
    def __init__(self, env, options):
        self.client = None
        self.env = env
        self.options = options
        self.run()

    def refresh(self):
        self.compile()
        self.kill()
        self.run()

    def kill(self):
        if self.client:
            self.client.kill()
            self.client.wait()

    def compile(self):
        current_working_dir = os.path.abspath(os.getcwd())

        client_logger.log(level=logging.INFO, msg=f"Starting compilation of the client | ENV: {self.env}")
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

        client_logger.log(level=logging.INFO, msg=f"Finished compilation of the client!")

    def run(self):
        current_working_dir = os.path.abspath(os.getcwd())

        os.chdir(CLIENT_DIR)

        executable = f"./bin/{self.env}/Aurora"
        client = subprocess.Popen(executable)

        os.chdir(current_working_dir)

        self.client = client
        return client
