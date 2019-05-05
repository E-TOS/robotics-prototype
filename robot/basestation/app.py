#!/usr/bin/env python
"""Flask server controller.

Flask is light-weight and modular so this is actually all we need to set up a simple HTML page.
"""

import os
import subprocess
from urllib.parse import urlparse, unquote
import flask
from flask import jsonify, request
from connection import Connection

app = flask.Flask(__name__)

def fetch_ros_master_uri():
    """Fetch and parse ROS Master URI from environment variable.

    The parsed URI is returned as a urllib.parse.ParseResult instance.

    Returns:
        urllib.parse.ParseResult: 6-tuple instance with various attributes.

    Attributes (urllib.parse.ParseResult):
    - hostname -- the ip address or the dns resolvable name
    - port -- the port number
    - etc...

    See https://docs.python.org/3/library/urllib.parse.html?highlight=urlparse#urllib.parse.urlparse
    """
    return urlparse(os.environ["ROS_MASTER_URI"])


def fetch_ros_master_ip():
    """Fetch only the hostname (host IP) portion of the parse URI."""
    return fetch_ros_master_uri().hostname


def run_shell(cmd, args=""):
    """Run script command supplied as string.

    Returns tuple of output and error.
    """
    cmd_list = cmd.split()
    arg_list = args.split()

    print("arg_list:", arg_list)

    for arg in arg_list:
        cmd_list.append(str(arg))

    print("cmd_list:", cmd_list)

    process = subprocess.Popen(cmd_list, stdout=subprocess.PIPE)
    output, error = process.communicate()

    return output, error

def get_pid(keyword):
    cmd = "ps aux"
    output, error = run_shell(cmd)

    ting = output.decode().split('\n')

    #print(ting)

    for line in ting:
        if keyword in line:
            #print("FOUND PID:", line)
            words = line.split()
            print("PID:", words[1])

            return words[1]

    return -1
# Once we launch this, this will route us to the "/" page or index page and
# automatically render the Robot GUI
@app.route("/")
def index():
    """Current landing page, the arm panel."""
    return flask.render_template("AsimovOperation.html", roverIP=fetch_ros_master_ip())


@app.route("/rover")
def rover():
    """Rover control panel."""
    return flask.render_template("Rover.html", roverIP=fetch_ros_master_ip())


@app.route("/ping_rover")
def ping_rover():
    """Pings ROS_MASTER_URI and return response object with resulting outputs.

    Pings rover first directly with Unix ping command,
    then using ros ping_acknowledgment service.

    Returns JSON object with the following fields:
    success -- whether requests was successful
    ping_msg -- output of Unix ping command
    ros_msg -- output of the ROS ping_acknowledgment service
    """
    ping_output, error = run_shell("ping -c 1 " + fetch_ros_master_ip())
    ping_output = ping_output.decode()

    print("Output: " + ping_output)

    if "Destination Net Unreachable" in ping_output:
        error_msg = "Basestation has no connection to network, aborting ROS ping."
        return jsonify(success=False, ping_msg=ping_output, ros_msg=error_msg)

    if "Destination Host Unreachable" in ping_output:
        error_msg = "Rover has no connection to network, aborting ROS ping."
        return jsonify(success=False, ping_msg=ping_output, ros_msg=error_msg)

    if error:
        print("Error: " + error.decode())

    ros_output, error = run_shell("rosrun ping_acknowledgment ping_response_client.py hello")
    ros_output = ros_output.decode()

    print("Pinging rover")
    print("Output: " + ros_output)

    if error:
        print("Error: " + error.decode())

    return jsonify(success=True, ping_msg=ping_output, ros_msg=ros_output)

@app.route("/select_mux", methods=["POST", "GET"])
def select_mux():
    print("select_mux")
    dev = str(request.get_data(), "utf-8")
    print("dev : " + dev)

    output, error = run_shell("rosrun mux_selector mux_select_client.py", dev)
    output = str(output, "utf-8")
    print("output: " + output)

    return jsonify(success=True, dev=dev, output=output)

