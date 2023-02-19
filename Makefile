SHELL := /bin/bash
arch=`dpkg --print-architecture`
export version=1.2.1
build:
	# 编译
	mkdir building -p
	
	cd building/ && qmake ..
	cd building/ && make -j$(nproc)

clean:
	rm -rf building
	rm -rf build
	rm -rf /tmp/simple-rdesktop-building

package:
	make build
	#export arch=`arch`
	cp building/simple-remote-desktop-accessor deb/opt/durapps/spark-simple-remote-desktop-accessor	
	mkdir -p /tmp/simple-rdesktop-building
	cp deb/* /tmp/simple-rdesktop-building -r
	sed -i "s/@@version@@/${version}/g" /tmp/simple-rdesktop-building/DEBIAN/control #> /tmp/simple-rdesktop-building/DEBIAN/control
	sed -i "s/@@arch@@/${arch}/g" /tmp/simple-rdesktop-building/DEBIAN/control #> /tmp/simple-rdesktop-building/DEBIAN/control
	dpkg -b /tmp/simple-rdesktop-building spark-simple-remote-desktop-accessor_${version}_${arch}.deb
	rm -rf /tmp/simple-rdesktop-building
