import json
from itertools import groupby

def compare_dates():
    """
    Compares the order of dates in all_dates.txt and all_liturgical_events.json.
    """
    with open('all_dates.txt', 'r') as f:
        dates_from_file = [line.strip() for line in f]

    with open('all_liturgical_events.json', 'r') as f:
        liturgical_events = json.load(f)

    liturgical_dates = [event['date'].split('T')[0] for event in liturgical_events]
    
    # Remove consecutive duplicates
    unique_liturgical_dates = [k for k, g in groupby(liturgical_dates)]

    print(f"Length of dates_from_file (all_dates.txt): {len(dates_from_file)}")
    print(f"Length of unique_liturgical_dates (all_liturgical_events.json, deduplicated): {len(unique_liturgical_dates)}")

    if dates_from_file == unique_liturgical_dates:
        print("Dates are in the same order.")
    else:
        print("Dates are in a different order.")
        # Find the first point of difference
        for i, (date1, date2) in enumerate(zip(dates_from_file, unique_liturgical_dates)):
            if date1 != date2:
                print(f"Mismatch at index {i}:")
                print(f"  all_dates.txt: {date1}")
                print(f"  all_liturgical_events.json: {date2}")
                break
        else: # This else block runs if the loop completes without a break
            if len(dates_from_file) != len(unique_liturgical_dates):
                print("The lists have different lengths but no date mismatch was found up to the shorter list's end.")
                print(f"  all_dates.txt has {len(dates_from_file)} dates.")
                print(f"  all_liturgical_events.json has {len(unique_liturgical_dates)} unique dates.")


if __name__ == "__main__":
    compare_dates()