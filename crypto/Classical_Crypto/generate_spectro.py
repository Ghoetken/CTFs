import wave
import numpy as np
import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend
import matplotlib.pyplot as plt
from scipy.signal import spectrogram

# Load audio
with wave.open('audio.wav', 'rb') as wav:
    n_channels = wav.getnchannels()
    framerate = wav.getframerate()
    n_frames = wav.getnframes()
    signal = np.frombuffer(wav.readframes(n_frames), dtype=np.int16)
    if n_channels == 2:
        signal = signal[::2]


# Compute spectrogram
f, t, Sxx = spectrogram(signal, fs=framerate)

# Plot and save (headless)
plt.figure(figsize=(12, 6))
plt.pcolormesh(t, f, 10 * np.log10(Sxx), shading='gouraud', cmap='magma')
plt.ylabel('Frequency [Hz]')
plt.xlabel('Time [sec]')
plt.title('Spectrogram (dB)')
plt.colorbar(label='Intensity [dB]')
plt.tight_layout()

plt.savefig('spectrogram.png', dpi=300)
plt.close()
