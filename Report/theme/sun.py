import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.collections import PatchCollection

fig, ax = plt.subplots()

circle1 = Circle((0.5, 0.5), 0.25, color='black', zorder=1)  # Large circle
circle2 = Circle((0.5, 0.5), 0.2, color='white', zorder=2)  # Large circle
circle3 = Circle((0.5, 0.5), 0.42, edgecolor='black', facecolor='none', zorder=2)  # Large circle
circle4 = Circle((0.5, 0.5), 0.28, edgecolor='black', facecolor='none', zorder=2)  # Large circle

# Add the circles to the axis
ax.add_patch(circle1)
ax.add_patch(circle2)
#ax.add_patch(circle3)
#ax.add_patch(circle4)

ax.plot([0.8, 0.90], [0.5, 0.5], c='black', linewidth=10)
ax.plot([0.2, 0.1], [0.5, 0.5], c='black', linewidth=10)
ax.plot([0.5, 0.5], [0.2, 0.1], c='black', linewidth=10)
ax.plot([0.5, 0.5], [0.8, 0.9], c='black', linewidth=10)
ax.plot([0.71, 0.785], [0.71, 0.785], c='black', linewidth=10)
ax.plot([0.71-0.495, 0.785-0.495], [0.71-0.495, 0.785-0.495], c='black', linewidth=10)
ax.plot([0.785-0.495, 0.71-0.495] , [0.71, 0.785], c='black', linewidth=10)
ax.plot([0.785, 0.71] , [0.71-0.495, 0.785-0.495], c='black', linewidth=10)

# Set limits and aspect ratio
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.set_aspect('equal')

ax.axis('off')

plt.savefig("light.png", bbox_inches="tight", pad_inches=0, dpi=60)

plt.show()
