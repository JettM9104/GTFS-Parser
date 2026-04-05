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

Read the (README)[https://github.com/JettM9104/GTFS-Parser/blob/main/gtfs-rt/readme.md] from the GTFS-RT Section to compile the RT Files.

**Step 5**: Run the server!

Run
```zsh
python3 server.py
```

Open up `http://localhost:5015` in your browser


**Step 6** (optional): Server that requires a token

Run
```zsh
python3 tokenindex_converter | python3 tokenserver_converter
```

Create a file called `t_confidental_info.py` and create a token:
```py
token = "generate_or_make_a_token_here"
```

Generate a token using
```zsh
python3 -c "import secrets; print(secrets.token_hex(32))"
```

Access it using the URL: `http://localhost:5015/?token=your_token`

