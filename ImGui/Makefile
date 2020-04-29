
OPENCV_FLAGS=-I/usr/local/include/opencv2 -L/usr/local/lib -lopencv_calib3d -lopencv_imgproc -lopencv_core -lopencv_ml -lopencv_objdetect -lopencv_flann -lopencv_video -lopencv_highgui -lopencv_imgcodecs
SFML_FLAGS=-I/usr/local/include/SFML -lsfml-system -lsfml-window -lsfml-graphics
SFML_IMGUI_FLAGS=${SFML_FLAGS} -lImGui-SFML


opencv:
	clear && g++ -ggdb main.cpp -o app ${OPENCV_FLAGS} && ./app
sfml:
	clear && g++ -ggdb main.cpp -o app ${SFML_FLAGS} && ./app
sfml_imgui:
	clear && g++ -ggdb main.cpp -o app ${SFML_IMGUI_FLAGS} && ./app