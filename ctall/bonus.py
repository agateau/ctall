import arcade

from ctall.scrollcomponent import ScrollComponent
from ctall.poolable import Poolable


class Bonus(arcade.Sprite, Poolable):
    def __init__(self, game):
        super().__init__("./images/bonus.png")
        self.game = game
        self.scrollcomponent = ScrollComponent(game, self)

    def setup(self, row):
        self.scrollcomponent.setup()
        self.center_y = self.game.y_for_row(row)

    def update(self, delta):
        self.scrollcomponent.update(delta)
        if self.scrollcomponent.has_scrolled_out():
            self.recycle()

    def on_captured(self):
        self.recycle()