@app.route("/serial_cmd", methods=["POST", "GET"])
def serial_cmd():
    print("serial_cmd")

    cmd = str(request.get_data('cmd'), "utf-8")
    print("cmd: " + cmd)
    # remove fluff, only command remains
    if cmd:
        cmd = cmd.split("=")[1]
        # decode URI
        cmd = unquote(cmd)

    print("cmd: " + cmd)

    ros_cmd = "rosrun serial_cmd serial_cmd_client.py '" + cmd + "'"
    print("ros_cmd: " + ros_cmd)

    output, error = run_shell("rosrun serial_cmd serial_cmd_client.py", cmd)
    output = str(output, "utf-8")

    print("output: " + output)

    return jsonify(success=True, cmd=cmd, output=output)

# only to be used when hacky implementation is fixed
# see odroid_rx package for details
@app.route("/odroid_rx", methods=["POST"])
def odroid_rx():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    log_file = script_dir + "/../rospackages/src/odroid_rx/scripts/odroid_rx.txt"
    print("odroid_rx")

    # query the topic exactly once
    output, error = run_shell("cat", log_file)
    output = str(output, "utf-8")

    print("output: " + output)

    return jsonify(success=True, odroid_rx=output)

# Automatic controls
@app.route("/click_btn_pitch_up")
def click_pitch_up():
    print("click_btn_pitch_up")
    return ""


@app.route("/click_btn_pitch_down")
def click_btn_pitch_down():
    print("click_btn_pitch_down")
    return ""


@app.route("/click_btn_roll_left")
def click_btn_roll_left():
    print("click_btn_roll_left")
    return ""


@app.route("/click_btn_roll_right")
def click_btn_roll_right():
    print("click_btn_roll_right")
    return ""


@app.route("/click_btn_claw_open")
def click_btn_claw_open():
    print("click_btn_claw_open")
    return ""


@app.route("/click_btn_claw_close")
def click_btn_claw_close():
    print("click_btn_claw_close")
    return ""


@app.route("/click_btn_arm_up")
def click_btn_arm_up():
    print("click_btn_arm_up")
    return ""


@app.route("/click_btn_arm_down")
def click_btn_arm_down():
    print("click_btn_arm_down")
    return ""


@app.route("/click_btn_arm_left")
def click_btn_arm_left():
    print("click_btn_arm_left")
    return ""


@app.route("/click_btn_arm_right")
def click_btn_arm_right():
    print("click_btn_arm_right")
    return ""

@app.route("/click_btn_arm_back")
def click_btn_arm_back():
    print("click_btn_arm_back")
    return ""


@app.route("/click_btn_arm_forward")
def click_btn_arm_forward():
    print("click_btn_arm_forward")
    return ""


# Manual controls
@app.route("/manual_control", methods=["POST"])
def manual_control():

    print("manual_control")

    cmd = str(request.get_data('cmd'), "utf-8")
    print("cmd: " + cmd)
    # remove fluff, only command remains
    if cmd:
        cmd = cmd.split("=")[1]
        # decode URI
        cmd = unquote(cmd)

    if local:
        rover_ip = "127.0.0.1"
        base_ip = rover_ip
        rover_port = 5005
        base_port = 5010
    else:
        rover_ip = "172.16.1.30"
        base_ip = "172.16.1.20"
        rover_port = 5015
        base_port = rover_port

    print("cmd: " + cmd)
    sender = Connection("arm_cmd_sender", rover_ip, rover_port)
    error = str(None)

    try:
        sender.send(cmd)
    except OSError:
        error = "Network is unreachable"
        print(error)

    receiver = Connection("arm_cmd_receiver", base_ip, base_port)
    feedback = str(None)

    try:
        feedback = receiver.receive(timeout=2)
    except OSError:
        error = "Network is unreachable"
        print(error)

    print("feedback:", feedback)

    if not feedback:
        feedback = "Timeout limit exceeded, no data received"

    return jsonify(success=True, cmd=cmd, error=error, feedback=feedback)

