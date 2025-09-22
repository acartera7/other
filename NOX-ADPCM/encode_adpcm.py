import matplotlib as plt

# Standard IMA ADPCM step size table (89 entries)
STEP_SIZE_TABLE = [
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143,
    157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544,
    598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707,
    1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871,
    5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635,
    13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
]

# Index adjustment table (based on encoded nibble)
INDEX_TABLE = [-1, -1, -1, -1, 2, 4, 6, 8]


def clamp(val, min_val, max_val):
    return max(min(val, max_val), min_val)

def encode_sample(pcm_sample, predictor, step_index, step_table):
    step = step_table[step_index]
    diff = pcm_sample - predictor
    encoded_nibble = 0

    if diff < 0:
        encoded_nibble = 8
        diff = -diff

    temp_step = step
    delta = 0
    if diff >= temp_step:
        encoded_nibble |= 4
        diff -= temp_step
        delta += temp_step
    temp_step >>= 1
    if diff >= temp_step:
        encoded_nibble |= 2
        diff -= temp_step
        delta += temp_step
    temp_step >>= 1
    if diff >= temp_step:
        encoded_nibble |= 1
        delta += temp_step

    if encoded_nibble & 8:
        delta = -delta

    predictor = clamp(predictor + delta, -32768, 32767)
    step_index += INDEX_TABLE[encoded_nibble & 0x07]
    step_index = clamp(step_index, 0, len(step_table) - 1)

    return encoded_nibble, predictor, step_index

if __name__ == "__main__":
      # Example PCM samples (16-bit signed integers)
  pcm_samples = [1000, 1020, 1010, 1030, 1045, 1060]
  #              0     20    -10   20    15    15
  predictor = pcm_samples[0]
  step_index = 0
  
  print(f"Initial predictor: {predictor}, step_index: {step_index}")
  
  for sample in pcm_samples[1:]:
      nibble, predictor, step_index = encode_sample(sample, predictor, step_index, STEP_SIZE_TABLE)
      print(f"PCM: {sample}, Encoded: {nibble:04b} (0x{nibble:X}), Predictor: {predictor} (0x{predictor:X}), StepIndex: {step_index}, StepSize: {STEP_SIZE_TABLE[step_index]}")
