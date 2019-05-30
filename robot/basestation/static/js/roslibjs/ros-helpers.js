function initRosWeb () {
  let ros = new ROSLIB.Ros({
    url: 'ws://localhost:9090'
  })
  ros.on('connection', function () {
    //appendToConsole('Connected to websocket server.')
    checkTaskStatuses()
    if (window.location.pathname == '/rover') {
      initNavigationPanel()
    }
  })
  ros.on('error', function (error) {
    appendToConsole('Error connecting to websocket server: ', error)
  })
  ros.on('close', function () {
    appendToConsole('Connection to websocket server closed.')
  })

  /* general controls */

  // setup a client for the mux_select service
  mux_select_client = new ROSLIB.Service({
    ros: ros,
    name: 'mux_select',
    serviceType: 'SelectMux'
  })
  // setup a client for the serial_cmd service
  serial_cmd_client = new ROSLIB.Service({
    ros: ros,
    name: 'serial_cmd',
    serviceType: 'SerialCmd'
  })
  // setup a client for the task_handler service
  task_handler_client = new ROSLIB.Service({
    ros: ros,
    name: 'task_handler',
    serviceType: 'HandleTask'
  })

  /* arm controls */

  // setup a client for the arm_request service
  arm_request_client = new ROSLIB.Service({
    ros: ros,
    name: 'arm_request',
    serviceType: 'ArmRequest'
  })
  // setup a publisher for the arm_command topic
  arm_command_publisher = new ROSLIB.Topic({
    ros: ros,
    name: 'arm_command',
    messageType: 'std_msgs/String'
  })
  // setup a publisher for the ik_command topic
  ik_command_publisher = new ROSLIB.Topic({
    ros: ros,
    name: 'ik_command',
    messageType: 'IkCommand'
  })
  // setup a subscriber for the arm_joint_states topic
  arm_joint_states_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'arm_joint_states',
    messageType: 'sensor_msgs/JointState'
  })
  arm_joint_states_listener.subscribe(function (message) {
    for (var angle in message.position) {
      // let motor = angle+1;
      let motor = String.fromCharCode(angle.charCodeAt(0) + 1)
      $('#m' + motor + '-angle').text(message.position[angle])
    }
  })
  // setup a subscriber for the arm_feedback topic
  arm_feedback_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'arm_feedback',
    messageType: 'std_msgs/String'
  })
  arm_feedback_listener.subscribe(function (message) {
    appendToConsole(message.data)
  })

  /* rover commands */

  // setup a client for the rover_request service
  rover_request_client = new ROSLIB.Service({
    ros: ros,
    name: 'rover_request',
    serviceType: 'ArmRequest' // for now... might change
  })
  // setup a publisher for the rover_command topic
  rover_command_publisher = new ROSLIB.Topic({
    ros: ros,
    name: 'rover_command',
    messageType: 'std_msgs/String'
  })
  // setup a subscriber for the rover_joint_states topic
  rover_joint_states_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'rover_joint_states',
    messageType: 'sensor_msgs/JointState'
  })
  rover_joint_states_listener.subscribe(function (message) {
    $('#right-front-rpm').text(message.velocity[0])
    $('#right-mid-rpm').text(message.velocity[1])
    $('#right-back-rpm').text(message.velocity[2])
    $('#left-front-rpm').text(message.velocity[3])
    $('#left-mid-rpm').text(message.velocity[4])
    $('#left-back-rpm').text(message.velocity[5])
  })
  // setup a subscriber for the rover_position topic
  rover_position_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'rover_position',
    messageType: 'geometry_msgs/Point'
  })
  rover_position_listener.subscribe(function (message) {
    $('#rover-latitude').text(message.x)
    $('#rover-longitude').text(message.y)
    $('#rover-heading').text(message.z)
  })
  // setup a subscriber for the rover_feedback topic
  rover_feedback_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'rover_feedback',
    messageType: 'std_msgs/String'
  })
  rover_feedback_listener.subscribe(function (message) {
    appendToConsole(message.data)
  })
  // setup a subscriber for the antenna_goal topic
  antenna_goal_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'antenna_goal',
    messageType: 'geometry_msgs/Point'
  })
  antenna_goal_listener.subscribe(function (message) {
    $('#recommended-antenna-angle').text(parseFloat(message.x).toFixed(3))
    $('#distance-to-rover').text(parseFloat(message.y).toFixed(2))
  })
  // setup gps parameters for antenna directing
  antenna_latitude = new ROSLIB.Param({
   ros : ros,
   name : 'antenna_latitude'
  });
  antenna_longitude = new ROSLIB.Param({
   ros : ros,
   name : 'antenna_longitude'
  });
  antenna_start_dir = new ROSLIB.Param({
   ros : ros,
   name : 'antenna_start_dir'
  });

  // setup a subscriber for the rover_goal topic
  rover_goal_listener = new ROSLIB.Topic({
    ros: ros,
    name: 'rover_goal',
    messageType: 'geometry_msgs/Point'
  })
  rover_goal_listener.subscribe(function (message) {
    $('#recommended-rover-heading').text(parseFloat(message.x).toFixed(3))
    $('#distance-to-goal').text(parseFloat(message.y).toFixed(2))
  })
  // setup gps parameters for rover goals
  goal_latitude = new ROSLIB.Param({
   ros : ros,
   name : 'goal_latitude'
  });
  goal_longitude = new ROSLIB.Param({
   ros : ros,
   name : 'goal_longitude'
  });
}

