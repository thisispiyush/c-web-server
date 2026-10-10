# C Web Server

A small HTTP web server written in C using POSIX sockets and pthreads. It listens on port **8080**, accepts browser requests, and serves HTML pages from the `pages/` directory.

## Features

- Creates a TCP socket and listens for HTTP connections on port `8080`.
- Handles each client connection in a separate thread.
- Serves the home page and other HTML files from the `pages/` directory.
- Returns a custom `404 - Page Not Found` page when a requested HTML file does not exist.
- Prints the HTTP method and requested path in the terminal.

## Project Structure

```text
for_git/
├── main.c
├── webrounting.c
├── pages/
│   ├── index.html
│   ├── about.html
│   └── 404.html
└── README.md
```

### Source files

- **`main.c`** — the file-based server implementation. It maps `/` to `pages/index.html` and other paths such as `/about` to `pages/about.html`.
- **`webrounting.c`** — an alternative implementation that defines page responses directly in C rather than reading HTML files. It is a separate server program; compile either `main.c` or `webrounting.c`, not both together.
- **`pages/`** — HTML pages served by `main.c`.

## Requirements

- A C compiler such as GCC.
- A POSIX-compatible environment (for example, Linux, macOS, or WSL on Windows).
- POSIX sockets and pthread support.

## Build and Run

Open a terminal in the `for_git` project directory.

### Build the file-based server

```bash
gcc main.c -o server -pthread
```

### Start the server

```bash
./server
```

You should see:

```text
Server running on http://localhost:8080
```

Keep the terminal open while using the server. Open the following URLs in your browser:

- Home: http://localhost:8080/
- About: http://localhost:8080/about
- Example not-found page: http://localhost:8080/unknown

To stop the server, press **Ctrl+C**.

### Build the alternative routing implementation

If you want to run the version whose page content is written directly in C:

```bash
gcc webrounting.c -o webserver-routing
./webserver-routing
```

This alternative also uses port `8080`, so stop the first server before starting it.

## Adding a Page

For the `main.c` implementation, create an HTML file under `pages/` that matches the route.

For example, to support `/contact`, create:

```text
pages/contact.html
```

Then visit `http://localhost:8080/contact`.

The current implementation removes one trailing slash, so `/about/` is treated like `/about`.

## How It Works

1. The server creates a TCP socket.
2. It binds the socket to port `8080` and starts listening.
3. It accepts an incoming client connection.
4. A worker thread reads the HTTP request and extracts the method and path.
5. The server selects an HTML file based on the path.
6. It sends an HTTP response with a status code and HTML content.
7. The connection is closed.

## Current Limitations

This is a learning project and a minimal HTTP server, not a production-ready web server.

- It currently handles basic requests and does not fully parse the HTTP protocol.
- The request method is parsed but not used to distinguish `GET`, `POST`, and other methods.
- It does not currently serve CSS, JavaScript, images, or other static file types with the correct content types.
- Request parsing, file paths, and response sizes need stronger validation before exposing the server to untrusted traffic.
- Error handling and partial socket reads/writes could be improved.
- The `main.c` version currently has no `pages/about/contact.html`, so `/about/contact` returns the 404 page unless that file is added.

## Possible Improvements

- Add proper MIME types for HTML, CSS, JavaScript, images, and other files.
- Improve HTTP request parsing and return appropriate status codes.
- Add safer path validation to prevent path traversal.
- Add logging, configuration, and automated tests.
- Add CSS to improve the appearance of the pages.

## License

No license has been specified yet. Add a `LICENSE` file if you intend to publish or share this project with a particular license.
