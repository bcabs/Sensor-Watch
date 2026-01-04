import requests
import re
from datetime import datetime, date, timedelta
import json
import argparse
import sys

def date_handler(obj):
    if hasattr(obj, 'isoformat'):
        return obj.isoformat()
    else:
        raise TypeError

def fetch_ics_data(url):
    response = requests.get(url)
    response.raise_for_status()
    return response.text

def unfold_lines(lines):
    """Unfold lines as per RFC 5545 (merging continuation lines)."""
    it = iter(lines)
    try:
        prev = next(it)
    except StopIteration:
        return

    for line in it:
        if not line:
            continue
        if line.startswith(' ') or line.startswith('\t'):
            prev += line[1:]
        else:
            yield prev
            prev = line
    yield prev

def unescape_text(text):
    """Unescape special characters in ICS text values."""
    if not text:
        return ""
    return text.replace('\\,', ',').replace('\\;', ';').replace('\\n', '\n').replace('\\N', '\n').replace('\\\\', '\\')

def parse_ics(ics_text):
    events = []
    # Split by lines and remove potential empty lines
    raw_lines = [l.rstrip('\r\n') for l in ics_text.splitlines() if l.strip()]
    
    current_event = {}
    in_event = False
    
    for line in unfold_lines(raw_lines):
        if line == "BEGIN:VEVENT":
            in_event = True
            current_event = {}
            continue
        elif line == "END:VEVENT":
            in_event = False
            if 'DTSTART' in current_event and 'SUMMARY' in current_event:
                # Post-process date
                dt_str = current_event['DTSTART']
                try:
                    if len(dt_str) == 8:
                        d = datetime.strptime(dt_str, "%Y%m%d").date()
                    elif 'T' in dt_str:
                         d = datetime.strptime(dt_str.split('T')[0], "%Y%m%d").date()
                    else:
                        continue # Skip invalid dates
                    
                    current_event['date'] = d
                    # Clean summary
                    current_event['SUMMARY'] = unescape_text(current_event.get('SUMMARY', ''))
                    events.append(current_event)
                except ValueError:
                    pass 
            continue
            
        if in_event:
            # Parse Key:Value
            if ":" in line:
                key_part, value = line.split(":", 1)
                
                # Handle parameters in key (e.g., DTSTART;VALUE=DATE)
                if ";" in key_part:
                    key = key_part.split(";")[0]
                    # We could parse params here if needed
                else:
                    key = key_part
                
                current_event[key] = value

    return events

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
            
        summary = e['SUMMARY']
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

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch Traditional Catholic Calendar.")
    parser.add_argument("--json", action="store_true", help="Output as JSON.")
    args = parser.parse_args()

    url = "https://gcatholic.org/calendar/ics/2026-en-Extraordinary.ics?v=3"
    try:
        ics_text = fetch_ics_data(url)
        events = parse_ics(ics_text)
        
        if args.json:
            print(json.dumps(events, default=date_handler, indent=2))
        else:
            generate_c_struct(events)
            
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
