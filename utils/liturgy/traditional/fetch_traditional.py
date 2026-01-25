import requests
import json
import argparse
import sys
from html.parser import HTMLParser
from datetime import datetime, date

class GCatholicParser(HTMLParser):
    def __init__(self, year):
        super().__init__()
        self.year = year
        self.events = []
        
        self.current_season = "Unknown"
        self.current_date = None
        self.current_color = None
        self.current_text = []
        
        self.in_data_row = False
        self.in_mass_col = False
        self.found_mass_for_day = False
        self.capture_text = False
        
        self.color_map = {
            'feastw': 'white',
            'feastr': 'red',
            'feastg': 'green',
            'feastv': 'violet',
            'feastb': 'black',
            'feastp': 'rose'
        }

    def handle_starttag(self, tag, attrs):
        attrs_dict = dict(attrs)
        
        # Season detection
        if tag == 'div' and attrs_dict.get('class') == 'season':
            self.capture_text = True
            self.current_text = []
            return

        # Date Row detection
        if tag == 'tr':
            row_id = attrs_dict.get('id')
            if row_id and row_id.isdigit() and len(row_id) == 4:
                # MM DD
                month = int(row_id[:2])
                day = int(row_id[2:])
                self.current_date = date(self.year, month, day)
                self.in_data_row = True # Keep this for tr tracking
                return

        # Mass/Commemoration detection
        # We capture ANY p.indent as a potential event for the current date.
        if tag == 'p' and 'indent' in attrs_dict.get('class', '') and self.current_date:
            self.in_mass_col = True
            self.capture_text = True
            self.current_text = []
            self.current_color = None # Reset color for this specific entry
            return

        # Color detection
        if self.in_mass_col and tag == 'span':
            cls = attrs_dict.get('class', '')
            for c in self.color_map:
                if c in cls:
                    self.current_color = self.color_map[c]

    def handle_endtag(self, tag):
        if tag == 'div' and self.capture_text and not self.in_data_row:
            # End of season div
            self.current_season = "".join(self.current_text).strip()
            self.capture_text = False
            self.current_text = []
        
        if tag == 'p' and self.in_mass_col:
            # End of mass/commem text
            summary = "".join(self.current_text).strip()
            if self.current_date and summary: # Ensure we have text
                self.events.append({
                    'date': self.current_date.isoformat(),
                    'summary': summary,
                    'season': self.current_season,
                    'color': self.current_color
                })
            self.in_mass_col = False
            self.capture_text = False
            self.current_text = []
            
        if tag == 'tr' and self.in_data_row:
            self.in_data_row = False

    def handle_data(self, data):
        if self.capture_text:
            self.current_text.append(data)

def fetch_traditional_calendar(year):
    url = f"https://gcatholic.org/calendar/{year}/Extraordinary-en"
    # User-Agent is often required for scraping
    headers = {'User-Agent': 'Mozilla/5.0 (compatible; LiturgyScraper/1.0)'}
    
    try:
        response = requests.get(url, headers=headers)
        response.raise_for_status()
        
        parser = GCatholicParser(year)
        parser.feed(response.text)
        return parser.events
        
    except requests.RequestException as e:
        print(f"Error fetching data: {e}", file=sys.stderr)
        return []

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fetch Traditional Catholic Calendar from gcatholic.org")
    parser.add_argument("--year", type=int, default=datetime.now().year, help="Year to fetch")
    parser.add_argument("--json", action="store_true", help="Output as JSON (default)")
    
    args = parser.parse_args()
    
    # We always output JSON for the other script to consume
    events = fetch_traditional_calendar(args.year)
    print(json.dumps(events, indent=2))
