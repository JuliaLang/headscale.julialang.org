SHELL:=/bin/bash
MAKEFILE_DIRECTORY:=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))
HEADSCALE_VERSION:=0.22.3
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
	envsubst '\$${HEADSCALE_FQDN}\$${HEADSCALE_OIDC_CLIENT_ID}\$${HEADSCALE_OIDC_CLIENT_SECRET}\$${HEADSCALE_OIDC_ALLOWED_GROUPS}\$${DEX_OIDC_ISSUER_URI}' < $< > $@

# Symlink the config
/etc/headscale/config.yaml: build/config.yaml
	sudo mkdir -p /etc/headscale
	sudo cp -f ${MAKEFILE_DIRECTORY}build/config.yaml $@

# Install and start the headscale daemon
install: $(HEADSCALE_DEB) | /etc/headscale/config.yaml
	sudo dpkg -i $<
	sudo chown headscale:headscale /etc/headscale/config.yaml
	sudo systemctl enable headscale
	sudo systemctl start headscale

clean:
	rm -rf build
