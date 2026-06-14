# CDN77 task

Nginx source extended by a custom HTTP module, that responds to the client with the "X-Cache-Key" header, holding the value of a computed cache key from the caching proxy module.
For this to work, the nginx proxy cache module has to be enabled, such as the configuration in sandbox/nginx.conf.

## Verification

Clone this repo, cd into it, and run:
- `./auto/configure --add-module=$PWD/custom_modules/cachekey`
- `make -j12`
- To start the nginx proxy, run: `./objs/nginx -p $PWD/sandbox -c nginx.conf`
- To simulate an origin server, run: `python sandbox/origin.py`
- `curl -I 127.0.0.1:8080/images/cat.jpg` - observe the newly added X-Cache-Key header
