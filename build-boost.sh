if [[ ! -f boost/lib/libboost_serialization.a ]]; then
	mkdir boost
	curl -O -L https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz;
	tar -xzf boost_1_66_0.tar.gz;
	cd boost_1_66_0;
	./bootstrap.sh --with-libraries=serialization,filesystem,test --prefix=/usr/local/opt
	./b2 install link=static runtime-link=static
	cd ..;
fi