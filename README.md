# problem

It's been observed under certain circumstances that MacOS `overlay` and `overlay2` storage drivers cause the syscall `copy_file_range` to return zero bytes written in Alpine Linux images.

When this happens, it does not appear to be user error. Or it's at least _unclear_ where that user error could be. The parameters passed to `copy_file_range` are valid: good file descriptors, good offset etc...

This has at least propagated into one known bug outside of docker land: [see nodejs copy_file* infinite loops](https://github.com/nodejs/node/issues/40200).

## Environment:

1. Alpine Linux base image.
1. MacOS Docker Engine
1. `overlay` `overlay2` storage driver

*Note* this doesn't appear to impact Docker on Linux, or MacOS using other storage drivers (such as vfs).

## deets

Initially, this error surfaced after a `cp` command was executed on a regular file, and then seeing a nodejs process hang with high cpu usage on `copy`.

This can be reproed with the following docker image:

More fundamentally, the [do-test shell script](./do-test.sh), demonstrates the high-level problem: 

`docker run dchampz/copy_file_range-test:alpine-312-sendfileRepro`

However, this can be refined even further ([see sendfile-repro.c](./sendfile-repro.c)). It appears that after the `sendfile` syscall has been used, `copy_file_range` will return zero when copying from that file to another file.