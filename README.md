# unixsockettest
Run `./build_imgs.sh` to load `client` and `server` images to podman.

## Run server container
`podman run --rm -d -v <VOL_NAME>:/tmp/xyz server`  
or   
`podman run --rm -d --mount type=bind,src=/path/to/mountpoint,dst=/tmp/xyz server`

## Run client container
Here `CUSTOM_MESSAGES` is optional.  
`podman run --rm -it -v <VOL_NAME>:/tmp/xyz client <CUSTOM_MESSAGES>`  
or   
`podman run --rm -it --mount type=bind,src=/path/to/mountpoint,dst=/tmp/xyz client <CUSTOM_MESSAGES>`

## Validation
The client should emit messages to the server ("this is the default" by default). Given we have a valid socket, the server will echo this back to the client. The expected output of `client` is the following.
```
socket opened
socket connected
read (1): this
read (2): is
read (3): the
read (4): default
sock closed
```
