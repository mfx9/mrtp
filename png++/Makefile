# . Makefile

version = 0.2.9
archive = png++-${version}.tar.gz
library = png++-${version}


default: ${library}

${library}: ${archive}
	tar xfvz ${archive}

${archive}:
	wget http://download.savannah.nongnu.org/releases/pngpp/${archive}

clean:
	if [ -e "${library}" ] ; then rm -r ${library}  ; fi

clean_all: clean
	if [ -e "${archive}" ] ; then rm    ${archive}  ; fi
