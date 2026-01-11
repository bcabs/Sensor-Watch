import requests
import json
from datetime import datetime, date
import os
import argparse

def parse_liturgical_data(data):
    """
    Parses a list of liturgical event data into the required calendar_data format.
    """
    calendar_data = {}
    holy_days_of_obligation_keys = {
        "ImmaculateConception",
        "Christmas",
        "MaryMotherOfGod",
        "Ascension",
        "Assumption",
        "AllSaints"
    }
    
    season_weeks = {}
    current_season = None
    
    # Sort data by date to ensure correct week_of_season calculation
    data.sort(key=lambda x: datetime.strptime(x["date"].split("T")[0], "%Y-%m-%d"))

    for item in data:
        date_str = item.get("date", "No date provided").split("T")[0]
        if date_str == "No date provided":
            continue
        
        event_key = item.get("event_key", "N/A")

        # Skip vigil events and SatMem entries
        if "vigil" in event_key or "SatMem" in event_key:
            continue
        
        season = item.get("liturgical_season_lcl", "N/A")
        name = item.get("name", "No name provided")
        
        is_holy_day = event_key in holy_days_of_obligation_keys
        
        if item.get("day_of_the_week_long", "") == "Sunday":
            is_holy_day = True # Sundays are always holy days for our purposes
        elif item.get("holy_day_of_obligation", False): # Directly from file data
            is_holy_day = True

        # Calculate week of season - same logic as get_liturgical_calendar
        if season != current_season:
            if "Ordinary Time" in season and season in season_weeks:
                # Special handling for Ordinary Time, which can be broken up
                # and weeks should continue numbering if it's the same Ordinary Time period
                # This logic might need refinement if the source JSON doesn't provide enough context
                # to differentiate between OT periods. For now, a simple increment.
                season_weeks[season] += 1 # Increment as if a new period, then reset if first week
            if season not in season_weeks:
                season_weeks[season] = 1
            current_season = season
        elif item.get("day_of_the_week_long", "") == "Sunday":
            if event_key != "Lent1": # Lent1 is the first Sunday, but not necessarily the first week start for calculation
                season_weeks[season] = season_weeks.get(season, 0) + 1
        
        # Ensure week is at least 1, especially for non-Sunday first entries
        if season_weeks.get(current_season, 0) == 0:
             season_weeks[current_season] = 1

        if date_str not in calendar_data:
            calendar_data[date_str] = {
                "season": season,
                "day_of_week": item.get("day_of_the_week_long", ""),
                "week_of_season": season_weeks.get(current_season, 1),
                "events": [(event_key, name)],
                "holy_day_of_obligation": is_holy_day
            }
        else:
            calendar_data[date_str]["events"].append((event_key, name))
            if is_holy_day:
                calendar_data[date_str]["holy_day_of_obligation"] = True
            # Update week of season if current entry has a more recent Sunday
            # and thus a more accurate week number for that season.
            calendar_data[date_str]["week_of_season"] = max(
                calendar_data[date_str].get("week_of_season", 1), 
                season_weeks.get(current_season, 1)
            )
    return calendar_data

def get_liturgical_calendar(year):
    """
    Fetches the liturgical calendar for the Diocese of Boston for the specified year,
    parses the data, and stores it in a dictionary.
    Caches the response to a file to avoid repeated requests.
    """
    url = f"https://litcal.johnromanodorazio.com/api/dev/calendar/{year}?epiphany=SUNDAY_JAN2_JAN8&ascension=THURSDAY&corpus_christi=SUNDAY"

    # url = f"https://litcal.johnromanodorazio.com:443/api/v5/calendar/diocese/boston_us/{year}"
    cache_filename = f"litcal_{year}_dev.json"
    data = None

    try:
        if os.path.exists(cache_filename):
            with open(cache_filename, 'r') as f:
                data = json.load(f)
        else:
            response = requests.get(url)
            response.raise_for_status()  # Raise an exception for bad status codes
            data = response.json()
            with open(cache_filename, 'w') as f:
                json.dump(data, f)

        if data and "litcal" in data and isinstance(data["litcal"], list):
            return parse_liturgical_data(data["litcal"])
        else:
            print("Could not find 'litcal' key or it's not a list in the response.")

    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")
    except json.JSONDecodeError:
        print("Error parsing JSON response.")
        
    return {}

def load_calendar_from_file(filename):
    """
    Loads liturgical calendar data from a local JSON file (all_liturgical_events.json)
    and processes it into the required calendar_data format.
    """
    try:
        if not os.path.exists(filename):
            print(f"Error: File not found at {filename}")
            return {}

        with open(filename, 'r') as f:
            data = json.load(f)
        
        if not isinstance(data, list):
            print("Error: Expected JSON file to contain a list of events.")
            return {}
            
        return parse_liturgical_data(data)

    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
        return {}
    except json.JSONDecodeError:
        print(f"Error: Could not decode JSON from '{filename}'.")
        return {}
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        return {}
    
    return {}

def get_display_event(events):
    event_key, name = None, None

    # Prioritize special events
    for key, n in events:
        if key in ["KateriTekakwitha", "StHildegardBingen"]:
            event_key, name = key, n
            break
    
    # If no special event, use default logic
    if event_key is None:
        if len(events) >= 2:
            event_key, name = events[1]
        elif events:
            event_key, name = events[0]
        else:
            event_key, name = "", ""
    
    display_event_key = event_key
    display_name = name

    # Conditional printing logic
    
    if "Weekday" in event_key or "Sunday" in event_key:
        display_event_key = ""
    if "Weekday" in name or "Sunday" in name or "Week" in name:
        display_name = ""

    return display_event_key, display_name

