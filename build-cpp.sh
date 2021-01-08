#!/bin/sh -e
docker build -t cpp - < $(dirname $0)/Dockerfile
docker run -it --rm -v $PWD:/src cpp "$@"
