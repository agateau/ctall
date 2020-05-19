from ctall.constants import SCREEN_WIDTH


class ScrollComponent:
    def __init__(self, game, sprite):
        self.game = game
        self.sprite = sprite

    def setup(self):
        self.start_x = self.game.scroll_x + SCREEN_WIDTH
        self._update_x_pos()

    def update(self, delta):
        self._update_x_pos()

    def _update_x_pos(self):
        self.sprite.left = self.start_x - self.game.scroll_x

    def has_scrolled_out(self):
        return self.sprite.right < 0
