#!/bin/sh
set -e

for pod in client server; do
	cp ./sock_cfg.h $pod/
	podman image rm $pod || true
	podman build $pod/ -t $pod
done
