import numpy as np
import matplotlib.pyplot as plt


data = np.loadtxt('Dice_python.txt')

values = data[448:548, 0]
probabilities = data[448:548, 1]

plt.figure(figsize=(10, 6))
plt.bar(values, probabilities, color='skyblue', edgecolor='black')

plt.title('DoubleDiceAlt')
plt.xlabel('Величины')
plt.ylabel('Вероятность')
plt.ylim(0, max(probabilities) * 1.5)

plt.savefig("Gr8", dpi = 300)
plt.grid(axis='y', alpha=0.3)
plt.show()

