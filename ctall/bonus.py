import random

from pathlib import Path

import arcade

from ctall.scrollcomponent import ScrollComponent
from ctall.poolable import Poolable


BONUS_TEXTURES = []


def _init_bonus_textures():
    global BONUS_TEXTURES
    if BONUS_TEXTURES:
        return

    for path in Path("images/bonus").glob("*.png"):
        BONUS_TEXTURES.append(arcade.load_texture(path))


def pick_random_texture():
    _init_bonus_textures()
    return random.choice(BONUS_TEXTURES)


class Bonus(arcade.Sprite, Poolable):
    def __init__(self, game):
        super().__init__()
        self.game = game
        self.scrollcomponent = ScrollComponent(game, self)

    def setup(self, row):
        self.texture = pick_random_texture()
        self.scrollcomponent.setup()
        self.center_y = self.game.y_for_row(row)

    def update(self, delta):
        self.scrollcomponent.update(delta)
        if self.scrollcomponent.has_scrolled_out():
            self.recycle()

    def on_captured(self):
        self.recycle()
