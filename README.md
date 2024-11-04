
# Breakout Game in C++

This project is a simple implementation of the classic Breakout game using the SFML (Simple and Fast Multimedia Library) framework in C++. The game features a paddle, a ball, and bricks that the player can break to score points.

## Features

- **Responsive Paddle Control**: Move the paddle left and right using keyboard inputs.
- **Ball Physics**: The ball bounces off the paddle and bricks, creating dynamic gameplay.
- **Brick Break Mechanics**: Break bricks by hitting them with the ball, with different types of bricks for added challenge.
- **Score Tracking**: Keep track of the player's score as they break bricks.
- **Game Over Condition**: The game ends when the ball falls below the paddle.
- **Highscore Logic**: Highest score gets memorized as best result.


## Getting Started

### Prerequisites

To build and run this project, you will need:

- A C++ compiler that supports C++11 or later (e.g., GCC, Clang, MSVC).
- SFML library installed on your system.

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/Dadyze/BreakoutGameCpp.git
   cd BreakoutGameCpp
   ```

2. Make sure to link against the SFML libraries when compiling. You can compile the project using `g++`:
   ```bash
   g++ -std=c++11 -o breakout main.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. Run the program:
   ```bash
   ./breakout
   ```

### Controls

- **Left Arrow Key**: Move the paddle to the left.
- **Right Arrow Key**: Move the paddle to the right.
- **Escape Key**: Exit the game.



## Contributing

Contributions are welcome! If you find a bug or have a feature request, feel free to open an issue or submit a pull request.


## Acknowledgments

This project is inspired by the classic Breakout game and utilizes SFML for graphics and input handling.
