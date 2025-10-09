# A GTFS Parser
## What is This?
This code is programmed in **C++** and is one big file in `main.cpp` and is used to parse general *GTFS Static Data*. 

## How do I use it?
Create a folder in the folder labelled `data` or wherever you want and change the paths inside the code. I didn't include the GTFS data that I used for testing as it is way too big to fit onto GitHub. 

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
├── .github
│   └── workflows
│       └── c-cpp.yml
├── .vscode
│   ├── launch.json
│   └── tasks.json
├── convertLIVEgtfs
│   ├── main.cpp
│   └── (read README.md on how to use)
├── data
│   ├── (put your own data here, read README.md on how to use)
│   └── tes_archive-1
├── testing
│   ├── externalFNS.cpp
│   └── testing.cpp
├── .gitignore
├── licence.md
├── README.md
├── todo.txt
├── version.txt
└── main.cpp
```
#### (Thanks to `tree.nathanfriend` for the tree diagram)

# How do I use the GTFS-RT Converter?
All I gave was the code. It's a big pain for you to get ready, but if you have too much spare time, go for it. (Most of the steps you only have to do once if you use the same computer and OS).

**Step 1**: Download the  [`gtfs-realtime.proto`](https://github.com/google/transit/blob/master/gtfs-realtime/proto/gtfs-realtime.proto) file from the `google/transit` repository and put these in the same folder as your parser code. 

**Step 2**: (On Mac) Install `protoc@21` with `brew install protoc@21`. If you have the latest version, first uninstall it with `brew uninstall protoc`. If you don't even have homebrew install it with `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`. 

(On Windows) Go to the downloads page of `Protocol Buffers` at [`https://github.com/protocolbuffers/protobuf/releases`](https://github.com/protocolbuffers/protobuf/releases), scroll down to `Protocol Buffers v31.1` and at the bottom of the click `assets` and download the one matching your operating system. Download it at `C:/probuf-21.1/`.

If you are on Windows, open the Start menu, Search `“Environment Variables”`, click on `Edit system environment variables`, then click on  `Environment Variables` at the bottom right. Click `PATH` on the top section, then `Edit...`, then `New...`. Paste in `C:\protobuf-21.1\bin`. 

**Step 3**: (On Windows) Convert the `.proto` file into two files with `protoc` by either opening Terminal (Mac/Linux), Command Prompt/Powershell (Windows). Navigate to the working directory by ether using `cd` or right-clicking the folder then pressing "Open in Powershell". Paste `protoc --cpp_out=. gtfs-realtime.proto` then press Enter. 

(On Mac) First run  `brew --prefix protobuf@21`. It should output something like `/opt/homebrew/opt/protobuf@21`. If not, don't worry. Remember to add the text `/bin/protoc` at the end of it. For example, `/opt/homebrew/opt/protobuf@21/bin/protoc`. Navigate to the working directory by ether using `cd` or right-clicking the folder then pressing "Open in Terminal". Use the path with `/bin/protoc` to generate your files, for example, `/opt/homebrew/opt/protobuf@21/bin/protoc --cpp_out=. gtfs-realtime.proto`.


**Step 4**: Congratulations on making it this far! Time to compile the code. Download a `.pb` GTFS-RT File from the internet or from your local transit agency and paste it in the same directory as your `main.cpp`. Name it `TripUpdates.pb` and the output file will be `decoded_gtfs.txt`.

On Windows:
```powershell
g++ -std=c++17 main.cpp gtfs-realtime.pb.cc ^
-I C:\protobuf-21.1\include ^
-L C:\protobuf-21.1\lib -lprotobuf -o parseGTFS.exe
```

On Mac:
First run  `brew --prefix protobuf@21` in Terminal. It should output something like `/opt/homebrew/opt/protobuf@21`. I will call that the path. 
```bash
clang++ -std=c++17 main.cpp gtfs-realtime.pb.cc \
-I[path]/include \
-L[path]/lib \
-lprotobuf -o parseGTFS
```
For example, if your path is `/opt/homebrew/opt/protobuf@21`:
```bash
clang++ -std=c++17 main.cpp gtfs-realtime.pb.cc \
-I/opt/homebrew/opt/protobuf@21/include \
-L/opt/homebrew/opt/protobuf@21/lib \
-lprotobuf -o parseGTFS
```

**Step 5**: How long did this take you? You could've spend this time doing something else. Run the code by navigating to the directory, opening terminal/powersheel and typing:

`./parseGTFS`

(sometimes run `./parseGTFS.exe`)

After compiling it you don't need to ever compile it again unless you change the code, update/switch your OS, or switch computers with a different processor.

Wasn't that bad, wasn't it?

*DISCLAMER: ALL COMMANDS WILL WERE TESTED AND WORKING ON MACOS 26 AND WINDOWS 11.*


