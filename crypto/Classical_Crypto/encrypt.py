import numpy as np
import wave

def base_4(ptext):
    ctext = []
    for c in ptext:
        ctext.append(ord(c)//64)
        ctext.append((ord(c) // 16) % 4)
        ctext.append((ord(c) // 4) % 4)
        ctext.append(ord(c) % 4)
    return ctext

def generate_tone(freq, duration, sample_rate, pad_duration = 0.1):
    t = np.linspace(0, duration, int(sample_rate * duration), False)
    tone = np.sin(freq * t * 2 * np.pi)
    pad_samples = int(sample_rate * pad_duration)
    padding = np.zeros(pad_samples)
    return np.concatenate([tone, padding])

def encode_base4_audio(digits, out_file='hidden.wav', tone_duration=.6):
    sample_rate = 44100
    output = np.array([])
    freq_map = {
    0: 18000,
    1: 18666,
    2: 19333,
    3: 20000
}

    for d in digits:
        output = np.concatenate((output, generate_tone(freq_map[d], tone_duration,sample_rate)))

    # Normalize
    output = output * (2**15 - 1) / np.max(np.abs(output))
    output = output.astype(np.int16)

    with wave.open(out_file, 'w') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(sample_rate)
        wf.writeframes(output.tobytes())


def fft_overlay(file1, file2, output_file, hidden_volume=0.05):
    with wave.open(file1, 'rb') as wav1, wave.open(file2, 'rb') as wav2:
        

        params = wav1.getparams()
        frames1 = wav1.readframes(params.nframes)
        frames2 = wav2.readframes(params.nframes)

        audio1 = np.frombuffer(frames1, dtype=np.int16).astype(np.float32)
        audio2 = np.frombuffer(frames2, dtype=np.int16).astype(np.float32)

        # Pad shorter signal with zeros if necessary
        max_len = max(len(audio1), len(audio2))
        audio1 = np.pad(audio1, (0, max_len - len(audio1)), 'constant')
        audio2 = np.pad(audio2, (0, max_len - len(audio2)), 'constant')

        # Apply volume scaling to hidden audio
        audio2 *= hidden_volume

        # FFT
        fft1 = np.fft.fft(audio1)
        fft2 = np.fft.fft(audio2)

        # Add frequency components
        combined_fft = fft1 + fft2

        # Inverse FFT
        combined = np.fft.ifft(combined_fft)

        # Take real part and normalize to int16 range
        combined_real = np.real(combined)

        # Normalize if needed to avoid clipping
        max_val = np.max(np.abs(combined_real))
        if max_val > 32767:
            combined_real = combined_real * (32767 / max_val)

        combined_int16 = combined_real.astype(np.int16)

        # Write output wav
        with wave.open(output_file, 'wb') as out_wav:
            out_wav.setparams(params)
            out_wav.writeframes(combined_int16.tobytes())


flag = "flag{y0u_c4n't_h3aR_mE}"
encode_base4_audio(base_4(flag))
fft_overlay('original.wav', 'hidden.wav', 'audio.wav')