This project was created for my need of a program that can periodically download
webcam images, store them according to time/date downloaded, and be able to make
a timelapse video of the downloaded images. I have searched for a program that filled
these requirements, but either they cost money or didnt include my requirements.

I have chosen to keep this project opensource and available to anyone. To anyone that
looks through the code, I will warn that I do not have the greatest programming
experience. This project is not only a creation of something I needed, but also a
learning experience for me.

This program depends on:

QT 4.8 (Currently built using QT Creator 2.4.1)
FFMpeg (Win32 Build Used: FFmpeg git-a254452 - compiled 2011-09-19)
QTFFMpegWrapper (http://code.google.com/p/qtffmpegwrapper)

Installing
	As of now this program is distributed via zip file and may be extracted to any 
	directory to run.
	
Creating a New Cam recording
	Use filemenu to create New Cam. Fill out the information needed. Everything is 
	required, with the exception of the advanced timing options. URL must point to
	an image. (eg, http://www.example.com/webcam/image.jpg) Directory will be the 
	location that all downloaded images are stored, as is the settings file for the
	cam. If advanced timing is not used, the program will pull the image from website
	at the interval specified. If will than compare it with the previously downloaded
	image as to make sure its not a copy. If a copy is detected, it will disregard 
	the downloaded image and repeat at the next interval. Advanced timing is provided
	to allow you to only retrieve between a specific time range. 
	
Creating Video
	Select the tab of cam you want to make a video of, then select Cam->Create Video.
	The video creator dialog will come up and you will be able to review all downloaded
	imaged and make a selection of the images you want included in the video. Click on
	Create Video to proceed making of the video. Once finished, it will be saved to 
	the directory which you originally set under the name video00000000000000.avi. 
	(the zeros will be replaced by the date/time the recording was made)
	