# A GTFS Parser
## What is This?
This code is programmed in **C++** and is one big file in `main.cpp` and is used to parse general *GTFS Static Data*. 

## How do I use it?
Create a folder in the enviroment labelled `data` or wherever you want and change the paths inside the code. I didn't include the GTFS data that I used for testing as it is way too big to fit onto Github.

## If you are in pain reading this code
If you are in pain reading this code just copy it and improve it. I don't care about licencing; Just do whatever you want with it. Go wild. (read [`licence.md`](licence.md))

## What libraries do I need?
Nothing. Just a C++ compiler. All of the libraries used are built in like `<string>`, `<vector>`, and `<map>`.

## What are the other files from the root?
There are some other random files that I use to create and test functions, structs, and classes from. You might see some snippets from those files into the `main.cpp` code but all you need to run it is the `main.cpp` code.

## What are my goals for the future?
Just trying to implement *Live GTFS* (also known as GTFS-RT) Data including schedules and bus locations. I already created a .bd file converter in ./convertLIVEgtfs/convert.cpp

# About this Repo

## File Tree System Thing

```
.
├── convertLIVEgtfs
│   └── convert.cpp
├── data
│   ├── (your data goes here)
│   └── tes-archive-1
│       └── stop_ordering.txt
├── .gitignore
├── externalFNS.cpp
├── licence.md
├── main.cpp
├── readme.md
├── testing.cpp
└── todo.txt
```
#### (Thanks to `tree.nathanfriend` for the tree diagram)

# How do I use the GTFS-RT Converter?
All I gave was the code. It's a big pain for you to get ready, but if you have too much spare time, go for it.

**Step 1: **  Download the  [`gtfs-realtime.proto` file](https://github.com/google/transit/blob/master/gtfs-realtime/proto/gtfs-realtime.proto).

