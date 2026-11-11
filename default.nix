{ pkgs, ... }: {
  packages = with pkgs; [
    qmk
    dos2unix # Aparrently needed
    # Better LSP support
    clang
    clang-tools
  ];

  # inputsFrom = with pkgs;[ ];

  env = {
    IS_NIX_SHELL = true;

    CONF = "./qmk_firmware/keyboards/zsa/voyager/keymaps/surbliss";
  };

  shellHook = ''
    echo Hello!
  '';
}
