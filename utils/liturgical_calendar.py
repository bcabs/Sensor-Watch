import requests
import json
from datetime import datetime
import os

import requests
import json
from datetime import datetime
import os

import requests
import json
from datetime import datetime
import os

import requests
import json
from datetime import datetime
import os
import argparse

def get_liturgical_calendar(year):
    """
    Fetches the liturgical calendar for the Diocese of Boston for the specified year,
    parses the data, and stores it in a dictionary.
    Caches the response to a file to avoid repeated requests.
    """
    url = f"https://litcal.johnromanodorazio.com:443/api/v5/calendar/diocese/boston_us/{year}"
    cache_filename = f"liturgical_calendar_{year}.json"
    calendar_data = {}
    data = None
    holy_days_of_obligation_keys = {
        "ImmaculateConception",
        "Christmas",
        "MaryMotherOfGod",
        "Ascension",
        "Assumption",
        "AllSaints"
    }

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
            season_weeks = {}
            current_season = None
            
            for item in data["litcal"]:
                date = item.get("date", "No date provided").split("T")[0]
                
                event_key = item.get("event_key", "N/A")

                # Skip vigil events and SatMem entries
                if "vigil" in event_key or "SatMem" in event_key:
                    continue
                
                season = item.get("liturgical_season_lcl", "N/A")
                if season != current_season:
                    if "Ordinary Time" in season and season in season_weeks:
                        season_weeks[season] += 2
                    if season not in season_weeks:
                        season_weeks[season] = 1
                    current_season = season
                elif item.get("day_of_the_week_long", "") == "Sunday":
                    # The first week of Lent is special, as it starts on Ash Wednesday.
                    # When the Sunday after that day occurs, do not increment the week num.
                    if event_key != "Lent1":
                        season_weeks[season] += 1
                
                name = item.get("name", "No name provided")
                
                # Check if it's a Holy Day of Obligation based on predefined keys
                is_holy_day = event_key in holy_days_of_obligation_keys
                
                # Additionally, mark all Sundays as Holy Days of Obligation
                if item.get("day_of_the_week_long", "") == "Sunday":
                    is_holy_day = True

                if date not in calendar_data:
                    calendar_data[date] = {
                        "season": season,
                        "day_of_week": item.get("day_of_the_week_long", ""),
                        "week_of_season": season_weeks.get(current_season, 1),
                        "events": [(event_key, name)],
                        "holy_day_of_obligation": is_holy_day
                    }
                else:
                    calendar_data[date]["events"].append((event_key, name))
                    # If any event on the day is a holy day, the day is a holy day
                    if is_holy_day:
                        calendar_data[date]["holy_day_of_obligation"] = True
        else:
            print("Could not find 'litcal' key or it's not a list in the response.")

    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")
    except json.JSONDecodeError:
        print("Error parsing JSON response.")
        
    return calendar_data

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
        
        event_key, name = None, None

        # Prioritize special events
        for key, n in entry_data["events"]:
            if key in ["KateriTekakwitha", "StHildegardBingen"]:
                event_key, name = key, n
                break
        
        # If no special event, use default logic
        if event_key is None:
            if len(entry_data["events"]) >= 2:
                event_key, name = entry_data["events"][1]
            elif entry_data["events"]:
                event_key, name = entry_data["events"][0]
            else:
                event_key, name = "", ""

        display_event_key = event_key
        display_name = name[:40]

        # Conditional printing logic
        if event_key not in ["KateriTekakwitha", "StHildegardBingen"]:
            if "Weekday" in event_key or "Sunday" in event_key:
                display_event_key = ""
            if "Weekday" in name or "Sunday" in name or "Week" in name:
                display_name = ""

        output_line = (
            f"{date:<11} {entry_data['season']:<13} {entry_data['day_of_week']:<15} "
            f"{entry_data['week_of_season']:<17} {display_event_key:<30} {display_name:<40} {hdo_column:<5}"
        )
        print(output_line)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch and display liturgical calendar data.")
    parser.add_argument("--raw", action="store_true", help="Display raw JSON data.")
    args = parser.parse_args()

    current_year = datetime.now().year
    for year_offset in range(3):
        year_to_fetch = current_year + year_offset
        calendar_data = get_liturgical_calendar(year_to_fetch)
        if args.raw:
            print_raw_calendar_data(calendar_data, year_to_fetch)
        else:
            print_formatted_calendar_data(calendar_data, year_to_fetch)
