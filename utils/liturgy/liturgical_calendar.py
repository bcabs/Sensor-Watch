import requests
import json
from datetime import datetime, date
import os
import argparse
import re

def get_season_and_week(summary, date_obj):
    summary_lower = summary.lower()
    
    season = "Ordinary Time"
    week = 1
    
    # Simple keyword matching for Season
    if "advent" in summary_lower:
        season = "Advent"
    elif "lent" in summary_lower or "ash wednesday" in summary_lower:
        season = "Lent"
    elif "easter" in summary_lower or "pascha" in summary_lower or "resurrection" in summary_lower:
        season = "Easter"
    elif "christmas" in summary_lower or "nativity" in summary_lower:
        season = "Christmas"
    elif "epiphany" in summary_lower:
        season = "Christmas" # Usually considered part of Christmas season
    
    # Try to extract week number
    match = re.search(r'(\d+)(?:st|nd|rd|th)', summary_lower)
    if match:
        week = int(match.group(1))
    else:
        words = {
            "first": 1, "second": 2, "third": 3, "fourth": 4, "fifth": 5,
            "sixth": 6, "seventh": 7, "eighth": 8, "ninth": 9, "tenth": 10,
            "eleventh": 11, "twelfth": 12, "thirteenth": 13, "fourteenth": 14,
            "fifteenth": 15, "sixteenth": 16, "seventeenth": 17, "eighteenth": 18,
            "nineteenth": 19, "twentieth": 20, "twenty-first": 21, "twenty-second": 22,
            "twenty-third": 23, "twenty-fourth": 24, "twenty-fifth": 25, "twenty-sixth": 26,
            "twenty-seventh": 27
        }
        for word, num in words.items():
            if word in summary_lower:
                week = num
                break
                
    return season, week

def parse_traditional_data(data):
    calendar_data = {}
    
    for item in data:
        date_str = item.get("date")
        raw_summary = item.get("summary", "")
        
        # 1. Clean up summary (remove emoji, brackets, roman numerals)
        if raw_summary.startswith("b'") and raw_summary.endswith("'"):
            raw_summary = raw_summary[2:-1]

        summary = raw_summary
        if "[" in summary and "]" in summary:
            parts = summary.split("[", 1)
            after_bracket = parts[1]
            roman_parts = after_bracket.split("]", 1)
            summary = roman_parts[1].strip()
            
        clean_summary = summary.encode('ascii', 'ignore').decode('ascii').strip()
        summary_lower = clean_summary.lower()

        # 2. Filtering Logic
        add_event = True
        if "feria" in summary_lower or "ember" in summary_lower or "octave" in summary_lower:
            add_event = False
            
        days_of_week = ["monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"]
        allowed_exceptions = [
            "ash wednesday", "good friday", "holy saturday", "palm sunday", 
            "easter sunday", "pentecost sunday", "trinity sunday", "low sunday",
            "rogations", "septuagesima", "sexagesima", "quinquagesima",
            "sunday of advent", "sunday of lent", "sunday after easter", "sunday after pentecost", "sunday after epiphany"
        ]
        
        has_day_name = any(day in summary_lower for day in days_of_week)
        is_exception = any(exc in summary_lower for exc in allowed_exceptions)
        
        # Drop generic "Thursday in..." entries unless they are special exceptions
        if has_day_name and not is_exception:
            add_event = False

        # 3. Determine Season and Week
        d_obj = datetime.strptime(date_str, "%Y-%m-%d")
        json_season = item.get('season', 'Unknown')
        
        # Normalize Season Names
        if json_season == "Time after Epiphany": json_season = "After Epiphany"
        if json_season == "Time after Pentecost": json_season = "After Pentecost"
        
        season = json_season
        # If Unknown, try to guess (shouldn't happen with good scraping)
        if season == "Unknown":
             season, _ = get_season_and_week(clean_summary, d_obj)
             
        # Week number logic (still needs get_season_and_week or better scraping)
        _, week = get_season_and_week(clean_summary, d_obj)
        
        # 4. Fasting Logic
        is_fast = False
        weekday = d_obj.weekday() # 0=Mon, 6=Sun
        
        # All Fridays are fast days (abstinence)
        if weekday == 4: 
            is_fast = True
            
        # Lent: Fasting on all weekdays (Mon-Sat)
        if season == "Lent" and weekday != 6:
            is_fast = True
            
        # Ember Days (Wed, Fri, Sat)
        if "ember" in summary_lower:
             is_fast = True
             
        # 5. Holy Day of Obligation Logic (Simplified)
        is_hdo = False
        if weekday == 6: # Sunday
            is_hdo = True
        # Add specific HDOs if needed (Christmas, Ascension, etc.)
        if "christmas" in summary_lower and "day" in summary_lower and "octave" not in summary_lower:
             is_hdo = True
        if "ascension" in summary_lower:
             is_hdo = True
        if "assumption" in summary_lower:
             is_hdo = True
        if "all saints" in summary_lower:
             is_hdo = True
        if "immaculate conception" in summary_lower:
             is_hdo = True

        # 6. Create Event List
        events = []
        if add_event:
            # Create Event Key (CamelCase)
            event_key = "".join(x for x in clean_summary.title() if x.isalnum())
            events.append((event_key, clean_summary))
        
        # Store in calendar_data (overwriting if multiple entries for same day, usually fine as we want the main one)
        # Or check if exists?
        if date_str not in calendar_data or add_event: # Prioritize entries with events
            calendar_data[date_str] = {
                "season": season,
                "day_of_week": d_obj.strftime("%A"),
                "week_of_season": week,
                "events": events,
                "holy_day_of_obligation": is_hdo,
                "fast": is_fast
            }
            
    return calendar_data

