# GTFS Webserver
## Description
This is a lil side project from the main API but here it is i guess!

## Instructions for use
**MAC**

Navigate to the directory. 

**Step 1**: Create a virtual enviroment (optional)

In the terminal, run:
```zsh
python3 -m venv .venv
```
Activate it:
```zsh
source .venv/bin/activate
```

**Step 2**: Install dependencies

Run:
```
python3 -m pip install flask
```

**Step 3**: Build all C++ files in this folder

```zsh
find . -name "*.cpp" | xargs -I{} bash -c 'clang++ -std=c++17 -O3 -o "${0%.cpp}" "$0"' {}
```

**Step 4**: Build RT Files

Read the (README)[]
