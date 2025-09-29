import sys
import os
import soundfile as sf
import glob

def infer_output_filename(input_path):
  base, ext = os.path.splitext(input_path)
  return f"{base} (copy){ext}"

def decode_adpcm_to_pcm(input_path, output_path=None):
  if output_path is None:
    output_path = infer_output_filename(input_path)

  # Open input file (soundfile will decode IMA ADPCM if supported)
  data, samplerate = sf.read(input_path, dtype='int16', always_2d=True)
  # Write as 16-bit PCM WAV
  sf.write(output_path, data, samplerate, subtype='PCM_16')
  print(f"Decoded and saved to: {output_path}")

def main():
  if len(sys.argv) < 2:
    print("Usage: python adpcm2wav2.py <input.wav|input_directory> [output_directory]")
    sys.exit(1)

  input_path = sys.argv[1]
  output_dir = sys.argv[2] if len(sys.argv) > 2 else None

  if os.path.isdir(input_path):
    # Directory mode
    wav_files = glob.glob(os.path.join(input_path, "*.wav"))
    if not wav_files:
      print("No .wav files found in the directory.")
      sys.exit(1)
    # Create output directory
    if output_dir is None:
      output_dir = input_path.rstrip(os.sep) + " (copy)"
    os.makedirs(output_dir, exist_ok=True)
    for wav_file in wav_files:
      out_file = os.path.join(output_dir, os.path.basename(wav_file))
      decode_adpcm_to_pcm(wav_file, out_file)
  else:
    # Single file mode
    decode_adpcm_to_pcm(input_path, output_dir)

if __name__ == "__main__":
  main()