import random

from pathlib import Path

import arcade

from ctall.constants import MIN_ROW, MAX_ROW, ROW_HEIGHT, SCREEN_WIDTH

BORDER_TEXTURES = []
ROAD_TEXTURES = []

TEXTURES = None

COLUMN_COUNT = SCREEN_WIDTH // ROW_HEIGHT
ROW_COUNT = MAX_ROW - MIN_ROW + 1

ROAD_TEXTURE_COUNT = 2


class Textures:
    def __init__(self):
        self._base_dir = Path("images/background")
        self.border = self._load("border")
        self.roads = [self._load(f"road-{x}")
                      for x in range(ROAD_TEXTURE_COUNT)]

    def _load(self, name):
        return arcade.load_texture(self._base_dir / (name + ".png"))


def _init_textures():
    global TEXTURES
    if TEXTURES is None:
        TEXTURES = Textures()


def _random_road_texture_id():
    return random.randint(0, ROAD_TEXTURE_COUNT - 1)


class Background:
    def __init__(self, game):
        self.game = game
        self.offset = 0

        def create_column():
            return [_random_road_texture_id() for x in range(ROW_COUNT)]
        self.columns = [create_column() for x in range(COLUMN_COUNT + 2)]

    def update(self):
        _init_textures()
        old_offset = self.offset
        self.offset = -int(self.game.scroll_x % ROW_HEIGHT)
        if self.offset > old_offset:
            column = self.columns.pop(0)
            for x in range(ROW_COUNT):
                column[x] = _random_road_texture_id()
            self.columns.append(column)

    def draw(self):
        x = self.offset
        start_y = self.game.y_for_row(MIN_ROW - 1)
        for column in self.columns:
            y = start_y
            self._draw_tile(x, y, TEXTURES.border)
            y += ROW_HEIGHT
            for cell in column:
                self._draw_tile(x, y, TEXTURES.roads[cell])
                y += ROW_HEIGHT
            self._draw_tile(x, y, TEXTURES.border)
            x += ROW_HEIGHT

    def _draw_tile(self, x, y, texture):
        texture.draw_sized(x, y, ROW_HEIGHT, ROW_HEIGHT)
