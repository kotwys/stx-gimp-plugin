#!/usr/bin/env sh

set -e

if [ $# -ne 1 ]
then
  echo "Usage: $0 [meson build directory]"
  exit -1
fi

if ! [ -d "$1" ]
then
  echo "Build directory $1 does not exist. First, run meson build."
  exit -1
fi

echo "Compiling..."
meson compile -C "$1"

tmpdir=$(mktemp -d)
echo "Install to $tmpdir..."
DESTDIR=$tmpdir meson install -C "$1"

# Add README, etc.
tar -C dist -c '.' | tar -C "$tmpdir/file-stx" -xv

version=$(meson introspect --projectinfo _build | jq -r .version)
archive_name="$tmpdir/file-stx-$version-win.zip"

initial_dir=$(pwd)
cd $tmpdir
echo "Generating archive $archive_name"
zip -r $archive_name file-stx
cd "$initial_dir"

echo "Generated $archive_name"
