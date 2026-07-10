SHELL := /bin/bash

ifeq ($(shell id -u headscale 2> /dev/null),)
$(error No headscale user, is headscale installed?)
endif

up: nginx/ffdhe2048.pem
	docker compose up -d

down:
	docker compose down

logs:
	docker compose logs -f

pull:
	docker compose pull

# Download DH params from Mozilla
nginx/ffdhe2048.pem:
	curl -fsSL https://ssl-config.mozilla.org/ffdhe2048.txt > $@
