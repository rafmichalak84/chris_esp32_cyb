static version ffmpeg from https://www.ffmpeg.org/download.html


.\ffmpeg.exe -i .\barber.avi -vf "fps=12,scale=-1:240:flags=lanczos,crop=320:in_h:(in_w-320)/2:0" -q:v 9 220_30fps.mjpeg