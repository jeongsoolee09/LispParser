{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        stdenv = pkgs.llvmPackages.stdenv;
      in
      {
        devShells.default = pkgs.mkShell.override { inherit stdenv; } {
          nativeBuildInputs = with pkgs; [
            cmake
            clang-tools # clangd, clang-format, etc.
          ];

          buildInputs = with pkgs; [
            boost
            fmt
          ];

          CMAKE_EXPORT_COMPILE_COMMANDS = "ON";
        };
      }
    );
}
