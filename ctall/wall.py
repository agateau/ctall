import arcade


class Wall(arcade.Sprite):
    def __init__(self, game):
        super().__init__("./images/wall.png")
        self.game = game

    def setup(self, row):
        self.left = self.game.width
        self.center_y = self.game.y_for_row(row)

    def update(self, delta):
        self.center_x -= self.game.scroll_speed * delta

        if self.right < 0:
            self.setup(1)
