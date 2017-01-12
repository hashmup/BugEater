SOURCES=$(wildcard *.cpp)
CFLAGS=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`

all:
	g++ -o main $(SOURCES) --std=c++11 $(CFLAGS) $(LDFLAGS)

#all:
#	g++ -o main -stdlib=libc++ --std=c++11 -I/usr/local/Cellar/opencv3/HEAD-f7d956e_4/include/opencv -I/usr/local/Cellar/opencv3/HEAD-f7d956e_4/include -L/usr/local/Cellar/opencv3/HEAD-f7d956e_4/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core main.cpp search.cpp zobristHash.cpp board.cpp
