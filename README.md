gtkimcocoa
==========

How to build
------------

### Build GTK+3 using jhbuild

Please see [GNOME Live!](https://live.gnome.org/GTK%2B/OSX/Building) for
more details.

1. Remove MacPorts paths from your PATH if you use it.

2. Download gtk-osx-build-setup.sh

   $ curl -s http://git.gnome.org/browse/gtk-osx/plain/gtk-osx-build-setup.sh \
          -o gtk-osx-build-setup.sh

3. Run gtk-osx-build-setup.sh

   $ sh gtk-osx-build-setup.sh

4. Launch jhbuild shell

   $ ~/.local/bin/jhbuild shell

5. Build GTK+3

   $ ~/.local/bin/jhbuild bootstrap
   $ ~/.local/bin/jhbuild build meta-gtk-osx-bootstrap
   $ ~/.local/bin/jhbuild build meta-gtk-osx-gtk3


### Build gtkimcocoa

1. Clone gtkimcocoa

   $ git clone git://github.com/ashie/gtkimcocoa.git

2. Build gtkimcocoa on jhbuild shell:

   $ cd gtkimcocoa
   $ ~/.local/bin/jhbuild shell
   $ ./autogen.sh
   $ ./configure
   $ make
   $ make install

3. Update immodules.cache

   $ gtk-query-immodules-3.0 > ~/gtk/inst/lib/gtk-3.0/3.0.0/immodules.cache

4. Run gtk3-demo then open a text widget

   $ gtk3-demo
