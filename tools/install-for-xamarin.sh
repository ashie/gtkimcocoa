#!/bin/sh

MONO_PATH="/Library/Frameworks/Mono.framework"
GTK_PATH="${MONO_PATH}/Versions/Current"

if test "!" -d ${GTK_PATH}; then
  GTK_PATH=`ls -d ${MONO_PATH}/Versions/2.10.* | sort -r | head -1`
  if test -z "${GTK_PATH}"; then
    echo "You don't have proper GTK+ in ${MONO_PATH}"
    exit 1
  fi
fi

GTK_BINARY_VERSION=`${GTK_PATH}/bin/pkg-config --variable=gtk_binary_version gtk+-2.0`
GTK_IMMODULES_PATH="${GTK_PATH}/lib/gtk-2.0/${GTK_BINARY_VERSION}/immodules"
IM_QUARTZ_PATH="${GTK_IMMODULES_PATH}/im-quartz.so"

update_immodules_cache () {
  "${GTK_PATH}/bin/gtk-query-immodules-2.0" > "${GTK_PATH}/etc/gtk-2.0/gtk.immodules" || exit 1
}

install () {
  if test -f "${IM_QUARTZ_PATH}"; then
    mv "${IM_QUARTZ_PATH}" "${IM_QUARTZ_PATH}.bak" || exit 1
  fi
  cp im-cocoa.so "${GTK_IMMODULES_PATH}/" || exit 1
  update_immodules_cache
  echo "Succeeded to install im-cooca.so"
}

uninstall () {
  rm "${GTK_IMMODULES_PATH}/im-cocoa.so" || exit 1
  if test -f "${IM_QUARTZ_PATH}.bak"; then
    mv "${IM_QUARTZ_PATH}.bak" "${IM_QUARTZ_PATH}" || exit 1
  fi
  update_immodules_cache
  echo "Succeeded to uninstall im-cooca.so"
}

case "$1" in
  uninstall)
    uninstall
    ;;
  *)
    install
    ;;
esac
