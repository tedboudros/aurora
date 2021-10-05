from wsgiref.simple_server import WSGIRequestHandler, make_server
from Utilities.logger import logger
from Resources.SteamResource import SteamResource

import falcon
import sys

should_log = False
if len(sys.argv) >= 3 and sys.argv[2] == '-v':
    should_log = True

if not should_log:
    logger.disabled = True


class NoLoggingWSGIRequestHandler(WSGIRequestHandler):
    def log_message(self, format, *args):
        pass


app = falcon.App()

# Resources:
steam = SteamResource()
app.add_route('/steam', steam)


if __name__ == '__main__':
    with make_server('', int(sys.argv[1]), app, handler_class=NoLoggingWSGIRequestHandler) as httpd:
        logger.info(f'Serving on port {sys.argv[1]}...')
        from Database.database import database

        # Serve until process is killed
        try:
            httpd.serve_forever()
        except:
            database.closeConnection()
            httpd.server_close()
