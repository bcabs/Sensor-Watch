import datetime
import sys

def generate_dates(start_date_str=None, end_date_str=None):
    """
    Generates and prints all dates from a given start date to a given end date.
    If no start date is given, it defaults to today.
    If no end date is given, it defaults to 10 years from the start date.
    """
    if start_date_str:
        try:
            start_date = datetime.datetime.strptime(start_date_str, "%Y-%m-%d").date()
        except ValueError:
            print("Error: Invalid start date format. Please use YYYY-MM-DD.", file=sys.stderr)
            sys.exit(1)
    else:
        start_date = datetime.date.today()

    if end_date_str:
        try:
            end_date = datetime.datetime.strptime(end_date_str, "%Y-%m-%d").date()
        except ValueError:
            print("Error: Invalid end date format. Please use YYYY-MM-DD.", file=sys.stderr)
            sys.exit(1)
    else:
        try:
            end_date = start_date.replace(year=start_date.year + 10)
        except ValueError:
            # Handle leap years (e.g., Feb 29)
            end_date = start_date.replace(year=start_date.year + 10, day=start_date.day - 1)

    current_date = start_date
    while current_date <= end_date:
        print(current_date.strftime("%Y-%m-%d"))
        current_date += datetime.timedelta(days=1)

if __name__ == "__main__":
    if len(sys.argv) > 2:
        generate_dates(sys.argv[1], sys.argv[2])
    elif len(sys.argv) > 1:
        generate_dates(start_date_str=sys.argv[1])
    else:
        generate_dates()