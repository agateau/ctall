import arcade

from ctall.constants import MIN_ROW, MAX_ROW

MOVE_SPEED = 400


class Player(arcade.Sprite):
    def __init__(self, game):
        super().__init__("./images/player.png")
        self.game = game
        self.current_row = 0
        self.target_row = 0
        self.left = 12
        self.center_y = game.y_for_row(self.current_row)

    def update(self, delta):
        keys = self.game.keys

        if keys[arcade.key.UP] and self.target_row < MAX_ROW \
                and self.target_row == self.current_row:
            self.target_row += 1
        elif keys[arcade.key.DOWN] and self.target_row > MIN_ROW \
                and self.target_row == self.current_row:
            self.target_row -= 1

        target_y = self.game.y_for_row(self.target_row)
        if self.target_row < self.current_row:
            self.center_y = max(target_y, self.center_y - MOVE_SPEED * delta)
        elif self.target_row > self.current_row:
            self.center_y = min(target_y, self.center_y + MOVE_SPEED * delta)

        if self.center_y == target_y:
            self.current_row = self.target_row
