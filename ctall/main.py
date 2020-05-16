import sys

from collections import defaultdict

import arcade

from ctall.player import Player


class Game(arcade.Window):
    def __init__(self):
        super().__init__(720, 480, "Catch Them All")
        arcade.set_background_color(arcade.color.BLACK)
        self.keys = defaultdict(lambda: False)

    def setup(self):
        self.player = Player(self)
        self.player_list = arcade.SpriteList()
        self.player_list.append(self.player)

    def on_update(self, delta):
        self.player.update(delta)

    def on_key_press(self, key, modifiers):
        self.keys[key] = True

    def on_key_release(self, key, modifiers):
        self.keys[key] = False

    def on_draw(self):
        arcade.start_render()
        self.player_list.draw()


def main():
    game = Game()
    game.setup()
    arcade.run()


if __name__ == "__main__":
    sys.exit(main())
