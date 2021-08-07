from watchdog.events import FileSystemEventHandler
from datetime import datetime
from Helpers.constants import \
    TIME_TO_ELAPSE_FROM_LAST_CLIENT_COMPILATION, \
    TIME_TO_ELAPSE_FROM_LAST_SERVER_COMPILATION, \
    DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION


class ClientEventHandler(FileSystemEventHandler):
    modified_files = []

    def __init__(self, client):
        super().__init__()
        self.has_compilation_started = False
        self.last_compilation_at = datetime.now()
        self.client = client

    def on_modified(self, event):
        super().on_modified(event)

        seconds_since_last_compilation = (datetime.now() - self.last_compilation_at).seconds

        if not event.is_directory:
            for dir_to_watch in DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION:
                if event.src_path.find(
                        dir_to_watch) != -1 and seconds_since_last_compilation >= TIME_TO_ELAPSE_FROM_LAST_CLIENT_COMPILATION:
                    # If the changed file is inside the directories we're watching for, refresh the client
                    self.client.refresh()
                    self.last_compilation_at = datetime.now()


class ServerEventHandler(FileSystemEventHandler):
    modified_files = []

    def __init__(self, server):
        super().__init__()
        self.last_ran_at = datetime.now()
        self.server = server

    def on_modified(self, event):
        super().on_modified(event)

        seconds_since_last_run = (datetime.now() - self.last_ran_at).seconds

        if not event.is_directory:
            if seconds_since_last_run >= TIME_TO_ELAPSE_FROM_LAST_SERVER_COMPILATION:
                # If the changed file is inside the directories we're watching for, refresh the client
                self.server.refresh()
                self.last_ran_at = datetime.now()