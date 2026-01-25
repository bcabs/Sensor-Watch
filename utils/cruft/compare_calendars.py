import re
import os
from datetime import date, timedelta

def parse_header_file(filename):
    if not os.path.exists(filename):
        print(f"Error: {filename} not found.")
        return {}

    with open(filename, 'r') as f:
        content = f.read()

    # Extract Start Date
    try:
        year = int(re.search(r'#define SPECIAL_DAYS_START_DATE_YEAR (\d+)', content).group(1))
        month = int(re.search(r'#define SPECIAL_DAYS_START_DATE_MONTH (\d+)', content).group(1))
        day = int(re.search(r'#define SPECIAL_DAYS_START_DATE_DAY (\d+)', content).group(1))
        start_date = date(year, month, day)
    except AttributeError:
        print(f"Error parsing start date in {filename}")
        return {}

    # Regex for array entries
    # Matches: /* 000 2026-01-24 */ { SEASON, WEEK, HDO, FAST, TEXT },
    # TEXT might be text_Something or NULL
    entry_pattern = re.compile(r'/\* (\d+) (\d{4}-\d{2}-\d{2}) \*/ \{ ([^,]+), (\d+), (true|false), (true|false), ([^ }]+) \}')
    
    data = {}
    
    for match in entry_pattern.finditer(content):
        # idx = match.group(1)
        date_str = match.group(2)
        season = match.group(3)
        week = match.group(4)
        hdo = match.group(5)
        fast = match.group(6)
        text = match.group(7)
        
        # Clean text (remove text_ prefix if desired, but keep for clarity)
        if text.startswith("text_"):
            text = text[5:] # Remove text_ prefix for compactness
        
        data[date_str] = {
            "season": season,
            "week": week,
            "hdo": "HDO" if hdo == "true" else "",
            "fast": "FAST" if fast == "true" else "",
            "text": text if text != "NULL" else ""
        }
        
    return data

def main():
    normal_file = "../../watch-faces/clock/special_days.h"
    trad_file = "../../watch-faces/clock/special_days_alt.h"
    output_file = "calendar_comparison.txt"
    
    print(f"Parsing {normal_file}...")
    normal_data = parse_header_file(normal_file)
    
    print(f"Parsing {trad_file}...")
    trad_data = parse_header_file(trad_file)
    
    # Collect all unique dates
    all_dates = sorted(list(set(normal_data.keys()) | set(trad_data.keys())))
    
    if not all_dates:
        print("No data found.")
        return

    print(f"Writing comparison to {output_file}...")
    
    with open(output_file, 'w') as f:
        # Header
        header = f"{ 'Date':<12} | {'Norm Season':<15} {'Wk':<2} {'HDO':<3} {'Fst':<4} {'Normal Text':<30} | {'Trad Season':<15} {'Wk':<2} {'HDO':<3} {'Fst':<4} {'Trad Text':<30}"
        f.write(header + "\n")
        f.write("-" * len(header) + "\n")
        
        for d in all_dates:
            n = normal_data.get(d, {"season": "-", "week": "-", "hdo": "", "fast": "", "text": "-"})
            t = trad_data.get(d, {"season": "-", "week": "-", "hdo": "", "fast": "", "text": "-"})
            
            # Formatting
            # Truncate text if too long
            n_text = (n['text'][:29] + '..') if len(n['text']) > 29 else n['text']
            t_text = (t['text'][:29] + '..') if len(t['text']) > 29 else t['text']
            
            line = (
                f"{d:<12} | "
                f"{n['season']:<15} {n['week']:<2} {n['hdo']:<3} {n['fast']:<4} {n_text:<30} | "
                f"{t['season']:<15} {t['week']:<2} {t['hdo']:<3} {t['fast']:<4} {t_text:<30}"
            )
            f.write(line + "\n")
            
    print("Done.")

if __name__ == "__main__":
    main()
