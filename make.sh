#!/bin/bash

function getbazel(){
	LINE=`readlink -f /home/$USER/tensorflow/bazel-bin/`

	POS1="_bazel_$USER/"
	STR=${LINE##*$POS1}

	BAZEL=${STR:0:32}

	echo $BAZEL
}


BAZEL=`getbazel`




IINCLUDE="-I/usr/local/opencv3/include -I/usr/local/opencv3/include/opencv -I/usr/local/include -I/usr/include/boost -I/usr/include/eigen3/Eigen -I/home/$USER/.cache/bazel/_bazel_$USER/$BAZEL/external/eigen_archive/Eigen"


LLIBPATH="-L/usr/local/opencv3/lib -L/usr/local/lib -L/usr/lib -L/home/$USER/AitoeLabs/DS/deepsort/FeatureGetter
-Wl,--allow-multiple-definition -Wl,--whole-archive"

rm DS -rf

LLIBS="-lopencv_stitching -lopencv_superres  -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d  -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann  -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core -lboost_system -lglog -lFeatureGetter"

function BOPENMP(){

	g++ --std=c++14 -O3 -fopenmp -DUDL -o DS $IINCLUDE $LLIBPATH  deepsort/munkres/munkres.cpp deepsort/munkres/adapters/adapter.cpp deepsort/munkres/adapters/boostmatrixadapter.cpp  NT.cpp fdsst/fdssttracker.cpp fdsst/fhog.cpp Main.cpp $LLIBS
}


function BTBB(){

	g++ --std=c++14 -DUSETBB -o DS $IINCLUDE $LLIBPATH deepsort/munkres/munkres.cpp deepsort/munkres/adapters/adapter.cpp deepsort/munkres/adapters/boostmatrixadapter.cpp  NT.cpp Main.cpp $LLIBS
}


function BOPENMPHOG(){

	g++ --std=c++14 -O3 -fopenmp -DUDL -o DS $IINCLUDE $LLIBPATH  deepsort/munkres/munkres.cpp deepsort/munkres/adapters/adapter.cpp deepsort/munkres/adapters/boostmatrixadapter.cpp NT.cpp fdsst/fdssttracker.cpp fdsst/fhog.cpp Main.cpp $LLIBS
}

BOPENMPHOG
