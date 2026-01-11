import requests
import re
from datetime import datetime, date, timedelta
import json
import argparse
import sys
import icalendar

def date_handler(obj):
    if hasattr(obj, 'isoformat'):
        return obj.isoformat()
    else:
        raise TypeError

def fetch_ics_data(url):
    response = requests.get(url)
    response.raise_for_status()
    return response.text

def get_season_and_week(summary, date_obj):
    summary_lower = summary.lower()
    
    season = "ORDINARY_TIME"
    week = 1
    
    # Simple keyword matching for Season
    if "advent" in summary_lower:
        season = "ADVENT"
    elif "lent" in summary_lower or "ash wednesday" in summary_lower:
        season = "LENT"
    elif "easter" in summary_lower or "pascha" in summary_lower or "resurrection" in summary_lower:
        season = "EASTER"
    elif "christmas" in summary_lower or "nativity" in summary_lower:
        season = "CHRISTMAS"
    elif "epiphany" in summary_lower:
        season = "CHRISTMAS" # Usually considered part of Christmas season
    
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

def sanitize_text(text):
    # Keep spaces, remove non-alphanumeric (except spaces)
    # This provides a more readable string for the watch face display
    clean = re.sub(r'[^a-zA-Z0-9 ]', '', text)
    return clean.strip()

def process_event_summary(raw_summary):
    summary = str(raw_summary)
    # Handle the b'...' wrapper if present
    if summary.startswith("b'") and summary.endswith("'"):
        summary = summary[2:-1]

    # Parsing logic: everything before first '[' is trimmed. 
    # Roman numeral is between '[' and ']'.
    # Rest is the event name.
    if "[" in summary and "]" in summary:
        parts = summary.split("[", 1)
        # Trim everything before '['
        after_bracket = parts[1]
        roman_parts = after_bracket.split("]", 1)
        # roman_col = roman_parts[0].strip() # We don't need to return this for now unless needed
        summary = roman_parts[1].strip()
    
    summary_lower = summary.lower()
    
    # Filtering Logic
    if "feria" in summary_lower:
        return None
        
    days_of_week = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
    allowed_exceptions = [
        "Ash Wednesday", "Good Friday", "Holy Saturday", "Palm Sunday", 
        "Easter Sunday", "Pentecost Sunday", "Trinity Sunday", "Low Sunday"
    ]
    
    has_day_name = any(day.lower() in summary_lower for day in days_of_week)
    is_exception = any(exc.lower() in summary_lower for exc in allowed_exceptions)
    
    if has_day_name and not is_exception:
        return None
        
    return summary

def generate_c_struct(events):
    # Sort events by date
    events.sort(key=lambda x: x['date'])
    
    # Determine start date
    # The user asked for 2026 calendar, so start is Jan 1, 2026.
    start_date = date(2026, 1, 1)
    
    # Map by days since start
    day_map = {}
    
    for e in events:
        days_since = (e['date'] - start_date).days
        
        # Skip days before start date
        if days_since < 0:
            continue
            
        summary = process_event_summary(e['summary'])
        if not summary:
            continue

        season, week = get_season_and_week(summary, e['date'])
        
        # HDO logic (Simplified: Sundays are HDOs)
        hdo = "false"
        if e['date'].weekday() == 6: # Sunday
            hdo = "true"
        
        # Special HDOs (Fixed dates or specific names)
        # Add more logic here if needed for exact Traditional HDOs
        
        text = sanitize_text(summary)
        
        # Quote the text
        text_str = f'"{text}"'
        
        # Note: If multiple valid events exist for a day, this overwrites, 
        # keeping the last one (often the most specific in many calendars).
        day_map[days_since] = f"{{ {season}, {week}, {hdo}, {text_str} }}"

    # Output
    print('#include "special_day_face.h"')
    print(f"#define SPECIAL_DAYS_START_DATE_YEAR {start_date.year}")
    print(f"#define SPECIAL_DAYS_START_DATE_MONTH {start_date.month}")
    print(f"#define SPECIAL_DAYS_START_DATE_DAY {start_date.day}")
    print("")
    print("static const SpecialDay special_days[] = {")
    
    max_day = max(day_map.keys()) if day_map else 365
    
    for i in range(max_day + 1):
        current_date = start_date + timedelta(days=i)
        date_str = current_date.strftime("%Y-%m-%d")
        
        if i in day_map:
            print(f"    /* {i:03} {date_str} */ {day_map[i]},")
        else:
            print(f"    /* {i:03} {date_str} */ {{ UNKNOWN, 0, false, NULL }},")
    print("};")

def generate_list_output(events):
    # Sort events by date
    events.sort(key=lambda x: x['date'])
    
    for e in events:
        # Re-implement parsing just to extract Roman column if needed, 
        # or just use the helper and lose the Roman column in print?
        # User asked for Roman column in the list output.
        # Helper 'process_event_summary' returns just the summary.
        # Let's keep the logic inline here or modify helper to return both?
        # For simplicity/speed, I'll modify the helper to return tuple?
        # Or just re-do the specific list formatting here since it's UI specific.
        
        # Actually, let's use the helper for filtering, but we need the Roman numeral for the list.
        # Let's dup the parsing logic slightly or accept that 'process_event_summary' is for the text content.
        
        summary = str(e['summary'])
        if summary.startswith("b'") and summary.endswith("'"):
            summary = summary[2:-1]

        roman_col = ""
        if "[" in summary and "]" in summary:
            parts = summary.split("[", 1)
            after_bracket = parts[1]
            roman_parts = after_bracket.split("]", 1)
            roman_col = roman_parts[0].strip()
            summary = roman_parts[1].strip()
            
        # Check filtering on the CLEANED summary
        filtered_summary = process_event_summary(e['summary'])
        if not filtered_summary: 
            continue
            
        # Use the filtered_summary which is already cleaned/filtered
        d_str = e['date'].isoformat()
        print(f"{d_str:<12} {roman_col:<6} {filtered_summary}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch Traditional Catholic Calendar.")
    parser.add_argument("--json", action="store_true", help="Output as JSON.")
    parser.add_argument("--list", action="store_true", help="Output as a simple date-event list.")
    args = parser.parse_args()

    url = "https://gcatholic.org/calendar/ics/2026-en-Extraordinary.ics?v=3"
    try:
        ics_text = fetch_ics_data(url)
        
        cal = icalendar.Calendar.from_ical(ics_text)
        events = []
        
        for component in cal.walk():
            if component.name == "VEVENT":
                summary = component.get('summary')
                dtstart = component.get('dtstart')
                
                if summary and dtstart:
                    dt = dtstart.dt
                    # Convert to date object if it's a datetime
                    if isinstance(dt, datetime):
                        d = dt.date()
                    else:
                        d = dt
                        
                    events.append({
                        'summary': str(summary),
                        'date': d
                    })

        if args.json:
            print(json.dumps(events, default=date_handler, indent=2))
        elif args.list:
            generate_list_output(events)
        else:
            generate_c_struct(events)
            
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
