import time
import os
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

CLIENT_DIR= '../client/'
SERVER_DIR= '../server/'

DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION = ["src\\", "include\\", "src/", "include/"]


def compile_client():
    os.chdir(CLIENT_DIR)
    client_process = subprocess.Popen(['compile.sh', 'debug'])
    os.chdir(CLIENT_DIR)
    return client_process


class ClientEventHandler(FileSystemEventHandler):
    modified_files = []

    def __init__(self):
        super().__init__()

    def on_modified(self, event):
        super().on_modified(event)

        if not event.is_directory:
            for dir_to_watch in DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION:
                if event.src_path.find(dir_to_watch) != -1:
                    # If the changed file is inside the directories we're watching for, recompile
                    compile_client()
                    print(event.src_path)


if __name__ == "__main__":
    event_handler = ClientEventHandler()
    observer = Observer()
    observer.schedule(event_handler, CLIENT_DIR, recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()