# Rover controls
@app.route("/rover_drive", methods=["POST"])
def rover_drive():
    print("rover_drive")

    cmd = str(request.get_data('cmd'), "utf-8")
    print("cmd: " + cmd)
    # remove fluff, only command remains
    if cmd:
        cmd = cmd.split("=")[1]
        # decode URI
        cmd = unquote(cmd)

    if local:
        rover_ip = "127.0.0.1"
        base_ip = rover_ip
        rover_port = 5020
        base_port = 5025
    else:
        rover_ip = "172.16.1.30"
        base_ip = "172.16.1.20"
        rover_port = 5030
        base_port = rover_port
    print("cmd: " + cmd)
    sender = Connection("rover_drive_sender", rover_ip, rover_port)

    error = str(None)

    try:
        sender.send(cmd)
    except OSError:
        error = "Network is unreachable"
        print(error)

    receiver = Connection("rover_drive_receiver", base_ip, base_port)
    feedback = str(None)
    error = str(None)

    try:
        feedback = receiver.receive(timeout=2)
    except OSError:
        error = "Network error"
        print(error)

    if not feedback:
        feedback = "Timeout limit exceeded, no data received"
    else:
        print("feedback:", feedback)
        lat = 45.520495; long = -73.392162;
        data = feedback.split("\n") # last message will be an empty string (or have "\r")
        data = reversed(data)
        i = 1 #skip the empty string
        while True:
            if data[i].find("GPS") != -1: # check if GPS is in the message
                if latestGpsMsg is None:
                    latestGpsMsg = i
                if data[i].find("OK") is not -1: # there is GPS data
                    print("calling NavigationClient")
                    output, error = run_shell("python NavigationClient.py",lat,long,data[i])
                    output = str(output, "utf-8")
                    print("output: " + output)
                    break
            if i>=5:
                if latestGpsMsg is not None:
                    print("calling NavigationClient")
                    output, error = run_shell("python NavigationClient.py",lat,long,data[latestGpsMsg])
                    output = str(output, "utf-8")
                    print("output: " + output)
                print("no GPS data, not calling NavigationClient")
                break
            i+=1

    return jsonify(success=True, cmd=cmd, feedback=feedback, error=error)

# Task handler services
@app.route("/task_handler", methods=["POST"])
def task_handler():
    print("task_handler")

    cmd = str(request.get_data('cmd'), "utf-8")
    print("cmd: " + cmd)
    # remove fluff, only command remains
    if cmd:
        cmd = cmd.split("=")[1]
        # decode URI
        cmd = unquote(cmd)

    print("cmd: " + cmd)

    ros_cmd = "rosrun task_handler task_handler_client.py"
    cmd_args = ""

    # choose appropriate arguments for ROS service client call
    if cmd == "enable-arm-listener":
        cmd_args = "arm_listener 1"
    elif cmd == "disable-arm-listener":
        cmd_args = "arm_listener 0"
    elif cmd == "enable-rover-listener":
        cmd_args = "rover_listener 1"
    elif cmd == "disable-rover-listener":
        cmd_args = "rover_listener 0"
    elif cmd == "enable-arm-stream":
        cmd_args = "camera_stream 1"
    elif cmd == "disable-arm-stream":
        cmd_args = "camera_stream 0"

    print("cmd_args:", cmd_args)

    output, error = run_shell(ros_cmd, cmd_args)
    output = output.decode()

    print("Output: " + output)

    if error:
        print("Error: " + error.decode())

    error = str(None) if not error else str(error)

    return jsonify(success=True, cmd=cmd, output=output, error=error)


if __name__ == "__main__":

    # feature toggles
    # the following two are used for UDP based communication with the Connection class
    global local
    local = True
    # print("fetch_ros_master_ip:", fetch_ros_master_ip())
    #
    # # either local or competition
    # ros_master_ip = fetch_ros_master_ip()
    # if ros_master_ip in ["127.0.0.1", "localhost"]
    #     local = True

    app.run(debug=True)
    # add param `host= '0.0.0.0'` if you want to run on your machine's IP address
