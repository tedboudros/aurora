import time
import os
import pathlib
from watchdog.observers import Observer

from Helpers.client import AuroraClient
from Helpers.server import AuroraServer
from Helpers.constants import CLIENT_DIR, SERVER_DIR, get_arg_exist, get_arg_value
from Helpers.event_handlers import ClientEventHandler

file_path = pathlib.Path(__file__).parent.resolve()
os.chdir(f"{file_path}/../")

env = get_arg_value('env')
port = get_arg_value('port')

server = AuroraServer(env, port)
client = AuroraClient(env, port)

if __name__ == "__main__":
    server.refresh()
    client.refresh()

    server_event_handler = ClientEventHandler(server)
    server_observer = Observer()
    server_observer.schedule(server_event_handler, f"./{SERVER_DIR}", recursive=True)
    server_observer.start()

    client_event_handler = ClientEventHandler(client)
    client_observer = Observer()
    client_observer.schedule(client_event_handler, f"./{CLIENT_DIR}", recursive=True)
    client_observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        client.kill()
        server.kill()
        client_observer.stop()
        server_observer.stop()
    client_observer.join()
    server_observer.join()
