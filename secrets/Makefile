default: unlock

SECRET_FILES =
SECRET_FILES += dex.env headscale.env nginx.env
include Makefile.inc

unlock: $(SECRET_FILES)

.PHONY: default unlock
