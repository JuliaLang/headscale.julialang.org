SHELL:=/bin/bash
MAKEFILE_DIRECTORY:=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
HEADSCALE_VERSION:=0.29.2
HEADSCALE_URL:=https://github.com/juanfont/headscale/releases/download/v$(HEADSCALE_VERSION)/headscale_$(HEADSCALE_VERSION)_linux_amd64.deb
HEADSCALE_DEB:=build/headscale-$(HEADSCALE_VERSION).deb

print:
	echo ${MAKEFILE_DIRECTORY}

build:
	mkdir -p $@

# Download the headscale .deb package
$(HEADSCALE_DEB): | build
	curl -fsSL -o $(HEADSCALE_DEB) $(HEADSCALE_URL)

# Secret unlocking
SECRET_FILES := ../secrets/headscale.env
include ../secrets/Makefile.inc

# Template in variables in config
build/config.yaml: config.yaml ../secrets/headscale.env | build
	source ../secrets/headscale.env && \
	export HEADSCALE_FQDN HEADSCALE_OIDC_CLIENT_ID HEADSCALE_OIDC_CLIENT_SECRET \
		HEADSCALE_OIDC_ALLOWED_GROUPS DEX_OIDC_ISSUER_URI TAILNET_BASE_DOMAIN && \
	envsubst '\$${HEADSCALE_FQDN}\$${HEADSCALE_OIDC_CLIENT_ID}\$${HEADSCALE_OIDC_CLIENT_SECRET}\$${HEADSCALE_OIDC_ALLOWED_GROUPS}\$${DEX_OIDC_ISSUER_URI}\$${TAILNET_BASE_DOMAIN}' < $< > $@

/etc/headscale:
	sudo mkdir -p $@

# Copy the config
/etc/headscale/config.yaml: build/config.yaml | /etc/headscale
	sudo cp -f ${MAKEFILE_DIRECTORY}build/config.yaml $@

# Copy the ACLs
/etc/headscale/acl.json: acl.json | /etc/headscale
	sudo cp -f $< $@

# Temporarily add extra records
/etc/headscale/extra-records.json: extra-records.json
	sudo cp -f $< $@

# Note: `systemctl reload headscale` only reloads ACLs, not the full config
reload: /etc/headscale/config.yaml /etc/headscale/extra-records.json /etc/headscale/acl.json
	sudo headscale configtest && \
		sudo systemctl stop headscale && \
		sudo systemctl start headscale

# Install and start the headscale daemon
install: $(HEADSCALE_DEB) | /etc/headscale/config.yaml /etc/headscale/extra-records.json /etc/headscale/acl.json
	sudo dpkg -i $<
	sudo chown headscale:headscale /etc/headscale/config.yaml
	sudo headscale configtest && \
		sudo systemctl enable --now headscale

clean:
	rm -rf build
