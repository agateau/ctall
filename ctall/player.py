import arcade

MIN_ROW = -2
MAX_ROW = 2

ROW_HEIGHT = 48
MOVE_SPEED = 400


class Player(arcade.Sprite):
    def __init__(self, game):
        super().__init__("./images/player.png")
        self.game = game
        self.current_row = 0
        self.target_row = 0
        self.left = 12
        self.center_y = self.y_for_row(self.current_row)

    def update(self, delta):
        keys = self.game.keys

        if keys[arcade.key.UP] and self.target_row < MAX_ROW \
                and self.target_row == self.current_row:
            self.target_row += 1
        elif keys[arcade.key.DOWN] and self.target_row > MIN_ROW \
                and self.target_row == self.current_row:
            self.target_row -= 1

        target_y = self.y_for_row(self.target_row)
        if self.target_row < self.current_row:
            self.center_y = max(target_y, self.center_y - MOVE_SPEED * delta)
        elif self.target_row > self.current_row:
            self.center_y = min(target_y, self.center_y + MOVE_SPEED * delta)

        if self.center_y == target_y:
            self.current_row = self.target_row

    def y_for_row(self, row):
        return self.game.height / 2 + row * ROW_HEIGHT
