SHELL := /bin/bash

ifeq ($(shell id -u headscale 2> /dev/null),)
$(error No headscale user, is headscale installed?)
endif

up:
	docker compose up -d

down:
	docker compose down

logs:
	docker compose logs -f
