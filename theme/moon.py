import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.collections import PatchCollection

fig, ax = plt.subplots()

circle1 = Circle((0.5, 0.5), 0.35, color='white', zorder=1)  # Large circle
circle2 = Circle((0.65, 0.6), 0.35, color='black', zorder=2)     # Smaller circle offset
fig.patch.set_facecolor('black')  # Background for the figure
ax.set_facecolor('black') 
# Add the circles to the axis
ax.add_patch(circle1)
ax.add_patch(circle2)

# Set limits and aspect ratio
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.set_aspect('equal')

ax.axis('off')

plt.savefig("dark.png", bbox_inches="tight", pad_inches=0, dpi=60)

plt.show()
