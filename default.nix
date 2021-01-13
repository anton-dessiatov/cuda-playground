{ pkgs ? import <nixpkgs> {} }:

let stdenv = pkgs.stdenv;
in {
  cuda-playground = stdenv.mkDerivation {
    name = "cuda-playground";
    src = ./.;
    buildInputs = [ pkgs.cudaPackages.cudatoolkit_11 pkgs.cmake ];
  };
}
