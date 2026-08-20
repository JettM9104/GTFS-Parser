#include "httplib.h"
using namespace httplib;

int main() {
    Server svr;

    // Define a route
    svr.Get("/", [](const Request& req, Response& res) {
        res.set_content("Hello World from C++!", "text/plain");
    });

    // Listen on port 8080
    svr.listen("0.0.0.0", 8080);
}
