import json
from datetime import datetime

def load_mappings(file):
    with open(file, 'r') as f:
        data = json.load(f)
        return data["levels"], data["codes"]

def decode_line(line, levels, codes):
    parts = line.strip().split()
    if len(parts) < 3:
        return "[MALFORMED] " + line

    timestamp = int(parts[0])
    level = levels.get(parts[1], f"LVL_{parts[1]}")
    code = codes.get(parts[2], f"CODE_{parts[2]}")
    args = parts[3:]

    dt = datetime.fromtimestamp(timestamp / 1000.0).strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
    return f"[{dt}] [{level}] [{code}] Args: {' '.join(args)}"

def decode_log(input_file, output_file, mapping_file):
    levels, codes = load_mappings(mapping_file)

    with open(input_file, 'r') as fin, open(output_file, 'w') as fout:
        for line in fin:
            decoded = decode_line(line, levels, codes)
            fout.write(decoded + "\n")

    print(f"✅ Decoded log written to: {output_file}")

if __name__ == "__main__":
    decode_log(
        input_file="../logs/app_encoded.log",
        output_file="../logs/decoded_app_log.txt",
        mapping_file="log_mappings.json"
    )