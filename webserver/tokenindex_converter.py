#!/usr/bin/env python3
"""
sync_tokenindex.py
Generates tokenindex.html from index.html automatically.
Usage: python3 sync_tokenindex.py
"""

import re

INPUT  = 'index.html'
OUTPUT = 'tokenindex.html'

# Injected just before todayStr()
TOKEN_JS = """    const token = new URLSearchParams(window.location.search).get('token');

    function apiFetch(url, options = {}) {
        return fetch(url, {
        ...options,
        headers: { ...options.headers, 'X-Auth-Token': token }
        });
    }

"""

def replace_fetch_in_scripts(html):
    """Replace bare fetch( with apiFetch( inside <script> blocks only."""
    result = []
    last = 0
    for m in re.finditer(r'(<script\b[^>]*>)(.*?)(</script>)', html, flags=re.DOTALL):
        result.append(html[last:m.start()])
        result.append(m.group(1))
        js = re.sub(r'(?<!api)fetch\(', 'apiFetch(', m.group(2))
        # Restore the real fetch( inside apiFetch's own return statement
        js = js.replace('return apiFetch(url,', 'return fetch(url,')
        result.append(js)
        result.append(m.group(3))
        last = m.end()
    result.append(html[last:])
    return ''.join(result)

def main():
    with open(INPUT, 'r') as f:
        src = f.read()

    # 1. Inject token + apiFetch just before function todayStr()
    src = re.sub(
        r'(\n\s*function todayStr\(\))',
        '\n' + TOKEN_JS + r'\1',
        src,
        count=1
    )

    # 2. Replace all bare fetch( with apiFetch( inside <script> blocks
    src = replace_fetch_in_scripts(src)

    with open(OUTPUT, 'w') as f:
        f.write(src)

    print(f"✓ {OUTPUT} generated from {INPUT}")

if __name__ == '__main__':
    main()