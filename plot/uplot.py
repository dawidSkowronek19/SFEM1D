import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Wczytanie wygenerowanych danych z pliku CSV
df = pd.read_csv("solution.csv")

# Wykres wyników MES
plt.figure(figsize=(8, 5))
plt.plot(df['x'], df['u'], 'o-', label='Rozwiązanie MES (p=1)', color='tab:blue', markersize=5)


plt.title('Rozwiązanie równania Poissona 1D (MES vs Analityczne)')
plt.xlabel('x')
plt.ylabel('u(x)')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()
plt.tight_layout()

# Zapis wykresu do pliku PNG i wyświetlenie
plt.savefig("solution_plot.png", dpi=300)
plt.show()