import subprocess
from subprocess import Popen, PIPE
import os
import datetime
import flask
from flask import jsonify, make_response
import threading

import robot.basestation.static.python.ros_utils as ros_utils
from robot.basestation.static.python.ros_utils import *

global proc_video
proc_video = {}

def start_recording_feed(stream):
    """
    Setup variables and start connection check
    """
    recording_log_msg = 'recording feed of ' + stream
    formatted_date = datetime.datetime.now().strftime("%I:%M:%S_%B_%d_%Y")
    return feed_connection_check(stream, formatted_date, recording_log_msg)

def stop_recording_feed(stream):
    """
    Stop recording feed and handle potential errors
    """
    recording_log_msg = 'saved recording of ' + stream
    error_state = 0
    try:
        proc_video[stream].communicate(b'q')
    except (KeyError, ValueError):
        error_state = 1

    return jsonify(recording_log_msg=recording_log_msg, error_state=error_state)

def feed_connection_check(stream, formatted_date, recording_log_msg):
    """
    Check if video feed is up before starting start ffmpeg
    """
    stream_url = "http://" + fetch_ros_master_ip() + ":8080/stream?topic=/cv_camera/image_raw"
    error_state = 0
    connection_check = os.system('ffprobe -select_streams v -i ' + stream_url)
    if connection_check == 0:
        threading.Thread(target = start_ffmpeg_record, args = (stream, stream_url, formatted_date)).start()
        print(recording_log_msg)
        return jsonify(recording_log_msg=recording_log_msg, error_state=error_state)

    else:
        recording_log_msg = 'failed to connect to video feed of ' + stream
        error_state = 1
        print(recording_log_msg)
        return jsonify(recording_log_msg=recording_log_msg, error_state=error_state)

def start_ffmpeg_record(stream, stream_url, formatted_date):
    """
    Start ffmpeg to start recording stream

    The stream_url varible in this function will have to be modified in order
    to handle recording multiple streams simultaneously
    """
    filename = stream + '_' + formatted_date
    save_video_dir = 'rover_stream/' + stream
    subprocess.Popen(['mkdir rover_stream'], shell=True)
    subprocess.Popen(['mkdir ' + save_video_dir], shell=True)
    proc_video[stream] = subprocess.Popen(['ffmpeg -i ' + stream_url + ' -acodec copy -vcodec copy ' + save_video_dir + '/' + filename + '.mp4'], stdin=PIPE, shell=True)

def run_capture_image():
    stream_url = "http://" + fetch_ros_master_ip() + ":8080/stream?topic=/cv_camera/image_raw"
    #lserror, lsoutput = run_shell("ls -1q img* | wc -l")
    # p1 = subprocess.Popen(split("ls -1q img*"), stdout=subprocess.PIPE)
    # p2 = subprocess.Popen(split("wc -l"), stdin=p1.stdout)
    # output, error = p2.communicate()
    output, error = run_shell('ls')
    output = output.decode()
    print('output:', output)
    i = 0

    if 'img' in output:
        i = output.rfind('img')
        i = int(output[i + 3]) + 1 # shift by 'img'
        print('i', i)

    error, output = run_shell("ffmpeg -i " + stream_url + " -ss 00:00:01.500 -f image2 -vframes 1 img" + str(i) + ".jpg")
    msg = "success"

    if error:
        msg = "F"

    print('msg', msg)

    return (msg)
