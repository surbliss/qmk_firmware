{ pkgs, ... }:
{
  packages = with pkgs; [
    qmk
    dos2unix # Apparently needed
    # Better LSP support
    clang
    clang-tools
  ];

  commands = [
    {
      name = "e";
      command = "hx ./keyboards/zsa/voyager/keymaps/surbliss";
      help = "Edit voyager configuration";
    }
    {
      name = "f";
      command = "qmk flash";
      help = "qmk flash";
    }
    {
      name = "c";
      command = "qmk compile";
      help = "qmk compile";
    }
  ];
}
