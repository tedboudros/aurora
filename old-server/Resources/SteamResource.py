from Integrations.steam import get_all_steam_games_from_filesystem, open_steam_game

import falcon


class SteamResource:
    def on_get(self, req, resp):

        steam_games = get_all_steam_games_from_filesystem();

        resp.status = falcon.HTTP_200
        resp.media = steam_games

    def on_post(self, req, resp):
        app_id = req.media['app_id']
        open_steam_game(app_id)
        resp.status = falcon.HTTP_200