initRosWeb()

/* functions used in main code */
function requestMuxChannel (elemID, callback) {
  let dev = elemID[elemID.length - 1]
  let request = new ROSLIB.ServiceRequest({ device: dev })
  let sentTime = new Date().getTime()

  console.log(request)
  appendToConsole(
    'Sending request to switch to channel ' + $('a' + elemID).text()
  )

  mux_select_client.callService(request, function (result) {
    let latency = millisSince(sentTime)
    console.log(result)
    let msg = result.response//.slice(0, result.response.length - 1) // remove newline character
    if (msg.includes('failed') || msg.includes('ERROR')) {
      // how to account for a lack of response?
      appendToConsole('Request failed. Received "' + msg + '"')
      callback([false, msg])
    } else {
      $('button#mux').text('Device ' + $('a' + elemID).text())
      appendToConsole(
        'Received "' + msg + '" with ' + latency.toString() + ' ms latency'
      )
      callback([true])
    }
  })
}
function requestSerialCommand (command, callback) {
  let request = new ROSLIB.ServiceRequest({ msg: command + '\n' })
  let sentTime = new Date().getTime()

  console.log(request)
  appendToConsole('Sending request to execute command "' + command + '"')

  mux_select_client.callService(request, function (result) {
    let latency = millisSince(sentTime)
    console.log(result)
    let msg = result.response//.slice(0, result.response.length - 1) // remove newline character
    if (msg.includes('failed') || msg.includes('ERROR')) {
      // how to account for a lack of response?
      appendToConsole('Request failed. Received "' + msg + '"')
      callback([false, msg])
    } else {
      appendToConsole(
        'Received "' + msg + '" with ' + latency.toString() + ' ms latency'
      )
      callback([true])
    }
  })
}
function requestTask (reqTask, reqStatus, buttonID, callback, reqArgs = '') {
  var request
  if (reqArgs=='') {
    request = new ROSLIB.ServiceRequest({ task: reqTask, status: reqStatus })
  } else {
    request = new ROSLIB.ServiceRequest({ task: reqTask, status: reqStatus, args : reqArgs })
  }
  let sentTime = new Date().getTime()

  console.log('request:', request)
  if (reqStatus == 0) {
    appendToConsole('Sending request to stop ' + reqTask + ' task')
  } else if (reqStatus == 1) {
    appendToConsole('Sending request to start ' + reqTask + ' task')
  } else if (reqStatus == 2) {
    appendToConsole('Sending request to check ' + reqTask + ' task status')
  }

  task_handler_client.callService(request, function (result) {
    let latency = millisSince(sentTime)
    console.log('result:', result)
    let msg = result.response
    if (
      msg.includes('Failed') ||
      msg.includes('shutdown request') ||
      msg.includes('unavailable') ||
      msg.includes('is already running') ||
      msg.includes('not available')
    ) {
      // how to account for a lack of response?
      appendToConsole('Request failed. Received "' + msg + '"')
      callback([false, msg])
    } else {
      appendToConsole(
        'Received "' + msg + '" with ' + latency.toString() + ' ms latency'
      )
      if (reqStatus == 0) {
        $(buttonID)[0].checked = false
      } else if (reqStatus == 1) {
        $(buttonID)[0].checked = true
      } else if (reqStatus == 2) {
        if (msg.includes('not')) {
          $(buttonID)[0].checked = false
        } else {
          $(buttonID)[0].checked = true
        }
      }
      callback([true, msg])
    }
  })
}

