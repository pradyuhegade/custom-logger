import re
import json
from datetime import datetime
import os

def parse_enum_file(path):
    enum_entries = {}
    with open(path, 'r') as f:
        lines = f.readlines()

    for line in lines:
        match = re.match(r'\s*([A-Z0-9_]+)\s*=\s*(\d+)', line)
        if match:
            name, value = match.groups()
            enum_entries[int(value)] = name
    return enum_entries

def generate_mapping(levels_path, codes_path, output_path):
    levels = parse_enum_file(levels_path)
    codes = parse_enum_file(codes_path)

    mapping = {
        "levels": {str(k): v for k, v in levels.items()},
        "codes":  {str(k): v for k, v in codes.items()}
    }

    with open(output_path, 'w') as out:
        json.dump(mapping, out, indent=4)

    print(f"✅ Generated mapping file: {output_path}")
    return mapping["levels"], mapping["codes"]

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

def decode_log(input_file, output_file, levels_path, codes_path):
    levels, codes = generate_mapping(levels_path, codes_path, "../tools/log_mappings.json")

    with open(input_file, 'r') as fin, open(output_file, 'w') as fout:
        for line in fin:
            decoded = decode_line(line, levels, codes)
            fout.write(decoded + "\n")

    print(f"✅ Decoded log written to: {output_file}")

if __name__ == "__main__":
    decode_log(
        input_file="../logs/app_encoded.log",
        output_file="../logs/decoded_app_log.txt",
        levels_path="../include/log_level.h",
        codes_path="../include/log_code.h"
    )