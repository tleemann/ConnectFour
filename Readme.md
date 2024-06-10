# Connect Four

Perform an alpha-beta pruning tree search to solve the connect four game.

## Getting started

1. Clone this repo locally.
1. Navigate to the top level directory of the repo from your perferred shell.
1. Run `make`.
1. Run `./fourconnect` to view the command usage
1. Run `./fourconnect` again with the proper arguments. The output of this command will be the index of the column (zero-based from left to right) for which the X player should move next.

## XO- format
Connect 4 state is represented in "XO-" format for this command line tool. To translate a connect 4 board to "XO-" format:

- read the columns of the board left to right and for each column:
    - Add the pieces in the columns from bottom to top to your "XO-" format where 'X' is the AI player and 'O' is the opponent. Stop when you reach an empty space.
    - Append a '.' character to signify the end of the column

For example, the following board can be represented as `O.XO.OXXO.OXX.XO..O.` in the "XO-" format:

|   |   |   |   |   |   |   |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|   |   |   |   |   |   |   |
|   |   | O |   |   |   |   |
|   |   | X | X | X |   |   |
|   | O | X | X | O |   |   |
| O | X | O | O | X |   | O |