function sendArmCommand (cmd) {
  let command = new ROSLIB.Message({ data: cmd })
  console.log(command)
  appendToConsole('Sending "' + cmd + '" to arm Teensy')
  arm_command_publisher.publish(command)
}
function sendArmRequest (command, callback) {
  let request = new ROSLIB.ServiceRequest({ msg: command })
  let sentTime = new Date().getTime()

  console.log(request)
  appendToConsole('Sending request to execute command \"' + command + '\"')

  arm_request_client.callService(request, function (result) {
    let latency = millisSince(sentTime)
    console.log(result)
    let msg = result.response//.slice(0, result.response.length - 1) // remove newline character
    if (!result.success) {
      // how to account for a lack of response?
      appendToConsole('Request failed. Received "' + msg + '"')
      // return false
      callback([false, msg])
    } else {
      appendToConsole(
        'Received "' + msg + '" with ' + latency.toString() + ' ms latency'
      )
      // return true
      callback([true, msg])
    }
  })
}

function sendRoverCommand (cmd) {
  let command = new ROSLIB.Message({ data: cmd })
  console.log(command)
  appendToConsole('Sending \"' + cmd + '\" to rover Teensy')
  rover_command_publisher.publish(command)
  //arm_command_publisher.publish(command)
}

function sendRoverRequest (command, callback) {
  let request = new ROSLIB.ServiceRequest({ msg: command })
  let sentTime = new Date().getTime()

  console.log(request)
  appendToConsole('Sending request to execute command \"' + command + '\"')

  rover_request_client.callService(request, function (result) {
    let latency = millisSince(sentTime)
    console.log(result)
    let msg = result.response//.slice(0, result.response.length - 1) // remove newline character
    if (!result.success) {
      // how to account for a lack of response?
      appendToConsole('Request failed. Received "' + msg + '"')
      // return false
      callback([false, msg])
    } else {
      appendToConsole(
        'Received "' + msg + '" with ' + latency.toString() + ' ms latency'
      )
      // return true
      callback([true, msg])
    }
  })
}

function checkTaskStatuses () {
  if (window.location.pathname == '/') {
    // check arm listener status
    requestTask('arm_listener', 2, '#toggle-arm-listener-btn', function(msgs) {
      console.log(msgs)
      appendToConsole(msgs)
      if (msgs[0]) {
        console.log('tru')
      } else {
        console.log('fals')
      }
    })
    // check arm camera stream status
    requestTask('camera_stream', 2, '#toggle-arm-stream-btn', function(msgs) {
      console.log(msgs)
      appendToConsole(msgs)
      if (msgs[0]) {
        console.log('truu')
      } else {
        console.log('falss')
      }
    })
  } else if (window.location.pathname == '/rover') {
    //do the same but for front and rear cameras
  } else if (window.location.pathname == '/science') {
    //do the same but for front and rear cameras
  } /*else if (window.location.pathname == '/rover') { //pds
    //do the same but for front and rear cameras
  }*/
}
function initNavigationPanel () {
  let hasAntennaParams = true
  antenna_latitude.get(function(val){
    if (val!=null) {
      $('#antenna-latitude').text(val)
      antenna_longitude.get(function(val){
        if (val!=null) {
          $('#antenna-longitude').text(val)
          antenna_start_dir.get(function(val){
            if (val!=null) {
              $('#antenna-start-dir').text(val)
              appendToConsole('Antenna goal parameters already set, displaying antenna directions')
              $('#antenna-inputs').hide()
              $('#antenna-unchanging').show()
            } else {
              appendToConsole('One or more antenna parameters is missing, if you would like antenna directions then please input them')
              $('#antenna-inputs').show()
              $('#antenna-unchanging').hide()
            }
          })
        } else {
          appendToConsole('One or more antenna parameters is missing, if you would like antenna directions then please input them')
          $('#antenna-inputs').show()
          $('#antenna-unchanging').hide()
        }
      })
    } else {
      appendToConsole('One or more antenna parameters is missing, if you would like antenna directions then please input them')
      $('#antenna-inputs').show()
      $('#antenna-unchanging').hide()
    }
  })

  goal_latitude.get(function(val){
    if(val!=null){
      $('#goal-latitude').text(val)
      goal_longitude.get(function(val){
        if (val!=null) {
          appendToConsole('GPS goal parameters already set, displaying directions to the goal')
          $('#goal-longitude').text(val)
          $('#goal-inputs').hide()
          $('#goal-unchanging').show()
        } else {
          appendToConsole('One or more GPS goal parameters is missing, if you would like directions to the goal then please input them')
          $('#goal-inputs').show()
          $('#goal-unchanging').hide()
        }
      })
    } else {
      appendToConsole('One or more GPS goal parameters is missing, if you would like directions to the goal then please input them')
      $('#goal-inputs').show()
      $('#goal-unchanging').hide()
    }
  })
}
