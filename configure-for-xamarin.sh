#!/bin/sh

MONO_PATH="/Library/Frameworks/Mono.framework"
GTK_PATH=`ls -d ${MONO_PATH}/Versions/2.10.* | sort -r | head -1`

if test -z $GTK_PATH; then
  echo "You don't have proper GTK+ in ${MONO_PATH}"
  exit 1
fi

./configure --enable-workaround \
  PATH="$GTK_PATH/bin:$PATH" \
  PKG_CONFIG_PATH="$GTK_PATH/lib/pkgconfig" \
  CFLAGS="-arch i386" \
  LDFLAGS="-arch i386"
