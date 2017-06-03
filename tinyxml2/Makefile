# . Makefile

version = 4.0.1
library = tinyxml2-${version}
archive = ${library}.tar.gz


default: ${library}/tinyxml2.o

${library}/tinyxml2.o: ${library}
	+$(MAKE) -C ${library}

${library}: ${archive}
	tar xfvz ${archive}

${archive}:
	wget https://github.com/leethomason/tinyxml2/archive/${version}.tar.gz -O ${archive}

clean:
	if [ -e "${library}" ] ; then rm -r ${library}  ; fi

clean_all: clean
	if [ -e "${archive}" ] ; then rm    ${archive}  ; fi
