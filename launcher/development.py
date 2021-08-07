import time
import os
import pathlib
from datetime import datetime
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

from Helpers.client import AuroraClient

CLIENT_DIR = 'client'
SERVER_DIR = 'server'

DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION = ["src\\", "include\\", "src/", "include/"]


def ch_dir_to_root():
    file_path = pathlib.Path(__file__).parent.resolve()
    os.chdir(f"{file_path}/../")


ch_dir_to_root()
client = AuroraClient('debug', {})

TIME_TO_ELAPSE_SINCE_LAST_COMPILATION = 5 # in seconds


class ClientEventHandler(FileSystemEventHandler):
    modified_files = []

    def __init__(self):
        super().__init__()
        self.has_compilation_started = False
        self.last_compilation_at = datetime.now()

    def on_modified(self, event):
        super().on_modified(event)

        seconds_since_last_compilation = (datetime.now() - self.last_compilation_at).seconds

        if not event.is_directory:
            for dir_to_watch in DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION:
                if event.src_path.find(dir_to_watch) != -1 and seconds_since_last_compilation >= TIME_TO_ELAPSE_SINCE_LAST_COMPILATION:
                    # If the changed file is inside the directories we're watching for, recompile
                    client.refresh()
                    self.last_compilation_at = datetime.now()


if __name__ == "__main__":
    event_handler = ClientEventHandler()
    observer = Observer()
    observer.schedule(event_handler, f"./{CLIENT_DIR}", recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        client.kill()
        observer.stop()
    observer.join()
