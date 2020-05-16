import random
import sys

from collections import defaultdict

import arcade

from ctall.constants import ROW_HEIGHT, START_SCROLL_SPEED, SPAWN_X_SPACING, \
    MIN_ROW, MAX_ROW, SCREEN_WIDTH, SCREEN_HEIGHT, SCROLL_SPEED_MULTIPLIER
from ctall.bonus import Bonus
from ctall.player import Player
from ctall.pool import Pool
from ctall.wall import Wall
from ctall.textdrawer import TextDrawer

ALPHABET = \
    " !*+,-./0" \
    "123\"45678" \
    "9:;<=#>?@" \
    "ABCDEFG$H" \
    "IJKLMNOPQ" \
    "%RSTUVWXY" \
    "2[&\\]^_`'" \
    "(){|}~   "

BG_COLOR = (60,) * 3


class Game(arcade.Window):
    def __init__(self):
        super().__init__(SCREEN_WIDTH, SCREEN_HEIGHT, "Catch Them All")
        arcade.set_background_color(BG_COLOR)
        self.keys = defaultdict(lambda: False)

    def setup(self):
        self.scroll_x = 0.0
        self.score = 0
        self.game_over = False
        self.scroll_speed = START_SCROLL_SPEED
        self.player = Player(self)
        self.player_list = arcade.SpriteList()
        self.player_list.append(self.player)

        self.wall_pool = Pool(lambda: Wall(self))
        self.bonus_pool = Pool(lambda: Bonus(self))

        self.text_drawer = TextDrawer("./images/font.png", ALPHABET,
                                      char_width=18, char_height=16, columns=9)

    def on_update(self, delta):
        if self.game_over:
            return
        self._world_update(delta)
        self.player.update(delta)
        for obj in self.wall_pool.sprite_list:
            obj.update(delta)
        for obj in self.bonus_pool.sprite_list:
            obj.update(delta)

        wall_hit_list = arcade.check_for_collision_with_list(
            self.player, self.wall_pool.sprite_list)
        if wall_hit_list:
            self.game_over = True

        for bonus in self.player.collides_with_list(
                self.bonus_pool.sprite_list):
            self.score += 1
            bonus.on_captured()

    def on_key_press(self, key, modifiers):
        self.keys[key] = True

    def on_key_release(self, key, modifiers):
        self.keys[key] = False
        if self.game_over and key == arcade.key.SPACE:
            self.setup()

    def on_draw(self):
        arcade.start_render()
        self.bonus_pool.sprite_list.draw()
        self.player_list.draw()
        self.wall_pool.sprite_list.draw()
        self._draw_hud()
        if self.game_over:
            self._draw_gameover()

    def y_for_row(self, row):
        return SCREEN_HEIGHT / 2 + row * ROW_HEIGHT

    def _draw_hud(self):
        distance = int(self.scroll_x / 100)
        self.text_drawer.draw(f"D: {distance}\nS: {self.score}",
                              0, SCREEN_HEIGHT,
                              align=TextDrawer.LEFT | TextDrawer.TOP)

    def _draw_gameover(self):
        self.text_drawer.draw("GAME OVER\n\n",
                              SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
                              align=TextDrawer.HCENTER | TextDrawer.BOTTOM)
        self.text_drawer.draw("PRESS SPACE TO RESTART",
                              SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
                              align=TextDrawer.HCENTER | TextDrawer.TOP)

    def _world_update(self, delta):
        old_scroll_x = self.scroll_x
        self.scroll_x += self.scroll_speed * delta
        if self.scroll_x // SPAWN_X_SPACING > old_scroll_x // SPAWN_X_SPACING:
            self._spawn_things()

    def _spawn_things(self):
        if random.randint(0, 1) == 0:
            wall = self.wall_pool.get()
            wall_row = random.randint(MIN_ROW, MAX_ROW)
            wall.setup(wall_row)
        else:
            wall_row = MIN_ROW - 1

        if random.randint(0, 3) == 0:
            bonus = self.bonus_pool.get()
            bonus_row = wall_row
            while bonus_row == wall_row:
                bonus_row = random.randint(MIN_ROW, MAX_ROW)
            bonus.setup(bonus_row)

        self.scroll_speed *= SCROLL_SPEED_MULTIPLIER


def main():
    game = Game()
    game.setup()
    arcade.run()


if __name__ == "__main__":
    sys.exit(main())
