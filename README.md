# C-MiniGame: Strategy Exploration

A terminal-based adventure game where strategy and resource management are key to survival. This project was developed in **C** and features mechanics inspired by classic puzzle games like Minesweeper.

## ⚙️ Game Mechanics

In this game, you navigate a 15x30 grid to find the hidden exit ('X') while managing your resources:

* **Lifepoints:** You start with 3 lives. Hitting an unsafe zone ('!') costs 1 life.
* **Energy:** Every action consumes 1 point of your 450 energy units. If you run out, you lose lifepoints.
* **Safety Check:** Use this tool to scan the immediate surroundings and reveal how many safe zones are adjacent to you.
* **Bombs:** You have 3 bombs that can be used to neutralize and reveal neighboring zones safely.

## 🎮 How to Play

1. **Move:** Use the menu to move Up, Down, Left, or Right. The board features edge wrapping.
2. **Explore:** Reveal the board by moving or using your safety check.
3. **Win:** Find the hidden exit ('X') to win the game.
4. **Game Over:** The game ends if you lose all lifepoints or run out of energy.

## 🛠️ How to Compile and Run

To run this game on your machine, ensure you have a C compiler (like GCC) installed.

1. Clone the repository:
   ```bash
   git clone [https://github.com/YourUsername/C-MiniGame.git](https://github.com/YourUsername/C-MiniGame.git)
