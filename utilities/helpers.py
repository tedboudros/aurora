import subprocess
import os
import time
from pathlib import Path
import logging

CLIENT_DIR = "../client"
MSYS2_MINGW64_EXECUTABLE = "C:\\msys64\\mingw64.exe"

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')
client_logger = logging.getLogger('Client Compilation')


def run_cmake_command(args_list):
    cmake_args = []

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

        while is_running:
            output = subprocess.run('tasklist', shell=True,  stdout=subprocess.PIPE).stdout.strip()
            print(output)
            if output.find('bash.exe') != -1:
                is_running = False
    else:
        process.wait()

    return process


def compile_client(env):
    current_working_dir = os.path.abspath(os.getcwd())

    client_logger.log(level=logging.INFO, msg=f"Starting compilation of the client | ENV: {env}")
    build_dir = f"{CLIENT_DIR}/build-{env}"

    build_path = Path(build_dir)
    if not build_path.exists():
        os.mkdir(build_dir)

    extra_build_args = []

    if os.name == 'nt':
        extra_build_args = ['-G', "MinGW Makefiles"]

    os.chdir(build_dir)

    run_cmake_command(extra_build_args + ['..', f"-DCMAKE_BUILD_TYPE={env}"])
    client_logger.info('wtf')
    run_cmake_command(['--build', "."])
    client_logger.info('wtf2')
    run_cmake_command(['--install', "."])

    os.chdir(current_working_dir)


compile_client('debug')
