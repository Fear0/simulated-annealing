# simulated-annealing
Using simulated annealing (SA) to optimize the arrangement of cells in floorplanning.

Simulated annealing is used for various optimization problems. A possible solution corresponds to the body in the algorithm, the oscillation of the molecules is realized by disturbances or CHANGES in the current solution to a new one.

At each temperature so many CHANGES are carried out until a state of equilibrium is reached for that temperature. This can be the case, for example, after a fixed number of change steps, or if the quality of the solution has not changed over a certain number of steps. In our implementation only the area is used as a cost function for EVALUATION
without taking in consideration the wiring plan.

The procedure is used in various "Electronic Desing Automation" areas. It is easy to implement, delivers good results, but is very time consuming. **_The method has the property that local minima can be overcome. The reason for this lies in the fact that placement results that generate higher costs than the previous placement can also be probabilistically accepted_**. During the cooling process, the less the temperature gets, the less probable a higher cost result will be accepted.

A monotonic optimizer modifies the floorplan ramndomly t times and eventually reaches the optimal or a close to optimal solution.
SA algorithm also reaches the optimal or a close to optimal solution in considerably less computation time even though it is a computing time intensive algorithm.

## RUN

### UNIX
clone the repo and compile: \\
git clone ...
make
./floorplanning

### Windows

install visual studio and open the solution file ".sln" then run.


