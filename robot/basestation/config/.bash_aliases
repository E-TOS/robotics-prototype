#!/usr/bin/env bash

# NOTE: be careful when choosing new aliases
# DO NOT overwrite already existing bash cmds, this will only lead to issues
# A quick way to test if your new alias would conflict is:
# run following command: 'ls /usr | grep <your_new_alias_name>'
# as long as you don't get a full match, you can use this name for a new alias
# this is why the alias is called 'loc' and not 'local', for example

ROBOTICS_WS="~/Programming/robotics-prototype"
BASE="$ROBOTICS_WS/robot/basestation"
ROVER="$ROBOTICS_WS/robot/rover"
ROSPACKAGES="$ROBOTICS_WS/robot/rospackages"
BASH_A="~/.bash_aliases"
NANORC="~/.nanorc"
CHANGE_MODE="$BASE/config/change_mode.sh"

# general shortcuts
alias ..="cd .."
alias b="cd -"
alias robotics="cd $ROBOTICS_WS"
alias base="cd $BASE"
alias rover="cd $ROVER"
alias arm="cd $ROVER/ArmDriverUnit"
alias wheels="cd $ROVER/MobilePlatform"
alias rostings="cd $ROSPACKAGES"
alias mcunode="cd $ROSPACKAGES/src/mcu_control/scripts"

# edit thyself
alias editalius="nano $BASH_A"
alias alius=". $BASH_A"
alias editnani="nano $NANORC"
alias nani=". $NANORC"

# to start the gui
alias updateEnv="sh $BASE/env.sh > $BASE/static/js/env.js"
alias roverenv=". $ROBOTICS_WS/venv/bin/activate"
alias startgui="updateEnv && roverenv && cd $BASE && python app.py"
# we have to deactivate venv for this launch file to not break, bug to be resolved eventually
alias rosgui="roslaunch rosbridge_server rosbridge_websocket.launch"

# switching between local and competition configurations
alias loc="$CHANGE_MODE local"
alias comp="$CHANGE_MODE comp"
