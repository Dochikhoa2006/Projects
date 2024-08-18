import cv2

file_classifer = cv2.CascadeClassifier ("/Users/KienDo/Desktop/Python/Application.py/Face_Detection.xml")
live_camera = cv2.VideoCapture (0)

if not live_camera.isOpened ():
	print ("Fail to open camera")
	exit ()

while True:
	found_recorder, image = live_camera.read ()

	if not found_recorder:
		print ("Fail to extract image in camera")
		break

	frame_base_color = cv2.cvtColor (image, cv2.COLOR_BGR2GRAY)
	frame_scale_property = file_classifer.detectMultiScale (frame_base_color, scaleFactor = 1.3, minNeighbors = 7)

	for x, y, w, h in frame_scale_property:
		cv2.rectangle (image, (x, y), (x + w, y + h), (0, 255, 0), 5)

	cv2.imshow ("khoa", image)

	key = cv2.waitKey (1)
	if key == ord (" "):
		break

live_camera.release ()
cv2.destroyAllWindows ()