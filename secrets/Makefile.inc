# vim: filetype=make

# Makefile.inc — encrypted-secrets helpers (age-based).
#
# This file and the accompanying bin/ directory form a small self-contained
# "library" for managing age-encrypted secrets. To use it in a project, copy
# Makefile.inc and bin/ into the project and include Makefile.inc from a
# Makefile:
#
#     SECRET_FILES := path/to/foo.env path/to/bar.pem   # optional; see below
#     include /path/to/secrets/Makefile.inc
#
# Requirements:
# - The `age` and `age-keygen` binaries on PATH (github.com/FiloSottile/age).
# - An age identity file at AGE_KEY_FILE. Defaults to <Makefile.inc-dir>/age.key
#   if not already set. Producing this file is the consumer's responsibility;
#   see the project's own README for repo-specific bootstrap instructions.
#
# Provides:
# - Exported variables (available to recipes and child processes):
#     AGE_KEY_FILE     — path to the age identity file.
#     AGE_PUBLIC_KEY   — the corresponding public recipient, extracted from
#                        AGE_KEY_FILE.
# - Utility targets that wrap the scripts in bin/:
#     encrypt-secret, decrypt-secret, edit-secret, view-secret
# - A static pattern rule for automatic decryption. Set SECRET_FILES (before
#   including this file) to a list of decrypted target paths, and each will
#   be built from its .age counterpart on demand.

SECRETS_MAKEFILE_INC := $(abspath $(lastword $(MAKEFILE_LIST)))
SECRETS_MAKEFILE_INC_DIR := $(shell dirname $(SECRETS_MAKEFILE_INC))
AGE_KEY_FILE ?= $(SECRETS_MAKEFILE_INC_DIR)/age.key
AGE_PUBLIC_KEY ?= $(shell age-keygen -y ${AGE_KEY_FILE} 2>/dev/null)
export AGE_KEY_FILE
export AGE_PUBLIC_KEY

# Utility make targets to invoke bin/(encrypt|decrypt|edit|view)-secret
encrypt-secret: $(AGE_KEY_FILE)
	$(SECRETS_MAKEFILE_INC_DIR)/bin/encrypt-secret
decrypt-secret: $(AGE_KEY_FILE)
	$(SECRETS_MAKEFILE_INC_DIR)/bin/decrypt-secret
edit-secret: $(AGE_KEY_FILE)
	$(SECRETS_MAKEFILE_INC_DIR)/bin/edit-secret
view-secret: $(AGE_KEY_FILE)
	$(SECRETS_MAKEFILE_INC_DIR)/bin/view-secret
.PHONY: encrypt-secret decrypt-secret edit-secret view-secret

# Automatically define rule to decrypt files in SECRET_FILES
SECRET_FILES ?=
$(SECRET_FILES): %: %.age $(AGE_KEY_FILE)
	$(SECRETS_MAKEFILE_INC_DIR)/bin/decrypt-secret $< $@
