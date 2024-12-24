import numpy as np
import matplotlib.pyplot as plt

number_of_move = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11])

average_move = np.array([1, 2, 3, 4, 5.361, 6.583, 8.1, 9.55, 9.32, 9.16, 9.39])

average_iteration = np.array([7.72, 46.9, 306.76, 1688.8, 8162.51, 45323.3, 285151, 913639, 709984, 444755, 768838])

scrambles = ["U", "B R", "L U L", "L' B' R U", "L U R' U' R", "R U' L' R L U'", "U L U' R B' R L", "R' L U B U B' L R", "R' L' R' L' B' L' U B' U", "L B' R U L U' L' R B' U'", "U' B U L U L R' B' L B' L"]


plt.rcParams['font.size'] = 20


plt.plot(number_of_move, average_move, color="k")


plt.xlim(1, 11)
plt.xticks([1, 3, 5, 7, 9, 11])
plt.ylim(0, None)
plt.xlabel("Number of moves of scramble")
plt.ylabel("Average moves")
plt.title("Average moves of solution found by Monte Carlo after running 100 times")
#plt.ylabel("Iterations")
#plt.title("Average of iterations needed for Monte Carlo to find the solution after running 100 times")


plt.show()

#for i in range(11):
#    print(f"|{number_of_move[i]}|{scrambles[i]}|{average_move[i]}|{average_iteration[i]}|")
