# AUC Campus Lost and Found System

A desktop application built with C++ and Qt to help AUC students and staff report, browse, and reclaim lost or found items on campus. Version 2 adds real-time client/server networking, live matching between lost and found reports, and image uploads.

## Features
- Submit, browse, search, and update lost/found item reports
- Student and Gate 4 security dashboards with role-based access
- Report filtering by category, status, location, and date
- Feedback & ratings, internal messaging, statistics dashboard, reward offers
- Automatic matching engine that suggests likely lost/found pairs
- Photo upload on reports
- Real-time updates pushed to every connected client

## Architecture
+--------------------+
                |   LostFoundServer  |
                |  (Boost.Asio TCP)  |
                |  ReportManager     |
                |  UserManager       |
                +---------+----------+
                          |
             JSON over TCP (shared protocol)
                          |
    +---------------------+---------------------+
    |                                             |
The server is the single source of truth for all report and user data. Clients never modify data directly — every action goes through the server, which broadcasts the updated state to all connected clients so everyone's view stays in sync.

`MatchingEngine` is intentionally decoupled from both networking and the GUI: it takes report data in and returns potential matches out, so it can be built, run, and unit tested completely on its own.

## Building
```bash
mkdir build && cd build
cmake ..
make -j4
./CampusLostAndFound
```

## Running the tests
```bash
cmake .. -DBUILD_TESTS=ON
make LostFoundTests -j4
./tests/LostFoundTests
```

## Team
- Server core (Boost.Asio, protocol, broadcast) — Person 1
- Client networking + GUI wiring — Person 2
- Matching engine, image upload, tests, docs — Jana
