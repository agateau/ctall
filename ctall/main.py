import sys

from collections import defaultdict

import arcade

from ctall.constants import ROW_HEIGHT, START_SCROLL_SPEED
from ctall.player import Player
from ctall.wall import Wall


class Game(arcade.Window):
    def __init__(self):
        super().__init__(720, 480, "Catch Them All")
        arcade.set_background_color(arcade.color.BLACK)
        self.keys = defaultdict(lambda: False)

    def setup(self):
        self.player = Player(self)
        self.player_list = arcade.SpriteList()
        self.player_list.append(self.player)

        wall = Wall(self)
        wall.setup(0)
        self.wall_list = arcade.SpriteList()
        self.wall_list.append(wall)

    def on_update(self, delta):
        self.player.update(delta)
        for wall in self.wall_list:
            wall.update(delta)

        wall_hit_list = arcade.check_for_collision_with_list(self.player,
                                                             self.wall_list)
        if wall_hit_list:
            sys.exit(0)

    def on_key_press(self, key, modifiers):
        self.keys[key] = True

    def on_key_release(self, key, modifiers):
        self.keys[key] = False

    def on_draw(self):
        arcade.start_render()
        self.player_list.draw()
        self.wall_list.draw()

    def y_for_row(self, row):
        return self.height / 2 + row * ROW_HEIGHT

    @property
    def scroll_speed(self):
        return START_SCROLL_SPEED


def main():
    game = Game()
    game.setup()
    arcade.run()


if __name__ == "__main__":
    sys.exit(main())
