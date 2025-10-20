# A GTFS Parser
## What is GTFS?
**GTFS**, or *General Transit Feed Specification* (look at repository [`google/transit`](https://github.com/google/transit) and the reference [here](https://gtfs.org/documentation/schedule/reference/)) is the general (and only) worldwide standard for transit agency arrival, departure, and shape data (and more!). A *GTFS Set* is created by a transit agency and used by other software to tell when the bus/vehicle will arrive on time.

## What is This?
This code is programmed in **C++** and is one big file in `main.cpp` and is used to parse general *GTFS Static Data*. Look into the path `static-gtfs/main.cpp`.

## How do I use it?
Create a folder in the folder labelled `data` or wherever you want and change the paths inside the code. I didn't include the GTFS data that I used for testing as it is way too big to fit onto GitHub. 

## If you are in pain reading this code
If you are in pain reading this code just copy it and improve it. I don't care about licencing except for the 4 rules i have; Just do whatever you want with it. Go wild. (read [`licence.md`](licence.md))

## What libraries do I need?
Nothing. Just a C++ compiler. All of the libraries used are built in like `<string>`, `<vector>`, and `<unorderd_map>`. In the static converter, there is a time library and some compilers do not come with that. It is only used in the getCurrentTime function and you can delete it if you need (with the exception of the GTFS-RT converter, read more below on [How do I use the GTFS-RT Converter?](gtfs-rt/readme.md#How-do-I-use-the-GTFS-RT-Converter)

## What are the other files from the root?
There are some other random files that I use to create and test functions, structs, and classes from. You might see some snippets from those files into the `main.cpp` code but all you need to run it is the `main.cpp` code.

## What are my goals for the future?
Just trying to implement *Live GTFS* (also known as GTFS-RT) Data including schedules and bus locations. I already created a .bd file converter in ./convertLIVEgtfs/main.cpp

## File System

```
.
├── .github
│   └── workflows
├── .vscode
│   ├── launch.json
│   ├── settings.json
│   └── tasks.json
├── gtfs-rt
│   ├── convertLIVEgtfs
│   │   └── main.cpp
│   ├── jsonConverter.cpp
│   └── main.cpp
├── static-gtfs
│   ├── data
│   │   ├── tes_archive-1
│   │   └── (put your data here for the parser)
│   ├── testing
│   │   ├── externalFNS.cpp
│   │   └── testing.cpp
│   └── main.cpp
├── .gitignore
├── licence.md
├── README.md
├── todo.txt
└── version.txt
```
#### (Thanks to `tree.nathanfriend` for the tree diagram)
