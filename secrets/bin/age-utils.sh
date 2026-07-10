#!/usr/bin/env bash

# age wrappers and shared helpers. Requires AGE_KEY_FILE to be set in the
# environment. AGE_PUBLIC_KEY is derived from it (via age-keygen) if not
# already set. Typically exported by the accompanying Makefile.inc, but the
# scripts can also be invoked directly with these env vars set on the command
# line.

decage() {
    age --decrypt --identity "${AGE_KEY_FILE}" "${@}"
}

encage() {
    age --encrypt --armor --recipient "${AGE_PUBLIC_KEY}" "${@}"
}

# Print the given value, or prompt the user for one if empty.
arg_or_prompt() {
    local prompt=$1 value=${2:-}
    if [[ -z "${value}" ]]; then
        read -rep "${prompt}: " value
    fi
    printf '%s\n' "${value}"
}

# Assert that the given file looks like a .age file and exists.
require_age_file() {
    local file=$1
    if [[ "${file}" != *.age ]]; then
        echo "Input file '${file}' doesn't look like an encrypted *.age file, exiting." >&2
        exit 1
    fi
    if [[ ! -f "${file}" ]]; then
        echo "Input file '${file}' doesn't exist, exiting." >&2
        exit 1
    fi
}

# Assert that the given file is not a .age file and exists.
require_plain_file() {
    local file=$1
    if [[ "${file}" == *.age ]]; then
        echo "Input file '${file}' looks like an encrypted .age file, exiting." >&2
        exit 1
    fi
    if [[ ! -f "${file}" ]]; then
        echo "Input file '${file}' doesn't exist, exiting." >&2
        exit 1
    fi
}

# If AGE_PUBLIC_KEY isn't set but AGE_KEY_FILE is, derive it. Lets the scripts
# be used standalone with just AGE_KEY_FILE=... .
if [[ -z "${AGE_PUBLIC_KEY:-}" ]] && [[ -f "${AGE_KEY_FILE:-}" ]]; then
    AGE_PUBLIC_KEY=$(age-keygen -y "${AGE_KEY_FILE}")
fi
