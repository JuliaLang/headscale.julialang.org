SHELL:=/bin/bash
MAKEFILE_DIRECTORY:=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
HEADSCALE_VERSION:=0.24.2
HEADSCALE_URL:=https://github.com/juanfont/headscale/releases/download/v$(HEADSCALE_VERSION)/headscale_$(HEADSCALE_VERSION)_linux_amd64.deb
HEADSCALE_DEB:=build/headscale-$(HEADSCALE_VERSION).deb

print:
	echo ${MAKEFILE_DIRECTORY}

build:
	mkdir -p $@

# Download the headscale .deb package
$(HEADSCALE_DEB): | build
	curl -fsSL -o $(HEADSCALE_DEB) $(HEADSCALE_URL)

# Template in variables in config
build/config.yaml: config.yaml ../.env | build
	source ../.env && \
	export HEADSCALE_FQDN="$${HEADSCALE_FQDN}" && \
	export HEADSCALE_OIDC_CLIENT_ID="$${HEADSCALE_OIDC_CLIENT_ID}" && \
	export HEADSCALE_OIDC_CLIENT_SECRET="$${HEADSCALE_OIDC_CLIENT_SECRET}" && \
	export HEADSCALE_OIDC_ALLOWED_GROUPS="$${HEADSCALE_OIDC_ALLOWED_GROUPS}" && \
	export DEX_OIDC_ISSUER_URI="$${DEX_OIDC_ISSUER_URI}" && \
	export TAILNET_BASE_DOMAIN="$${TAILNET_BASE_DOMAIN}" && \
	envsubst '\$${HEADSCALE_FQDN}\$${HEADSCALE_OIDC_CLIENT_ID}\$${HEADSCALE_OIDC_CLIENT_SECRET}\$${HEADSCALE_OIDC_ALLOWED_GROUPS}\$${DEX_OIDC_ISSUER_URI}\$${TAILNET_BASE_DOMAIN}' < $< > $@

# Symlink the config
/etc/headscale/config.yaml: build/config.yaml
	sudo mkdir -p /etc/headscale
	sudo cp -f ${MAKEFILE_DIRECTORY}build/config.yaml $@

# Temporarily add extra records
/etc/headscale/extra-records.json: extra-records.json /etc/headscale/config.yaml
	sudo cp -f $< $@

# Install and start the headscale daemon
install: $(HEADSCALE_DEB) | /etc/headscale/config.yaml /etc/headscale/extra-records.json
	sudo dpkg -i $<
	sudo chown headscale:headscale /etc/headscale/config.yaml
	sudo systemctl enable headscale
	sudo systemctl start headscale

clean:
	rm -rf build
