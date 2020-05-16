import arcade

from ctall.constants import SCREEN_WIDTH


class Wall(arcade.Sprite):
    def __init__(self, game):
        super().__init__("./images/wall.png")
        self.game = game

    def set_pool(self, pool):
        self.pool = pool

    def setup(self, row):
        self.start_x = self.game.scroll_x + SCREEN_WIDTH
        self._update_x_pos()
        self.center_y = self.game.y_for_row(row)

    def update(self, delta):
        self._update_x_pos()
        if self.right < 0:
            self.pool.recycle(self)

    def _update_x_pos(self):
        self.left = self.start_x - self.game.scroll_x
