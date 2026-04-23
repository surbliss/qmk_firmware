{
  description = "Minimal development flake";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    devshell.url = "github:numtide/devshell";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" ];
      imports = [ inputs.devshell.flakeModule ];

      perSystem =
        { config, ... }:
        {
          devshells.default = import ./. config.allModuleArgs;
        };
    };
}
