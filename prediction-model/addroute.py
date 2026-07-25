import csv

INPUT_FILE = "data.csv"
OUTPUT_FILE = "data.csv"  # overwrite in place; change if you want a separate output file

TRIPS_FILE = "/Users/jettmu/Documents/VSCode/GTFS Parser/static-gtfs/data/yrt_archive/trips.txt"

def load_trip_to_route(trips_file):
    trip_to_route = {}
    with open(trips_file, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            trip_to_route[row["trip_id"]] = row["route_id"]
    return trip_to_route

def main():
    trip_to_route = load_trip_to_route(TRIPS_FILE)

    with open(INPUT_FILE, newline="") as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames + ["route_id"]
        rows = []
        for row in reader:
            row["route_id"] = trip_to_route.get(row["trip_id"], "")
            rows.append(row)

    with open(OUTPUT_FILE, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    print(f"Added route_id column to {len(rows)} rows.")

if __name__ == "__main__":
    main()
