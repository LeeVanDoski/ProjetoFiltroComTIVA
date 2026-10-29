import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

fs = 10000.0 
numtaps = 65 

nyq=fs/2
output = []
filters=['lowpass','highpass','bandpass']
freqs=[250,500,1000,1500,2000,2500,3000,3500,4000,4500]

output.append(f"#define NUM_FILTERS {len(filters)}")
output.append(f"#define NUM_FREQS {len(freqs)}")
output.append(f"#define NUM_TAPS {numtaps}\n")
output.append("const float32_t fir_coeffs[NUM_FILTERS][NUM_FREQS][NUM_TAPS] = {")

# Inverti a ordem do loop original (ftype primeiro) para bater com a estrutura da matriz
for ftype in filters:
    output.append("    {") 
    
    for freq in freqs:
        if ftype == 'lowpass':
            cutoff = min(freq, nyq * 0.999)
            b = signal.firwin(numtaps, cutoff=cutoff, fs=fs, pass_zero='lowpass')

        elif ftype == 'highpass':
            cutoff = min(freq, nyq * 0.999)
            b = signal.firwin(numtaps, cutoff=cutoff, fs=fs, pass_zero='highpass')

        elif ftype == 'bandpass':
            low = max(1e-6, freq - 250)
            high = min(freq + 250, nyq * 0.999)

            if low >= high:
                b = np.zeros(numtaps) # Preenche com zeros para não quebrar a matriz
            else:
                b = signal.firwin(numtaps, cutoff=[low, high], fs=fs, pass_zero=False)

        coeffs = ", ".join([f"{coef:.10f}" for coef in b])
        output.append(f"        {{ {coeffs} }},")
    
    output.append("    },")

output.append("};")

filepath = "coeficientes_filtros.txt"
with open(filepath, "w") as f:
    f.write("\n".join(output))

print("Arquivo gerado:", filepath)