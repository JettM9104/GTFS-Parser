import csv
from datetime import datetime, timezone, timedelta

INPUT_FILE = "data.csv"
OUTPUT_FILE = "data.csv"  # overwrite in place; change if you want a separate output file

def last_monday_0000z(now=None):
    now = now or datetime.now(timezone.utc)
    last_monday = (now - timedelta(days=now.weekday())).replace(
        hour=0, minute=0, second=0, microsecond=0
    )
    return int(last_monday.timestamp())

def to_unix(ts_str, cutoff):
    # Parses ISO 8601 timestamps like 2026-07-24T22:04:55+00:00
    dt = datetime.fromisoformat(ts_str)
    return int(dt.timestamp()) - cutoff

def main():
    cutoff = last_monday_0000z()

    with open(INPUT_FILE, newline="") as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames + ["unix_timestamp"]
        rows = []
        for row in reader:
            row["unix_timestamp"] = to_unix(row["timestamp"], cutoff)
            rows.append(row)

    with open(OUTPUT_FILE, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    print(f"Added unix_timestamp column to {len(rows)} rows.")

if __name__ == "__main__":
    main()