import random
import sys

from collections import defaultdict

import arcade

from ctall.constants import ROW_HEIGHT, START_SCROLL_SPEED, SPAWN_X_SPACING, \
    MIN_ROW, MAX_ROW, SCREEN_WIDTH, SCREEN_HEIGHT
from ctall.player import Player
from ctall.pool import Pool
from ctall.wall import Wall


class Game(arcade.Window):
    def __init__(self):
        super().__init__(SCREEN_WIDTH, SCREEN_HEIGHT, "Catch Them All")
        arcade.set_background_color(arcade.color.BLACK)
        self.keys = defaultdict(lambda: False)

    def setup(self):
        self.scroll_x = 0
        self.player = Player(self)
        self.player_list = arcade.SpriteList()
        self.player_list.append(self.player)

        self.wall_pool = Pool(lambda: Wall(self))

    def on_update(self, delta):
        self._world_update(delta)
        self.player.update(delta)
        for wall in self.wall_pool.sprite_list:
            wall.update(delta)

        wall_hit_list = arcade.check_for_collision_with_list(
            self.player, self.wall_pool.sprite_list)
        if wall_hit_list:
            sys.exit(0)

    def on_key_press(self, key, modifiers):
        self.keys[key] = True

    def on_key_release(self, key, modifiers):
        self.keys[key] = False

    def on_draw(self):
        arcade.start_render()
        self.player_list.draw()
        self.wall_pool.sprite_list.draw()

    def y_for_row(self, row):
        return SCREEN_HEIGHT / 2 + row * ROW_HEIGHT

    @property
    def scroll_speed(self):
        return START_SCROLL_SPEED

    def _world_update(self, delta):
        old_scroll_x = self.scroll_x
        self.scroll_x += self.scroll_speed * delta
        if self.scroll_x // SPAWN_X_SPACING > old_scroll_x // SPAWN_X_SPACING:
            self._spawn_things()

    def _spawn_things(self):
        wall = self.wall_pool.get()
        wall.setup(random.randint(MIN_ROW, MAX_ROW))


def main():
    game = Game()
    game.setup()
    arcade.run()


if __name__ == "__main__":
    sys.exit(main())
