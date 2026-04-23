set working-directory := "keyboards/zsa/voyager/keymaps/surbliss"

default:
    just --list

keymap:
    hx keymap.c

key: keymap

config:
    hx config.h

flash:
    qmk flash

link:
    qmk lint

compile:
    qmk compile

# Regenerate database to fix lsp-commands
lsp:
    qmk compile -kb zsa/voyager -km surbliss --compiledb

# Fetch the git-submoules, necessary after fresh clone
update:
    git submodule update --init --recursive
