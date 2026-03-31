
## To convert the normal server to the token server, follow the steps below:
1. In tokenserver.py, add 
```py
SECRET_TOKEN = token 

def require_token(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = request.headers.get("X-Auth-Token") or request.args.get("token")
        if token != SECRET_TOKEN:
            abort(401)
        return f(*args, **kwargs)
    return decorated
```
2. Create t_confidential_info.py and create 
```py
token = "enter_token_here"
```

3. Add `@require_token` under **all** of the `@app.route` statements

4. Under `@app.route('/')`, replace `return send_from_directory('.', 'index.html')` with `return send_from_directory('.', 'tokenindex.html')`. 

5. In `tokenindex.html`, under `todayStr()`, add 
```js
const token = new URLSearchParams(window.location.search).get('token');

function apiFetch(url, options = {}) {
    return fetch(url, {
    ...options,
    headers: { ...options.headers, 'X-Auth-Token': token }
    });
}
```
and replace all instances of `fetch()` with `apiFetch()`.