def parse_liturgical_data(data):
        
    suppressed_events = data['metadata']['suppressed_events']
    data = data['litcal']

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

    # Ember days logic state
    active_ember_searches = []
    last_date_processed = None
    is_ember_day_calculated = False

    for item in data:
        date_str = item.get("date", "No date provided").split("T")[0]
        if date_str == "No date provided":
            continue
        
        event_key = item.get("event_key", "N/A")

        # Skip vigil events and SatMem entries
        if "vigil" in event_key or "SatMem" in event_key:
            continue
        
        # --- Start Fast Logic ---
        is_new_day = (date_str != last_date_processed)
        
        if is_new_day:
            is_ember_day_calculated = False
            day_of_week = item.get("day_of_the_week_long", "")
            
            # Check for Ember Day Matches (Wed/Sat after Trigger)
            # Iterate a copy of active searches
            for search in active_ember_searches[:]:
                if day_of_week == "Wednesday" and not search['wed_found']:
                    search['wed_found'] = True
                    is_ember_day_calculated = True
                elif day_of_week == "Saturday" and not search['sat_found']:
                    search['sat_found'] = True
                    is_ember_day_calculated = True
                
                if search['wed_found'] and search['sat_found']:
                    active_ember_searches.remove(search)
            
            # Check for Fixed Date Triggers (Sept 14, Dec 13) - Only check once per day
            if date_str.endswith("-09-14") or date_str.endswith("-12-13"):
                active_ember_searches.append({'wed_found': False, 'sat_found': False})

            last_date_processed = date_str

        # Check for Event Triggers (Ash Wed, Pentecost) - Check every item
        if event_key == "AshWednesday" or event_key == "PentecostSunday":
             active_ember_searches.append({'wed_found': False, 'sat_found': False})
        
        season = item.get("liturgical_season_lcl", "N/A")
        
        # Override Easter Sunday to be part of the Triduum per user request
        if event_key == "Easter":
            season = "Easter Triduum"
            
        is_fast = False
        if item.get("day_of_the_week_long", "") == "Friday":
            is_fast = True
        if "Lent" in season:
            is_fast = True
        if is_ember_day_calculated:
            is_fast = True
        # --- End Fast Logic ---

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
                
                # Special adjustment per user request: Increase week by one when restarting Ordinary Time after Easter
                if current_season and "Easter" in current_season:
                    season_weeks[season] += 1
                    
            if season not in season_weeks:
                season_weeks[season] = 1
            current_season = season

        elif item.get("day_of_the_week_long", "") == "Sunday" and is_new_day:
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
                "holy_day_of_obligation": is_holy_day,
                "fast": is_fast
            }
        else:
            calendar_data[date_str]["events"].append((event_key, name))
            
            if is_holy_day:
                calendar_data[date_str]["holy_day_of_obligation"] = True
                
            if is_fast:
                calendar_data[date_str]["fast"] = True
            
            # Update week of season if current entry has a more recent Sunday
            # and thus a more accurate week number for that season.
            calendar_data[date_str]["week_of_season"] = max(
                calendar_data[date_str].get("week_of_season", 1), 
                season_weeks.get(current_season, 1)
            )

    for suppressed in suppressed_events:
        date_str = suppressed.get("date").split("T")[0]
        s_key = suppressed.get("event_key")
        s_name = suppressed.get("name", s_key)

        if date_str in calendar_data: 
            calendar_data[date_str]["events"].append((s_key, s_name))



    return calendar_data

def get_liturgical_calendar(year):
    """
    Fetches the liturgical calendar for the Diocese of Boston for the specified year,
    parses the data, and stores it in a dictionary.
    Caches the response to a file to avoid repeated requests.
    """
    url = f"https://litcal.johnromanodorazio.com/api/dev/calendar/{year}?epiphany=SUNDAY_JAN2_JAN8&ascension=THURSDAY&corpus_christi=SUNDAY&locale=en"

    # url = f"https://litcal.johnromanodorazio.com:443/api/v5/calendar/diocese/boston_us/{year}"
    cache_filename = f"litcal_{year}.json"
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
            return parse_liturgical_data(data)
        else:
            print("Could not find 'litcal' key or it's not a list in the response.")

    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")
    except json.JSONDecodeError:
        print("Error parsing JSON response.")
        
    return {}

