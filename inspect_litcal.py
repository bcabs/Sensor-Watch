import json

try:
    with open('utils/liturgy/litcal_2026_dev.json', 'r') as f:
        data = json.load(f)
        if "litcal" in data:
            print(json.dumps(data["litcal"][:5], indent=2))
        else:
            print("No 'litcal' key found.")
except FileNotFoundError:
    print("File not found.")
