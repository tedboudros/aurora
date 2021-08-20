import time
import os
import pathlib
from watchdog.observers import Observer

from Helpers.client import AuroraClient
from Helpers.server import AuroraServer
from Helpers.constants import CLIENT_DIR, SERVER_DIR, get_arg_exist, get_arg_value, AURORA_ASCII_ART
from Helpers.event_handlers import ClientEventHandler, ServerEventHandler
from Helpers.logger import make_logger

client_logger = make_logger('Client  ')
launcher_logger = make_logger('Launcher')
should_log = get_arg_exist('verbose')

file_path = pathlib.Path(__file__).parent.resolve()
os.chdir(f"{file_path}/../")

env = get_arg_value('env')
port = get_arg_value('port')

client = AuroraClient(env, port)
server = AuroraServer(env, port, should_log)

if __name__ == "__main__":
    print(AURORA_ASCII_ART)

    client.refresh()
    server.refresh()


    client_event_handler = ClientEventHandler(client)
    client_observer = Observer()
    client_observer.schedule(client_event_handler, f"./{CLIENT_DIR}", recursive=True)
    client_observer.start()

    server_event_handler = ServerEventHandler(server)
    server_observer = Observer()
    server_observer.schedule(server_event_handler, f"./{SERVER_DIR}", recursive=True)
    server_observer.start()

    def destroy():
        launcher_logger.info("Shutting down gracefully...")
        client.kill()
        server.kill()
        client_observer.stop()
        server_observer.stop()

    try:
        while True:
            client_output = client.readline()
            if client.poll() is None and client_output == '':
                destroy()
                break
            if client_output and should_log:
                client_logger.info(client_output.strip())
        
    except KeyboardInterrupt:
        destroy()

    client_observer.join()
    server_observer.join()