def generate_c_struct_output(calendar_data):
    """
    Generates C code for the special_days array.
    """
    # Create a dictionary to hold the C struct data, indexed by days since start_date
    c_struct_data = {}

    season_map = {
        "Advent": "ADVENT",
        "Christmas": "CHRISTMAS",
        "Ordinary Time": "ORDINARY_TIME",
        "Lent": "LENT",
        "Easter Triduum": "EASTER_TRIDUUM",
        "Easter": "EASTER",
    }
    
    now = datetime.now()
    START_DATE_YEAR = now.year
    START_DATE_MONTH = now.month
    START_DATE_DAY = now.day
    start_date = date(START_DATE_YEAR, START_DATE_MONTH, START_DATE_DAY)

    for date_str, entry in calendar_data.items():
        d = datetime.strptime(date_str, "%Y-%m-%d").date()
        days_since = (d - start_date).days
        
        
        if days_since < 0:
            continue

        season_enum = season_map.get(entry['season'], "UNKNOWN")
        week = entry['week_of_season']
        hdo = "true" if entry['holy_day_of_obligation'] else "false"
        
        event_key, _ = get_display_event(entry['events'])
        text = event_key.replace('"', '\\"')

        c_struct_data[days_since] = f"{{ {season_enum}, {week}, {hdo}, \"{text}\" }}"

    print('#include "special_day_face.h"')
    print(f"#define SPECIAL_DAYS_START_DATE_YEAR {START_DATE_YEAR}")
    print(f"#define SPECIAL_DAYS_START_DATE_MONTH {START_DATE_MONTH}")
    print(f"#define SPECIAL_DAYS_START_DATE_DAY {START_DATE_DAY}")
    print("")
    print("static const SpecialDay special_days[] = {")

    # Assuming the C code can handle a large array. Let's find the max day.
    max_day = 0
    if c_struct_data:
        max_day = max(c_struct_data.keys())

    for i in range(max_day + 1):
        # Calculate date for the current index i
        current_date_obj = start_date + __import__('datetime').timedelta(days=i)
        current_date_str = current_date_obj.strftime("%Y-%m-%d")

        if i in c_struct_data:
            print(f"    /* {i:03} {current_date_str} */ {c_struct_data[i]},")
        else:
            print(f"    /* {i:03} {current_date_str} */ {{ UNKNOWN, 0, false, NULL }},")
            
    print("};")

def print_raw_calendar_data(calendar_data, year):
    """
    Pretty-prints the liturgical calendar data for a given year.
    """
    print(f"\n--- Liturgical Calendar for Year: {year} (Raw Data) ---")
    print(json.dumps(calendar_data, indent=2))

def print_formatted_calendar_data(calendar_data, year):
    """
    Prints the liturgical calendar data for a given year in a formatted way.
    """
    print(f"\n--- Liturgical Calendar for Year: {year} ---")
    print(f"{'Date':<11} {'Season':<13} {'Day of Week':<15} {'Week of Season':<17} {'Event Key':<30} {'Name':<40} {'HDO':<5}")
    print(f"{'-'*11:<11} {'-'*13:<13} {'-'*15:<15} {'-'*17:<17} {'-'*30:<30} {'-'*40:<40} {'-'*5:<5}")

    for date, entry_data in sorted(calendar_data.items()):
        hdo_column = "HDO" if entry_data["holy_day_of_obligation"] else ""
        
        display_event_key, display_name = get_display_event(entry_data["events"])
        display_name = display_name[:40]

        output_line = (
            f"{date:<11} {entry_data['season']:<13} {entry_data['day_of_week']:<15} "
            f"{entry_data['week_of_season']:<17} {display_event_key:<30} {display_name:<40} {hdo_column:<5}"
        )
        print(output_line)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch and display liturgical calendar data.")
    parser.add_argument("--raw", action="store_true", help="Display raw JSON data.")
    parser.add_argument("--c-struct", action="store_true", help="Generate C struct array.")
    parser.add_argument("--from-file", action="store_true", help="Read liturgical data from all_liturgical_events.json.")
    args = parser.parse_args()

    full_calendar_data = {}
    
    if args.from_file:
        print("Reading liturgical data from all_liturgical_events.json...")
        full_calendar_data = load_calendar_from_file("all_liturgical_events.json") # Corrected path- STOP DOING THIS - ASSUME THAT IT'S BEING INVOKED IN THE SAME DIR IT'S IN
        if not full_calendar_data:
            print("Failed to load data from all_liturgical_events.json. Exiting.")
            exit(1)
        
        # When reading from file, assume all years are included and process once
        if args.c_struct:
            generate_c_struct_output(full_calendar_data)
        elif args.raw:
            print_raw_calendar_data(full_calendar_data, "All Years (from file)")
        else:
            # Need to iterate through years if printing formatted
            years_in_data = sorted(list(set([datetime.strptime(d, "%Y-%m-%d").year for d in full_calendar_data.keys()])))
            for year_to_display in years_in_data:
                year_data = {d: data for d, data in full_calendar_data.items() if datetime.strptime(d, "%Y-%m-%d").year == year_to_display}
                print_formatted_calendar_data(year_data, year_to_display)
    else:
        current_year = datetime.now().year
        for year_offset in range(10):
            year_to_fetch = current_year + year_offset
            calendar_data = get_liturgical_calendar(year_to_fetch)
            if args.c_struct:
                full_calendar_data.update(calendar_data)
            elif args.raw:
                print_raw_calendar_data(calendar_data, year_to_fetch)
            else:
                print_formatted_calendar_data(calendar_data, year_to_fetch)
        
        if args.c_struct:
            generate_c_struct_output(full_calendar_data)
