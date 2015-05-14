set (borzoi_SRC
	aes.cpp
	asn.cpp
	curve.cpp
	ec_key_pair.cpp
	ecdsa.cpp
	ecies.cpp
	eckas_dh1.cpp
	ec_primitives.cpp
	hash.cpp
	rijndael-alg-fst.c
	rng.c
	mpi.c
	math_mpi.cpp
	utilities.cpp)

set_source_files_properties(${borzoi_SRC} PROPERTIES COMPILE_FLAGS 
    "-fPIC -w")
add_library(borzoi STATIC ${borzoi_SRC})

# install(TARGETS borzoi DESTINATION lib)
