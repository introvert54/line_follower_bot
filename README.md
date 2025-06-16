# line_follower_bot

This repository has 2 separate codes, both of line following bots but slightly different functionality.

common part:- The bot follows underlying lines using reading from IR sensors and deciding the speed and direction of each tyre based on these readings and the PID algorithm.
PID(Proportional-Integral-Derivative) -> what it does basically is that if the bot is very tilted with respect to the lines, it will veer sharply. For small tilting, veers less sharply, and if it is already following the lines, stay straight and move both tyres with the same speed.

1) maze-solver
Apart from line following, the bot is capable of solving acyclic mazes using a left-turn rule. Firstly, it traverses the whole maze once, every path, and precomputes the path required to reach the end efficiently in the next traversal.
The left-turn rule -> every time it encounters a left turn, it... turns left. Then second priority is moving straight ahead followed by turning right. It also prunes all paths leading to dead ends.

2) e-yantra
This is the code of the line-follower bot as required in the e-yantra competition organized by IIT Bombay. It is a slight modification to a simple line follower. Mainly, the concept of line itself has been changed. now instead of only following a single line, the bot is required to stay in between 2 straight lines as well, kind of like following a lane instead of line. Also, the path keeps changing from line to lane, or lane to line and the bot has to decide based on the IR sensor readings, on which type of path it currently is.
Still, at the heart of it lies the PID algorithm.