def should_filter(event_key): 
    # Conditional printing logic
    for start_key in ['Easter', "Lent", "DayAfterEpiphany", "Christmas", "Advent"]:
        if (event_key.startswith(start_key) and event_key[-1].isdigit()):
            return True
            
    for filter_key in ["DayAfterEpiphany", "AfterAshWednesday", "Sunday", "Weekday", "HolyWeek", "OctaveEaster", "IndependenceDay", "ThanksgivingDay"]:
        if filter_key in event_key:
            return True

def get_display_event(events):
    event_key, name = None, None

    # Specific overrides
    for key, n in events:
        if key in ["KateriTekakwitha", "StHildegardBingen"]:
            event_key, name = key, n
            break
    
    # If no special event, use default logic
    for key, name in events:
        if not should_filter(key):
            return key, name

    return "", ""

def generate_c_struct_output(calendar_data):
    """
    Generates C code for the special_days array.
    """
    # Create a dictionary to hold the C struct data, indexed by days since start_date
    c_struct_data = {}
    unique_strings = set()

    season_map = {
        "Advent": "ADVENT",
        "Christmas": "CHRISTMAS",
        "Ordinary Time": "ORDINARY_TIME",
        "Lent": "LENT",
        "Easter Triduum": "EASTER_TRIDUUM",
        "Easter": "EASTER",
        "Septuagesima": "SEPTUAGESIMA",
        "Passiontide": "PASSIONTIDE",
        "After Epiphany": "AFTER_EPIPHANY",
        "After Pentecost": "AFTER_PENTECOST",
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
        is_fast = "true" if entry.get('fast', False) else "false"
        
        event_key, _ = get_display_event(entry['events'])
        
        if event_key:
            unique_strings.add(event_key)
            text_ref = f"text_{event_key}"
        else:
            text_ref = "NULL"

        c_struct_data[days_since] = f"{{ {season_enum}, {week}, {hdo}, {is_fast}, {text_ref} }}"

    print('#include "special_day_face.h"')
    print(f"#define SPECIAL_DAYS_START_DATE_YEAR {START_DATE_YEAR}")
    print(f"#define SPECIAL_DAYS_START_DATE_MONTH {START_DATE_MONTH}")
    print(f"#define SPECIAL_DAYS_START_DATE_DAY {START_DATE_DAY}")
    print("")
    
    for s in sorted(list(unique_strings)):
        safe_s = s.replace('"', '\\"')
        print(f"static const char text_{s}[] = \"{safe_s}\";")
        
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
            print(f"    /* {i:03} {current_date_str} */ {{ UNKNOWN, 0, false, false, NULL }},")
            
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
    print(f"{'Date':<11} {'Season':<13} {'Day of Week':<15} {'Week of Season':<17} {'Event Key':<30} {'Name':<40} {'HDO':<5} {'Fast':<5}")
    print(f"{'-'*11:<11} {'-'*13:<13} {'-'*15:<15} {'-'*17:<17} {'-'*30:<30} {'-'*40:<40} {'-'*5:<5} {'-'*5:<5}")

    for date, entry_data in sorted(calendar_data.items()):
        hdo_column = "HDO" if entry_data["holy_day_of_obligation"] else ""
        fast_column = "FAST" if entry_data.get("fast", False) else ""
        
        display_event_key, display_name = get_display_event(entry_data["events"])
        display_name = display_name[:40]

        output_line = (
            f"{date:<11} {entry_data['season']:<13} {entry_data['day_of_week']:<15} "
            f"{entry_data['week_of_season']:<17} {display_event_key:<30} {display_name:<40} {hdo_column:<5} {fast_column:<5}"
        )
        print(output_line)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch and display liturgical calendar data.")
    parser.add_argument("--raw", action="store_true", help="Display raw JSON data.")
    parser.add_argument("--c-struct", action="store_true", help="Generate C struct array.")
    parser.add_argument("--traditional", action="store_true", help="Use traditional calendar data from utils/liturgy/traditional/out.json")
    args = parser.parse_args()

    full_calendar_data = {}
    
    if args.traditional:
        try:
            with open("traditional/out.json", 'r') as f:
                trad_data = json.load(f)
                full_calendar_data = parse_traditional_data(trad_data)
        except FileNotFoundError:
            print("Error: utils/liturgy/traditional/out.json not found.")
            exit(1)
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
    elif args.traditional and args.raw: # Handle raw print for traditional
         print(json.dumps(full_calendar_data, indent=2))
    elif args.traditional:
         print_formatted_calendar_data(full_calendar_data, "Traditional")
