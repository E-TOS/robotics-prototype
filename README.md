[![Build Status](https://travis-ci.org/space-concordia-robotics/robotics-prototype.svg?branch=master)](https://travis-ci.org/space-concordia-robotics/robotics-prototype)

# robotics-prototype
This repo contains the Robotics software team code. For a quick primer on our workflow using git, [CLICK HERE :)](https://github.com/space-concordia-robotics/robotics-prototype/wiki/Git-Workflow-and-Conventions)

## Contributing and Development Environment Instructions

Firstly, this project is built in Python 3.6+ and JavaScript (ES6). You need to have a version of Python installed that is 3.6+. Make sure that whenever you use `python`, `python3` or `python3.6` or whatever later on meets this requirement.

Secondly, it is imperative you use a virtual env (instead of your system Python) to use/contribute to the project, else things could get messy.
### Prerequisites
Make sure you are using Ubuntu 16.04 (The ROS distribution we use doesn't support anything newer than 16.04).

Make sure the following are installed
```
$ sudo apt install python-pip
$ sudo apt install virtualenv
$ sudo apt install git
```
You will also need python3.6, which is by default not in Ubuntu 16.04's APT's available packages.
#### Installing python3.6
```
$ sudo add-apt-repository ppa:deadsnakes/ppa
$ sudo apt update
$ sudo apt install python3.6
```
### Setup the root repository

```
$ cd ~/
$ mkdir Programming
$ cd Programming
$ git clone --recursive https://github.com/space-concordia-robotics/robotics-prototype robotics-prototype
```
A local repository should now be created. `robotics-prototype` is the root directory for this project.

### Setup [virtualenv](https://docs.python.org/3.6/library/venv.html#modulevenvhttps://virtualenv.pypa.io/en/stable/userguide/)

```
$ cd robotics-prototype
$ virtualenv -p `which python3.6` venv
$ source venv/bin/activate
```
The virtual environment should now be activated.
### Install [dependencies](https://pip.pypa.io/en/stable/user_guide/#requirements-files)
```
(venv) $ pip install -r requirements.txt -r requirements-dev.txt
```
### Setup [setuptools](https://setuptools.readthedocs.io/en/latest/setuptools.html#development-mode)
Still in the root directory,
```
(venv) $ python setup.py develop
(venv) $ pytest
```
Running `pytest` without doing `python setup.py develop` will give an ModuleNotFound error. To read up more on this, click [here]( https://github.com/space-concordia-robotics/robotics-prototype/edit/master/README.md)

To deactivate virtualenv, run `deactivate`(you can do this now or later).
### Install ROS-Kinetic
```
$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
$ sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install ros-kinetic-desktop-full
```

To verify ROS-Kinetic has been successfully installed, you should do
```
$ source /opt/ros/kinetic/setup.bash
$ roscore
```
### Install rosbridge-suite
`$ sudo apt install ros-kinetic-rosbridge-suite`

At this point, the virtualenv must be deactivated (or the following won't work). You should be able to now run
```
$ source /opt/ros/kinetic/setup.bash
$ roslaunch rosbridge_server rosbridge_websocket.launch
```
### Setup catkin workspaces
```
$ cd ~
$ mkdir catkin_ws
$ cd catkin_ws
$ mkdir src
$ catkin_make
```
You need to setup another catkin workspace in robot/rospackages
```
$ cd ~/Programming/robotics-prototype/robot/rospackages
$ catkin_make
```
You can verify that the GUI by running the app.py file in basestation
```
$ cd ~/Programming/robotics-prototype/robot/basestation
$ python app.py
```
### .bashrc edits
You should add this to your `~/.bashrc` file.
```
source /opt/ros/kinetic/setup.bash
source ~/catkin_ws/devel/setup.bash
# local mode
export ROS_MASTER_URI=http://localhost:11311
export ROS_HOSTNAME=localhost
# competition mode
#export ROS_MASTER_URI=http://172.16.1.30:11311
#export ROS_HOSTNAME=beep
. ~/Programming/robotics-prototype/robot/rospackages/devel/setup.bash
. ~/Programming/robotics-prototype/venv/bin/activate

source ~/Programming/robotics-prototype/robot/basestation/config/.bash_aliases
```
Open a new terminal for changes to apply. You should automatically have a virtual environment activated. The last line added a couple of shortcut aliases. You should now be able to do `startgui` to start the GUI. You can read the .bash_aliases file to see all the new shortcuts.

### ffmpeg

For the feature of capturing a snapshot of our camera stream we need `ffmpeg`:
- sudo apt install ffmpeg

You can read about the formatting guide [here](https://github.com/space-concordia-robotics/robotics-prototype/wiki/Code-Formatting-and-Conventions)

## Using Git

### Commit message hook

This explains how to use and setup a git hook that will prepend the issue number to a commit message.
Git hooks are used to automate git commands and functions.
There are a number of different options, but this particular hook will add the issue number to your
`git commit -m` message once it is pushed to the repository.
Git hooks live in a local `.git/hooks` file of each repo, so initializing this hook in the this repo will not change any other repos you might have.

### Setting up a git hook

If you're in Linux run the following command. If you're on windows, install [Git Bash](https://git-scm.com/downloads) to be able to run the same command.

From the root of the repository:
- `cp commit-message-hook.sh .git/hooks/prepare-commit-msg`

### Using the commit hook prepender

Now, when ever you using `git commit -m` the hook will prepend the issue number to your message.
This will show up in the repo as [#\<issue number\>], so there is no longer a need to add this to your commit message.
This will work as long as your branches are named using our branch naming standards defined in our wiki, otherwise the commit will be aborted. For more information on our conventions check [here](https://github.com/space-concordia-robotics/robotics-prototype/wiki/Git-Workflow-and-Conventions).

In order to write a long commit message using `git commit -m`, write a concise title and then press enter twice.
Then, type as long a message as is appropriate and close the quotation mark. This ensures it will be formatted nicely on github.

Finish the commit and `git push` as usual.

### Cloning and Pulling

We are using git submodules in `robotics-prototype`. This means that we are using code that is external to our repository. To ensure that it also downloads all the packages from the external repository, use the commands below :

Clone : `git clone --recursive https://github.com/space-concordia-robotics/robotics-prototype`

Pull : `git pull; git submodule update --init --recursive`

If you do not have access to the GitLab repository, you will not be able to successfully authenticate to clone `rover2018-elec`. This is fine as long as you do not need the PDS code.

If you have access to the GitLab repository, you will need additional setups for GitLab pulling and pushing. Please see [how to create and add your SSH key](https://docs.gitlab.com/ee/gitlab-basics/create-your-ssh-keys.html) and this [issue](https://stackoverflow.com/a/51133684/4048657) you may encounter

### Atom
If you're using Atom (it can be installed via Ubuntu software), setting up should be fairly easy.

- Run `apm install --packages-file .atom/package-list.txt` (from project root). This should install all needed packages.
- Note that the config file `./atom/config.cson` (still in the project root) is where the configurations for said packages are stored/versioned for this project.
