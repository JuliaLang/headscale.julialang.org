# Secrets

Age-encrypted secrets. The encryption/decryption machinery (`Makefile.inc` + `bin/`) is a
self-contained "library" — see the header of `Makefile.inc` for what it exposes and how a
consuming Makefile plugs it in.

## Bootstrapping the age key

The age identity for this repo is stored in 1Password:

- Vault: **Infrastructure**
- Item: **Headscale/Dex repository encryption key (age.key)**

Fetch it into `age.key` in this directory:

```bash
OP_VAULT_ID=67wsppqjxtmagf37mc6747qoom
OP_ITEM_ID=iqlobmwlwjm2qv5bceos7om63m
OP_FIELD=notesPlain
(umask 077 && op read "op://${OP_VAULT_ID}/${OP_ITEM_ID}/${OP_FIELD}" > age.key)
```

Once `age.key` exists, the make targets defined by `Makefile.inc` (`encrypt-secret`,
`decrypt-secret`, `edit-secret`, `view-secret`) and any `SECRET_FILES` rules will work.
