mkdir -p rsc/lib/SFML
cd rsc/lib/SFML

# wget https://www.sfml-dev.org/files/SFML-<version>-sources.zip
#unzip SFML-<version>-sources.zip
wget "https://www.sfml-dev.org/files/SFML-2.6.1-linux-gcc-64-bit.tar.gz"
tar -xvf SFML-2.6.1-linux-gcc-64-bit.tar.gz
rm -rf  SFML-2.6.1-linux-gcc-64-bit.tar.gz

# cd SFML-2.6.1
# mkdir -p subprojects
# # Get the dependencies
# meson wrap install openal-soft
# meson wrap install flac
# meson wrap install ogg
# meson wrap install vorbis
# meson wrap install freetype2

# cat <<EOF > meson.build

# project('SFML with OpenAL-Soft', 'cpp')

# libogg_dep = dependency('ogg', fallback : ['ogg'], required : true)
# openal_dep = dependency('openal', fallback : ['openal-soft'], required : true)
# flac_dep = dependency('flac', fallback : ['flac'], required : true)
# vorbis_dep = dependency('vorbis', fallback : ['vorbis'], required : true)
# freetype_dep = dependency('freetype', fallback : ['freetype2'], required : true)
# openGL_dep = dependency('opengl', required : true)

# EOF

# # Build deps
# meson setup builddir
