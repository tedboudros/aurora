from Database.database import database
import falcon


class SteamResource:
    def on_get(self, req, resp):



        resp.status = falcon.HTTP_200
        resp.media = steam_games

    def on_post(self, req, resp):
        app_id = req.media['app_id']
        open_steam_game(app_id)
        resp.status = falcon.HTTP_200