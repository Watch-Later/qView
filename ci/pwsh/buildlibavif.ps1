#! /usr/bin/pwsh

# Clone
git clone https://github.com/AOMediaCodec/libavif.git
cd libavif
git checkout $(git tag | select -last 1)

# vcvars on windows
if ($IsWindows) {
    & "$env:BUILD_REPOSITORY_LOCALPATH/ci/pwsh/vcvars.ps1"
}

cd ext

# Build dav1d
if ($IsWindows) {
    
} else {
    bash dav1d.cmd
}

# Build libavif 

mkdir ../build
cd ../build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DAVIF_CODEC_DAV1D=ON -DAVIF_LOCAL_DAV1D=ON ..
ninja
$env:DESTDIR = "installed/"
ninja install