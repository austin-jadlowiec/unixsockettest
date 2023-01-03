# unixsockettest
Run `./build_imgs.sh` to load `client` and `server` images to podman.

## Run server container
`podman run --rm -d -v <VOL_NAME>:/tmp/xyz server`
or 
`podman run --rm -d --mount type=bind,src=/path/to/mountpoint,dest=/tmp/xyz server`

## Run client container
`podman run --rm -it -v <VOL_NAME>:/tmp/xyz client`
or 
`podman run --rm -it --mount type=bind,src=/path/to/mountpoint,dest=/tmp/xyz server`
