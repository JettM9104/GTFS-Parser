# GTFS Parser — index.html Features

## Map
- Interactive Leaflet map (OpenStreetMap tiles) as the main view, centered on the transit region by default.
- Displays trip shapes as colored polylines with directional arrow markers along the route.
- Displays stop markers for a loaded trip, with popups showing stop ID and scheduled time.
- Live vehicle marker (pulsing dot) that tracks a trip's real-time GPS position, with a popup showing vehicle/route/status.
- User location marker ("Show my location on map" / device geolocation).
- "Pick a point on the map" mode — click the map to drop a pin and auto-search nearest stops at that location.
- Nearest-stop markers (red circles) shown after a nearby-stop search, with popups showing distance.
- Multi-stop search markers (purple circles) for stops sharing the same name.
- Zoom controls (desktop) / hidden on mobile in favor of touch gestures.

## Stop lookup
- Search stops by name (autocomplete/typeahead dropdown via `/api/searchstop`).
- Load a stop directly by Stop ID.
- Date picker (with "Today" shortcut) to control which service day's departures are shown.
- Stop detail header: name, stop code, stop ID, lat/lon.
- Departures grid for the selected stop, each card showing route, color-coded dot, time, headsign, trip ID.
- Departure view cycling: "Next 3 hours" → "Next 4" → "Show all" (button toggles view + count/label).
- Auto-refreshing departures (polls every 15s) without flicker (only rebuilds cards when the trip set changes).

## Multi-stop ("combined") search
- Search for all stops sharing an exact name (e.g. an intersection with stops on each corner).
- "Merge reversed cross-streets" toggle — treats "A / B" and "B / A" as the same intersection when matching.
- Combined departures grid across all matching stops, each card tagged with its originating stop ID.
- Auto-refreshing combined departures (polls every 15s).
- Map auto-fits bounds to show all matched stops.

## Trip lookup
- Load a trip by Trip ID.
- Displays trip details: trip ID, route (short + long name), stop count.
- Draws the trip's shape/polyline and stops on the map, auto-fitting the map bounds.
- Starts real-time vehicle polling for the loaded trip.

## Route lookup
- Search a route by Route ID + date, listing all trips for that route/day.
- Paginated trip list (8 per page) with Previous/Next controls and page indicator.
- Click a trip card to load it (shape, stops, live vehicle).

## Real-time (GTFS-RT) features
- Live vehicle panel showing: vehicle ID, route, trip, direction, start time/date, status (in transit/stopped/incoming), stop sequence, next stop, lat/lon, bearing, speed (km/h), occupancy status, and GPS timestamp with "time ago".
- Status pulse indicator (green = live, red = error/stale).
- Per-departure real-time delay badges (on time / early / delayed / live-only) computed from GTFS-RT trip updates, cached per stop with a staleness window so badges don't flicker when a poll briefly fails.

## Nearby stops ("Discover")
- Find nearest stops by manually entered latitude/longitude.
- "Use my location" (browser geolocation) to auto-fill coordinates and search.
- List of nearest stops with distance in km; clicking one loads that stop.
- Map markers for all nearby results with distance popups, auto-fit to bounds.

## Theming
- Light/dark theme toggle, persisted to localStorage, and defaults to the OS `prefers-color-scheme` on first load.
- Theme applied before first paint (inline script) to avoid flash of wrong theme.
- Respects `prefers-reduced-motion` (disables animations/transitions).

## Responsive layout
- **Desktop (>640px):** collapsible full-height sidebar with tabs (Stops / Trips & routes / Discover); collapse state and last-active tab persisted to localStorage; sidebar collapse/expand button with animated icon.
- **Mobile (≤640px):** draggable bottom sheet (swipe or tap handle to expand/collapse) with its own tab bar (Stop / Trip / Route / Nearby), synced with the desktop sidebar tabs; floating map control buttons (pick-location, my-location) positioned above the sheet.

## Misc UX
- Toast notifications for errors/empty states (e.g. "Enter both latitude and longitude first").
- Keyboard shortcuts: Enter key submits Trip ID / Stop ID / Route ID inputs.
- Safe-area inset handling for notched mobile devices.
