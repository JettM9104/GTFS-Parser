import csv
import matplotlib.pyplot as plt

INPUT_FILE = "data.csv"
OUTPUT_FILE = "delay_vs_unix_timestamp.png"
SAMPLE_EVERY = 3  # plot every Nth row; data.csv is ~600k rows
TZ_OFFSET_HOURS = -4  # Eastern time (EDT) relative to UTC timestamps in data.csv

MARK_COLOR = "#4C72B0"
GRID_COLOR = "#D9D9D9"
TEXT_COLOR = "#333333"


def load_sampled(path, every):
    xs, ys = [], []
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for i, row in enumerate(reader):
            if i % every != 0:
                continue
            xs.append((int(row["unix_timestamp"]) + TZ_OFFSET_HOURS * 3600) % 604800)
            ys.append(int(row["delay"]))
    return xs, ys


def main():
    xs, ys = load_sampled(INPUT_FILE, SAMPLE_EVERY)

    fig, ax = plt.subplots(figsize=(12, 6), dpi=150)
    ax.scatter(xs, ys, s=4, c=MARK_COLOR, alpha=0.15, linewidths=0, edgecolors="none")

    ax.set_xlim(0, 604800)
    ax.set_xlabel("Seconds since Monday 00:00 ET", color=TEXT_COLOR)
    ax.set_ylabel("Delay (s)", color=TEXT_COLOR)
    ax.set_title(f"Delay vs. time of week ({len(xs):,} points, 1 of every {SAMPLE_EVERY} rows)", color=TEXT_COLOR)

    # tick every 6 hours, labeled with day + time; day boundaries in bold
    days = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
    hour_step = 6
    ticks = [i * hour_step * 3600 for i in range(int(7 * 24 / hour_step) + 1)]
    labels = []
    for t in ticks:
        day = days[(t // 86400) % 7]
        hour = (t % 86400) // 3600
        labels.append(f"{day}\n{hour:02d}:00" if hour == 0 else f"{hour:02d}:00")
    ax.set_xticks(ticks)
    ax.set_xticklabels(labels, fontsize=8)

    ax.grid(True, color=GRID_COLOR, linewidth=0.6)
    ax.set_axisbelow(True)
    for spine in ("top", "right"):
        ax.spines[spine].set_visible(False)
    for spine in ("left", "bottom"):
        ax.spines[spine].set_color(GRID_COLOR)

    ax.tick_params(colors=TEXT_COLOR)

    fig.tight_layout()
    fig.savefig(OUTPUT_FILE)
    print(f"Wrote {OUTPUT_FILE} ({len(xs):,} points)")


if __name__ == "__main__":
    main()
