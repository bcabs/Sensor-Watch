import json
import glob

def combine_liturgical_calendars():
    """
    Combines liturgical calendar data from multiple JSON files into one file.
    """
    file_paths = sorted(glob.glob("liturgical_calendar_20*.json"))
    all_events = []

    for file_path in file_paths:
        with open(file_path, 'r') as f:
            try:
                data = json.load(f)
                if "litcal" in data:
                    all_events.extend(data["litcal"])
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON from {file_path}: {e}")

    with open("all_liturgical_events.json", 'w') as f:
        json.dump(all_events, f, indent=2)

if __name__ == "__main__":
    combine_liturgical_calendars()
